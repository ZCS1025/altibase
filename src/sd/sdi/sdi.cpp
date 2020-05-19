/** 
 *  Copyright (c) 1999~2017, Altibase Corp. and/or its affiliates. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License, version 3,
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
 

/***********************************************************************
 * $Id$
 **********************************************************************/

#include <sdDef.h>
#include <sdi.h>
#include <sdm.h>
#include <sdf.h>
#include <sda.h>
#include <qci.h>
#include <qcm.h>
#include <qcg.h>
#include <qmn.h>
#include <sdl.h>
#include <sdlStatement.h>
#include <sdlStatementManager.h>
#include <sdSql.h>
#include <qdk.h>
#include <sdmShardPin.h>
#include <sduProperty.h>

#define SAVEPOINT_FOR_SHARD_STMT_PARTIAL_ROLLBACK   ("$$SHARD_PARTIAL_ROLLBACK")

extern iduFixedTableDesc gShardConnectionInfoTableDesc;
extern iduFixedTableDesc gShardMetaNodeInfoTableDesc;
extern iduFixedTableDesc gShardDataNodeInfoTableDesc;

static sdiAnalyzeInfo    gAnalyzeInfoForAllNodes;

/* BUG-45899 */
static SChar * gCanNotMergeReasonArr[] = { SDI_CAN_NOT_MERGE_REASONS, NULL };

/* BUG-46039 SLCN(Shard Linker Change Number) -> SMN(Shard Meta Number) */
static ULong             gSMNForMetaNode = ID_ULONG(0);

/* BUG-46090 Meta Node SMN 전파 */
static ULong             gSMNForDataNode = ID_ULONG(0);

/* PROJ-2701 Sharding online data rebuild */
static UInt              gShardUserID = ID_UINT_MAX;

static inline idBool isNameInDoubleQuotation( qcNamePosition    * aNamePosition )
{
    SInt      i = 0;
    SChar   * sNodeName = aNamePosition->stmtText + aNamePosition->offset;
    idBool    sIsDoubleQuoted = ID_FALSE;

    for ( i = 0; i < aNamePosition->size; i++ )
    {
        if ( sNodeName[i] == '"' )
        {
            sIsDoubleQuoted = ID_TRUE;
            break;
        }
    }

    return sIsDoubleQuoted;
}

static inline UInt removeDoulbeQuotationFromNodeName( qcShardNodes     * aNodeName,
                                                      SChar            * aBuffer,
                                                      SInt               aBufferLength )
{
    SChar           * sName = aNodeName->namePos.stmtText + aNodeName->namePos.offset;
    SInt              sNameLength = aNodeName->namePos.size;
    SInt              sNameOffset = 0;
    SInt              sBufferOffset = 0;

    while ( ( sNameOffset < sNameLength ) &&
            ( sBufferOffset < aBufferLength - 1 ) )
    {
        if ( sName[sNameOffset] != '"' )
        {
            aBuffer[sBufferOffset] = sName[sNameOffset];
            sBufferOffset++;
        }

        sNameOffset++;
    }

    aBuffer[sBufferOffset] = '\0';

    return sBufferOffset;
}

static inline idBool isMatchedNodeName( qcShardNodes     * aNodeName1,
                                        const SChar      * aNodeName2 )
{
    idBool            sIsSame = ID_FALSE;
    SChar             sNodeName1Buf[SDI_NODE_NAME_MAX_SIZE + 1];
    UInt              sNodeName1BufLength = 0;

    if ( isNameInDoubleQuotation( &(aNodeName1->namePos) ) == ID_FALSE )
    {
        sIsSame = QC_IS_STR_CASELESS_MATCHED( aNodeName1->namePos, aNodeName2 );
    }
    else
    {
        sNodeName1BufLength = removeDoulbeQuotationFromNodeName( aNodeName1,
                                                                sNodeName1Buf,
                                                                SDI_NODE_NAME_MAX_SIZE + 1 );

        sIsSame = ( idlOS::strMatch( sNodeName1Buf,
                                     sNodeName1BufLength,
                                     aNodeName2,
                                     idlOS::strlen( aNodeName2 ) ) == 0 )
                  ? ID_TRUE : ID_FALSE;
    }
    
    return sIsSame;
}
static inline idBool needShardStmtPartialRollback( qciStmtType aStmtKind )
{
    idBool      sNeedShardStmtPartialRollback = ID_FALSE;

    if ( ( aStmtKind == QCI_STMT_INSERT ) ||
         ( aStmtKind == QCI_STMT_UPDATE ) ||
         ( aStmtKind == QCI_STMT_DELETE ) ||
         ( aStmtKind == QCI_STMT_SELECT_FOR_UPDATE ) )
    {
        sNeedShardStmtPartialRollback = ID_TRUE;
    }
    else
    {
        sNeedShardStmtPartialRollback = ID_FALSE;
    }

    return sNeedShardStmtPartialRollback;
}

static void processExecuteError( qcStatement       * aStatement,
                                 sdiClientInfo     * aClientInfo,
                                 sdiDataNode       * aDataNode )
{
    UInt                  i = 0;
    sdiConnectInfo      * sConnectInfo = aClientInfo->mConnectInfo;
    idBool                sIsAutoCommit;
    idBool                sNeedShardStmtPartialRollback;
   
    sIsAutoCommit = QCG_GET_SESSION_IS_AUTOCOMMIT( aStatement );
    sNeedShardStmtPartialRollback = needShardStmtPartialRollback( aStatement->myPlan->parseTree->stmtKind ) ;

    for ( i = 0; i < aClientInfo->mCount; i++, sConnectInfo++, aDataNode++ )
    {
        switch( aDataNode->mState )
        {
            case SDI_NODE_STATE_EXECUTED:
                if ( sIsAutoCommit == ID_TRUE )
                {
                    sdl::appendResultInfoToErrorMessage( sConnectInfo,
                                                         aStatement->myPlan->parseTree->stmtKind );
                }
                else
                {
                    if ( sNeedShardStmtPartialRollback == ID_TRUE )
                    {
                        if ( sdl::shardStmtPartialRollback( sConnectInfo, 
                                                            &(sConnectInfo->mLinkFailure) ) 
                             != IDE_SUCCESS )
                        {
                            IDE_ERRLOG( IDE_SD_0 );

                            sdi::setTransactionBroken( sIsAutoCommit,
                                                       (void*)QCG_GET_DATABASE_LINK_SESSION( aStatement ),
                                                       QC_SMI_STMT(aStatement)->mTrans );
                        }
                    }
                }
                break;

            case SDI_NODE_STATE_EXECUTE_SELECTED:
                aDataNode->mState = SDI_NODE_STATE_EXECUTE_CANDIDATED;
                break;

            default:
                break;
        }
    }
}

void sdi::initialize()
{
    sdmShardPinMgr::initialize();
}

