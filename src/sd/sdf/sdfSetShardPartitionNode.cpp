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
 *
 * Description :
 *
 *     ALTIBASE SHARD management function
 *
 * Syntax :
 *    SHARD_SET_SHARD_PARTITION_NODE_LOCAL( user_name      VARCHAR,
 *                                          table_name     VARCHAR,
 *                                          partition_name VARCHAR,
 *                                          node_name      VARCHAR )
 *    RETURN 0
 *
 **********************************************************************/

#include <sdf.h>
#include <sdm.h>
#include <smi.h>
#include <qcg.h>
#include <sdiZookeeper.h>

extern mtdModule mtdInteger;
extern mtdModule mtdVarchar;

static mtcName sdfFunctionName[1] = {
    { NULL, 30, (void*)"SHARD_SET_SHARD_PARTITION_NODE" }
};

static IDE_RC sdfEstimate( mtcNode*        aNode,
                           mtcTemplate*    aTemplate,
                           mtcStack*       aStack,
                           SInt            aRemain,
                           mtcCallBack*    aCallBack );

mtfModule sdfSetShardPartitionNodeModule = {
    1|MTC_NODE_OPERATOR_MISC|MTC_NODE_VARIABLE_TRUE,
    ~0,
    1.0,                    // default selectivity (비교 연산자 아님)
    sdfFunctionName,
    NULL,
    mtf::initializeDefault,
    mtf::finalizeDefault,
    sdfEstimate
};

IDE_RC sdfCalculate_SetShardPartitionNode( mtcNode*     aNode,
                                           mtcStack*    aStack,
                                           SInt         aRemain,
                                           void*        aInfo,
                                           mtcTemplate* aTemplate );

static const mtcExecute sdfExecute = {
    mtf::calculateNA,
    mtf::calculateNA,
    mtf::calculateNA,
    mtf::calculateNA,
    sdfCalculate_SetShardPartitionNode,
    NULL,
    mtx::calculateNA,
    mtk::estimateRangeNA,
    mtk::extractRangeNA
};