IDE_RC sdi::loadMetaNodeInfo()
{
    sdiGlobalMetaNodeInfo   sMetaNodeInfo = { ID_ULONG(0) };
    UInt                    sShardUserID = ID_UINT_MAX;

    IDE_TEST_CONT( isShardEnable() != ID_TRUE, NO_SHARD_ENABLE );
    IDE_TEST_CONT( checkMetaCreated() != ID_TRUE, NO_SHARD_ENABLE );

    /* ShardUserID를 세팅한다. */
    IDE_TEST_CONT( sdm::getShardUserID( &sShardUserID ) != IDE_SUCCESS, NO_SHARD_ENABLE );
    setShardUserID( sShardUserID );

    /* Global Shard Meta Info */
    IDE_TEST( sdm::getGlobalMetaNodeInfo( &sMetaNodeInfo ) != IDE_SUCCESS );
    setSMNCacheForMetaNode( sMetaNodeInfo.mShardMetaNumber );

    /* PROJ-2701 Sharding online data rebuild
     * start-up시에 dataSMN은 초기화 되어있다.
     * 이를 dataSMN을 metaSMN과 동일하게 설정한다.
     *
     * BUGBUG data rebuild수행중 장애시에, 해당 node가 내려갔다 올라오면(failback)
     *        dataSMN이 하나 낮은 상태로 설정 될 것이다.
     *        다른 살아있던 노드들 중 가장 큰 dataSMN으로 설정되도록 해야한다.
     */
    setSMNForDataNode( sMetaNodeInfo.mShardMetaNumber );

    /* Local Shard Meta Info */
    sdmShardPinMgr::loadShardPinInfo();

    IDE_EXCEPTION_CONT( NO_SHARD_ENABLE );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

void sdi::finalize()
{
    sdmShardPinMgr::finalize();
}

UInt sdi::getShardUserID()
{
    return gShardUserID;
}

void sdi::setShardUserID( UInt aShardUserID )
{
    gShardUserID = aShardUserID;
}

ULong sdi::getSMNForMetaNode()
{
    return (ULong)idCore::acpAtomicGet64( &gSMNForMetaNode );
}

void sdi::setSMNCacheForMetaNode( ULong aNewSMN )
{
    (void)idCore::acpAtomicSet64( &gSMNForMetaNode, aNewSMN );
}

IDE_RC sdi::getIncreasedSMNForMetaNode( smiTrans * aTrans,
                                        ULong    * aNewSMN )
{
    sdiGlobalMetaNodeInfo   sMetaNodeInfo = { ID_ULONG(0) };
    smiStatement            sSmiStmt;
    idBool                  sIsStmtBegun  = ID_FALSE;

    IDE_TEST( sSmiStmt.begin( aTrans->getStatistics(),
                              aTrans->getStatement(),
                              ( SMI_STATEMENT_NORMAL |
                                SMI_STATEMENT_MEMORY_CURSOR ) )
              != IDE_SUCCESS );
    sIsStmtBegun = ID_TRUE;

    /* PROJ-2701 Online data rebuild
     *
     * // IDE_TEST( sdm::increaseShardMetaNumberCore( &sSmiStmt ) != IDE_SUCCESS );
     *
     * GLOBAL_META_INFO_의 shard_meta_number에 대한 increase시점은
     * Tx의 첫 shard meta 변경 시로 이동한다.
     */
    IDE_TEST( sdm::getGlobalMetaNodeInfoCore( &sSmiStmt, &sMetaNodeInfo ) != IDE_SUCCESS );

    IDE_TEST( sSmiStmt.end( SMI_STATEMENT_RESULT_SUCCESS ) != IDE_SUCCESS );
    sIsStmtBegun = ID_FALSE;

    *aNewSMN = sMetaNodeInfo.mShardMetaNumber;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    IDE_PUSH();

    if ( sIsStmtBegun == ID_TRUE )
    {
        (void)sSmiStmt.end( SMI_STATEMENT_RESULT_FAILURE );
    }
    else
    {
        /* Nothing to do */
    }

    IDE_POP();

    return IDE_FAILURE;
}

IDE_RC sdi::reloadSMNForDataNode( smiStatement * aSmiStmt )
{
    sdiGlobalMetaNodeInfo   sMetaNodeInfo = { ID_ULONG(0) };

    IDE_TEST_CONT( isShardEnable() != ID_TRUE, NO_SHARD_ENABLE );
    IDE_TEST_CONT( checkMetaCreated() != ID_TRUE, NO_SHARD_ENABLE );

    if ( aSmiStmt != NULL )
    {
        IDE_TEST( sdm::getGlobalMetaNodeInfoCore( aSmiStmt,
                                                  &sMetaNodeInfo ) != IDE_SUCCESS );
    }
    else
    {
        IDE_TEST( sdm::getGlobalMetaNodeInfo( &sMetaNodeInfo ) != IDE_SUCCESS );
    }

    if ( sMetaNodeInfo.mShardMetaNumber > getSMNForDataNode() )
    {
        setSMNForDataNode( sMetaNodeInfo.mShardMetaNumber );
    }
    else
    {
        // Nothing to do.
    }

    IDE_EXCEPTION_CONT( NO_SHARD_ENABLE );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

ULong sdi::getSMNForDataNode()
{
    return (ULong)idCore::acpAtomicGet64( &gSMNForDataNode );
}

void sdi::setSMNForDataNode( ULong aNewSMN )
{
    (void)idCore::acpAtomicSet64( &gSMNForDataNode, aNewSMN );
}

IDE_RC sdi::addExtMT_Module( void )
{
    if ( isShardEnable() == ID_TRUE )
    {
        IDE_TEST( mtc::addExtFuncModule( (mtfModule**)
                                         sdf::extendedFunctionModules )
                  != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    IDE_TEST( mtc::addExtFuncModule( (mtfModule**)
                                     sdf::extendedFunctionModules2 )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdi::initSystemTables( void )
{
    if ( isShardEnable() == ID_TRUE )
    {
        // initialize fixed table
        IDU_FIXED_TABLE_DEFINE_RUNTIME( gShardMetaNodeInfoTableDesc );
        IDU_FIXED_TABLE_DEFINE_RUNTIME( gShardDataNodeInfoTableDesc );
        IDU_FIXED_TABLE_DEFINE_RUNTIME( gShardConnectionInfoTableDesc );

        // initialize gAnalyzeInfoForAllNodes
        SDI_INIT_ANALYZE_INFO( &gAnalyzeInfoForAllNodes );

        gAnalyzeInfoForAllNodes.mIsCanMerge = ID_TRUE;
    }
    else
    {
        IDU_FIXED_TABLE_DEFINE_RUNTIME( gShardDataNodeInfoTableDesc );
    }

    return IDE_SUCCESS;
}

IDE_RC sdi::checkStmt( qcStatement * aStatement )
{
    return sda::checkStmt( aStatement );
}

IDE_RC sdi::analyze( qcStatement * aStatement,
                     ULong         aSMN )
{
    IDU_FIT_POINT_FATAL( "sdi::analyze::__FT__" );

    IDE_TEST( sda::analyze( aStatement,
                            aSMN )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdi::setAnalysisResult( qcStatement * aStatement )
{
    IDU_FIT_POINT_FATAL( "sdi::setAnalysisResult::__FT__" );

    IDE_TEST( sda::setAnalysisResult( aStatement ) != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdi::setAnalysisResultForInsert( qcStatement    * aStatement,
                                        sdiAnalyzeInfo * aAnalyzeInfo,
                                        sdiObjectInfo  * aShardObjInfo )
{
    SDI_INIT_ANALYZE_INFO(aAnalyzeInfo);

    // analyzer를 통하지 않고 직접 analyze 정보를 생성한다.
    if ( aShardObjInfo->mTableInfo.mSplitMethod != SDI_SPLIT_SOLO )
    {
        IDE_TEST( QC_QMP_MEM(aStatement)->alloc( ID_SIZEOF(sdiValueInfo),
                                                 (void**)&aAnalyzeInfo->mValue )
                  != IDE_SUCCESS );

        aAnalyzeInfo->mValueCount = 1;
        aAnalyzeInfo->mValue[0].mType = 0;  // host variable
        aAnalyzeInfo->mValue[0].mValue.mBindParamId = aShardObjInfo->mTableInfo.mKeyColOrder;
        aAnalyzeInfo->mIsCanMerge = ID_TRUE;
        aAnalyzeInfo->mSplitMethod = aShardObjInfo->mTableInfo.mSplitMethod;
        aAnalyzeInfo->mKeyDataType = aShardObjInfo->mTableInfo.mKeyDataType;
    }
    else
    {
        aAnalyzeInfo->mValueCount = 0;
        aAnalyzeInfo->mIsCanMerge = ID_TRUE;
        aAnalyzeInfo->mSplitMethod = aShardObjInfo->mTableInfo.mSplitMethod;
        aAnalyzeInfo->mKeyDataType = aShardObjInfo->mTableInfo.mKeyDataType;
    }

    aAnalyzeInfo->mSubKeyExists = aShardObjInfo->mTableInfo.mSubKeyExists;
    

    if ( aAnalyzeInfo->mSubKeyExists == ID_TRUE )
    {
        IDE_TEST( QC_QMP_MEM(aStatement)->alloc( ID_SIZEOF(sdiValueInfo),
                                                 (void**)&aAnalyzeInfo->mSubValue )
                  != IDE_SUCCESS );

        aAnalyzeInfo->mSubValueCount = 1;
        aAnalyzeInfo->mSubValue[0].mType = 0;  // host variable
        aAnalyzeInfo->mSubValue[0].mValue.mBindParamId = aShardObjInfo->mTableInfo.mSubKeyColOrder;
        aAnalyzeInfo->mSubSplitMethod = aShardObjInfo->mTableInfo.mSubSplitMethod;
        aAnalyzeInfo->mSubKeyDataType = aShardObjInfo->mTableInfo.mSubKeyDataType;
    }
    else
    {
        // Already initialized
        // Nothing to do.
    }

    aAnalyzeInfo->mDefaultNodeId = aShardObjInfo->mTableInfo.mDefaultNodeId;

    IDE_TEST( sdi::allocAndCopyRanges( aStatement,
                                       &aAnalyzeInfo->mRangeInfo,
                                       &aShardObjInfo->mRangeInfo )
              != IDE_SUCCESS );

    // PROJ-2685 online rebuild
    IDE_TEST( QC_QMP_MEM(aStatement)->alloc(
                  ID_SIZEOF(sdiTableInfoList),
                  (void**) &(aAnalyzeInfo->mTableInfoList) )
              != IDE_SUCCESS );

    aAnalyzeInfo->mTableInfoList->mTableInfo = &(aShardObjInfo->mTableInfo);
    aAnalyzeInfo->mTableInfoList->mNext = NULL;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdi::setAnalysisResultForTable( qcStatement    * aStatement,
                                       sdiAnalyzeInfo * aAnalyzeInfo,
                                       sdiObjectInfo  * aShardObjInfo )
{
    SDI_INIT_ANALYZE_INFO(aAnalyzeInfo);

    // analyzer를 통하지 않고 직접 analyze 정보를 생성한다.
    aAnalyzeInfo->mIsCanMerge = ID_TRUE;
    aAnalyzeInfo->mSplitMethod = aShardObjInfo->mTableInfo.mSplitMethod;
    aAnalyzeInfo->mKeyDataType = aShardObjInfo->mTableInfo.mKeyDataType;

    aAnalyzeInfo->mSubKeyExists = aShardObjInfo->mTableInfo.mSubKeyExists;
    aAnalyzeInfo->mSubSplitMethod = aShardObjInfo->mTableInfo.mSubSplitMethod;
    aAnalyzeInfo->mSubKeyDataType = aShardObjInfo->mTableInfo.mSubKeyDataType;

    aAnalyzeInfo->mDefaultNodeId = aShardObjInfo->mTableInfo.mDefaultNodeId;

    IDE_TEST( sdi::allocAndCopyRanges( aStatement,
                                       &aAnalyzeInfo->mRangeInfo,
                                       &aShardObjInfo->mRangeInfo )
              != IDE_SUCCESS );

    // PROJ-2685 online rebuild
    IDE_TEST( QC_QMP_MEM(aStatement)->alloc(
                  ID_SIZEOF(sdiTableInfoList),
                  (void**) &(aAnalyzeInfo->mTableInfoList) )
              != IDE_SUCCESS );

    aAnalyzeInfo->mTableInfoList->mTableInfo = &(aShardObjInfo->mTableInfo);
    aAnalyzeInfo->mTableInfoList->mNext = NULL;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdi::copyAnalyzeInfo( qcStatement    * aStatement,
                             sdiAnalyzeInfo * aAnalyzeInfo,
                             sdiAnalyzeInfo * aSrcAnalyzeInfo )
{
    sdiTableInfoList * sTableInfoList;
    sdiTableInfoList * sTableInfo;

    /*
     * sdiAnalyzeInfo 전체를 memcpy한 후,
     * pointer variable인 mRangeInfo->mRanges, mValue, mSubValue 에 대해서는
     * 별도로 memory를 alloc하고 복사한다.
     */
    idlOS::memcpy( (void*)aAnalyzeInfo,
                   (void*)aSrcAnalyzeInfo,
                   ID_SIZEOF(sdiAnalyzeInfo) );

    aAnalyzeInfo->mRangeInfo.mCount = 0;
    aAnalyzeInfo->mRangeInfo.mRanges = NULL;
    aAnalyzeInfo->mValueCount = 0;
    aAnalyzeInfo->mValue = NULL;
    aAnalyzeInfo->mSubValueCount = 0;
    aAnalyzeInfo->mSubValue = NULL;
    aAnalyzeInfo->mTableInfoList = NULL;

    IDE_TEST( allocAndCopyRanges( aStatement,
                                  &aAnalyzeInfo->mRangeInfo,
                                  &aSrcAnalyzeInfo->mRangeInfo )
              != IDE_SUCCESS );

    IDE_TEST( allocAndCopyValues( aStatement,
                                  &aAnalyzeInfo->mValue,
                                  &aAnalyzeInfo->mValueCount,
                                  aSrcAnalyzeInfo->mValue,
                                  aSrcAnalyzeInfo->mValueCount )
              != IDE_SUCCESS );

    IDE_TEST( allocAndCopyValues( aStatement,
                                  &aAnalyzeInfo->mSubValue,
                                  &aAnalyzeInfo->mSubValueCount,
                                  aSrcAnalyzeInfo->mSubValue,
                                  aSrcAnalyzeInfo->mSubValueCount )
              != IDE_SUCCESS );

    for ( sTableInfoList = aSrcAnalyzeInfo->mTableInfoList;
          sTableInfoList != NULL;
          sTableInfoList = sTableInfoList->mNext )
    {
        IDE_TEST( QC_QMP_MEM(aStatement)->alloc( ID_SIZEOF(sdiTableInfoList) +
                                                 ID_SIZEOF(sdiTableInfo),
                                                 (void**) &sTableInfo )
                  != IDE_SUCCESS );

        sTableInfo->mTableInfo =
            (sdiTableInfo*)((UChar*)sTableInfo + ID_SIZEOF(sdiTableInfoList));

        idlOS::memcpy( sTableInfo->mTableInfo,
                       sTableInfoList->mTableInfo,
                       ID_SIZEOF(sdiTableInfo) );

        // link
        sTableInfo->mNext = aAnalyzeInfo->mTableInfoList;
        aAnalyzeInfo->mTableInfoList = sTableInfo;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

sdiAnalyzeInfo * sdi::getAnalysisResultForAllNodes()
{
    return & gAnalyzeInfoForAllNodes;
}

idBool sdi::checkMetaCreated()
{
    smiTrans       sTrans;
    smiStatement   sSmiStmt;
    smiStatement * sDummySmiStmt = NULL;
    smSCN          sDummySCN;
    UInt           sStage = 0;

    IDE_TEST( sTrans.initialize() != IDE_SUCCESS );
    sStage = 1;

    IDE_TEST( sTrans.begin( &sDummySmiStmt, NULL ) != IDE_SUCCESS );
    sStage = 2;

    IDE_TEST( sSmiStmt.begin( NULL,
                              sDummySmiStmt,
                              ( SMI_STATEMENT_UNTOUCHABLE |
                                SMI_STATEMENT_MEMORY_CURSOR ) )
              != IDE_SUCCESS );
    sStage = 3;

    IDE_TEST( sdm::isShardMetaCreated( &sSmiStmt ) != ID_TRUE );

    IDE_TEST( sSmiStmt.end( SMI_STATEMENT_RESULT_SUCCESS ) != IDE_SUCCESS );
    sStage = 2;

    IDE_TEST( sTrans.commit( &sDummySCN ) != IDE_SUCCESS );
    sStage = 1;

    sStage = 0;
    IDE_TEST( sTrans.destroy( NULL ) != IDE_SUCCESS );

    return ID_TRUE;

    IDE_EXCEPTION_END;

    switch ( sStage )
    {
        case 3:
            ( void )sSmiStmt.end( SMI_STATEMENT_RESULT_FAILURE );
            /* fall through */
        case 2:
            ( void )sTrans.rollback();
            /* fall through */
        case 1:
            ( void )sTrans.destroy( NULL );
            /* fall through */
        default:
            break;
    }

    IDE_CLEAR();

    return ID_FALSE;
}

IDE_RC sdi::getExternalNodeInfo( sdiNodeInfo * aNodeInfo,
                                 ULong         aSMN )
{
    smiTrans       sTrans;
    smiStatement   sSmiStmt;
    smiStatement * sDummySmiStmt;
    smSCN          sDummySCN;
    UInt           sStage = 0;

    ULong          sDataSMNForMetaInfo = ID_ULONG(0);

    IDE_DASSERT( aNodeInfo != NULL );

    /* init */
    aNodeInfo->mCount = 0;

    /* PROJ-2446 ONE SOURCE MM 에서 statistics정보를 넘겨 받아야 한다.
     * 추후 작업 */
    IDE_TEST( sTrans.initialize() != IDE_SUCCESS );
    sStage = 1;

    IDE_TEST( sTrans.begin( &sDummySmiStmt, NULL )
              != IDE_SUCCESS );
    sStage = 2;

    /* PROJ-2701 Online data rebuild */
    if ( getSMNForMetaNode() < aSMN )
    {
        IDE_TEST( waitAndSetSMNForMetaNode( NULL,
                                            sDummySmiStmt,
                                            ( SMI_STATEMENT_UNTOUCHABLE | SMI_STATEMENT_MEMORY_CURSOR ),
                                            aSMN,
                                            &sDataSMNForMetaInfo )
                  != IDE_SUCCESS );

        IDE_DASSERT( aSMN == sDataSMNForMetaInfo );
    }
    else
    {
        sDataSMNForMetaInfo = aSMN;
    }

    IDE_TEST( sSmiStmt.begin( NULL,
                              sDummySmiStmt,
                              (SMI_STATEMENT_UNTOUCHABLE |
                               SMI_STATEMENT_MEMORY_CURSOR) )
              != IDE_SUCCESS );
    sStage = 3;

    IDE_TEST( sdm::getExternalNodeInfo( &sSmiStmt,
                                        aNodeInfo,
                                        sDataSMNForMetaInfo ) != IDE_SUCCESS );

    IDE_TEST( sSmiStmt.end( SMI_STATEMENT_RESULT_SUCCESS ) != IDE_SUCCESS );
    sStage = 2;

    IDE_TEST( sTrans.commit( &sDummySCN ) != IDE_SUCCESS );
    sStage = 1;

    sStage = 0;
    IDE_TEST( sTrans.destroy( NULL ) != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    switch ( sStage )
    {
        case 3:
            ( void )sSmiStmt.end( SMI_STATEMENT_RESULT_FAILURE );
            /* fall through */
        case 2:
            ( void )sTrans.rollback();
            /* fall through */
        case 1:
            ( void )sTrans.destroy( NULL );
            /* fall through */
        default:
            break;
    }

    ideLog::log( IDE_SD_0, "[SHARD META : FAILURE] errorcode 0x%05"ID_XINT32_FMT" %s\n",
                           E_ERROR_CODE(ideGetErrorCode()),
                           ideGetErrorMsg(ideGetErrorCode()));
    return IDE_FAILURE;
}

IDE_RC sdi::getInternalNodeInfo( smiTrans    * aTrans,
                                 sdiNodeInfo * aNodeInfo,
                                 idBool        aIsShardMetaChanged,
                                 ULong         aSMN )
{
    smiTrans       sTrans;
    smiStatement   sSmiStmt;
    smiStatement * sDummySmiStmt = NULL;
    smSCN          sDummySCN;
    UShort         sTransStage = 0;
    idBool         sIsStmtBegun = ID_FALSE;

    ULong          sDataSMNForMetaInfo = ID_ULONG(0);
    idvSQL       * sStatistics = NULL;
    smiStatement * sSmiStmtForMetaInfo = NULL;

    IDE_DASSERT( aNodeInfo != NULL );

    /* init */
    aNodeInfo->mCount = 0;

    if ( aTrans == NULL )
    {
        IDE_TEST( sTrans.initialize() != IDE_SUCCESS );
        sTransStage = 1;

        IDE_TEST( sTrans.begin( &sDummySmiStmt, NULL )
                  != IDE_SUCCESS );
        sTransStage = 2;

        sStatistics = NULL;
        sSmiStmtForMetaInfo = sDummySmiStmt;        
    }
    else
    {
        IDE_TEST_RAISE( aTrans->isBegin() == ID_FALSE, ERR_INVALID_TRANS );

        sStatistics = aTrans->getStatistics();
        sSmiStmtForMetaInfo = aTrans->getStatement();
    }

    // PROJ-2701 Online data rebuild
    if ( aIsShardMetaChanged == ID_TRUE )
    {
        /* Shard Meta를 변경한 Transaction이므로 aSMN에 해당하는 Internal Node Info를 얻을 수 있다. */
        sDataSMNForMetaInfo = aSMN;
    }
    else if ( getSMNForMetaNode() < aSMN )
    {
        IDE_TEST( waitAndSetSMNForMetaNode( sStatistics,
                                            sSmiStmtForMetaInfo,
                                            ( SMI_STATEMENT_UNTOUCHABLE | SMI_STATEMENT_MEMORY_CURSOR ),
                                            aSMN,
                                            &sDataSMNForMetaInfo )
                  != IDE_SUCCESS );

        IDE_DASSERT( aSMN == sDataSMNForMetaInfo );
    }
    else
    {
        sDataSMNForMetaInfo = aSMN;
    }

    IDE_TEST( sSmiStmt.begin( sStatistics,
                              sSmiStmtForMetaInfo,
                              (SMI_STATEMENT_UNTOUCHABLE |
                               SMI_STATEMENT_MEMORY_CURSOR) )
              != IDE_SUCCESS );
    sIsStmtBegun = ID_TRUE;

    IDE_TEST( sdm::getInternalNodeInfo( &sSmiStmt,
                                        aNodeInfo,
                                        sDataSMNForMetaInfo )
              != IDE_SUCCESS );

    IDE_TEST( sSmiStmt.end( SMI_STATEMENT_RESULT_SUCCESS ) != IDE_SUCCESS );
    sIsStmtBegun = ID_FALSE;

    if ( aTrans == NULL )
    {
        IDE_TEST( sTrans.commit( &sDummySCN ) != IDE_SUCCESS );
        sTransStage = 1;

        sTransStage = 0;
        IDE_TEST( sTrans.destroy( NULL ) != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_INVALID_TRANS )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDC_UNEXPECTED_ERROR,
                                  "sdi::getInternalNodeInfo",
                                  "Invalid transaction" ) );
    }
    IDE_EXCEPTION_END;    

    if ( sIsStmtBegun == ID_TRUE )
    {
        (void)sSmiStmt.end( SMI_STATEMENT_RESULT_FAILURE );
    }
    else
    {
        /* Nothing to do */
    }

    switch ( sTransStage )
    {
        case 2:
            ( void )sTrans.rollback();
            /* fall through */
        case 1:
            ( void )sTrans.destroy( NULL );
            /* fall through */
        default:
            break;
    }

    ideLog::log( IDE_SD_0, "[SHARD META : FAILURE] errorcode 0x%05"ID_XINT32_FMT" %s\n",
                           E_ERROR_CODE(ideGetErrorCode()),
                           ideGetErrorMsg(ideGetErrorCode()));
    return IDE_FAILURE;
}

/* PROJ-2655 Composite shard key */
IDE_RC sdi::getRangeIndexByValue( qcTemplate     * aTemplate,
                                  mtcTuple       * aShardKeyTuple,
                                  sdiAnalyzeInfo * aShardAnalysis,
                                  UShort           aValueIndex,
                                  sdiValueInfo   * aValue,
                                  sdiRangeIndex  * aRangeIndex,
                                  UShort         * aRangeIndexCount,
                                  idBool         * aHasDefaultNode,
                                  idBool           aIsSubKey )
{
    IDU_FIT_POINT_FATAL( "sdi::getRangeIndexByValue::__FT__" );

    IDE_TEST( sda::getRangeIndexByValue( aTemplate,
                                         aShardKeyTuple,
                                         aShardAnalysis,
                                         aValueIndex,
                                         aValue,
                                         aRangeIndex,
                                         aRangeIndexCount,
                                         aHasDefaultNode,
                                         aIsSubKey )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdi::checkValuesSame( qcTemplate   * aTemplate,
                             mtcTuple     * aShardKeyTuple,
                             UInt           aKeyDataType,
                             sdiValueInfo * aValue1,
                             sdiValueInfo * aValue2,
                             idBool       * aIsSame )
{
    IDU_FIT_POINT_FATAL( "sdi::checkValuesSame::__FT__" );

    IDE_TEST( sda::checkValuesSame( aTemplate,
                                    aShardKeyTuple,
                                    aKeyDataType,
                                    aValue1,
                                    aValue2,
                                    aIsSame )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdi::validateNodeNames( qcStatement  * aStatement,
                               qcShardNodes * aNodeNames )
{
    qcShardNodes     * sNodeName;
    qcShardNodes     * sNodeName2;
    qcuSqlSourceInfo   sqlInfo;

    for ( sNodeName = aNodeNames;
          sNodeName != NULL;
          sNodeName = sNodeName->next )
    {
        // length 검사
        if ( ( sNodeName->namePos.size <= 0 ) ||
             ( sNodeName->namePos.size > SDI_NODE_NAME_MAX_SIZE ) )
        {
            sqlInfo.setSourceInfo( aStatement,
                                   & sNodeName->namePos );
            IDE_RAISE( ERR_INVALID_NODE_NAME );
        }
        else
        {
            // Nothing to do.
        }

        for ( sNodeName2 = sNodeName->next;
              sNodeName2 != NULL;
              sNodeName2 = sNodeName2->next )
        {
            // duplicate 검사
            if ( QC_IS_NAME_CASELESS_MATCHED( sNodeName->namePos,
                                              sNodeName2->namePos ) == ID_TRUE )
            {
                sqlInfo.setSourceInfo( aStatement,
                                       & sNodeName2->namePos );
                IDE_RAISE( ERR_DUP_NODE_NAME );
            }
            else
            {
                // Nothing to do.
            }
        }
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_INVALID_NODE_NAME )
    {
        (void)sqlInfo.init( aStatement->qmeMem );
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDI_INVALID_NODE_NAME,
                                  sqlInfo.getErrMessage() ) );
        (void)sqlInfo.fini();
    }
    IDE_EXCEPTION( ERR_DUP_NODE_NAME );
    {
        (void)sqlInfo.init( aStatement->qmeMem );
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDI_DUPLICATED_NODE_NAME,
                                  sqlInfo.getErrMessage() ) );
        (void)sqlInfo.fini();
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdi::allocAndCopyRanges( qcStatement  * aStatement,
                                sdiRangeInfo * aTo,
                                sdiRangeInfo * aFrom )
{
    IDE_TEST( sda::allocAndCopyRanges( aStatement,
                                       aTo,
                                       aFrom )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdi::allocAndCopyValues( qcStatement   * aStatement,
                                sdiValueInfo ** aTo,
                                UShort        * aToCount,
                                sdiValueInfo  * aFrom,
                                UShort          aFromCount )
{
    IDE_TEST( sda::allocAndCopyValues( aStatement,
                                       aTo,
                                       aToCount,
                                       aFrom,
                                       aFromCount )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdi::checkShardLinker( qcStatement * aStatement )
{
    ULong sSMN = ID_ULONG(0);

    if ( ( qci::getStartupPhase() == QCI_STARTUP_SERVICE ) &&
         ( aStatement->session->mQPSpecific.mClientInfo == NULL ) )
    {
        if ( sdi::isShardCoordinator( aStatement ) == ID_TRUE )
        {
            // shardCoordinator using sessionSMN 
            sSMN = QCG_GET_SESSION_SHARD_META_NUMBER( aStatement );
        }
        else // ( sdi::isRebuildCoordinator( aStatement ) == ID_TRUE )
        {
            IDE_DASSERT( sdi::isRebuildCoordinator( aStatement ) == ID_TRUE );

            // rebuildCoordinator using dataSMN
            sSMN = sdi::getSMNForDataNode();
        }

        if ( sSMN != ID_ULONG(0) )
        {
            IDE_TEST( initializeSession( aStatement->session,
                                         QCG_GET_DATABASE_LINK_SESSION( aStatement ),
                                         ( QC_SMI_STMT( aStatement ) )->getTrans(),
                                         ID_FALSE,
                                         sSMN )
                      != IDE_SUCCESS );
        }
        else
        {
            // Nothing to do.
        }
    }
    else
    {
        // Nothing to do.
    }

    IDE_TEST_RAISE( aStatement->session->mQPSpecific.mClientInfo == NULL,
                    ERR_SHARD_LINKER_NOT_INITIALIZED );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_SHARD_LINKER_NOT_INITIALIZED )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDI_SHARD_LINKER_NOT_INITIALIZED ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

sdiConnectInfo * sdi::findConnect( sdiClientInfo * aClientInfo,
                                   UInt            aNodeId )
{
    sdiConnectInfo * sConnectInfo = NULL;
    UInt             i;

    sConnectInfo = aClientInfo->mConnectInfo;

    for ( i = 0; i < aClientInfo->mCount; i++, sConnectInfo++ )
    {
        if ( sConnectInfo->mNodeId == aNodeId )
        {
            return sConnectInfo;
        }
        else
        {
            // Nothing to do.
        }
    }

    return NULL;
}

idBool sdi::findBindParameter( sdiAnalyzeInfo * aAnalyzeInfo )
{
    idBool  sFound = ID_FALSE;
    UInt    i;

    for ( i = 0; i < aAnalyzeInfo->mValueCount; i++ )
    {
        if ( aAnalyzeInfo->mValue[i].mType == 0 )
        {
            // a bind parameter exists
            sFound = ID_TRUE;
            break;
        }
        else
        {
            // Nothing to do.
        }
    }

    if ( aAnalyzeInfo->mSubKeyExists == ID_TRUE )
    {
        for ( i = 0; i < aAnalyzeInfo->mSubValueCount; i++ )
        {
            if ( aAnalyzeInfo->mSubValue[i].mType == 0 )
            {
                // a bind parameter exists
                sFound = ID_TRUE;
                break;
            }
            else
            {
                // Nothing to do.
            }
        }
    }
    else
    {
        // Nothing to do.
    }

    return sFound;
}

idBool sdi::findRangeInfo( sdiRangeInfo * aRangeInfo,
                           UInt           aNodeId )
{
    return sda::findRangeInfo( aRangeInfo,
                               aNodeId );
}

IDE_RC sdi::getProcedureInfo( qcStatement      * aStatement,
                              UInt               aUserID,
                              qcNamePosition     aUserName,
                              qcNamePosition     aProcName,
                              qsProcParseTree  * aProcPlanTree,
                              sdiObjectInfo   ** aShardObjInfo )
{
/***********************************************************************
 *
 * Description : PROJ-2598 Shard Meta
 *
 * Implementation :
 *
 ***********************************************************************/

    smiStatement      sSmiStmt;
    idBool            sIsBeginStmt = ID_FALSE;
    qsVariableItems * sParaDecls;
    SChar             sUserName[QC_MAX_OBJECT_NAME_LEN + 1];
    SChar             sProcName[QC_MAX_OBJECT_NAME_LEN + 1];
    sdiTableInfo      sShardTableInfo;
    sdiObjectInfo   * sShardObjInfo = NULL;
    idBool            sExistKey = ID_FALSE;
    idBool            sExistSubKey = ID_FALSE;
    idBool            sIsTableFound = ID_FALSE;
    UInt              sKeyType;
    UInt              sSubKeyType;
    UInt              i = 0;

    /*
     * PROJ-2701 Online data rebuild
     *
     * sSMN[0] : sessionSMN
     * sSMN[1] : dataSMN
     */
    ULong  sSMN[2] = { ID_ULONG(0), ID_ULONG(0) };
    ULong  sSMNForShardObj = ID_ULONG(0);

    if ( ( ( qciMisc::isStmtDML( aStatement->myPlan->parseTree->stmtKind ) == ID_TRUE ) ||
           ( qciMisc::isStmtSP( aStatement->myPlan->parseTree->stmtKind ) == ID_TRUE ) )  &&
         ( aUserID != QC_SYSTEM_USER_ID ) &&
         ( aUserID != getShardUserID() ) &&
         ( *aShardObjInfo == NULL ) )
    {
        IDE_DASSERT( QC_IS_NULL_NAME( aProcName ) == ID_FALSE );

        if ( QC_IS_NULL_NAME( aUserName ) == ID_TRUE )
        {
            idlOS::snprintf( sUserName, ID_SIZEOF(sUserName),
                             "%s",
                             QCG_GET_SESSION_USER_NAME( aStatement ) );
            QC_STR_COPY( sProcName, aProcName );
        }
        else
        {
            QC_STR_COPY( sUserName, aUserName );
            QC_STR_COPY( sProcName, aProcName );
        }

        // PROJ-2701 Online date rebuild
        if ( ( aStatement->session->mQPSpecific.mFlag & QC_SESSION_ALTER_META_MASK )
             == QC_SESSION_ALTER_META_DISABLE )
        {
            sSMN[0] = QCG_GET_SESSION_SHARD_META_NUMBER( aStatement );

            if ( sSMN[0] != ID_ULONG(0) )
            {
                IDE_TEST( waitAndSetSMNForMetaNode( aStatement->mStatistics,
                                                    QC_SMI_STMT( aStatement ),
                                                    ( SMI_STATEMENT_NORMAL |
                                                      SMI_STATEMENT_SELF_TRUE |
                                                      SMI_STATEMENT_ALL_CURSOR ),
                                                    ID_ULONG(0),
                                                    &sSMN[1] )
                          != IDE_SUCCESS );
            }
            else
            {
                // sSMNCount = 0;
                // Sharding을 쓰지 않음.
                // Nothing to do.
            }
        }
        else
        {
            // At shard meta creation phase ( Initial SMN )
            // Nothing to do.
        }

        // 별도의 stmt를 열어 항상 최신의 view를 본다.
        // (shard meta는 일반 memory table이므로 normal로 열되,
        // 상위 stmt가 untouchable일 수 있으므로 self를 추가한다.)
        IDE_TEST( sSmiStmt.begin( aStatement->mStatistics,
                                  QC_SMI_STMT( aStatement ),
                                  SMI_STATEMENT_NORMAL |
                                  SMI_STATEMENT_SELF_TRUE |
                                  SMI_STATEMENT_ALL_CURSOR )
                  != IDE_SUCCESS );
        sIsBeginStmt = ID_TRUE;

        // SessionSMN ~ dataSMN 사이의 shardObjInfo를 생성한다.
        for ( sSMNForShardObj = sSMN[0]; sSMNForShardObj <= sSMN[1]; sSMNForShardObj++ )
        {
            if ( sdm::getTableInfo( &sSmiStmt,
                                    sUserName,
                                    sProcName,
                                    sSMNForShardObj,
                                    &sShardTableInfo,
                                    &sIsTableFound ) == IDE_SUCCESS )
            {
                if ( sIsTableFound == ID_TRUE )
                {
                    if ( ( sShardTableInfo.mObjectType == 'P' ) &&
                         ( sShardTableInfo.mSplitMethod != SDI_SPLIT_NONE ) )
                    {
                        IDE_TEST_RAISE( ( sShardTableInfo.mSplitMethod != SDI_SPLIT_CLONE ) &&
                                        ( sShardTableInfo.mSplitMethod != SDI_SPLIT_SOLO ) &&
                                        ( sShardTableInfo.mKeyColumnName[0] == '\0' ),
                                        ERR_NO_SHARD_KEY_COLUMN );

                        // keyFlags를 추가생성한다.
                        IDE_TEST( QC_QMP_MEM(aStatement)->alloc(
                                      ID_SIZEOF( sdiObjectInfo ) + aProcPlanTree->paraDeclCount,
                                      (void**) & sShardObjInfo )
                                  != IDE_SUCCESS );

                        // set shard table info
                        idlOS::memcpy( (void*)&(sShardObjInfo->mTableInfo),
                                       (void*)&sShardTableInfo,
                                       ID_SIZEOF( sdiTableInfo ) );

                        // set key flags
                        idlOS::memset( sShardObjInfo->mKeyFlags, 0x00, aProcPlanTree->paraDeclCount );

                        // set SMN
                        sShardObjInfo->mSMN = sSMNForShardObj;

                        if ( ( sShardTableInfo.mSplitMethod != SDI_SPLIT_CLONE ) &&
                             ( sShardTableInfo.mSplitMethod != SDI_SPLIT_SOLO ) )
                        {
                            // Hash, Range or List-based sharding 의 shard key column을 찾는다.
                            for ( sParaDecls = aProcPlanTree->paraDecls, i = 0;
                                  sParaDecls != NULL;
                                  sParaDecls = sParaDecls->next, i++ )
                            {
                                if ( idlOS::strMatch( sParaDecls->name.stmtText + sParaDecls->name.offset,
                                                      sParaDecls->name.size,
                                                      sShardTableInfo.mKeyColumnName,
                                                      idlOS::strlen(sShardTableInfo.mKeyColumnName) ) == 0 )
                                {
                                    IDE_TEST_RAISE( sParaDecls->itemType != QS_VARIABLE,
                                                    ERR_INVALID_SHARD_KEY_TYPE );
                                    IDE_TEST_RAISE( ((qsVariables*)sParaDecls)->inOutType != QS_IN,
                                                    ERR_INVALID_SHARD_KEY_TYPE );

                                    sKeyType = aProcPlanTree->paramModules[i]->id;
                                    IDE_TEST_RAISE( isSupportDataType( sKeyType ) == ID_FALSE,
                                                    ERR_INVALID_SHARD_KEY_TYPE );

                                    sShardObjInfo->mTableInfo.mKeyDataType = sKeyType;
                                    sShardObjInfo->mTableInfo.mKeyColOrder = (UShort)i;
                                    sShardObjInfo->mKeyFlags[i] = 1;
                                    sExistKey = ID_TRUE;


                                    if ( ( sShardTableInfo.mSubKeyExists == ID_FALSE ) ||
                                         ( sExistSubKey == ID_TRUE ) )
                                    {
                                        break;
                                    }
                                    else
                                    {
                                        // Nothing to do.
                                    }
                                }
                                else
                                {
                                    // Nothing to do.
                                }

                                /* PROJ-2655 Composite shard key */
                                if ( sShardTableInfo.mSubKeyExists == ID_TRUE )
                                {
                                    if ( idlOS::strMatch( sParaDecls->name.stmtText + sParaDecls->name.offset,
                                                          sParaDecls->name.size,
                                                          sShardTableInfo.mSubKeyColumnName,
                                                          idlOS::strlen(sShardTableInfo.mSubKeyColumnName) ) == 0 )
                                    {
                                        sExistSubKey = ID_TRUE;
                                        IDE_TEST_RAISE( sParaDecls->itemType != QS_VARIABLE,
                                                        ERR_INVALID_SHARD_KEY_TYPE );
                                        IDE_TEST_RAISE( ((qsVariables*)sParaDecls)->inOutType != QS_IN,
                                                        ERR_INVALID_SHARD_KEY_TYPE );

                                        sSubKeyType = aProcPlanTree->paramModules[i]->id;
                                        IDE_TEST_RAISE( isSupportDataType( sSubKeyType ) == ID_FALSE,
                                                        ERR_INVALID_SHARD_KEY_TYPE );

                                        sShardObjInfo->mTableInfo.mSubKeyDataType = sSubKeyType;
                                        sShardObjInfo->mTableInfo.mSubKeyColOrder = (UShort)i;
                                        sShardObjInfo->mKeyFlags[i] = 2;

                                        if ( sExistKey == ID_TRUE )
                                        {
                                            break;
                                        }
                                        else
                                        {
                                            // Nothing to do.
                                        }
                                    }
                                    else
                                    {
                                        // Nothing to do.
                                    }
                                }
                                else
                                {
                                    // Nothing to do.
                                }
                            }

                            IDE_TEST_RAISE( sExistKey == ID_FALSE, ERR_NOT_EXIST_SHARD_KEY );

                            IDE_TEST_RAISE( ( ( sShardTableInfo.mSubKeyExists == ID_TRUE ) &&
                                              ( sExistSubKey == ID_FALSE ) ),
                                            ERR_NOT_EXIST_SUB_SHARD_KEY );
                        }
                        else
                        {
                            // Nothing to do.
                        }

                        // set shard range info
                        IDE_TEST( sdm::getRangeInfo( aStatement,
                                                     &sSmiStmt,
                                                     sShardObjInfo->mSMN,
                                                     &(sShardObjInfo->mTableInfo),
                                                     &(sShardObjInfo->mRangeInfo) )
                                  != IDE_SUCCESS );

                        if ( ( sShardObjInfo->mSMN == sSMN[0] ) ||
                             ( sShardObjInfo->mSMN == sSMN[1] ) )
                        {
                            // default node도 없고 range 정보도 없다면 에러
                            IDE_TEST_RAISE(
                                ( sShardObjInfo->mTableInfo.mDefaultNodeId == ID_UINT_MAX ) &&
                                ( sShardObjInfo->mRangeInfo.mCount == 0 ),
                                ERR_INCOMPLETE_RANGE_SET );
                        }
                        else
                        {
                            // Nothing to do.
                        }

                        sShardObjInfo->mNext = *aShardObjInfo;
                        *aShardObjInfo = sShardObjInfo;
                    }
                    else
                    {
                        // Nothing to do.
                    }
                }
                else
                {
                    // Nothing to do.
                }
            }
        }

        sIsBeginStmt = ID_FALSE;
        IDE_TEST( sSmiStmt.end( SMI_STATEMENT_RESULT_SUCCESS ) != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_INCOMPLETE_RANGE_SET )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDI_INCOMPLETE_RANGE_SET,
                                  sUserName,
                                  sProcName ) );
    }
    IDE_EXCEPTION( ERR_NOT_EXIST_SHARD_KEY )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_SHARD_KEY_COLUMN_NOT_EXIST,
                                  sUserName,
                                  sProcName,
                                  sShardTableInfo.mKeyColumnName ) );
    }
    IDE_EXCEPTION( ERR_NOT_EXIST_SUB_SHARD_KEY )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_SHARD_KEY_COLUMN_NOT_EXIST,
                                  sUserName,
                                  sProcName,
                                  sShardTableInfo.mSubKeyColumnName ) );
    }
    IDE_EXCEPTION( ERR_INVALID_SHARD_KEY_TYPE )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_UNSUPPORTED_SHARD_KEY_COLUMN_TYPE,
                                  sUserName,
                                  sProcName,
                                  ( sExistSubKey == ID_TRUE )? sShardTableInfo.mSubKeyColumnName:
                                                               sShardTableInfo.mKeyColumnName ) );
    }
    IDE_EXCEPTION( ERR_NO_SHARD_KEY_COLUMN )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDC_UNEXPECTED_ERROR,
                                  "sdi::getProcedureInfo",
                                  "There is no shard key column for the shard procedure" ) );
    }
    IDE_EXCEPTION_END;

    if ( sIsBeginStmt == ID_TRUE )
    {
        (void)sSmiStmt.end( SMI_STATEMENT_RESULT_FAILURE );
    }
    else
    {
        /* Nothing to do */
    }

    return IDE_FAILURE;
}

IDE_RC sdi::getTableInfo( qcStatement    * aStatement,
                          qcmTableInfo   * aTableInfo,
                          sdiObjectInfo ** aShardObjInfo )
{
/***********************************************************************
 *
 * Description : PROJ-2598 Shard Meta
 *
 * Implementation :
 *
 ***********************************************************************/

    smiStatement     sSmiStmt;
    idBool           sIsBeginStmt = ID_FALSE;
    sdiTableInfo     sShardTableInfo;
    sdiObjectInfo  * sShardObjInfo = NULL;
    idBool           sExistKey = ID_FALSE;
    idBool           sExistSubKey = ID_FALSE;
    idBool           sIsTableFound = ID_FALSE;
    UInt             sKeyType;
    UInt             sSubKeyType;
    UInt             i = 0;

    /*
     * PROJ-2701 Online data rebuild
     *
     * sSMN[0] : sessionSMN
     * sSMN[1] : dataSMN
     */
    ULong  sSMN[2] = { ID_ULONG(0), ID_ULONG(0) };
    ULong  sSMNForShardObj = ID_ULONG(0);

    if ( ( ( qciMisc::isStmtDML( aStatement->myPlan->parseTree->stmtKind ) == ID_TRUE ) ||
           ( qciMisc::isStmtSP( aStatement->myPlan->parseTree->stmtKind ) == ID_TRUE ) )  &&
         ( aTableInfo->tableOwnerID != QC_SYSTEM_USER_ID ) &&
         ( aTableInfo->tableOwnerID != getShardUserID() ) &&
         ( aTableInfo->tableType == QCM_USER_TABLE ) &&
         ( *aShardObjInfo == NULL ) )
    {
        // PROJ-2701 Online date rebuild
        if ( ( aStatement->session->mQPSpecific.mFlag & QC_SESSION_ALTER_META_MASK )
             == QC_SESSION_ALTER_META_DISABLE )
        {
            sSMN[0] = QCG_GET_SESSION_SHARD_META_NUMBER( aStatement );

            if ( sSMN[0] != ID_ULONG(0) )
            {
                IDE_TEST( waitAndSetSMNForMetaNode( aStatement->mStatistics,
                                                    QC_SMI_STMT( aStatement ),
                                                    ( SMI_STATEMENT_NORMAL |
                                                      SMI_STATEMENT_SELF_TRUE |
                                                      SMI_STATEMENT_ALL_CURSOR ),
                                                    ID_ULONG(0),
                                                    &sSMN[1] )
                          != IDE_SUCCESS );
            }
            else
            {
                // sSMNCount = 0;
                // Sharding을 쓰지 않음.
                // Nothing to do.
            }
        }
        else
        {
            // Nothing to do. ( Initial SMN for shard meta creation )
            // sMetaNodeInfo = { ID_ULONG(0) };
        }
        
        // 별도의 stmt를 열어 항상 최신의 view를 본다.
        // (shard meta는 일반 memory table이므로 normal로 열되,
        // 상위 stmt가 untouchable일 수 있으므로 self를 추가한다.)
        IDE_TEST( sSmiStmt.begin( aStatement->mStatistics,
                                  QC_SMI_STMT( aStatement ),
                                  SMI_STATEMENT_NORMAL |
                                  SMI_STATEMENT_SELF_TRUE |
                                  SMI_STATEMENT_ALL_CURSOR )
                  != IDE_SUCCESS );
        sIsBeginStmt = ID_TRUE;

        // SessionSMN ~ dataSMN 사이의 shardObjInfo를 생성한다.
        for ( sSMNForShardObj = sSMN[0]; sSMNForShardObj <= sSMN[1]; sSMNForShardObj++ )
        {
            if ( sdm::getTableInfo( &sSmiStmt,
                                    aTableInfo->tableOwnerName,
                                    aTableInfo->name,
                                    sSMNForShardObj,
                                    &sShardTableInfo,
                                    &sIsTableFound ) == IDE_SUCCESS )
            {
                if ( sIsTableFound == ID_TRUE )
                {
                    if ( ( sShardTableInfo.mObjectType == 'T' ) &&
                         ( sShardTableInfo.mSplitMethod != SDI_SPLIT_NONE ) )
                    {
                        IDE_TEST_RAISE( ( sShardTableInfo.mSplitMethod != SDI_SPLIT_CLONE ) &&
                                        ( sShardTableInfo.mSplitMethod != SDI_SPLIT_SOLO ) &&
                                        ( sShardTableInfo.mKeyColumnName[0] == '\0' ),
                                        ERR_NO_SHARD_KEY_COLUMN );

                        // keyFlags를 추가생성한다.
                        IDE_TEST( QC_QMP_MEM(aStatement)->alloc(
                                      ID_SIZEOF( sdiObjectInfo ) + aTableInfo->columnCount,
                                      (void**) & sShardObjInfo )
                                  != IDE_SUCCESS );

                        // set shard table info
                        idlOS::memcpy( (void*)&(sShardObjInfo->mTableInfo),
                                       (void*)&sShardTableInfo,
                                       ID_SIZEOF( sdiTableInfo ) );

                        // set key flags
                        idlOS::memset( sShardObjInfo->mKeyFlags, 0x00, aTableInfo->columnCount );

                        // set SMN
                        sShardObjInfo->mSMN = sSMNForShardObj;

                        if ( ( sShardTableInfo.mSplitMethod != SDI_SPLIT_CLONE ) &&
                             ( sShardTableInfo.mSplitMethod != SDI_SPLIT_SOLO ) )
                        {
                            // Hash, Range or List-based sharding 의 shard key column을 찾는다.
                            for ( i = 0; i < aTableInfo->columnCount; i++ )
                            {
                                if ( idlOS::strMatch( aTableInfo->columns[i].name,
                                                      idlOS::strlen(aTableInfo->columns[i].name),
                                                      sShardTableInfo.mKeyColumnName,
                                                      idlOS::strlen(sShardTableInfo.mKeyColumnName) ) == 0 )
                                {
                                    sKeyType = aTableInfo->columns[i].basicInfo->module->id;
                                    IDE_TEST_RAISE( isSupportDataType( sKeyType ) == ID_FALSE,
                                                    ERR_INVALID_SHARD_KEY_TYPE );

                                    sShardObjInfo->mTableInfo.mKeyDataType = sKeyType;
                                    sShardObjInfo->mTableInfo.mKeyColOrder = (UShort)i;
                                    sShardObjInfo->mKeyFlags[i] = 1;
                                    sExistKey = ID_TRUE;

                                    if ( ( sShardTableInfo.mSubKeyExists == ID_FALSE ) ||
                                         ( sExistSubKey == ID_TRUE ) )
                                    {
                                        break;
                                    }
                                    else
                                    {
                                        // Nothing to do.
                                    }
                                }
                                else
                                {
                                    // Nothing to do.
                                }

                                /* PROJ-2655 Composite shard key */
                                if ( sShardTableInfo.mSubKeyExists == ID_TRUE )
                                {   
                                    if ( idlOS::strMatch( aTableInfo->columns[i].name,
                                                          idlOS::strlen(aTableInfo->columns[i].name),
                                                          sShardTableInfo.mSubKeyColumnName,
                                                          idlOS::strlen(sShardTableInfo.mSubKeyColumnName) ) == 0 )
                                    {   
                                        sExistSubKey = ID_TRUE;

                                        sSubKeyType = aTableInfo->columns[i].basicInfo->module->id;
                                        IDE_TEST_RAISE( isSupportDataType( sSubKeyType ) == ID_FALSE,
                                                        ERR_INVALID_SHARD_KEY_TYPE );

                                        sShardObjInfo->mTableInfo.mSubKeyDataType = sSubKeyType;
                                        sShardObjInfo->mTableInfo.mSubKeyColOrder = (UShort)i;
                                        sShardObjInfo->mKeyFlags[i] = 2;

                                        if ( sExistKey == ID_TRUE )
                                        {
                                            break;
                                        }
                                        else
                                        {
                                            // Nothing to do.
                                        }
                                    }
                                    else
                                    {
                                        // Nothing to do.
                                    }
                                }
                                else
                                {
                                    // Nothing to do.
                                }
                            }

                            IDE_TEST_RAISE( sExistKey == ID_FALSE, ERR_NOT_EXIST_SHARD_KEY );

                            IDE_TEST_RAISE( ( ( sShardTableInfo.mSubKeyExists == ID_TRUE ) &&
                                              ( sExistSubKey == ID_FALSE ) ),
                                            ERR_NOT_EXIST_SUB_SHARD_KEY );
                        }
                        else
                        {
                            // Nothing to do.
                        }

                        // set shard range info
                        IDE_TEST( sdm::getRangeInfo( aStatement,
                                                     &sSmiStmt,
                                                     sShardObjInfo->mSMN,
                                                     &(sShardObjInfo->mTableInfo),
                                                     &(sShardObjInfo->mRangeInfo) )
                                  != IDE_SUCCESS );

                        if ( ( sShardObjInfo->mSMN == sSMN[0] ) ||
                             ( sShardObjInfo->mSMN == sSMN[1] ) )
                        {
                            // default node도 없고 range 정보도 없다면 에러
                            IDE_TEST_RAISE(
                                ( sShardObjInfo->mTableInfo.mDefaultNodeId == ID_UINT_MAX ) &&
                                ( sShardObjInfo->mRangeInfo.mCount == 0 ),
                                ERR_INCOMPLETE_RANGE_SET );
                        }
                        else
                        {
                            // Nothing to do.
                        }

                        sShardObjInfo->mNext = *aShardObjInfo;
                        *aShardObjInfo = sShardObjInfo;
                    }
                    else
                    {
                        // Nothing to do.
                    }
                }
                else
                {
                    // Nothing to do.
                }
            }
        }

        sIsBeginStmt = ID_FALSE;
        IDE_TEST( sSmiStmt.end( SMI_STATEMENT_RESULT_SUCCESS ) != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_INCOMPLETE_RANGE_SET )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDI_INCOMPLETE_RANGE_SET,
                                  aTableInfo->tableOwnerName,
                                  aTableInfo->name ) );
    }
    IDE_EXCEPTION( ERR_NOT_EXIST_SHARD_KEY )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_SHARD_KEY_COLUMN_NOT_EXIST,
                                  aTableInfo->tableOwnerName,
                                  aTableInfo->name,
                                  sShardTableInfo.mKeyColumnName ) );
    }
    IDE_EXCEPTION( ERR_NOT_EXIST_SUB_SHARD_KEY )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_SHARD_KEY_COLUMN_NOT_EXIST,
                                  aTableInfo->tableOwnerName,
                                  aTableInfo->name,
                                  sShardTableInfo.mSubKeyColumnName ) );
    }
    IDE_EXCEPTION( ERR_INVALID_SHARD_KEY_TYPE )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_UNSUPPORTED_SHARD_KEY_COLUMN_TYPE,
                                  aTableInfo->tableOwnerName,
                                  aTableInfo->name,
                                  ( sExistSubKey == ID_TRUE )? sShardTableInfo.mSubKeyColumnName:
                                                               sShardTableInfo.mKeyColumnName ) );
    }
    IDE_EXCEPTION( ERR_NO_SHARD_KEY_COLUMN )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDC_UNEXPECTED_ERROR,
                                  "sdi::getTableInfo",
                                  "There is no shard key column for the shard table" ) );
    }
    IDE_EXCEPTION_END;

    if ( sIsBeginStmt == ID_TRUE )
    {
        (void)sSmiStmt.end( SMI_STATEMENT_RESULT_FAILURE );
    }
    else
    {
        /* Nothing to do */
    }

    return IDE_FAILURE;
}

// PROJ-2638
void sdi::initOdbcLibrary()
{
    (void)sdl::initOdbcLibrary();
}

void sdi::finiOdbcLibrary()
{
    sdl::initOdbcLibrary();
}

IDE_RC sdi::initializeSession( qcSession * aSession,
                               void      * aDkiSession,
                               smiTrans  * aTrans,
                               idBool      aIsShardMetaChanged,
                               ULong       aConnectSMN )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation : set connect info for data node
 *
 ***********************************************************************/

    sdiNodeInfo       sNodeInfo;
    sdiNodeInfo       sNodeInfo4BeforeSMN;
    sdiClientInfo   * sClientInfo       = NULL;
    sdiConnectInfo  * sConnectInfo      = NULL;
    sdiNode         * sDataNode         = NULL;
    static SChar      sNullName[1]      = { '\0' };
    SChar           * sUserName         = NULL;
    static SChar      sNullPassword[1]  = { '\0' };
    SChar           * sUserPassword     = NULL;
    UInt              sPasswordLen      = 0;
    sdiShardPin       sShardPin         = SDI_SHARD_PIN_INVALID;
    UInt              sDBLinkGTXLevel   = 0;
    UInt              sIsShardClient    = ID_FALSE;
    idBool            sIsUserAutoCommit = ID_TRUE;
    idBool            sIsMetaAutoCommit = ID_TRUE;
    UChar             sPlanAttr         = (UChar)ID_FALSE;
    UShort            i                 = 0;

    IDE_DASSERT( aSession->mQPSpecific.mClientInfo == NULL );

    if ( aSession->mMmSession != NULL )
    {
        sUserName         = qci::mSessionCallback.mGetUserName( aSession->mMmSession );
        sUserPassword     = qci::mSessionCallback.mGetUserPassword( aSession->mMmSession );
        sDBLinkGTXLevel   = qci::mSessionCallback.mGetDBLinkGTXLevel( aSession->mMmSession );
        sShardPin         = qci::mSessionCallback.mGetShardPIN( aSession->mMmSession );
        sIsShardClient    = qci::mSessionCallback.mIsShardClient( aSession->mMmSession );
        sPlanAttr         = qci::mSessionCallback.mGetExplainPlan( aSession->mMmSession );
        sIsUserAutoCommit = qci::mSessionCallback.mIsAutoCommit( aSession->mMmSession );
        sIsMetaAutoCommit = sIsUserAutoCommit;
    }
    else
    {
        sUserName         = sNullName;
        sUserPassword     = sNullPassword;
        sPlanAttr         = (UChar)ID_FALSE;
        sIsUserAutoCommit = ID_TRUE;
        sIsMetaAutoCommit = sIsUserAutoCommit;

        IDE_DASSERT( 0 );
    }

    // Get nodeInfo for internal session connectSMN
    IDE_TEST( getInternalNodeInfo( aTrans,
                                   &sNodeInfo,
                                   aIsShardMetaChanged,
                                   aConnectSMN )
              != IDE_SUCCESS );

    if ( aIsShardMetaChanged == ID_TRUE )
    {
        // Get nodeInfo for oldSMN
        IDE_TEST( getInternalNodeInfo( aTrans,
                                       &sNodeInfo4BeforeSMN,
                                       ID_FALSE,
                                       aConnectSMN - 1 )
                  != IDE_SUCCESS );

        // Make nodeInfo for oldSMN union newSMN
        IDE_TEST( unionNodeInfo( &sNodeInfo4BeforeSMN,
                                 &sNodeInfo )
                  != IDE_SUCCESS );
    }
    else
    {
        /* Nothing to do. */
    }

    if ( sNodeInfo.mCount != 0 )
    {
        IDE_TEST( iduMemMgr::calloc( IDU_MEM_QSN,  // BUGBUG check memory
                                     1,
                                     ID_SIZEOF(sdiClientInfo) +
                                     ID_SIZEOF(sdiConnectInfo) * sNodeInfo.mCount,
                                     (void **)&sClientInfo )
                  != IDE_SUCCESS );

        /* BUG-45967 Data Node의 Shard Session 정리 */
        sClientInfo->mCount         = sNodeInfo.mCount;

        /* BUG-46100 Session SMN Update */
        sClientInfo->mNeedToDisconnect = ID_FALSE;

        sConnectInfo = sClientInfo->mConnectInfo;
        sDataNode = sNodeInfo.mNodes;

        for ( i = 0; i < sNodeInfo.mCount; i++, sConnectInfo++, sDataNode++ )
        {
            // qcSession
            sConnectInfo->mSession = aSession;
            // dkiSession
            sConnectInfo->mDkiSession       = aDkiSession;
            sConnectInfo->mShardPin         = sShardPin;
            sConnectInfo->mIsShardClient    = (sdiShardClient)sIsShardClient;
            sConnectInfo->mShardMetaNumber  = aConnectSMN;
            sConnectInfo->mFailoverTarget   = SDI_FAILOVER_NOT_USED;

            sdl::clearInternalConnectResult( sConnectInfo );

            sdi::initAffectedRow( sConnectInfo );

            idlOS::memcpy( &(sConnectInfo->mNodeInfo),
                           sDataNode,
                           ID_SIZEOF( sdiNode ) );

            if ( SDU_SHARD_TEST_ENABLE == 0 )
            {
                idlOS::strncpy( sConnectInfo->mUserName,
                                sUserName,
                                QCI_MAX_OBJECT_NAME_LEN );
                sConnectInfo->mUserName[ QCI_MAX_OBJECT_NAME_LEN ] = '\0';
                idlOS::strncpy( sConnectInfo->mUserPassword,
                                sUserPassword,
                                IDS_MAX_PASSWORD_LEN );
                sConnectInfo->mUserPassword[ IDS_MAX_PASSWORD_LEN ] = '\0';
            }
            else
            {
                idlOS::strncpy( sConnectInfo->mUserName,
                                sDataNode->mNodeName,
                                SDI_NODE_NAME_MAX_SIZE );
                sConnectInfo->mUserName[ SDI_NODE_NAME_MAX_SIZE ] = '\0';
                idlOS::strncpy( sConnectInfo->mUserPassword,
                                sDataNode->mNodeName,
                                SDI_NODE_NAME_MAX_SIZE );
                sConnectInfo->mUserPassword[ SDI_NODE_NAME_MAX_SIZE ] = '\0';

                sPasswordLen = idlOS::strlen( sConnectInfo->mUserPassword );
                sdi::charXOR( sConnectInfo->mUserPassword, sPasswordLen );
            }

            sConnectInfo->mConnectType = sDataNode->mConnectType;

            // node id와 node name은 미리 설정한다.
            sConnectInfo->mNodeId = sDataNode->mNodeId;
            idlOS::strncpy( sConnectInfo->mNodeName,
                            sDataNode->mNodeName,
                            SDI_NODE_NAME_MAX_SIZE );
            sConnectInfo->mNodeName[ SDI_NODE_NAME_MAX_SIZE ] = '\0';

            // connect를 위한 세션정보
            sConnectInfo->mMessageCallback.mFunction = sdi::printMessage;
            sConnectInfo->mMessageCallback.mArgument = sConnectInfo;

            sConnectInfo->mPlanAttr = sPlanAttr;

            // Two Phase Commit인 경우, Meta Connection은 Non-Autocommit이어야 한다.
            if ( ( sDBLinkGTXLevel == 2 ) && ( sIsMetaAutoCommit == ID_TRUE ) )
            {
                sIsMetaAutoCommit = ID_FALSE;
            }
            else
            {
                /* Nothing to do */
            }

            sConnectInfo->mFlag &= ~SDI_CONNECT_USER_AUTOCOMMIT_MODE_MASK;
            sConnectInfo->mFlag |= ( ( sIsUserAutoCommit == ID_TRUE ) ?
                                     SDI_CONNECT_USER_AUTOCOMMIT_MODE_ON :
                                     SDI_CONNECT_USER_AUTOCOMMIT_MODE_OFF );

            sConnectInfo->mFlag &= ~SDI_CONNECT_COORD_AUTOCOMMIT_MODE_MASK;
            sConnectInfo->mFlag |= ( ( sIsMetaAutoCommit == ID_TRUE ) ?
                                     SDI_CONNECT_COORD_AUTOCOMMIT_MODE_ON :
                                     SDI_CONNECT_COORD_AUTOCOMMIT_MODE_OFF );
            // xid
            xidInitialize( sConnectInfo );
        }
    }
    else
    {
        // Nothing to do.
    }

    aSession->mQPSpecific.mClientInfo = sClientInfo;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    if ( sClientInfo != NULL )
    {
        (void)iduMemMgr::free( sClientInfo );
    }
    else
    {
        // Nothing to do.
    }

    return IDE_FAILURE;
}