IDE_RC sdfEstimate( mtcNode*     aNode,
                    mtcTemplate* aTemplate,
                    mtcStack*    aStack,
                    SInt      /* aRemain */,
                    mtcCallBack* aCallBack )
{
    const mtdModule* sModules[4] =
    {
        &mtdVarchar, // user_name
        &mtdVarchar, // table_name
        &mtdVarchar, // partition_name
        &mtdVarchar, // node_name
    };
    const mtdModule* sModule = &mtdInteger;

    IDE_TEST_RAISE( ( aNode->lflag & MTC_NODE_QUANTIFIER_MASK ) ==
                    MTC_NODE_QUANTIFIER_TRUE,
                    ERR_NOT_AGGREGATION );

    IDE_TEST_RAISE( ( aNode->lflag & MTC_NODE_ARGUMENT_COUNT_MASK ) != 4,
                    ERR_INVALID_FUNCTION_ARGUMENT );

    IDE_TEST( mtf::makeConversionNodes( aNode,
                                        aNode->arguments,
                                        aTemplate,
                                        aStack + 1,
                                        aCallBack,
                                        sModules )
              != IDE_SUCCESS );

    aStack[0].column = aTemplate->rows[aNode->table].columns + aNode->column;

    IDE_TEST( mtc::initializeColumn( aStack[0].column,
                                     sModule,
                                     0,
                                     0,
                                     0 )
              != IDE_SUCCESS );

    aTemplate->rows[aNode->table].execute[aNode->column] = sdfExecute;

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_NOT_AGGREGATION );
    IDE_SET(ideSetErrorCode(mtERR_ABORT_NOT_AGGREGATION));

    IDE_EXCEPTION( ERR_INVALID_FUNCTION_ARGUMENT );
    IDE_SET(ideSetErrorCode(mtERR_ABORT_INVALID_FUNCTION_ARGUMENT));

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdfCalculate_SetShardPartitionNode( mtcNode*     aNode,
                                           mtcStack*    aStack,
                                           SInt         aRemain,
                                           void*        aInfo,
                                           mtcTemplate* aTemplate )
{
/***********************************************************************
 *
 * Description :
 *      Set Shard Partition Node
 *
 * Implementation :
 *      Argument Validation
 *      Begin Statement for meta
 *      Check Privilege
 *      End Statement
 *
 ***********************************************************************/

    qcStatement             * sStatement;
    mtdCharType             * sUserName;
    SChar                     sUserNameStr[QC_MAX_OBJECT_NAME_LEN + 1];
    mtdCharType             * sTableName;
    SChar                     sTableNameStr[QC_MAX_OBJECT_NAME_LEN + 1];
    mtdCharType             * sNodeName;
    SChar                     sNodeNameStr[SDI_NODE_NAME_MAX_SIZE + 1];
    mtdCharType             * sPartitionName;
    SChar                     sPartitionNameStr[QC_MAX_OBJECT_NAME_LEN + 1];
    SChar                     sValueStr[SDI_RANGE_VARCHAR_MAX_PRECISION + 1];
    sdiSplitMethod            sSplitMethod = SDI_SPLIT_NONE;
    SChar                     sSetFuncTypeStr[2] = "";
    UInt                      sRowCnt = 0;
    smiStatement            * sOldStmt;
    smiStatement              sSmiStmt;
    UInt                      sSmiStmtFlag;
    SInt                      sState = 0;
    idBool                    sIsOldSessionShardMetaTouched = ID_FALSE;
    idBool                    sIsDefault = ID_FALSE;

    ULong                     sSMN = ID_ULONG(0);
    idBool                    sIsTableFound = ID_FALSE;
    sdiTableInfo              sTableInfo;
    sdiLocalMetaInfo          sLocalMetaInfo;
    UInt                      i = 0;

    sdiReplicaSetInfo         sReplicaSetInfo;
    sdiReplicaSetInfo         sFoundReplicaSetInfo;

    sStatement   = ((qcTemplate*)aTemplate)->stmt;
    
    sStatement->mFlag &= ~QC_STMT_SHARD_META_CHANGE_MASK;
    sStatement->mFlag |= QC_STMT_SHARD_META_CHANGE_TRUE;

    /* BUG-47623 샤드 메타 변경에 대한 trc 로그중 commit 로그를 작성하기전에 DASSERT 로 죽는 경우가 있습니다. */
    if ( ( sStatement->session->mQPSpecific.mFlag & QC_SESSION_SHARD_META_TOUCH_MASK ) ==
         QC_SESSION_SHARD_META_TOUCH_TRUE )
    {
        sIsOldSessionShardMetaTouched = ID_TRUE;
    }

    // BUG-46366
    IDE_TEST_RAISE( ( QC_SMI_STMT(sStatement)->getTrans() == NULL ) ||
                    ( ( sStatement->myPlan->parseTree->stmtKind & QCI_STMT_MASK_DML ) == QCI_STMT_MASK_DML ) ||
                    ( ( sStatement->myPlan->parseTree->stmtKind & QCI_STMT_MASK_DCL ) == QCI_STMT_MASK_DCL ),
                    ERR_INSIDE_QUERY );

    // Check Privilege
    IDE_TEST_RAISE( QCG_GET_SESSION_USER_ID(sStatement) != QCI_SYS_USER_ID,
                    ERR_NO_GRANT );

    if ( SDU_SHARD_LOCAL_FORCE != 1 )
    {
        /* Shard Local Operation은 internal 에서만 수행되어야 한다.  */
        IDE_TEST_RAISE( ( QCG_GET_SESSION_IS_SHARD_INTERNAL_LOCAL_OPERATION( sStatement ) != ID_TRUE ) &&
                        ( ( sStatement->session->mQPSpecific.mFlag & QC_SESSION_ALTER_META_MASK )
                             != QC_SESSION_ALTER_META_ENABLE),
                        ERR_INTERNAL_OPERATION );
    }

    // shard package에서 subprogram으로 수행 되는 경우 procedure job type 설정
    if ( (sdiInternalOperation)QCG_GET_SESSION_SHARD_INTERNAL_LOCAL_OPERATION( sStatement ) ==
         SDI_INTERNAL_OP_SHARD_PKG )
    {
        sdf::setProcedureJobType();
    }
    
    IDE_TEST( mtf::postfixCalculate( aNode,
                                     aStack,
                                     aRemain,
                                     aInfo,
                                     aTemplate )
              != IDE_SUCCESS );

    IDE_TEST_RAISE((aStack[1].column->module->isNull (aStack[1].column,
                                                      aStack[1].value) == ID_TRUE) || // user name
                   (aStack[2].column->module->isNull (aStack[2].column,
                                                      aStack[2].value) == ID_TRUE) || // table name
                   (aStack[3].column->module->isNull (aStack[3].column,
                                                      aStack[3].value) == ID_TRUE) || // partition name
                   (aStack[4].column->module->isNull (aStack[4].column,
                                                      aStack[4].value) == ID_TRUE), // node name
                   ERR_ARGUMENT_NOT_APPLICABLE);

    //---------------------------------
    // Argument Validation
    //---------------------------------

    // user name
    sUserName = (mtdCharType*)aStack[1].value;

    IDE_TEST_RAISE( sUserName->length > QC_MAX_OBJECT_NAME_LEN,
                    ERR_SHARD_USER_NAME_TOO_LONG );
    idlOS::strncpy( sUserNameStr,
                    (SChar*)sUserName->value,
                    sUserName->length );
    sUserNameStr[sUserName->length] = '\0';

    // object name
    sTableName = (mtdCharType*)aStack[2].value;

    IDE_TEST_RAISE( sTableName->length > QC_MAX_OBJECT_NAME_LEN,
                    ERR_SHARD_TABLE_NAME_TOO_LONG );
    idlOS::strncpy( sTableNameStr,
                    (SChar*)sTableName->value,
                    sTableName->length );
    sTableNameStr[sTableName->length] = '\0';

    // partition name
    sPartitionName = (mtdCharType*)aStack[3].value;

    IDE_TEST_RAISE( sPartitionName->length > QC_MAX_OBJECT_NAME_LEN,
                    ERR_SHARD_MAX_PARTITION_TOO_LONG );
    idlOS::strncpy( sPartitionNameStr,
                    (SChar*)sPartitionName->value,
                    sPartitionName->length );
    sPartitionNameStr[sPartitionName->length] = '\0';

    // shard node name
    sNodeName = (mtdCharType*)aStack[4].value;

    IDE_TEST_RAISE( sNodeName->length > SDI_CHECK_NODE_NAME_MAX_SIZE,
                    ERR_SHARD_GROUP_NAME_TOO_LONG );
    idlOS::strncpy( sNodeNameStr,
                    (SChar*)sNodeName->value,
                    sNodeName->length );
    sNodeNameStr[sNodeName->length] = '\0';

    IDE_TEST( sdm::getPartitionValueByName( QC_SMI_STMT( sStatement ),
                                            sUserNameStr,
                                            sTableNameStr,
                                            sPartitionNameStr,
                                            SDI_RANGE_VARCHAR_MAX_PRECISION + 1,
                                            sValueStr,
                                            &sIsDefault ) != IDE_SUCCESS);

    // get split method
    IDE_TEST( sdm::getSplitMethodByPartition( QC_SMI_STMT ( sStatement ),
                                              sUserNameStr,
                                              sTableNameStr,
                                              &sSplitMethod ) != IDE_SUCCESS );
    IDE_TEST( sdi::getSplitMethodCharByType(sSplitMethod, &(sSetFuncTypeStr[0])) != IDE_SUCCESS );
    sSetFuncTypeStr[1] = '\0';

    //---------------------------------
    // Begin Statement for meta
    //---------------------------------
    IDE_TEST( sdi::getIncreasedSMNForMetaNode( ( QC_SMI_STMT( sStatement ) )->getTrans() , 
                                               &sSMN ) 
              != IDE_SUCCESS );

    sSmiStmtFlag = SMI_STATEMENT_NORMAL | SMI_STATEMENT_MEMORY_CURSOR;
    sOldStmt                = QC_SMI_STMT(sStatement);
    QC_SMI_STMT(sStatement) = &sSmiStmt;
    sState = 1;

    IDE_TEST( sSmiStmt.begin( sStatement->mStatistics,
                              sOldStmt,
                              sSmiStmtFlag )
              != IDE_SUCCESS );
    sState = 2;

    // 등록된 Shard 객체여야 한다.
    IDE_TEST( sdm::getTableInfo( QC_SMI_STMT( sStatement ),
                                 sUserNameStr,
                                 sTableNameStr,
                                 sSMN, //sMetaNodeInfo.mShardMetaNumber,
                                 &sTableInfo,
                                 &sIsTableFound )
              != IDE_SUCCESS );

    IDE_TEST_RAISE( sIsTableFound == ID_FALSE,
                    ERR_NOT_EXIST_TABLE );

    IDE_TEST( sdm::getAllReplicaSetInfoSortedByPName( QC_SMI_STMT( sStatement ),
                                                      &sReplicaSetInfo,
                                                      sSMN ) 
              != IDE_SUCCESS );

    //---------------------------------
    // Insert or Update Meta
    //---------------------------------
    if ( sIsDefault == ID_TRUE )
    {
        IDE_TEST( sdm::updateDefaultNodeAndPartititon( sStatement,
                                                       sUserNameStr,
                                                       sTableNameStr,
                                                       sNodeNameStr,
                                                       sPartitionNameStr,
                                                       &sRowCnt ) != IDE_SUCCESS );
    }
    else
    {
        IDE_TEST( sdm::insertRange( sStatement,
                                    (SChar*)sUserNameStr,
                                    (SChar*)sTableNameStr,
                                    (SChar*)sPartitionNameStr,
                                    (SChar*)sValueStr, //value
                                    (SChar*)"",
                                    (SChar*)sNodeNameStr,
                                    (SChar*)sSetFuncTypeStr,
                                    &sRowCnt )
                  != IDE_SUCCESS );
    }

    //---------------------------------
    // End Statement
    //---------------------------------

    sState = 1;
    IDE_TEST( sSmiStmt.end( SMI_STATEMENT_RESULT_SUCCESS ) != IDE_SUCCESS );

    sState = 0;
    QC_SMI_STMT(sStatement) = sOldStmt;

    IDE_TEST_RAISE( sRowCnt == 0,
                    ERR_INVALID_SHARD_TABLE );
    
    //---------------------------------
    // Replication Add
    //---------------------------------
    /* Table 대상으로만 이중화를 생성한다. */
    if ( sTableInfo.mObjectType == 'T' )
    {
        IDE_TEST( sdm::getLocalMetaInfo( &sLocalMetaInfo ) != IDE_SUCCESS );

        /* 지정된 Node와 내 NodeName이 일치하면 내가 Sender 이다. */
        if ( idlOS::strncmp( sLocalMetaInfo.mNodeName, sNodeNameStr, SDI_NODE_NAME_MAX_SIZE + 1 ) == 0 )
        {
            /* TASK-7307 DML Data Consistency in Shard 
             *   지정된 Node와 내 NodeName이 일치하면 usable로 변경
             *   그렇지 않으면 unusable로 변경 */
            if ( SDU_SHARD_LOCAL_FORCE != 1 )
            {
                IDE_TEST( sdm::alterUsable( sStatement,
                                            sUserNameStr,
                                            sTableNameStr,
                                            sPartitionNameStr,
                                            ID_TRUE,  // isUsable,
                                            ID_TRUE ) // isNewTrans
                          != IDE_SUCCESS );
            }
            else
            {
                /* Nothing to do */
            }

            /* 이미 생성되어 있는 Repl에 Table을 추가해 주어야 함 */
            for ( i = 0 ; i < sLocalMetaInfo.mKSafety; i++ )
            {
                sSmiStmtFlag = SMI_STATEMENT_NORMAL | SMI_STATEMENT_MEMORY_CURSOR;
                sOldStmt                = QC_SMI_STMT(sStatement);
                QC_SMI_STMT(sStatement) = &sSmiStmt;
                sState = 1;

                IDE_TEST( sSmiStmt.begin( sStatement->mStatistics,
                                          sOldStmt,
                                          sSmiStmtFlag )
                          != IDE_SUCCESS );
                sState = 2;

                /* 내가 보내는 ReplName을 찾아야 한다. */
                IDE_TEST( sdm::findSendReplInfoFromReplicaSet( &sReplicaSetInfo,
                                                               sLocalMetaInfo.mNodeName,
                                                               &sFoundReplicaSetInfo )
                          != IDE_SUCCESS );

                IDE_TEST( sdm::addReplicationItemUsingRPSets( sStatement,
                                                              &sFoundReplicaSetInfo,
                                                              SDM_NA_STR,
                                                              sUserNameStr,
                                                              sTableNameStr,
                                                              sPartitionNameStr,
                                                              i,
                                                              SDM_REPL_SENDER )
                          != IDE_SUCCESS );

                sState = 1;
                IDE_TEST( sSmiStmt.end( SMI_STATEMENT_RESULT_SUCCESS ) != IDE_SUCCESS );

                sState = 0;
                QC_SMI_STMT(sStatement) = sOldStmt;
            }
        }
        else
        {
            /* 내가 Recv Node 인지 확인하여야 한다. */
            for ( i = 0 ; i < sLocalMetaInfo.mKSafety; i++ )
            {
                sSmiStmtFlag = SMI_STATEMENT_NORMAL | SMI_STATEMENT_MEMORY_CURSOR;
                sOldStmt                = QC_SMI_STMT(sStatement);
                QC_SMI_STMT(sStatement) = &sSmiStmt;
                sState = 1;

                IDE_TEST( sSmiStmt.begin( sStatement->mStatistics,
                                          sOldStmt,
                                          sSmiStmtFlag )
                          != IDE_SUCCESS );
                sState = 2;

                /* 내가 받는 ReplName를 찾아야 한다. */
                IDE_TEST( sdm::findRecvReplInfoFromReplicaSet( &sReplicaSetInfo,
                                                               sLocalMetaInfo.mNodeName,
                                                               i,
                                                               &sFoundReplicaSetInfo )
                          != IDE_SUCCESS );

                /* 내가 Recv 중인 RPSet 중에 지정된 Node가 없을수도 있다.
                 * 함수 내부에서 확인 후 처리 */
                IDE_TEST( sdm::addReplicationItemUsingRPSets( sStatement,
                                                              &sFoundReplicaSetInfo,
                                                              sNodeNameStr,
                                                              sUserNameStr,
                                                              sTableNameStr,
                                                              sPartitionNameStr,
                                                              i,
                                                              SDM_REPL_RECEIVER )
                          != IDE_SUCCESS );

                sState = 1;
                IDE_TEST( sSmiStmt.end( SMI_STATEMENT_RESULT_SUCCESS ) != IDE_SUCCESS );

                sState = 0;
                QC_SMI_STMT(sStatement) = sOldStmt;
               
            }
        }

        IDU_FIT_POINT_RAISE( "sdfCalculate_SetShardPartitionNode::replicationItem",
                             ERR_INTERNAL_OPERATION );
        
        // flush 는 SET_SHARD_TABLE_SHARDKEY() 에서 수행.

    }
    
    *(mtdIntegerType*)aStack[0].value = 0;

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_INTERNAL_OPERATION )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDC_UNEXPECTED_ERROR,
                                  "sdfCalculate_SetShardPartitionNode",
                                  "SHARD_INTERNAL_LOCAL_OPERATION is not 1" ) );
    }
    IDE_EXCEPTION( ERR_NOT_EXIST_TABLE )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_SHARD_TABLE_NOT_EXIST ) );
    }
    IDE_EXCEPTION( ERR_INSIDE_QUERY )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QSX_PSM_INSIDE_QUERY ) );
    }
    IDE_EXCEPTION( ERR_SHARD_USER_NAME_TOO_LONG );
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDF_SHARD_USER_NAME_TOO_LONG ) );
    }
    IDE_EXCEPTION( ERR_SHARD_TABLE_NAME_TOO_LONG );
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDF_SHARD_TABLE_NAME_TOO_LONG ) );
    }
    IDE_EXCEPTION( ERR_SHARD_GROUP_NAME_TOO_LONG );
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDF_SHARD_NODE_NAME_TOO_LONG ) );
    }
    IDE_EXCEPTION( ERR_SHARD_MAX_PARTITION_TOO_LONG );
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDF_SHARD_PARTITION_NAME_TOO_LONG ) );
    }
    IDE_EXCEPTION( ERR_ARGUMENT_NOT_APPLICABLE );
    {
        IDE_SET(ideSetErrorCode(mtERR_ABORT_ARGUMENT_NOT_APPLICABLE));
    }
    IDE_EXCEPTION( ERR_INVALID_SHARD_TABLE );
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDF_INVALID_SHARD_TABLE ) );
    }
    IDE_EXCEPTION( ERR_NO_GRANT )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QRC_NO_GRANT ) );
    }
    IDE_EXCEPTION_END;

    IDE_PUSH();

    switch ( sState )
    {
        case 2:
            if ( sSmiStmt.end(SMI_STATEMENT_RESULT_FAILURE) != IDE_SUCCESS )
            {
                IDE_ERRLOG(IDE_SD_1);
            }
            else
            {
                /* Nothing to do */
            }
        case 1:
            QC_SMI_STMT(sStatement) = sOldStmt;
        default:
            break;
    }

    /* BUG-47623 샤드 메타 변경에 대한 trc 로그중 commit 로그를 작성하기전에 DASSERT 로 죽는 경우가 있습니다. */
    if ( sIsOldSessionShardMetaTouched == ID_TRUE )
    {
        sdi::setShardMetaTouched( sStatement->session );
    }
    else
    {
        sdi::unsetShardMetaTouched( sStatement->session );
    }
    
    IDE_POP();
    
    return IDE_FAILURE;
}