void sdi::finalizeSession( qcSession * aSession )
{
/***********************************************************************
 *
 * Description :
 *
 * Implementation : disconnect & freeConnect for data node
 *
 ***********************************************************************/

    sdiClientInfo  * sClientInfo = NULL;
    sdiConnectInfo * sConnectInfo = NULL;
    UShort           i = 0;

    IDE_DASSERT( aSession != NULL );

    sClientInfo = aSession->mQPSpecific.mClientInfo;

    if ( sClientInfo != NULL )
    {
        sConnectInfo = sClientInfo->mConnectInfo;

        for ( i = 0; i < sClientInfo->mCount; i++, sConnectInfo++ )
        {
            /* Close shard connection. */
            qdkCloseShardConnection( sConnectInfo ); 
        }

        (void)iduMemMgr::free( sClientInfo );
        aSession->mQPSpecific.mClientInfo = NULL;
    }
    else
    {
        // Nothing to do.
    }
}

IDE_RC sdi::allocConnect( sdiConnectInfo * aConnectInfo )
{
    if ( aConnectInfo->mDbc == NULL )
    {
        IDE_TEST( sdl::allocConnect( aConnectInfo ) != IDE_SUCCESS );

        aConnectInfo->mFlag &= ~SDI_CONNECT_PLANATTR_CHANGE_MASK;
        aConnectInfo->mFlag |= SDI_CONNECT_PLANATTR_CHANGE_FALSE;

        // xid info 초기화
        xidInitialize( aConnectInfo );
    }
    else
    {
        // 이전 connection을 재사용하는 경우
        if ( ( aConnectInfo->mFlag & SDI_CONNECT_TRANSACTION_END_MASK )
             == SDI_CONNECT_TRANSACTION_END_TRUE )
        {
            aConnectInfo->mFlag &= ~SDI_CONNECT_TRANSACTION_END_MASK;
            aConnectInfo->mFlag |= SDI_CONNECT_TRANSACTION_END_FALSE;
        }
        else
        {
            // Nothing to do.
        }
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    freeConnectImmediately( aConnectInfo );

    return IDE_FAILURE;
}

void sdi::freeConnectImmediately( sdiConnectInfo * aConnectInfo )
{
    if ( aConnectInfo->mDbc != NULL )
    {
        (void)sdl::disconnectLocal( aConnectInfo );

        if ( aConnectInfo->mSession != NULL )
        {
            if ( aConnectInfo->mSession->mMmSession != NULL )
            {
                qci::mSessionCallback.mFreeShardStmt( aConnectInfo->mSession->mMmSession,
                                                      aConnectInfo->mNodeInfo.mNodeId,
                                                      SQL_DROP );
            }
        }

        (void)sdl::freeConnect( aConnectInfo );

        aConnectInfo->mDbc = NULL;
        aConnectInfo->mLinkFailure = ID_FALSE;
    }
    else
    {
        // Nothing to do.
    }
}

IDE_RC sdi::checkNode( sdiConnectInfo * aConnectInfo )
{
    IDE_TEST_RAISE( aConnectInfo->mDbc == NULL, ERR_NULL_DBC );

    IDE_TEST( sdl::checkNode( aConnectInfo,
                              &(aConnectInfo->mLinkFailure) )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_NULL_DBC )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_EXECUTE_NULL_DBC,
                                  aConnectInfo->mNodeName,
                                  "checkNode" ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

idBool sdi::checkNeedFailover( sdiConnectInfo * aConnectInfo )
{
    idBool  sNeedFailover;

    IDE_TEST_CONT( aConnectInfo->mDbc == NULL, SET_FALSE );

    IDE_TEST_CONT( sdl::checkNeedFailover( aConnectInfo,
                                           &sNeedFailover )
                   != IDE_SUCCESS, SET_FALSE );

    return sNeedFailover;

    IDE_EXCEPTION_CONT( SET_FALSE );

    return ID_FALSE;
}

IDE_RC sdi::endPendingTran( sdiConnectInfo * aConnectInfo,
                            idBool           aIsCommit )
{
    IDE_TEST_RAISE( aConnectInfo->mDbc == NULL, ERR_NULL_DBC );

    IDE_TEST( sdl::endPendingTran( aConnectInfo,
                                   &(aConnectInfo->mXID),
                                   aIsCommit,
                                   &(aConnectInfo->mLinkFailure) )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_NULL_DBC )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_EXECUTE_NULL_DBC,
                                  aConnectInfo->mNodeName,
                                  "commit pending" ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdi::commit( sdiConnectInfo * aConnectInfo )
{
    IDE_TEST_RAISE( aConnectInfo->mDbc == NULL, ERR_NULL_DBC );

    IDE_TEST( sdl::commit( aConnectInfo,
                           aConnectInfo->mSession->mQPSpecific.mClientInfo,
                           &(aConnectInfo->mLinkFailure) )
              != IDE_SUCCESS );

    aConnectInfo->mTouchCount = 0;
    aConnectInfo->mFlag &= ~SDI_CONNECT_TRANSACTION_END_MASK;
    aConnectInfo->mFlag |= SDI_CONNECT_TRANSACTION_END_TRUE;

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_NULL_DBC )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_EXECUTE_NULL_DBC,
                                  aConnectInfo->mNodeName,
                                  "commit" ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdi::rollback( sdiConnectInfo * aConnectInfo,
                      const SChar    * aSavepoint )
{
    sdlRemoteStmt sRemoteStmt;

    sdlStatementManager::initRemoteStmt( &sRemoteStmt );

    IDE_TEST_RAISE( aConnectInfo->mDbc == NULL, ERR_NULL_DBC );

    IDE_TEST( sdl::rollback( aConnectInfo,
                             aSavepoint,
                             aConnectInfo->mSession->mQPSpecific.mClientInfo,
                             &(aConnectInfo->mLinkFailure) )
              != IDE_SUCCESS );

        aConnectInfo->mTouchCount = 0;
        aConnectInfo->mFlag &= ~SDI_CONNECT_TRANSACTION_END_MASK;
        aConnectInfo->mFlag |= SDI_CONNECT_TRANSACTION_END_TRUE;

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_NULL_DBC )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_EXECUTE_NULL_DBC,
                                  aConnectInfo->mNodeName,
                                  "rollback" ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdi::savepoint( sdiConnectInfo * aConnectInfo,
                       const SChar    * aSavepoint )
{
    IDE_TEST_RAISE( aConnectInfo->mDbc == NULL, ERR_NULL_DBC );

    IDE_TEST( sdl::setSavepoint( aConnectInfo,
                                 aSavepoint,
                                 &(aConnectInfo->mLinkFailure) )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_NULL_DBC )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_EXECUTE_NULL_DBC,
                                  aConnectInfo->mNodeName,
                                  "savepoint" ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

void sdi::setExplainPlanAttr( qcSession * aSession,
                              UChar       aValue )
{
    sdiClientInfo    * sClientInfo = NULL;
    sdiConnectInfo   * sConnectInfo = NULL;
    UShort             i = 0;

    IDE_DASSERT( aSession != NULL );

    sClientInfo = aSession->mQPSpecific.mClientInfo;

    if ( sClientInfo != NULL )
    {
        sConnectInfo = sClientInfo->mConnectInfo;

        for ( i = 0; i < sClientInfo->mCount; i++, sConnectInfo++ )
        {
            if ( sConnectInfo->mPlanAttr != aValue )
            {
                sConnectInfo->mPlanAttr = aValue;

                sConnectInfo->mFlag &= ~SDI_CONNECT_PLANATTR_CHANGE_MASK;
                sConnectInfo->mFlag |= SDI_CONNECT_PLANATTR_CHANGE_TRUE;
            }
            else
            {
                // Nothing to do.
            }
        }
    }
    else
    {
        // Nothing to do.
    }
}

IDE_RC sdi::shardExecDirect( qcStatement * aStatement,
                             SChar       * aNodeName,
                             SChar       * aQuery,
                             UInt          aQueryLen,
                             UInt        * aExecCount )
{
    sdiStatement   * sSdStmt      = NULL;
    sdlRemoteStmt  * sRemoteStmt  = NULL;
    sdiClientInfo  * sClientInfo  = NULL;
    sdiConnectInfo * sConnectInfo = NULL;
    UShort           i = 0;
    idBool           sSuccess = ID_TRUE;

    IDE_DASSERT( aStatement->session != NULL );

    sClientInfo = aStatement->session->mQPSpecific.mClientInfo;

    *aExecCount = 0;

    sSdStmt = (sdiStatement *)qci::mSessionCallback.mGetShardStmt( QC_MM_STMT( aStatement ) );

    if ( sClientInfo != NULL )
    {
        sConnectInfo = sClientInfo->mConnectInfo;

        for ( i = 0; i < sClientInfo->mCount; i++, sConnectInfo++ )
        {
            if ( aNodeName != NULL )
            {
                if ( ( idlOS::strlen( aNodeName ) > 0 ) &&
                     ( idlOS::strMatch( aNodeName,
                                        idlOS::strlen( aNodeName ),
                                        sConnectInfo->mNodeName,
                                        idlOS::strlen( sConnectInfo->mNodeName ) ) )
                     != 0 )
                {
                    continue;
                }
                else
                {
                    // Nothing to do.
                }
            }
            else
            {
                // Nothing to do.
            }

            IDE_TEST( sdlStatementManager::allocRemoteStatement( sSdStmt,
                                                                 sConnectInfo->mNodeId,
                                                                 &sRemoteStmt )
                      != IDE_SUCCESS );

            // open shard connection
            IDE_TEST( qdkOpenShardConnection( sConnectInfo )
                      != IDE_SUCCESS );

            IDE_TEST( sdl::allocStmt( sConnectInfo,
                                      sRemoteStmt,
                                      &(sConnectInfo->mLinkFailure) )
                      != IDE_SUCCESS );

            // add shard transaction
            IDE_TEST( qdkAddShardTransaction(
                          aStatement->mStatistics,
                          QC_SMI_STMT(aStatement)->mTrans->getTransID(),
                          sConnectInfo )
                      != IDE_SUCCESS );

            // set first message flag
            sConnectInfo->mFlag &= ~SDI_CONNECT_MESSAGE_FIRST_MASK;
            sConnectInfo->mFlag |= SDI_CONNECT_MESSAGE_FIRST_TRUE;

            if ( sdl::execDirect( sConnectInfo,
                                  sRemoteStmt,
                                  aQuery,
                                  aQueryLen,
                                  sClientInfo,
                                  &(sConnectInfo->mLinkFailure) )
                 == IDE_SUCCESS )
            {
                (*aExecCount)++;
            }
            else
            {
                // 수행이 실패한 경우
                sSuccess = ID_FALSE;
            }

            if ( sdl::freeStmt( sConnectInfo,
                                sRemoteStmt,
                                SQL_DROP,
                                &(sConnectInfo->mLinkFailure) )
                 != IDE_SUCCESS )
            {
                // 수행이 실패한 경우
                sSuccess = ID_FALSE;
            }

            sdlStatementManager::setUnused( &sRemoteStmt );
        }
    }
    else
    {
        // Nothing to do.
    }

    IDE_TEST( sSuccess == ID_FALSE );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    if ( sRemoteStmt != NULL )
    {
        if ( sRemoteStmt->mStmt != NULL )
        {
            (void)sdl::freeStmt( sConnectInfo,
                                 sRemoteStmt,
                                 SQL_DROP,
                                 &(sConnectInfo->mLinkFailure) );
        }
        sdlStatementManager::setUnused( &sRemoteStmt );
    }

    return IDE_FAILURE;
}

void sdi::initDataInfo( qcShardExecData * aExecData )
{
    aExecData->planCount = 0;
    aExecData->execInfo = NULL;
    aExecData->dataSize = 0;
    aExecData->data = NULL;
}

IDE_RC sdi::allocDataInfo( qcShardExecData * aExecData,
                           iduVarMemList   * aMemory )
{
    sdiDataNodes * sDataInfo;
    UInt           i;

    if ( aExecData->planCount > 0 )
    {
        IDE_TEST( aMemory->alloc( ID_SIZEOF(sdiDataNodes) *
                                  aExecData->planCount,
                                  (void**) &(aExecData->execInfo) )
                  != IDE_SUCCESS );

        sDataInfo = (sdiDataNodes*)aExecData->execInfo;

        for ( i = 0; i < aExecData->planCount; i++, sDataInfo++ )
        {
            sDataInfo->mInitialized = ID_FALSE;
        }
    }
    else
    {
        // Nothing to do.
    }

    if ( aExecData->dataSize > 0 )
    {
        IDE_TEST( aMemory->alloc( aExecData->dataSize,
                                  (void**) &(aExecData->data) )
                  != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

void sdi::setDataNodePrepared( sdiClientInfo * aClientInfo,
                               sdiDataNodes  * aDataNode )
{
    sdiConnectInfo * sConnectInfo;
    sdiDataNode    * sDataNode;
    UInt             i;

    sConnectInfo = aClientInfo->mConnectInfo;
    sDataNode = aDataNode->mNodes;

    // executed -> prepared
    for ( i = 0; i < aClientInfo->mCount; i++, sConnectInfo++, sDataNode++ )
    {
        if ( sDataNode->mState >= SDI_NODE_STATE_EXECUTED )
        {
            sDataNode->mState = SDI_NODE_STATE_PREPARED;
        }
        else
        {
            // Nothing to do.
        }
    }
}

void sdi::closeDataInfo( qcStatement     * aStatement,
                         qcShardExecData * aExecData )
{
    sdiClientInfo * sClientInfo;
    sdiDataNodes  * sDataInfo;
    UInt            i;

    if ( aExecData->execInfo != NULL )
    {
        sClientInfo = aStatement->session->mQPSpecific.mClientInfo;
        sDataInfo = (sdiDataNodes*)aExecData->execInfo;

        // statement가 clear되는 경우 stmt들을 free한다.
        for ( i = 0; i < aExecData->planCount; i++, sDataInfo++ )
        {
            if ( sDataInfo->mInitialized == ID_TRUE )
            {
                setDataNodePrepared( sClientInfo, sDataInfo );
            }
            else
            {
                // Nothing to do.
            }
        }
    }
    else
    {
        // Nothing to do.
    }
}

void sdi::clearDataInfo( qcStatement     * aStatement,
                         qcShardExecData * aExecData )
{
    sdiClientInfo  * sClientInfo;
    sdiConnectInfo * sConnectInfo;
    sdiDataNodes   * sDataInfo;
    sdiDataNode    * sDataNode;
    UInt             i;
    UInt             j;

    if ( ( aExecData->execInfo != NULL ) && ( aStatement->session != NULL ) )
    {
        sClientInfo = aStatement->session->mQPSpecific.mClientInfo;
        sDataInfo = (sdiDataNodes*)aExecData->execInfo;

        // statement가 clear되는 경우 stmt들을 free한다.
        for ( i = 0; i < aExecData->planCount; i++, sDataInfo++ )
        {
            if ( sDataInfo->mInitialized == ID_TRUE )
            {
                sConnectInfo = sClientInfo->mConnectInfo;
                sDataNode    = sDataInfo->mNodes;

                for ( j = 0; j < sClientInfo->mCount; j++, sConnectInfo++, sDataNode++ )
                {
                    sdlStatementManager::setUnused( &sDataNode->mRemoteStmt );
                }

                sDataInfo->mInitialized = ID_FALSE;
            }
            else
            {
                // Nothing to do.
            }
        }

        aExecData->planCount = 0;
        aExecData->execInfo = NULL;
    }
    else
    {
        // Nothing to do.
    }

    if ( aExecData->data != NULL )
    {
        aExecData->dataSize = 0;
        aExecData->data = NULL;
    }
    else
    {
        // Nothing to do.
    }
}

IDE_RC sdi::initShardDataInfo( qcTemplate     * aTemplate,
                               sdiAnalyzeInfo * aShardAnalysis,
                               sdiClientInfo  * aClientInfo,
                               sdiDataNodes   * aDataInfo,
                               sdiDataNode    * aDataArg )
{
    sdiConnectInfo * sConnectInfo = NULL;
    sdiDataNode    * sDataNode    = NULL;
    UInt             i;
    UInt             j;

    /* PROJ-2655 Composite shard key */
    UShort  sRangeIndexCount = 0;
    UShort  sRangeIndex[SDI_VALUE_MAX_COUNT];

    idBool  sExecDefaultNode = ID_FALSE;
    idBool  sIsAllNodeExec = ID_FALSE;

    SChar          sNodeNameBuf[SDI_NODE_NAME_MAX_SIZE + 1];
    qcShardNodes * sNodeName;
    idBool         sFound;

    sdiStatement * sSdStmt = NULL;

    IDE_TEST_RAISE( aShardAnalysis == NULL, ERR_NOT_EXIST_SHARD_ANALYSIS );

    //----------------------------------------
    // data info 초기화
    //----------------------------------------

    aDataInfo->mCount = aClientInfo->mCount;

    sConnectInfo = aClientInfo->mConnectInfo;
    sDataNode = aDataInfo->mNodes;

    sSdStmt = (sdiStatement *)qci::mSessionCallback.mGetShardStmt( QC_MM_STMT( aTemplate->stmt ) );
    IDE_DASSERT( sSdStmt != NULL );

    for ( i = 0; i < aClientInfo->mCount; i++, sConnectInfo++, sDataNode++ )
    {
        idlOS::memcpy( sDataNode, aDataArg, ID_SIZEOF(sdiDataNode) );

        IDE_DASSERT( sDataNode->mRemoteStmt == NULL );

        IDE_TEST( sdlStatementManager::allocRemoteStatement( sSdStmt,
                                                             sConnectInfo->mNodeId,
                                                             &sDataNode->mRemoteStmt )
                  != IDE_SUCCESS );
    }

    //----------------------------------------
    // data node prepare 후보 선택
    //----------------------------------------

    if ( aShardAnalysis == & gAnalyzeInfoForAllNodes )
    {
        // 전체 data node를 선택
        for ( i = 0; i < aClientInfo->mCount; i++ )
        {
            aDataInfo->mNodes[i].mState = SDI_NODE_STATE_PREPARE_CANDIDATED;
        }
    }
    else
    {
        if ( ( aShardAnalysis->mValueCount > 0 ) &&
             ( findBindParameter( aShardAnalysis ) == ID_FALSE ) &&
             ( aShardAnalysis->mSplitMethod != SDI_SPLIT_CLONE ) &&
             ( aShardAnalysis->mSplitMethod != SDI_SPLIT_SOLO ) )
        {
            sConnectInfo = aClientInfo->mConnectInfo;
            sDataNode = aDataInfo->mNodes;

            IDE_TEST( getExecNodeRangeIndex( NULL, // aTemplate
                                             NULL, // aShardKeyTuple
                                             NULL, // aShardSubKeyTuple
                                             aShardAnalysis,
                                             sRangeIndex,
                                             &sRangeIndexCount,
                                             &sExecDefaultNode,
                                             &sIsAllNodeExec )
                      != IDE_SUCCESS );

            IDE_TEST_RAISE( ( sRangeIndexCount == 0 ) &&
                            ( sExecDefaultNode == ID_FALSE ) &&
                            ( sIsAllNodeExec == ID_FALSE ),
                            ERR_NO_EXEC_NODE_FOUND );

            if ( sIsAllNodeExec == ID_TRUE )
            {
                for ( i = 0; i < aClientInfo->mCount; i++, sConnectInfo++, sDataNode++ )
                {
                    // rangeInfo에 포함되어 있거나, default node는 prepare 대상이다.
                    if ( ( findRangeInfo( &(aShardAnalysis->mRangeInfo),
                                          sConnectInfo->mNodeId ) == ID_TRUE ) ||
                         ( aShardAnalysis->mDefaultNodeId == sConnectInfo->mNodeId ) )
                    {
                        sDataNode->mState = SDI_NODE_STATE_PREPARE_CANDIDATED;
                    }
                    else
                    {
                        // Nothing to do.
                    }
                }
            }
            else
            {
                // constant value only
                for ( i = 0; i < aClientInfo->mCount; i++, sConnectInfo++, sDataNode++ )
                {
                    for ( j = 0; j < sRangeIndexCount; j++ )
                    {
                        if ( aShardAnalysis->mRangeInfo.mRanges[sRangeIndex[j]].mNodeId ==
                             sConnectInfo->mNodeId )
                        {
                            sDataNode->mState = SDI_NODE_STATE_PREPARE_CANDIDATED;
                        }
                        else
                        {
                            // Nothing to do.
                        }
                    }

                    if ( ( sExecDefaultNode == ID_TRUE ) &&
                         ( aShardAnalysis->mDefaultNodeId == sConnectInfo->mNodeId ) )
                    {
                        sDataNode->mState = SDI_NODE_STATE_PREPARE_CANDIDATED;
                    }
                    else
                    {
                        // Nothing to do.
                    }
                }
            }
        }
        else
        {
            if ( aShardAnalysis->mSplitMethod == SDI_SPLIT_NODES )
            {
                for ( sNodeName = aShardAnalysis->mNodeNames;
                      sNodeName != NULL;
                      sNodeName = sNodeName->next )
                {
                    sFound = ID_FALSE;

                    sConnectInfo = aClientInfo->mConnectInfo;
                    sDataNode = aDataInfo->mNodes;

                    for ( i = 0; i < aClientInfo->mCount; i++, sConnectInfo++, sDataNode++ )
                    {
                        if ( isMatchedNodeName( sNodeName,
                                                sConnectInfo->mNodeName ) == ID_TRUE )
                        {
                            sDataNode->mState = SDI_NODE_STATE_PREPARE_CANDIDATED;

                            sFound = ID_TRUE;
                            break;
                        }
                        else
                        {
                            // Nothing to do.
                        }
                    }

                    IDE_TEST_RAISE( sFound != ID_TRUE, ERR_NODE_NOT_FOUND );
                }
            }
            else
            {
                sConnectInfo = aClientInfo->mConnectInfo;
                sDataNode = aDataInfo->mNodes;

                // clone이라도 range node정보는 있다.
                for ( i = 0; i < aClientInfo->mCount; i++, sConnectInfo++, sDataNode++ )
                {
                    // rangeInfo에 포함되어 있거나, default node는 prepare 대상이다.
                    if ( ( findRangeInfo( &(aShardAnalysis->mRangeInfo),
                                          sConnectInfo->mNodeId ) == ID_TRUE ) ||
                         ( aShardAnalysis->mDefaultNodeId == sConnectInfo->mNodeId ) )
                    {
                        sDataNode->mState = SDI_NODE_STATE_PREPARE_CANDIDATED;
                    }
                    else
                    {
                        // Nothing to do.
                    }
                }
            }
        }
    }

    aDataInfo->mInitialized = ID_TRUE;

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_NOT_EXIST_SHARD_ANALYSIS )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDI_NOT_EXIST_SHARD_ANALYSIS ) );
    }
    IDE_EXCEPTION( ERR_NO_EXEC_NODE_FOUND )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDI_DATA_NODE_NOT_FOUND ) );
    }
    IDE_EXCEPTION( ERR_NODE_NOT_FOUND )
    {
        // 길이검사는 이미 했다.
        IDE_DASSERT( sNodeName->namePos.size <= SDI_NODE_NAME_MAX_SIZE );

        if ( isNameInDoubleQuotation( &(sNodeName->namePos) ) == ID_FALSE )
        {
            idlOS::memcpy( sNodeNameBuf,
                           sNodeName->namePos.stmtText + sNodeName->namePos.offset,
                           sNodeName->namePos.size );
            sNodeNameBuf[sNodeName->namePos.size] = '\0';
        }
        else
        {
            (void)removeDoulbeQuotationFromNodeName( sNodeName,
                                                     sNodeNameBuf,
                                                     SDI_NODE_NAME_MAX_SIZE + 1 );
        }

        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDI_INVALID_NODE_NAME2,
                                  sNodeNameBuf ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdi::reuseShardDataInfo( qcTemplate     * aTemplate,
                                sdiClientInfo  * aClientInfo,
                                sdiDataNodes   * aDataInfo,
                                sdiBindParam   * aBindParams,
                                UShort           aBindParamCount,
                                sdiSVPStep       aSVPStep)
{
    sdiDataNode    * sDataNode = NULL;
    UInt             i;

    SD_UNUSED( aTemplate );
    SD_UNUSED( aClientInfo );

    //----------------------------------------
    // data info 초기화
    //----------------------------------------

    if ( aDataInfo->mCount > 0 )
    {
        sDataNode = aDataInfo->mNodes;

        for ( i = 0; i < aDataInfo->mCount; i++, sDataNode++ )
        {
            if ( sDataNode->mState >= SDI_NODE_STATE_PREPARED )
            {
                IDE_TEST_RAISE( sDataNode->mRemoteStmt == NULL,
                                ERR_NOT_EXIST_STATEMENT );

                IDE_TEST_RAISE( ( sDataNode->mRemoteStmt->mFreeFlag & SDL_STMT_FREE_FLAG_ALLOCATED_MASK )
                                == SDL_STMT_FREE_FLAG_ALLOCATED_FALSE,
                                ERR_NOT_EXIST_STATEMENT );
            }

            sDataNode->mSVPStep = aSVPStep;
        }

        sDataNode = aDataInfo->mNodes;

        if ( ( aBindParamCount > 0 ) &&
             ( sDataNode->mBindParamCount > 0 ) )
        {
            IDE_DASSERT( sDataNode->mBindParamCount == aBindParamCount );

            // bind parameter가 변경되었나?
            if ( idlOS::memcmp( sDataNode->mBindParams,
                                aBindParams,
                                ID_SIZEOF( sdiBindParam ) * aBindParamCount ) != 0 )
            {
                // bind 정보는 현재 한벌이므로 한번만 복사한다.
                idlOS::memcpy( sDataNode->mBindParams,
                               aBindParams,
                               ID_SIZEOF( sdiBindParam ) * aBindParamCount );

                for ( i = 0; i < aDataInfo->mCount; i++, sDataNode++ )
                {
                    sDataNode->mBindParamChanged = ID_TRUE;
                }
            }
            else
            {
                // Nothing to do.
            }
        }
        else
        {
            // Nothing to do.
        }
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_NOT_EXIST_STATEMENT )
    {
        /* statement is not allocated. need prepare. */
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDC_UNEXPECTED_ERROR,
                                  "sdi::reuseShardDataInfo",
                                  "not exist statement" ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdi::decideShardDataInfo( qcTemplate     * aTemplate,
                                 mtcTuple       * aShardKeyTuple,
                                 sdiAnalyzeInfo * aShardAnalysis,
                                 sdiClientInfo  * aClientInfo,
                                 sdiDataNodes   * aDataInfo,
                                 qcNamePosition * aShardQuery )
{
    sdiConnectInfo * sConnectInfo = NULL;
    UInt             i = 0;

    /* PROJ-2655 Composite shard key */
    UShort  sRangeIndex[SDI_VALUE_MAX_COUNT];
    UShort  sRangeIndexCount = 0;

    idBool  sExecDefaultNode = ID_FALSE;
    idBool  sIsAllNodeExec  = ID_FALSE;

    qcShardNodes * sNodeName;
    idBool         sFound;

    IDE_TEST_RAISE( aShardAnalysis == NULL, ERR_NOT_EXIST_SHARD_ANALYSIS );

    IDE_TEST_RAISE( aShardQuery->size == QC_POS_EMPTY_SIZE,
                    ERR_NULL_SHARD_QUERY );

    //----------------------------------------
    // data node execute 후보 선택
    //----------------------------------------

    if ( aShardAnalysis == & gAnalyzeInfoForAllNodes )
    {
        // 전체 data node를 선택
        IDE_TEST( setPrepareSelected( aClientInfo,
                                      aDataInfo,
                                      ID_TRUE,  // all nodes
                                      0 )
                  != IDE_SUCCESS );
    }
    else
    {
        switch ( aShardAnalysis->mSplitMethod )
        {
            case SDI_SPLIT_CLONE:

                if ( aShardAnalysis->mValueCount == 0 )
                {
                    if ( QCU_DISPLAY_PLAN_FOR_NATC == 0 )
                    {
                        // split clone ( random execution )
                        i = (UInt)(((UShort)idlOS::rand()) % aShardAnalysis->mRangeInfo.mCount);
                    }
                    else
                    {
                        i = 0;
                    }

                    IDE_TEST( setPrepareSelected( aClientInfo,
                                                  aDataInfo,
                                                  ID_FALSE,
                                                  aShardAnalysis->mRangeInfo.mRanges[i].mNodeId )
                              != IDE_SUCCESS );
                }
                else
                {
                    sConnectInfo = aClientInfo->mConnectInfo;

                    // BUG-44711
                    for ( i = 0; i < aClientInfo->mCount; i++, sConnectInfo++ )
                    {
                        if ( ( findRangeInfo( &(aShardAnalysis->mRangeInfo),
                                              sConnectInfo->mNodeId ) == ID_TRUE ) ||
                             ( aShardAnalysis->mDefaultNodeId == sConnectInfo->mNodeId ) )
                        {
                            IDE_TEST( setPrepareSelected( aClientInfo,
                                                          aDataInfo,
                                                          ID_FALSE,
                                                          sConnectInfo->mNodeId )
                                      != IDE_SUCCESS );
                        }
                        else
                        {
                            // Nothing to do.
                        }
                    }
                }
                break;

            case SDI_SPLIT_SOLO:

                IDE_DASSERT( aShardAnalysis->mRangeInfo.mCount == 1 );

                IDE_TEST( setPrepareSelected( aClientInfo,
                                              aDataInfo,
                                              ID_FALSE,
                                              aShardAnalysis->mRangeInfo.mRanges[0].mNodeId )
                          != IDE_SUCCESS );
                break;

            case SDI_SPLIT_HASH:
            case SDI_SPLIT_RANGE:
            case SDI_SPLIT_LIST:

                /*
                 * Shard value( bind or constant value )가
                 * Analysis result상의 range info에서 몇 번 째(range index) 위치한 value인지 찾는다.
                 */
                IDE_TEST( getExecNodeRangeIndex( aTemplate,
                                                 aShardKeyTuple,
                                                 aShardKeyTuple,
                                                 aShardAnalysis,
                                                 sRangeIndex,
                                                 &sRangeIndexCount,
                                                 &sExecDefaultNode,
                                                 &sIsAllNodeExec )
                          != IDE_SUCCESS );

                IDE_TEST_RAISE( ( sRangeIndexCount == 0 ) &&
                                ( sExecDefaultNode == ID_FALSE ) &&
                                ( sIsAllNodeExec == ID_FALSE ),
                                ERR_NO_EXEC_NODE_FOUND );

                if ( sIsAllNodeExec == ID_FALSE )
                {
                    for ( i = 0; i < sRangeIndexCount; i++ )
                    {
                        IDE_TEST( setPrepareSelected( aClientInfo,
                                                      aDataInfo,
                                                      ID_FALSE,
                                                      aShardAnalysis->mRangeInfo.mRanges[sRangeIndex[i]].mNodeId )
                                  != IDE_SUCCESS );
                    }

                    if ( sExecDefaultNode == ID_TRUE )
                    {
                        /* BUG-45738 */
                        // Default node외에 수행 대상 노드가 없는데
                        // Default node가 설정 되어있지 않다면 에러
                        IDE_TEST_RAISE( ( sRangeIndexCount == 0 ) &&
                                        ( aShardAnalysis->mDefaultNodeId == ID_UINT_MAX ),
                                        ERR_NO_EXEC_NODE_FOUND );

                        // Default node가 없더라도, 수행 대상 노드가 하나라도 있으면
                        // 그 노드에서만 수행시킨다. ( for SELECT )
                        if ( aShardAnalysis->mDefaultNodeId != ID_UINT_MAX )
                        {
                            IDE_TEST( setPrepareSelected( aClientInfo,
                                                          aDataInfo,
                                                          ID_FALSE,
                                                          aShardAnalysis->mDefaultNodeId )
                                      != IDE_SUCCESS );
                        }
                        else
                        {
                            // Nothing to do.
                        }
                    }
                    else
                    {
                        // Nothing to do.
                    }
                }
                else
                {
                    sConnectInfo = aClientInfo->mConnectInfo;

                    // BUG-44711
                    for ( i = 0; i < aClientInfo->mCount; i++, sConnectInfo++ )
                    {
                        if ( ( findRangeInfo( &(aShardAnalysis->mRangeInfo),
                                              sConnectInfo->mNodeId ) == ID_TRUE ) ||
                             ( aShardAnalysis->mDefaultNodeId == sConnectInfo->mNodeId ) )
                        {
                            IDE_TEST( setPrepareSelected( aClientInfo,
                                                          aDataInfo,
                                                          ID_FALSE,
                                                          sConnectInfo->mNodeId )
                                      != IDE_SUCCESS );
                        }
                        else
                        {
                            // Nothing to do.
                        }
                    }
                }
                break;

            case SDI_SPLIT_NODES:
                for ( sNodeName = aShardAnalysis->mNodeNames;
                      sNodeName != NULL;
                      sNodeName = sNodeName->next )
                {
                    sFound = ID_FALSE;

                    sConnectInfo = aClientInfo->mConnectInfo;

                    for ( i = 0; i < aClientInfo->mCount; i++, sConnectInfo++ )
                    {
                        if ( isMatchedNodeName( sNodeName,
                                                sConnectInfo->mNodeName ) == ID_TRUE )
                        {
                            IDE_TEST( setPrepareSelected( aClientInfo,
                                                          aDataInfo,
                                                          ID_FALSE,
                                                          sConnectInfo->mNodeId )
                                      != IDE_SUCCESS );

                            sFound = ID_TRUE;
                            break;
                        }
                        else
                        {
                            // Nothing to do.
                        }
                    }

                    IDE_TEST_RAISE( sFound == ID_FALSE, ERR_NOT_FOUND );
                }
                break;

            default:
                break;
        }
    }

    //----------------------------------------
    // data node execute 후보 준비
    //----------------------------------------

    IDE_TEST( prepare( aClientInfo,
                       aDataInfo,
                       aShardQuery )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_NOT_EXIST_SHARD_ANALYSIS )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDI_NOT_EXIST_SHARD_ANALYSIS ) );
    }
    IDE_EXCEPTION( ERR_NO_EXEC_NODE_FOUND )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDI_DATA_NODE_NOT_FOUND ) );
    }
    IDE_EXCEPTION( ERR_NULL_SHARD_QUERY )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDC_UNEXPECTED_ERROR,
                                  "sdi::decideShardDataInfo",
                                  "null shard query" ) );
    }
    IDE_EXCEPTION( ERR_NOT_FOUND )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_SHARD_NODE_NOT_EXIST ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdi::getExecNodeRangeIndex( qcTemplate        * aTemplate,
                                   mtcTuple          * aShardKeyTuple,
                                   mtcTuple          * aShardSubKeyTuple,
                                   sdiAnalyzeInfo    * aShardAnalysis,
                                   UShort            * aRangeIndex,
                                   UShort            * aRangeIndexCount,
                                   idBool            * aExecDefaultNode,
                                   idBool            * aExecAllNodes )
{
    UInt    i;
    UInt    j;

    UShort  sFirstRangeIndexCount = 0;
    sdiRangeIndex sFirstRangeIndex[SDI_VALUE_MAX_COUNT];

    UShort  sSecondRangeIndexCount = 0;
    sdiRangeIndex sSecondRangeIndex[SDI_VALUE_MAX_COUNT];

    UShort sSubValueIndex = ID_USHORT_MAX;

    idBool  sIsFound = ID_FALSE;
    idBool  sIsSame = ID_FALSE;

    if ( ( aShardAnalysis->mValueCount == 0 ) && ( aShardAnalysis->mSubValueCount == 0 ) )
    {
        /*
         * CASE 1 : ( mValueCount == 0 && mSubValueCount == 0 )
         *
         * Shard value가 없다면, 모든 노드가 수행 대상이다.
         *
         */
        *aExecAllNodes = ID_TRUE;
        *aExecDefaultNode = ID_TRUE;
    }
    else
    {
        // shard key value에 해당하는 range info의 index들을 구한다.
        for ( i = 0; i < aShardAnalysis->mValueCount; i++ )
        {
            IDE_TEST( getRangeIndexByValue( aTemplate,
                                            aShardKeyTuple,
                                            aShardAnalysis,
                                            i,
                                            &aShardAnalysis->mValue[i],
                                            sFirstRangeIndex,
                                            &sFirstRangeIndexCount,
                                            aExecDefaultNode,
                                            ID_FALSE )
                      != IDE_SUCCESS );
        }

        // Sub-shard key가 존재하는 경우
        if ( aShardAnalysis->mSubKeyExists == ID_TRUE )
        {
            if ( aShardAnalysis->mValueCount > 1 )
            {
                /*
                 * Sub-shard key가 있는 경우,
                 * 첫 번 째 shard key에 대한 value 가 둘 이상이라면, 수행노드를 정확히 구분 해 낼 수 없다.
                 * 다만, 첫 번 째 shard key에 대한 value가 여러개라도 모두 같은 값이라면,
                 * 수행노드를 판별 할 수 있기 때문에 허용한다.
                 *
                 * e.x.    WHERE ( KEY1 = 100 AND KEY2 = 200 ) OR ( KEY1 = 100 AND KEY2 = 300 )
                 *       = WHERE ( KEY1 = 100 ) AND ( KEY2 = 100 OR KEY2 = 200 )
                 */
                for ( i = 1; i < aShardAnalysis->mValueCount; i++ )
                {
                    IDE_TEST( sdi::checkValuesSame( aTemplate,
                                                    aShardKeyTuple,
                                                    aShardAnalysis->mKeyDataType,
                                                    &aShardAnalysis->mValue[0],
                                                    &aShardAnalysis->mValue[i],
                                                    &sIsSame )
                              != IDE_SUCCESS );

                    if ( sIsSame == ID_FALSE )
                    {
                        *aExecAllNodes = ID_TRUE;
                        break;
                    }
                    else
                    {
                        // Nothing to do.
                    }
                }
            }
            else
            {
                // Nothing to do.
            }

            if ( *aExecAllNodes == ID_FALSE )
            {
                for ( i = 0; i < aShardAnalysis->mSubValueCount; i++ )
                {
                    IDE_TEST( getRangeIndexByValue( aTemplate,
                                                    aShardSubKeyTuple,
                                                    aShardAnalysis,
                                                    i,
                                                    &aShardAnalysis->mSubValue[i],
                                                    sSecondRangeIndex,
                                                    &sSecondRangeIndexCount,
                                                    aExecDefaultNode,
                                                    ID_TRUE )
                              != IDE_SUCCESS );
                }
            }
            else
            {
                // Nothing to do.
            }
        }
        else
        {
            // Nothing to do.
        }

        if ( *aExecAllNodes == ID_FALSE )
        {
            if ( aShardAnalysis->mValueCount > 0 )
            {
                if ( ( aShardAnalysis->mSubKeyExists == ID_TRUE ) && ( aShardAnalysis->mSubValueCount > 0 ) )
                {
                    /*
                     * CASE 2 : ( mValueCount > 0 && mSubValueCount > 0 )
                     *
                     * value와 sub value의 range index가 같은 노드들이 수행대상이 된다.
                     *
                     */

                    sSubValueIndex = sSecondRangeIndex[0].mValueIndex;

                    for ( i = 0; i < sSecondRangeIndexCount; i++ )
                    {
                        if ( sSubValueIndex != sSecondRangeIndex[i].mValueIndex )
                        {
                            if ( sIsFound == ID_FALSE )
                            {
                                *aExecDefaultNode = ID_TRUE;
                            }
                            else
                            {
                                // Nothing to do.
                            }

                            sSubValueIndex = sSecondRangeIndex[i].mValueIndex;
                            sIsFound = ID_FALSE;
                        }
                        else
                        {
                            // Nothing to do.
                        }

                        for ( j = 0; j < sFirstRangeIndexCount; j++ )
                        {
                            if ( sFirstRangeIndex[j].mRangeIndex == sSecondRangeIndex[i].mRangeIndex )
                            {
                                aRangeIndex[*aRangeIndexCount] = sFirstRangeIndex[j].mRangeIndex;
                                (*aRangeIndexCount)++;

                                sIsFound = ID_TRUE;
                            }
                            else
                            {
                                // Nothing to do.
                            }
                        }
                    }

                    if ( sIsFound == ID_FALSE )
                    {
                        *aExecDefaultNode = ID_TRUE;
                    }
                    else
                    {
                        // Nothing to do.
                    }
                }
                else
                {
                    /*
                     * CASE 3 : ( mValueCount > 0 && mSubValueCount == 0 )
                     *
                     * value의 range index에 해당하는 노드들이 수행대상이 된다.
                     *
                     */
                    for ( i = 0; i < sFirstRangeIndexCount; i++ )
                    {
                        aRangeIndex[*aRangeIndexCount] = sFirstRangeIndex[i].mRangeIndex;
                        (*aRangeIndexCount)++;

                        sIsFound = ID_TRUE;
                    }

                    /* BUG-45738 */
                    if ( aShardAnalysis->mSubKeyExists == ID_TRUE )
                    {
                        *aExecDefaultNode = ID_TRUE;
                    }
                    else
                    {
                        // Nothing to do.
                    }
                }
            }
            else
            {
                /*
                 * CASE 4 : ( mValueCount == 0 && mSubValueCount > 0 )
                 *
                 * sub value의 range index에 해당하는 노드들이 수행대상이 된다.
                 *
                 */
                for ( j = 0; j < sSecondRangeIndexCount; j++ )
                {
                    aRangeIndex[*aRangeIndexCount] = sSecondRangeIndex[j].mRangeIndex;
                    (*aRangeIndexCount)++;

                    *aExecDefaultNode = ID_TRUE;

                    sIsFound = ID_TRUE;
                }
            }

            if ( sIsFound == ID_FALSE )
            {
                *aExecDefaultNode = ID_TRUE;
            }
            else
            {
                // Nothing to do.
            }
        }
        else
        {
            // Nothing to do.
        }
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdi::setPrepareSelected( sdiClientInfo    * aClientInfo,
                                sdiDataNodes     * aDataInfo,
                                idBool             aAllNodes,
                                UInt               aNodeId )
{
    sdiConnectInfo * sConnectInfo = NULL;
    sdiDataNode    * sDataNode = NULL;
    UInt             i = 0;

    IDE_DASSERT( aClientInfo != NULL );

    sConnectInfo = aClientInfo->mConnectInfo;
    sDataNode    = aDataInfo->mNodes;

    for ( i = 0; i < aClientInfo->mCount; i++, sConnectInfo++, sDataNode++ )
    {
        if ( ( aAllNodes == ID_TRUE ) ||
             ( sConnectInfo->mNodeId == aNodeId ) )
        {
            //-------------------------------
            // shard statement 준비
            //-------------------------------

            if ( sDataNode->mState == SDI_NODE_STATE_PREPARED )
            {
                IDE_TEST( sdl::freeStmt( sConnectInfo,
                                         sDataNode->mRemoteStmt,
                                         SQL_CLOSE,
                                         &(sConnectInfo->mLinkFailure) )
                          != IDE_SUCCESS );

                sDataNode->mState = SDI_NODE_STATE_EXECUTE_CANDIDATED;
            }
            else if ( sDataNode->mState == SDI_NODE_STATE_EXECUTED )
            {
                sDataNode->mState = SDI_NODE_STATE_EXECUTE_CANDIDATED;
            }
            else
            {
                // Nothing to do.
            }

            //-------------------------------
            // 후보 선정
            //-------------------------------

            if ( sDataNode->mState == SDI_NODE_STATE_PREPARE_CANDIDATED )
            {
                sDataNode->mState = SDI_NODE_STATE_PREPARE_SELECTED;
            }
            else if ( sDataNode->mState == SDI_NODE_STATE_EXECUTE_CANDIDATED )
            {
                sDataNode->mState = SDI_NODE_STATE_EXECUTE_SELECTED;
            }
            else
            {
                // Nothing to do.
            }
        }
        else
        {
            // Nothing to do.
        }
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdi::prepare( sdiClientInfo    * aClientInfo,
                     sdiDataNodes     * aDataInfo,
                     qcNamePosition   * aShardQuery )
{
    sdiConnectInfo * sConnectInfo = NULL;
    sdiDataNode    * sDataNode    = NULL;
    void           * sCallback    = NULL;
    idBool           sSuccess     = ID_TRUE;
    UInt             i = 0;
    UInt             j = 0;

    IDE_DASSERT( aClientInfo != NULL );

    sConnectInfo = aClientInfo->mConnectInfo;
    sDataNode    = aDataInfo->mNodes;

    for ( i = 0; i < aClientInfo->mCount; i++, sConnectInfo++, sDataNode++ )
    {
        //-------------------------------
        // shard statement 초기화
        //-------------------------------

        if ( sDataNode->mState == SDI_NODE_STATE_PREPARE_SELECTED )
        {
            // open shard connection
            IDE_TEST( qdkOpenShardConnection( sConnectInfo )
                      != IDE_SUCCESS );
        }
        else
        {
            // Nothing to do.
        }

        if ( ( ( sDataNode->mState == SDI_NODE_STATE_PREPARE_SELECTED ) ||
               ( sDataNode->mState == SDI_NODE_STATE_EXECUTE_SELECTED ) )
             &&
             ( ( sConnectInfo->mFlag & SDI_CONNECT_PLANATTR_CHANGE_MASK )
               == SDI_CONNECT_PLANATTR_CHANGE_TRUE ) )
        {
            IDE_TEST( sdl::setConnAttr( sConnectInfo,
                                        SDL_ALTIBASE_EXPLAIN_PLAN,
                                        (sConnectInfo->mPlanAttr > 0) ?
                                        SDL_EXPLAIN_PLAN_ON :
                                        SDL_EXPLAIN_PLAN_OFF,
                                        &(sConnectInfo->mLinkFailure) )
                      != IDE_SUCCESS );

            sConnectInfo->mFlag &= ~SDI_CONNECT_PLANATTR_CHANGE_MASK;
            sConnectInfo->mFlag |= SDI_CONNECT_PLANATTR_CHANGE_FALSE;
        }
        else
        {
            // Nothing to do.
        }

        if ( sDataNode->mState == SDI_NODE_STATE_PREPARE_SELECTED )
        {
            IDE_TEST( sdl::allocStmt( sConnectInfo,
                                      sDataNode->mRemoteStmt,
                                      &(sConnectInfo->mLinkFailure) )
                      != IDE_SUCCESS );

            IDE_TEST( sdl::addPrepareCallback( &sCallback,
                                               i,
                                               sConnectInfo,
                                               sDataNode->mRemoteStmt,
                                               aShardQuery->stmtText +
                                               aShardQuery->offset,
                                               aShardQuery->size,
                                               &(sConnectInfo->mLinkFailure) )
                      != IDE_SUCCESS );
        }
        else
        {
            // Nothing to do.
        }
    }

    // PROJ-2670 nested execution
    sdl::doCallback( sCallback );

    sConnectInfo = aClientInfo->mConnectInfo;
    sDataNode    = aDataInfo->mNodes;

    for ( i = 0; i < aClientInfo->mCount; i++, sConnectInfo++, sDataNode++ )
    {
        if ( sDataNode->mState == SDI_NODE_STATE_PREPARE_SELECTED )
        {
            if ( sdl::resultCallback( sCallback,
                                      i,
                                      sConnectInfo,
                                      ID_FALSE,
                                      aClientInfo,
                                      SQL_HANDLE_STMT,
                                      sDataNode->mRemoteStmt->mStmt,
                                      (SChar*)"SQLPrepare",
                                      &(sConnectInfo->mLinkFailure) )
                 == IDE_SUCCESS )
            {
                // bind를 해야한다.
                sDataNode->mBindParamChanged = ID_TRUE;
                sDataNode->mExecCount = 0;
                sDataNode->mState = SDI_NODE_STATE_PREPARED;
                sDataNode->mState = SDI_NODE_STATE_EXECUTE_CANDIDATED;
                sDataNode->mState = SDI_NODE_STATE_EXECUTE_SELECTED;
            }
            else
            {
                sSuccess = ID_FALSE;
            }
        }
        else
        {
            // Nothing to do.
        }
    }

    IDE_TEST( sSuccess == ID_FALSE );

    sConnectInfo = aClientInfo->mConnectInfo;
    sDataNode    = aDataInfo->mNodes;

    for ( i = 0; i < aClientInfo->mCount; i++, sConnectInfo++, sDataNode++ )
    {
        //-------------------------------
        // shard statement 재바인드
        //-------------------------------

        if ( ( sDataNode->mState == SDI_NODE_STATE_EXECUTE_SELECTED ) &&
             ( sDataNode->mBindParamChanged == ID_TRUE ) )
        {
            for ( j = 0; j < sDataNode->mBindParamCount; j++ )
            {
                IDE_TEST( sdl::bindParam(
                              sConnectInfo,
                              sDataNode->mRemoteStmt,
                              sDataNode->mBindParams[j].mId,
                              sDataNode->mBindParams[j].mInoutType,
                              sDataNode->mBindParams[j].mType,
                              sDataNode->mBindParams[j].mData,
                              sDataNode->mBindParams[j].mDataSize,
                              sDataNode->mBindParams[j].mPrecision,
                              sDataNode->mBindParams[j].mScale,
                              &(sConnectInfo->mLinkFailure) )
                          != IDE_SUCCESS );
            }

            sDataNode->mBindParamChanged = ID_FALSE;
        }
        else
        {
            // Nothing to do.
        }
    }

    sdl::removeCallback( sCallback );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    sConnectInfo = aClientInfo->mConnectInfo;
    sDataNode = aDataInfo->mNodes;

    for ( i = 0; i < aClientInfo->mCount; i++, sConnectInfo++, sDataNode++ )
    {
        if ( sDataNode->mState == SDI_NODE_STATE_PREPARE_SELECTED )
        {
            sDataNode->mState = SDI_NODE_STATE_PREPARE_CANDIDATED;
        }
        else if ( sDataNode->mState == SDI_NODE_STATE_EXECUTE_SELECTED )
        {
            sDataNode->mState = SDI_NODE_STATE_EXECUTE_CANDIDATED;
        }
        else
        {
            // Nothing to do.
        }
    }

    sdl::removeCallback( sCallback );

    return IDE_FAILURE;
}

IDE_RC sdi::executeDML( qcStatement    * aStatement,
                        sdiClientInfo  * aClientInfo,
                        sdiDataNodes   * aDataInfo,
                        vSLong         * aNumRows )
{
    sdiConnectInfo * sConnectInfo = NULL;
    sdiDataNode    * sDataNode    = NULL;
    void           * sCallback    = NULL;
    idBool           sSuccess     = ID_TRUE;
    vSLong           sNumRows = 0;
    SInt             i;

    sConnectInfo = aClientInfo->mConnectInfo;
    sDataNode    = aDataInfo->mNodes;

    for ( i = 0; i < aClientInfo->mCount; i++, sConnectInfo++, sDataNode++ )
    {
        if ( sDataNode->mState == SDI_NODE_STATE_EXECUTE_SELECTED )
        {
            // add shard transaction
            IDE_TEST( qdkAddShardTransaction(
                          aStatement->mStatistics,
                          QC_SMI_STMT(aStatement)->mTrans->getTransID(),
                          sConnectInfo )
                      != IDE_SUCCESS );

            IDE_TEST( sdl::addExecuteCallback( &sCallback,
                                               i,
                                               sConnectInfo,
                                               sDataNode,
                                               &(sConnectInfo->mLinkFailure) )
                      != IDE_SUCCESS );
        }
        else
        {
            // Nothing to do.
        }
    }

    IDE_TEST_CONT( sCallback == NULL, NORMAL_EXIT );

    // PROJ-2670 nested execution
    sdl::doCallback( sCallback );

    // add shard tx 순서의 반대로 del shard tx를 수행해야한다.
    for ( i--, sConnectInfo--, sDataNode--;
          i >= 0;
          i--, sConnectInfo--, sDataNode-- )
    {
        if ( sDataNode->mState == SDI_NODE_STATE_EXECUTE_SELECTED )
        {
            // 수행전 touch count 증가
            sConnectInfo->mTouchCount++;

            if ( sdl::resultCallback( sCallback,
                                      i,
                                      sConnectInfo,
                                      ID_FALSE,
                                      aClientInfo,
                                      SQL_HANDLE_STMT,
                                      sDataNode->mRemoteStmt->mStmt,
                                      (SChar*)"SQLExecute",
                                      &(sConnectInfo->mLinkFailure) )
                 == IDE_SUCCESS )
            {
                // 수행후
                sDataNode->mState = SDI_NODE_STATE_EXECUTED;
                sDataNode->mExecCount++;

                if ( sdl::rowCount( sConnectInfo,
                                    sDataNode->mRemoteStmt,
                                    &sNumRows,
                                    &(sConnectInfo->mLinkFailure) )
                     == IDE_SUCCESS )
                {
                    // result row count
                    (*aNumRows) += sNumRows;
                    sConnectInfo->mAffectedRowCount = sNumRows;
                }
                else
                {
                    // Nothing to do.
                }
            }
            else
            {
                // 수행이 실패한 경우
                sDataNode->mState = SDI_NODE_STATE_EXECUTE_CANDIDATED;

                sSuccess = ID_FALSE;
            }
        }
        else
        {
            // Nothing to do.
        }
    }

    IDE_TEST( sSuccess == ID_FALSE );

    sdl::removeCallback( sCallback );

    IDE_EXCEPTION_CONT( NORMAL_EXIT );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    processExecuteError( aStatement,
                         aClientInfo,
                         aDataInfo->mNodes );

    sdl::removeCallback( sCallback );

    return IDE_FAILURE;
}

IDE_RC sdi::executeInsert( qcStatement    * aStatement,
                           sdiClientInfo  * aClientInfo,
                           sdiDataNodes   * aDataInfo,
                           vSLong         * aNumRows )
{
    sdiConnectInfo * sConnectInfo = NULL;
    sdiDataNode    * sDataNode    = NULL;
    void           * sCallback    = NULL;
    idBool           sSuccess     = ID_TRUE;
    SInt             i;

    sConnectInfo = aClientInfo->mConnectInfo;
    sDataNode    = aDataInfo->mNodes;

    for ( i = 0; i < aClientInfo->mCount; i++, sConnectInfo++, sDataNode++ )
    {
        if ( sDataNode->mState == SDI_NODE_STATE_EXECUTE_SELECTED )
        {
            // add shard transaction
            IDE_TEST( qdkAddShardTransaction(
                          aStatement->mStatistics,
                          QC_SMI_STMT(aStatement)->mTrans->getTransID(),
                          sConnectInfo )
                      != IDE_SUCCESS );

            if ( sDataNode->mSVPStep == SDI_SVP_STEP_NEED_SAVEPOINT )
            {
                IDE_TEST( sdl::setSavepoint( sConnectInfo,
                                             SAVEPOINT_FOR_SHARD_STMT_PARTIAL_ROLLBACK )
                          != IDE_SUCCESS );

                sDataNode->mSVPStep = SDI_SVP_STEP_SET_SAVEPOINT;

            }

            IDE_TEST( sdl::addExecuteCallback( &sCallback,
                                               i,
                                               sConnectInfo,
                                               sDataNode,
                                               &(sConnectInfo->mLinkFailure) )
                      != IDE_SUCCESS );
        }
        else
        {
            // Nothing to do.
        }
    }

    IDE_TEST_CONT( sCallback == NULL, NORMAL_EXIT );

    // PROJ-2670 nested execution
    sdl::doCallback( sCallback );

    // add shard tx 순서의 반대로 del shard tx를 수행해야한다.
    for ( i--, sConnectInfo--, sDataNode--;
          i >= 0;
          i--, sConnectInfo--, sDataNode-- )
    {
        if ( sDataNode->mState == SDI_NODE_STATE_EXECUTE_SELECTED )
        {
            // 수행전 touch count 증가
            sConnectInfo->mTouchCount++;

            if ( sdl::resultCallback( sCallback,
                                      i,
                                      sConnectInfo,
                                      ID_FALSE,
                                      aClientInfo,
                                      SQL_HANDLE_STMT,
                                      sDataNode->mRemoteStmt->mStmt,
                                      (SChar*)"SQLExecute",
                                      &(sConnectInfo->mLinkFailure) )
                 == IDE_SUCCESS )
            {
                // 수행후
                sDataNode->mState = SDI_NODE_STATE_EXECUTED;
                sDataNode->mExecCount++;

                // result row count
                (*aNumRows)++;
                sConnectInfo->mAffectedRowCount = 1;
            }
            else
            {
                // 수행이 실패한 경우
                sDataNode->mState = SDI_NODE_STATE_EXECUTE_CANDIDATED;

                sSuccess = ID_FALSE;
            }
        }
        else
        {
            // Nothing to do.
        }
    }

    IDE_TEST( sSuccess == ID_FALSE );

    sdl::removeCallback( sCallback );

    IDE_EXCEPTION_CONT( NORMAL_EXIT );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    processExecuteError( aStatement,
                         aClientInfo,
                         aDataInfo->mNodes );

    sdl::removeCallback( sCallback );

    return IDE_FAILURE;
}

IDE_RC sdi::executeSelect( qcStatement    * aStatement,
                           sdiClientInfo  * aClientInfo,
                           sdiDataNodes   * aDataInfo )
{
    sdiConnectInfo * sConnectInfo = NULL;
    sdiDataNode    * sDataNode    = NULL;
    void           * sCallback    = NULL;
    idBool           sSuccess     = ID_TRUE;
    SInt             i;

    sConnectInfo = aClientInfo->mConnectInfo;
    sDataNode    = aDataInfo->mNodes;

    for ( i = 0; i < aClientInfo->mCount; i++, sConnectInfo++, sDataNode++ )
    {
        if ( sDataNode->mState == SDI_NODE_STATE_EXECUTE_SELECTED )
        {
            // add shard transaction
            IDE_TEST( qdkAddShardTransaction(
                          aStatement->mStatistics,
                          QC_SMI_STMT(aStatement)->mTrans->getTransID(),
                          sConnectInfo )
                      != IDE_SUCCESS );

            IDE_TEST( sdl::addExecuteCallback( &sCallback,
                                               i,
                                               sConnectInfo,
                                               sDataNode,
                                               &(sConnectInfo->mLinkFailure) )
                      != IDE_SUCCESS );
        }
        else
        {
            // Nothing to do.
        }
    }

    IDE_TEST_CONT( sCallback == NULL, NORMAL_EXIT );

    // PROJ-2670 nested execution
    sdl::doCallback( sCallback );

    // add shard tx 순서의 반대로 del shard tx를 수행해야한다.
    for ( i--, sConnectInfo--, sDataNode--;
          i >= 0;
          i--, sConnectInfo--, sDataNode-- )
    {
        if ( sDataNode->mState == SDI_NODE_STATE_EXECUTE_SELECTED )
        {
            if ( sdl::resultCallback( sCallback,
                                      i,
                                      sConnectInfo,
                                      ID_FALSE,
                                      aClientInfo,
                                      SQL_HANDLE_STMT,
                                      sDataNode->mRemoteStmt->mStmt,
                                      (SChar*)"SQLExecute",
                                      &(sConnectInfo->mLinkFailure) )
                 == IDE_SUCCESS )
            {
                // 수행후
                sDataNode->mState = SDI_NODE_STATE_EXECUTED;
                sDataNode->mExecCount++;
            }
            else
            {
                // 수행이 실패한 경우
                sDataNode->mState = SDI_NODE_STATE_EXECUTE_CANDIDATED;

                sSuccess = ID_FALSE;
            }
        }
        else
        {
            // Nothing to do.
        }
    }

    IDE_TEST( sSuccess == ID_FALSE );

    sdl::removeCallback( sCallback );

    IDE_EXCEPTION_CONT( NORMAL_EXIT );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    processExecuteError( aStatement,
                         aClientInfo,
                         aDataInfo->mNodes );

    sdl::removeCallback( sCallback );

    return IDE_FAILURE;
}

IDE_RC sdi::fetch( sdiConnectInfo  * aConnectInfo,
                   sdiDataNode     * aDataNode,
                   idBool          * aExist )
{
    SShort  sResult = ID_SSHORT_MAX;

    IDE_TEST( sdl::fetch( aConnectInfo,
                          aDataNode->mRemoteStmt,
                          &sResult,
                          &(aConnectInfo->mLinkFailure) )
              != IDE_SUCCESS );

    if ( ( sResult == SQL_SUCCESS ) || ( sResult == SQL_SUCCESS_WITH_INFO ) )
    {
        *aExist = ID_TRUE;
    }
    else
    {
        IDE_DASSERT( sResult == SQL_NO_DATA_FOUND );

        *aExist = ID_FALSE;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdi::getPlan( sdiConnectInfo  * aConnectInfo,
                     sdiDataNode     * aDataNode )
{
    IDE_TEST( sdl::getPlan( aConnectInfo,
                            aDataNode->mRemoteStmt,
                            &(aDataNode->mPlanText),
                            &(aConnectInfo->mLinkFailure) )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

void sdi::charXOR( SChar * aText, UInt aLen )
{
    const UChar sEnc[8] = { 172, 134, 132, 122, 114, 109, 117, 134 };
    SChar     * sText;
    UInt        sIndex;
    UInt        i;

    sText = aText;

    for ( i = 0, sIndex = 0; i < aLen; i++ )
    {
        if ( sText[i] != 0 )
        {
            sText[i] ^= sEnc[sIndex];
        }
        else
        {
            // Nothing to do.
        }

        if ( sIndex < 7 )
        {
            sIndex++;
        }
        else
        {
            sIndex = 0;
        }
    }
}

IDE_RC sdi::printMessage( SChar * aMessage,
                          UInt    aLength,
                          void  * aArgument )
{
    sdiConnectInfo * sConnectInfo = (sdiConnectInfo*)aArgument;
    SChar            sFirstMessage[42 + SDI_NODE_NAME_MAX_SIZE + 2 + 42 + 1];
    UInt             sFirstMessageLength;

    if ( ( sConnectInfo->mFlag & SDI_CONNECT_MESSAGE_FIRST_MASK ) ==
         SDI_CONNECT_MESSAGE_FIRST_TRUE )
    {
        sFirstMessageLength = (UInt)idlOS::snprintf( sFirstMessage,
                                                     ID_SIZEOF(sFirstMessage),
                                                     ":----------------------------------------\n"
                                                     ":%s\n"
                                                     ":----------------------------------------\n",
                                                     sConnectInfo->mNodeName );

        (void) qci::mSessionCallback.mPrintToClient(
            sConnectInfo->mSession->mMmSession,
            (UChar*)sFirstMessage,
            sFirstMessageLength );

        sConnectInfo->mFlag &= ~SDI_CONNECT_MESSAGE_FIRST_MASK;
        sConnectInfo->mFlag |= SDI_CONNECT_MESSAGE_FIRST_FALSE;
    }
    else
    {
        // Nothing to do.
    }

    (void) qci::mSessionCallback.mPrintToClient(
        sConnectInfo->mSession->mMmSession,
        (UChar*)aMessage,
        aLength );

    return IDE_SUCCESS;
}

void sdi::setShardMetaTouched( qcSession * aSession )
{
    if ( ( aSession->mQPSpecific.mFlag & QC_SESSION_SHARD_META_TOUCH_MASK ) ==
         QC_SESSION_SHARD_META_TOUCH_FALSE )
    {
        aSession->mQPSpecific.mFlag &= ~QC_SESSION_SHARD_META_TOUCH_MASK;
        aSession->mQPSpecific.mFlag |= QC_SESSION_SHARD_META_TOUCH_TRUE;
    }
    else
    {
        // Nothing to do.
    }

    // 현재 세션에서 plan cache 사용금지
    if ( ( aSession->mQPSpecific.mFlag & QC_SESSION_PLAN_CACHE_MASK ) ==
         QC_SESSION_PLAN_CACHE_ENABLE )
    {
        aSession->mQPSpecific.mFlag &= ~QC_SESSION_PLAN_CACHE_MASK;
        aSession->mQPSpecific.mFlag |= QC_SESSION_PLAN_CACHE_DISABLE;
    }
    else
    {
        // Nothing to do.
    }
}

void sdi::unsetShardMetaTouched( qcSession * aSession )
{
    if ( ( aSession->mQPSpecific.mFlag & QC_SESSION_SHARD_META_TOUCH_MASK ) ==
         QC_SESSION_SHARD_META_TOUCH_TRUE )
    {
        aSession->mQPSpecific.mFlag &= ~QC_SESSION_SHARD_META_TOUCH_MASK;
        aSession->mQPSpecific.mFlag |= QC_SESSION_SHARD_META_TOUCH_FALSE;
    }
    else
    {
        // Nothing to do.
    }

    if ( ( aSession->mQPSpecific.mFlag & QC_SESSION_PLAN_CACHE_MASK ) ==
         QC_SESSION_PLAN_CACHE_DISABLE )
    {
        aSession->mQPSpecific.mFlag &= ~QC_SESSION_PLAN_CACHE_MASK;
        aSession->mQPSpecific.mFlag |= QC_SESSION_PLAN_CACHE_ENABLE;
    }
    else
    {
        // Nothing to do.
    }
}

IDE_RC sdi::touchShardNode( qcSession * aSession,
                            idvSQL    * aStatistics,
                            smTID       aTransID,
                            UInt        aNodeId )
{
    sdiClientInfo    * sClientInfo = NULL;
    sdiConnectInfo   * sConnectInfo = NULL;
    idBool             sFound = ID_FALSE;
    UShort             i = 0;

    IDE_DASSERT( aSession != NULL );

    sClientInfo = aSession->mQPSpecific.mClientInfo;

    if ( sClientInfo != NULL )
    {
        sConnectInfo = sClientInfo->mConnectInfo;

        for ( i = 0; i < sClientInfo->mCount; i++, sConnectInfo++ )
        {
            if ( sConnectInfo->mNodeInfo.mNodeId == aNodeId )
            {
                sFound = ID_TRUE;
                break;
            }
            else
            {
                // Nothing to do.
            }
        }
    }
    else
    {
        // Nothing to do.
    }

    IDE_TEST_RAISE( sFound == ID_FALSE, ERR_NOT_FOUND );

    // open shard connection
    IDE_TEST( qdkOpenShardConnection( sConnectInfo )
              != IDE_SUCCESS );

    // add shard transaction
    IDE_TEST( qdkAddShardTransaction( aStatistics,
                                      aTransID,
                                      sConnectInfo )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_NOT_FOUND )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_SHARD_NODE_NOT_EXIST ) );
    }

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdi::openAllShardConnections( qcSession * aSession )
{
    sdiClientInfo    * sClientInfo  = NULL;
    sdiConnectInfo   * sConnectInfo = NULL;
    UShort             i            = 0;

    IDE_DASSERT( aSession != NULL );

    sClientInfo = aSession->mQPSpecific.mClientInfo;

    if ( sClientInfo != NULL )
    {
        sConnectInfo = sClientInfo->mConnectInfo;

        for ( i = 0; i < sClientInfo->mCount; i++, sConnectInfo++ )
        {
            /* BUG-46100 SMN Propagation Failure Ignore */
            if ( SDU_SHARD_IGNORE_SMN_PROPAGATION_FAILURE == 0 )
            {
                IDE_TEST( qdkOpenShardConnection( sConnectInfo ) != IDE_SUCCESS );
            }
            else
            {
                (void)qdkOpenShardConnection( sConnectInfo );
            }
        }
    }
    else
    {
        // Nothing to do.
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    IDE_PUSH();

    /* Meta Node SMN 전파를 위해서 모든 Data Node에 연결을 시도한다. */
    for ( i++, sConnectInfo++; i < sClientInfo->mCount; i++, sConnectInfo++ )
    {
        (void)qdkOpenShardConnection( sConnectInfo );
    }

    IDE_POP();

    return IDE_FAILURE;
}

idBool sdi::getNeedToDisconnect( qcSession * aSession )
{
    sdiClientInfo    * sClientInfo       = NULL;
    idBool             sNeedToDisconnect = ID_FALSE;

    IDE_DASSERT( aSession != NULL );

    sClientInfo = aSession->mQPSpecific.mClientInfo;

    if ( sClientInfo != NULL )
    {
        sNeedToDisconnect = sClientInfo->mNeedToDisconnect;
    }
    else
    {
        /* Nothing to do */
    }

    return sNeedToDisconnect;
}

void sdi::setNeedToDisconnect( sdiClientInfo * aClientInfo,
                               idBool          aNeedToDisconnect )
{
    IDE_DASSERT( aClientInfo != NULL );

    if ( aNeedToDisconnect == ID_TRUE )
    {
        aClientInfo->mNeedToDisconnect = ID_TRUE;
    }
    else
    {
        /* Nothing to do */
    }

    return;
}

void sdi::xidInitialize( sdiConnectInfo * aConnectInfo )
{
    IDE_DASSERT( aConnectInfo != NULL );

    // null XID
    aConnectInfo->mXID.formatID = -1;
    aConnectInfo->mXID.gtrid_length = 0;
    aConnectInfo->mXID.bqual_length = 0;
}

IDE_RC sdi::addPrepareTranCallback( void           ** aCallback,
                                    sdiConnectInfo  * aNode )
{
    IDE_TEST( sdl::addPrepareTranCallback( aCallback,
                                           aNode->mNodeId,
                                           aNode,
                                           &(aNode->mXID),
                                           &(aNode->mReadOnly),
                                           &(aNode->mLinkFailure) )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdi::addEndTranCallback( void           ** aCallback,
                                sdiConnectInfo  * aNode,
                                idBool            aIsCommit )
{
    IDE_TEST( sdl::addEndTranCallback( aCallback,
                                       aNode->mNodeId,
                                       aNode,
                                       aIsCommit,
                                       &(aNode->mLinkFailure) )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

void sdi::doCallback( void * aCallback )
{
    sdl::doCallback( aCallback );
}

IDE_RC sdi::resultCallback( void           * aCallback,
                            sdiConnectInfo * aNode,
                            SChar          * aFuncName,
                            idBool           aReCall )
{
    IDE_TEST( sdl::resultCallback( aCallback,
                                   aNode->mNodeId,
                                   aNode,
                                   aReCall,
                                   aNode->mSession->mQPSpecific.mClientInfo,
                                   SQL_HANDLE_DBC,
                                   aNode->mDbc,
                                   aFuncName,
                                   &(aNode->mLinkFailure) )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

void sdi::removeCallback( void * aCallback )
{
    sdl::removeCallback( aCallback );
}

sdiShardPin sdi::makeShardPin()
{
    return sdmShardPinMgr::getNewShardPin();
}

void sdi::shardPinToString( SChar *aDst, UInt aLen, sdiShardPin aShardPin )
{
    sdmShardPinMgr::shardPinToString( aDst, aLen, aShardPin );
}

IDE_RC sdi::setCommitMode( qcSession * aSession,
                           idBool      aIsAutoCommit,
                           UInt        aDBLinkGTXLevel )
{
    sdiClientInfo  * sClientInfo        = NULL;
    sdiConnectInfo * sConnectInfo       = NULL;
    idBool           sOldIsUserAutoCommit = ID_FALSE;
    UInt             sOldIsMetaAutoCommit = ID_FALSE;
    idBool           sNewIsUserAutoCommit = aIsAutoCommit;
    idBool           sNewIsMetaAutoCommit = aIsAutoCommit;
    UShort           i                  = 0;
    UShort           j                  = 0;

    IDE_DASSERT( aSession != NULL );

    sClientInfo = aSession->mQPSpecific.mClientInfo;
    if ( sClientInfo != NULL )
    {
        // Global Transaction인 경우, Meta Connection은 Non-Autocommit이어야 한다.
        if ( ( aDBLinkGTXLevel == 2 ) && ( sNewIsMetaAutoCommit == ID_TRUE ) )
        {
            sNewIsMetaAutoCommit = ID_FALSE;
        }
        else
        {
            /* Nothing to do */
        }

        sConnectInfo = sClientInfo->mConnectInfo;
        for ( i = 0; i < sClientInfo->mCount; i++, sConnectInfo++ )
        {
            sOldIsUserAutoCommit = isUserAutoCommit( sConnectInfo );
            sOldIsMetaAutoCommit = isMetaAutoCommit( sConnectInfo );

            if ( sOldIsMetaAutoCommit != sNewIsMetaAutoCommit )
            {
                if ( sConnectInfo->mDbc != NULL )
                {
                    IDE_TEST( sdl::setConnAttr( sConnectInfo,
                                                SDL_SQL_ATTR_AUTOCOMMIT,
                                                ( sNewIsMetaAutoCommit == ID_TRUE
                                                  ? SDL_COMMITMODE_AUTOCOMMIT
                                                  : SDL_COMMITMODE_NONAUTOCOMMIT ),
                                                & sConnectInfo->mLinkFailure )
                              != IDE_SUCCESS );
                }
                else
                {
                    /* Nothing to do */
                }
            }
            else
            {
                /* Nothing to do */
            }

            sConnectInfo->mFlag &= ~SDI_CONNECT_USER_AUTOCOMMIT_MODE_MASK;
            sConnectInfo->mFlag |= ( ( sNewIsUserAutoCommit == ID_TRUE ) ?
                                     SDI_CONNECT_USER_AUTOCOMMIT_MODE_ON :
                                     SDI_CONNECT_USER_AUTOCOMMIT_MODE_OFF );

            sConnectInfo->mFlag &= ~SDI_CONNECT_COORD_AUTOCOMMIT_MODE_MASK;
            sConnectInfo->mFlag |= ( ( sNewIsMetaAutoCommit == ID_TRUE ) ?
                                     SDI_CONNECT_COORD_AUTOCOMMIT_MODE_ON :
                                     SDI_CONNECT_COORD_AUTOCOMMIT_MODE_OFF );
        }
    }
    else
    {
        /* Nothing to do */
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    IDE_PUSH();

    sConnectInfo = sClientInfo->mConnectInfo;
    for ( j = 0; ( j <= i ) && ( j < sClientInfo->mCount ); j++, sConnectInfo++ )
    {
        if ( sOldIsMetaAutoCommit != sNewIsMetaAutoCommit )
        {
            if ( sConnectInfo->mDbc != NULL )
            {
                (void) sdl::setConnAttr( sConnectInfo,
                                         SDL_SQL_ATTR_AUTOCOMMIT,
                                         ( sOldIsMetaAutoCommit == ID_TRUE
                                           ? SDL_COMMITMODE_AUTOCOMMIT
                                           : SDL_COMMITMODE_NONAUTOCOMMIT ),
                                         & sConnectInfo->mLinkFailure );
            }
            else
            {
                /* Nothing to do */
            }
        }
        else
        {
            /* Nothing to do */
        }

        sConnectInfo->mFlag &= ~SDI_CONNECT_USER_AUTOCOMMIT_MODE_MASK;
        sConnectInfo->mFlag |= ( ( sOldIsUserAutoCommit == ID_TRUE ) ?
                                 SDI_CONNECT_USER_AUTOCOMMIT_MODE_ON :
                                 SDI_CONNECT_USER_AUTOCOMMIT_MODE_OFF );

        sConnectInfo->mFlag &= ~SDI_CONNECT_COORD_AUTOCOMMIT_MODE_MASK;
        sConnectInfo->mFlag |= ( ( sOldIsMetaAutoCommit == ID_TRUE ) ?
                                 SDI_CONNECT_COORD_AUTOCOMMIT_MODE_ON :
                                 SDI_CONNECT_COORD_AUTOCOMMIT_MODE_OFF );

        sConnectInfo->mLinkFailure = ID_TRUE;
    }

    IDE_POP();

    return IDE_FAILURE;
}

/* BUG-45967 Rebuild Data 완료 대기 */
IDE_RC sdi::waitToRebuildData( idvSQL * aStatistics )
{
    const SChar * sName     = (const SChar *)"SHARD_REBUILD_DATA_STEP";
    SChar         sValue[2] = { '2', '\0' };

    // SHARD_REBUILD_DATA_STEP이 1 이면, 2 로 갱신
    if ( SDU_SHARD_REBUILD_DATA_STEP == 1 )
    {
        // BUG-19498 value range check
        IDE_TEST( idp::validate( sName, sValue ) != IDE_SUCCESS );

        IDE_TEST( idp::update( NULL, sName, sValue, 0, NULL ) != IDE_SUCCESS );

        ideLog::log( IDE_SD_0, "[SET-PROP-INTERNAL] SHARD_REBUILD_DATA_STEP=[2]\n" );
    }
    else
    {
        /* Nothing to do */
    }

    // SHARD_REBUILD_DATA_STEP이 0 이 될 때까지 대기
    while ( SDU_SHARD_REBUILD_DATA_STEP != 0 )
    {
        idlOS::sleep( 1 );

        IDE_TEST( iduCheckSessionEvent( aStatistics ) != IDE_SUCCESS );
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/* BUG-45899 */
void sdi::setNonShardQueryReason( sdiPrintInfo * aPrintInfo,
                                  UShort         aReason )
{
    sda::setNonShardQueryReason( aPrintInfo, aReason );
}

void sdi::setPrintInfoFromAnalyzeInfo( sdiPrintInfo   * aPrintInfo,
                                       sdiAnalyzeInfo * aAnalyzeInfo )
{
    sda::setPrintInfoFromAnalyzeInfo( aPrintInfo, aAnalyzeInfo );
}

void sdi::setPrintInfoFromPrintInfo( sdiPrintInfo * aDst,
                                     sdiPrintInfo * aSrc )
{
    sda::setPrintInfoFromPrintInfo( aDst, aSrc );
}

SChar * sdi::getCanNotMergeReasonArr( UShort aArrIdx )
{
    return gCanNotMergeReasonArr[aArrIdx];
}

idBool sdi::isAnalysisInfoPrintable( qcStatement * aStatement )
{
    idBool sResult = ID_FALSE;

    if ( ( isShardCoordinator( aStatement ) == ID_TRUE ) &&
         ( qci::getStartupPhase() == QCI_STARTUP_SERVICE ) &&
         ( QCG_GET_SESSION_TRCLOG_DETAIL_SHARD( aStatement ) == 1 ) )
    {
        sResult = ID_TRUE;
    }

    return sResult;
}

void sdi::printAnalysisInfo( qcStatement  * aStatement,
                             iduVarString * aString )
{
    sdiPrintInfo * sPrintInfo = NULL;

    if ( isAnalysisInfoPrintable( aStatement ) == ID_TRUE )
    {
        sPrintInfo = &(aStatement->mShardPrintInfo);

        iduVarStringAppend( aString, "[ SHARD ANALYSIS INFORMATION ]\n" );

        iduVarStringAppendFormat( aString,
                                  "ANALYSIS COST          : %"ID_UINT32_FMT"\n",
                                  sPrintInfo->mAnalyzeCount );

        if ( sPrintInfo->mQueryType == SDI_QUERY_TYPE_SHARD )
        {
            iduVarStringAppend( aString, "QUERY TYPE             : Shard query\n" );
        }
        else
        {
            if ( sPrintInfo->mQueryType == SDI_QUERY_TYPE_NONSHARD )
            {
                if ( sPrintInfo->mAnalyzeCount > 0 )
                {
                    iduVarStringAppend( aString, "QUERY TYPE             : Non-shard query\n" );
                }
                else
                {
                    // 샤드 객체가 없거나 node[meta]로 지정해서 수행되는 경우 분석 skip
                    iduVarStringAppend( aString, "QUERY TYPE             : -\n" );
                }

                iduVarStringAppend( aString, "NON-SHARD QUERY REASON : ");
                if ( sPrintInfo->mNonShardQueryReason < SDI_SUB_KEY_EXISTS )
                {
                    iduVarStringAppend( aString, gCanNotMergeReasonArr[sPrintInfo->mNonShardQueryReason] );
                }
                else
                {
                    //IDE_DASSERT(0);
                    iduVarStringAppend( aString, gCanNotMergeReasonArr[SDI_UNSUPPORT_SHARD_QUERY] );
                }

                if ( sPrintInfo->mTransformable )
                {
                    iduVarStringAppend( aString, ".\nQUERY TRANSFORMABLE    : Yes\n" );
                }
                else
                {
                    iduVarStringAppend( aString, ".\nQUERY TRANSFORMABLE    : No\n" );
                }
            }
            else
            {
                //IDE_DASSERT(0);
                iduVarStringAppend( aString, gCanNotMergeReasonArr[SDI_UNSUPPORT_SHARD_QUERY] );
                iduVarStringAppend( aString, "\n" );
            }
        }

        iduVarStringAppend( aString,
                            "------------------------------------------------------------\n" );
    }
}

sdiQueryType sdi::getQueryType( qciStatement * aStatement )
{
    // s$statement 로 보여주는 shard query type
    // - trclog_detail_shard 과 무관하게 동작
    // - plan cache 가 적용
    // => 따라서 qcStatement.mShardPrintInfo 가 아닌
    //    qcStatement.myPlan->mShardAnalysis 정보를 우선적으로 제공한다.

    IDE_DASSERT( aStatement != NULL );

    qciShardAnalyzeInfo * sAnalyzeInfo = NULL;
    sdiQueryType          sQueryType = SDI_QUERY_TYPE_NONSHARD;

    if ( ( qci::getStartupPhase() == QCI_STARTUP_SERVICE ) &&
         ( isShardCoordinator( &aStatement->statement ) == ID_TRUE ) )
    {
        if ( qci::getShardAnalyzeInfo( aStatement, &sAnalyzeInfo ) == IDE_SUCCESS )
        {
            sQueryType = ( sAnalyzeInfo->mIsCanMerge == ID_TRUE ) ? SDI_QUERY_TYPE_SHARD: SDI_QUERY_TYPE_NONSHARD;
        }
        else
        {
            if ( aStatement->statement.mShardPrintInfo.mQueryType != SDI_QUERY_TYPE_NONE )
            {
                sQueryType = aStatement->statement.mShardPrintInfo.mQueryType;
            }
        }
    }

    return sQueryType;
}

/* PROJ-2638 */
idBool sdi::isShardEnable()
{
    return ( SDU_SHARD_ENABLE == 1 ? ID_TRUE : ID_FALSE );
}

idBool sdi::isShardCoordinator( qcStatement * aStatement )
{
    idBool   sIsShard = ID_FALSE;
    SChar  * sNodeName;

    // shard enable이어야 하고
    // data node connection이 아니어야 한다. (shard_node_name이 없어야 한다.)
    if ( SDU_SHARD_ENABLE == 1 )
    {
        sNodeName = QCG_GET_SESSION_SHARD_NODE_NAME( aStatement );

        if ( sNodeName == NULL )
        {
            sIsShard = ID_TRUE;
        }
        else
        {
            if ( sNodeName[0] == '\0' )
            {
                sIsShard = ID_TRUE;
            }
        }
    }

    return sIsShard;
}

idBool sdi::isRebuildCoordinator( qcStatement * aStatement )
{
    idBool   sIsRebuildCoord = ID_FALSE;
    SChar  * sNodeName = NULL;

    /*
     * PROJ-2701 Sharding online data rebuild
     *
     * Rebuild coordinator 수행의 조건
     *     1. shard_meta enable이어야 한다.
     *     2. data node connection이어야 한다. (shard_node_name이 있어야 한다.)
     *     3. sessionSMN < dataSMN이어야 한다.
     *
     */
    if ( SDU_SHARD_ENABLE == 1 )
    {
        sNodeName = QCG_GET_SESSION_SHARD_NODE_NAME( aStatement );

        if ( sNodeName != NULL )
        {
            if ( sNodeName[0] != '\0' )
            {
                // Data (node) session
                if ( QCG_GET_SESSION_SHARD_META_NUMBER( aStatement ) < getSMNForDataNode() )
                {
                    // session SMN < data SMN
                    sIsRebuildCoord = ID_TRUE;
                }
            }
        }
    }

    return sIsRebuildCoord;
}

UInt sdi::getShardInternalConnAttrRetryCount()
{
    return SDU_SHARD_INTERNAL_CONN_ATTR_RETRY_COUNT;
}

UInt sdi::getShardInternalConnAttrRetryDelay()
{
    return SDU_SHARD_INTERNAL_CONN_ATTR_RETRY_DELAY;
}

UInt sdi::getShardInternalConnAttrConnTimeout()
{
    return SDU_SHARD_INTERNAL_CONN_ATTR_CONN_TIMEOUT;
}

UInt sdi::getShardInternalConnAttrLoginTimeout()
{
    return SDU_SHARD_INTERNAL_CONN_ATTR_LOGIN_TIMEOUT;
}

void sdi::closeShardSessionByNodeId( qcSession * aSession,
                                     UInt        aNodeId,
                                     UChar       aDestination )
{
    sdiClientInfo  * sClientInfo        = NULL;
    sdiConnectInfo * sConnectInfo       = NULL;
    sdiNode        * sNodeInfo          = NULL;
    UShort           i                  = 0;
    idBool           sFound             = ID_FALSE;

    sClientInfo = aSession->mQPSpecific.mClientInfo;
    IDE_DASSERT( sClientInfo != NULL );

    sConnectInfo = sClientInfo->mConnectInfo;
    for ( i = 0; i < sClientInfo->mCount; i++, sConnectInfo++ )
    {
        sNodeInfo = &sConnectInfo->mNodeInfo;
        if ( sNodeInfo->mNodeId == aNodeId )
        {
            sFound = ID_TRUE;

            if ( sConnectInfo->mFailoverTarget != aDestination )
            {
                qdkCloseShardConnection( sConnectInfo );
            }
            break;
        }
    }

    if ( sFound != ID_TRUE )
    {
        /* impossible case. */
        ideLog::log( IDE_SD_0, "[SHARD META : FAILURE] Not found shard fail-over align destination data node.: node-id %"ID_UINT32_FMT"\n",
                               aNodeId );

        IDE_DASSERT( 0 );
    }
}

void sdi::setTransactionBroken( idBool     aIsUserAutoCommit,
                                void     * aDkiSession,
                                smiTrans * aTrans )
{
    sdl::setTransactionBrokenByTransactionID( aIsUserAutoCommit,
                                              aDkiSession,
                                              aTrans );
}

idBool sdi::isSupportDataType( UInt aModuleID )
{
    return sdm::isSupportDataType( aModuleID );
}

void sdi::shardStmtPartialRollbackUsingSavepoint( qcStatement    * aStatement,
                                                  sdiClientInfo  * aClientInfo,
                                                  sdiDataNodes   * aDataInfo )
{
    sdiConnectInfo * sConnectInfo = aClientInfo->mConnectInfo;
    sdiDataNode    * sDataNode    = aDataInfo->mNodes;
    SInt             i;

    for ( i = 0; i < aClientInfo->mCount; i++, sConnectInfo++, sDataNode++ )
    {
        if ( sDataNode->mSVPStep == SDI_SVP_STEP_SET_SAVEPOINT )
        {
            sDataNode->mSVPStep = SDI_SVP_STEP_ROLLBACK_TO_SAVEPOINT;
            if ( sdi::rollback( sConnectInfo, SAVEPOINT_FOR_SHARD_STMT_PARTIAL_ROLLBACK ) != IDE_SUCCESS )
            {
                sdi::setTransactionBroken( QCG_GET_SESSION_IS_AUTOCOMMIT( aStatement ),
                                           (void*)QCG_GET_DATABASE_LINK_SESSION( aStatement ),
                                           QC_SMI_STMT(aStatement)->mTrans );
            }
        }
    }
}

IDE_RC sdi::waitAndSetSMNForMetaNode( idvSQL       * aStatistics,
                                      smiStatement * aSmiStmt,
                                      UInt           aFlag,
                                      ULong          aNeededSMN,
                                      ULong        * aDataSMN )
{
    /*
     * PROJ-2701 Online data rebuild
     */
    sdiGlobalMetaNodeInfo sMetaNodeInfo;
    ULong                 sDataSMN = ID_ULONG(0);
    UInt                  sSleep = 0;

    smiStatement          sSmiStmt;
    idBool                sIsBeginStmt = ID_FALSE;

    if ( aNeededSMN == ID_ULONG(0) )
    {
        // get dataSMN
        sDataSMN = getSMNForDataNode();
    }
    else
    {
        sDataSMN = aNeededSMN;
    }

    // metaSMNCache가 아직 갱신되지 않았을 수 있다.
    // Shard meta table에서 shard meta number를 직접 읽는다.
    while( getSMNForMetaNode() < sDataSMN )
    {
        IDE_TEST_RAISE( sSleep > SDU_SHARD_META_PROPAGATION_TIMEOUT,
                        ERR_SHARD_META_PROPAGATION_TIMEOUT );

        IDE_TEST( sSmiStmt.begin( aStatistics,
                                  aSmiStmt,
                                  aFlag)
                  != IDE_SUCCESS );
        sIsBeginStmt = ID_TRUE;
                
        IDE_TEST( sdm::getGlobalMetaNodeInfoCore( &sSmiStmt,
                                                  &sMetaNodeInfo )
                  != IDE_SUCCESS );

        IDE_TEST( sSmiStmt.end( SMI_STATEMENT_RESULT_SUCCESS ) != IDE_SUCCESS );
        sIsBeginStmt = ID_FALSE;

        if ( sMetaNodeInfo.mShardMetaNumber >= sDataSMN )
        {
            setSMNCacheForMetaNode( sMetaNodeInfo.mShardMetaNumber );
        }
        else
        {
            acpSleepUsec(1000);
            sSleep += 1000;
        }
    }

    *aDataSMN = sDataSMN;

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_SHARD_META_PROPAGATION_TIMEOUT )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDI_SHARD_META_PROPAGATION_TIMEOUT ) );
    }
    IDE_EXCEPTION_END;

    if ( sIsBeginStmt == ID_TRUE )
    {
        (void)sSmiStmt.end( SMI_STATEMENT_RESULT_FAILURE );
    }

    return IDE_FAILURE;    
}

IDE_RC sdi::convertRangeValue( SChar       * aValue,
                               UInt          aLength,
                               UInt          aKeyType,
                               sdiValue    * aRangeValue )
{
    IDE_TEST( sdm::convertRangeValue( aValue,
                                      aLength,
                                      aKeyType,
                                      aRangeValue )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdi::compareKeyData( UInt       aKeyDataType,
                            sdiValue * aValue1,
                            sdiValue * aValue2,
                            SShort   * aResult )
{
    IDE_TEST( sdm::compareKeyData( aKeyDataType,
                                   aValue1,
                                   aValue2,
                                   aResult )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

idBool sdi::hasShardCoordPlan( qcStatement * aStatement )
{
    qmnPlan     * sPlan = NULL;
    idBool        sRet = ID_FALSE;

    if ( ( isShardCoordinator( aStatement ) == ID_TRUE ) ||
         ( isRebuildCoordinator( aStatement ) == ID_TRUE ) )
    {
        sPlan = aStatement->myPlan->plan;

        if ( sPlan != NULL )
        {
            if ( ( sPlan->type == QMN_SDEX ) ||
                 ( sPlan->type == QMN_SDIN ) )
            {
                sRet = ID_TRUE;
            }
            else
            {
                /* Nothing to do. */
            }
        }
        else
        {
            /* Nothing to do. */
        }

        if ( sRet == ID_FALSE )
        {
            isShardSelectExists( sPlan,
                                 &sRet );
        }
        else
        {
            /* Nothing to do. */
        }
    }
    else
    {
        /* Nothing to do. */
    }

    return sRet;
}

void sdi::isShardSelectExists( qmnPlan * aPlan,
                               idBool  * aIsSDSEExists  )
{
    qmnChildren * sCurrChild = NULL;

    if ( ( *aIsSDSEExists == ID_FALSE ) &&
         ( aPlan != NULL ) )
    {
        if ( aPlan->type == QMN_SDSE )
        {
            *aIsSDSEExists = ID_TRUE;
        }
        else
        {
            isShardSelectExists( aPlan->left,
                                 aIsSDSEExists );

            isShardSelectExists( aPlan->right,
                                 aIsSDSEExists );

            for ( sCurrChild = aPlan->children;
                  sCurrChild != NULL;
                  sCurrChild = sCurrChild->next )
            {
                isShardSelectExists( sCurrChild->childPlan,
                                     aIsSDSEExists );
            }
        }
    }
    else
    {
        /* Nothing to do. */
    }
}

void sdi::getShardObjInfoForSMN( ULong            aSMN,
                                 sdiObjectInfo  * aShardObjectList,
                                 sdiObjectInfo ** aRet )
{
    sdiObjectInfo * sShardObject = NULL;

    IDE_DASSERT( aShardObjectList != NULL );

    *aRet = NULL;

    for ( sShardObject  = aShardObjectList;
          sShardObject != NULL;
          sShardObject  = sShardObject->mNext )
    {
        if ( sShardObject->mSMN == aSMN )
        {
            *aRet = sShardObject;
            break;
        }
        else
        {
            /* Nothing to do */
        }
    }
}

IDE_RC sdi::unionNodeInfo( sdiNodeInfo * aSourceNodeInfo,
                           sdiNodeInfo * aTargetNodeInfo )
{
    UShort sSourceNodeIdx = 0;
    UShort sTargetNodeIdx = 0;
    idBool sIsFound = ID_FALSE;
    
    for ( sSourceNodeIdx = 0;
          sSourceNodeIdx < aSourceNodeInfo->mCount;
          sSourceNodeIdx++, sIsFound = ID_FALSE )
    {
        for ( sTargetNodeIdx = 0;
              sTargetNodeIdx < aTargetNodeInfo->mCount;
              sTargetNodeIdx++ )
        {
            if ( aSourceNodeInfo->mNodes[sSourceNodeIdx].mNodeId == aTargetNodeInfo->mNodes[sTargetNodeIdx].mNodeId )
            {
                sIsFound = ID_TRUE;
                break;
            }
            else
            {
                /* Nothing to do */
            }
        }

        if ( sIsFound == ID_FALSE )
        {
            IDE_TEST_RAISE( aTargetNodeInfo->mCount >= SDI_NODE_MAX_COUNT, ERR_NODE_INFO_OVERFLOW );

            idlOS::memcpy( (void*) &aTargetNodeInfo->mNodes[aTargetNodeInfo->mCount],
                           (void*) &aSourceNodeInfo->mNodes[sSourceNodeIdx],
                           ID_SIZEOF(sdiNode) );

            aTargetNodeInfo->mCount++;
        }
        else
        {
            /* Nothing to do */
        }
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_NODE_INFO_OVERFLOW )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDC_UNEXPECTED_ERROR,
                                  "sdi::unionNodeInfo",
                                  "Too many node count for shard meta changes" ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}
