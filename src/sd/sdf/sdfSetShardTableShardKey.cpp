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
 *    SHARD_SET_SHARD_TABLE_SHARDKEY( user_name                  VARCHAR,
 *                                    table_name                 VARCHAR,
 *                                    partition_node_list        VARCHAR,
 *                                    method_fo1r_irregular      VARCHAR,
 *                                    replication_parallel_count INTEGER )
 *    RETURN 0
 *
 **********************************************************************/
#include <sdm.h>
#include <sdf.h>
#include <smi.h>
#include <qcg.h>
#include <sdiZookeeper.h>

extern mtdModule mtdInteger;
extern mtdModule mtdVarchar;

static mtcName sdfFunctionName[1] = {
    { NULL, 30, (void*)"SHARD_SET_SHARD_TABLE_SHARDKEY" }
};

static IDE_RC executeSetShardPartitionNode( qcStatement * aStatement,
                                            SChar       * aUserNameStr,
                                            SChar       * aTableNameStr,
                                            SChar       * aPartitionNameStr,
                                            SChar       * aNodeNameStr );

static IDE_RC executeRemainDataToBackupTableSync( qcStatement    * aStatement,
                                                  iduList        * aNodeInfoList,
                                                  sdfArgument    * aArgumentList,
                                                  SChar          * aUserNameStr,
                                                  SChar          * aTableNameStr,
                                                  SInt             aReplParallelCnt );

static IDE_RC executeReplicationFunction( qcStatement            * aStatement,
                                          iduList                * aNodeInfoList,
                                          sdfArgument            * aArgumentList,
                                          SChar                  * aUserNameStr,
                                          SChar                  * aTableNameStr,
                                          SInt                     aReplParallelCnt,
                                          sdfReplicationFunction   aReplicationFunction );

static IDE_RC irregularOptionError( qcStatement * aStatement,
                                    sdfArgument * aArgumentList,
                                    SChar       * aUserNameStr,
                                    SChar       * aTableNameStr,
                                    SChar       * aIrregularOptStr );

static IDE_RC irregularOptionRemain( qcStatement * aStatement,
                                     iduList     * aNodeInfoList,
                                     sdfArgument * aArgumentList,
                                     SChar       * aUserNameStr,
                                     SChar       * aTableNameStr,
                                     SInt          aReplParallelCnt );

static IDE_RC irregularOptionTruncate( qcStatement * aStatement,
                                       sdfArgument * aArgumentList,
                                       SChar       * aUserNameStr,
                                       SChar       * aTableNameStr,
                                       SChar       * aNodeNameStr,
                                       SChar       * aIrregularOptStr );

static IDE_RC sdfEstimate( mtcNode*        aNode,
                           mtcTemplate*    aTemplate,
                           mtcStack*       aStack,
                           SInt            aRemain,
                           mtcCallBack*    aCallBack );

mtfModule sdfSetShardTableShardKeyModule = {
    1|MTC_NODE_OPERATOR_MISC|MTC_NODE_VARIABLE_TRUE,
    ~0,
    1.0,                    // default selectivity (비교 연산자 아님)
    sdfFunctionName,
    NULL,
    mtf::initializeDefault,
    mtf::finalizeDefault,
    sdfEstimate
};

IDE_RC sdfCalculate_SetShardTableShardKey( mtcNode*     aNode,
                                           mtcStack*    aStack,
                                           SInt         aRemain,
                                           void*        aInfo,
                                           mtcTemplate* aTemplate );

IDE_RC replicationFlushForNode( qcStatement * aStatement,
                                SChar       * aUserNameStr,
                                sdfArgument * aArgumentList );

static const mtcExecute sdfExecute = {
    mtf::calculateNA,
    mtf::calculateNA,
    mtf::calculateNA,
    mtf::calculateNA,
    sdfCalculate_SetShardTableShardKey,
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
    const mtdModule* sModules[5] =
    {
        &mtdVarchar, // user_name
        &mtdVarchar, // table_name
        &mtdVarchar, // partition_node_list 
        &mtdVarchar, // method_for_irregular
        &mtdInteger  // replication_parallel_count     
    };
    const mtdModule* sModule = &mtdInteger;

    IDE_TEST_RAISE( ( aNode->lflag & MTC_NODE_QUANTIFIER_MASK ) ==
                    MTC_NODE_QUANTIFIER_TRUE,
                    ERR_NOT_AGGREGATION );

    IDE_TEST_RAISE( ( aNode->lflag & MTC_NODE_ARGUMENT_COUNT_MASK ) != 5,
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

IDE_RC sdfCalculate_SetShardTableShardKey( mtcNode*     aNode,
                                           mtcStack*    aStack,
                                           SInt         aRemain,
                                           void*        aInfo,
                                           mtcTemplate* aTemplate )
{
    qcStatement             * sStatement = NULL;
    mtdCharType             * sUserName = NULL;
    mtdCharType             * sTableName = NULL;
    SChar                     sUserNameStr[QC_MAX_OBJECT_NAME_LEN + 1];
    SChar                     sTableNameStr[QC_MAX_OBJECT_NAME_LEN + 1];
    SChar                     sKeyColumnNameStr[QC_MAX_OBJECT_NAME_LEN + 1];
    sdiSplitMethod            sSdSplitMethod = SDI_SPLIT_NONE;
    SChar                     sSplitMethodStr[1 + 1];
    ULong                     sSMN = ID_ULONG(0);
    idBool                    sIsAlive = ID_FALSE;
    mtdIntegerType            sReplParallelCnt;
    mtdCharType             * sPartitionNodeName = NULL;
    SChar                     sPartitionNodeNameStr[32000];
    SChar                   * sSqlStr = NULL;
    mtdCharType             * sIrregularOpt = NULL;
    SChar                     sIrregularOptStr[2];
    iduList                 * sNodeInfoList = NULL;
    UInt                      sNodeCount = 0;
    UInt                      sPartitionCnt = 0;
    idBool                    sExistIndex = ID_FALSE;
    sdfArgument             * sPartAndNodeIterator = NULL;
    sdfArgument             * sPartAndNodeList = NULL;
    SChar                     sDefaultPartitionNameStr[QC_MAX_OBJECT_NAME_LEN + 1];
    qcmTablePartitionType     sTablePartitionType = QCM_PARTITIONED_TABLE;
    
    sStatement = ((qcTemplate*)aTemplate)->stmt;

    sStatement->mFlag &= ~QC_STMT_SHARD_META_CHANGE_MASK;
    sStatement->mFlag |= QC_STMT_SHARD_META_CHANGE_TRUE;

    IDE_TEST( STRUCT_ALLOC_WITH_SIZE( sStatement->qmxMem,
                                      SChar,
                                      QD_MAX_SQL_LENGTH,
                                      &sSqlStr )
              != IDE_SUCCESS );
    
    // BUG-46366
    IDE_TEST_RAISE( ( QC_SMI_STMT(sStatement)->getTrans() == NULL ) ||
                    ( ( sStatement->myPlan->parseTree->stmtKind & QCI_STMT_MASK_DML ) == QCI_STMT_MASK_DML ) ||
                    ( ( sStatement->myPlan->parseTree->stmtKind & QCI_STMT_MASK_DCL ) == QCI_STMT_MASK_DCL ),
                    ERR_INSIDE_QUERY );

    // Check Privilege
    IDE_TEST_RAISE( QCG_GET_SESSION_USER_ID(sStatement) != QCI_SYS_USER_ID,
                    ERR_NO_GRANT );

    IDE_TEST( mtf::postfixCalculate( aNode,
                                     aStack,
                                     aRemain,
                                     aInfo,
                                     aTemplate )
              != IDE_SUCCESS );

    if ( ( aStack[1].column->module->isNull( aStack[1].column,
                                             aStack[1].value ) == ID_TRUE ) ||
         ( aStack[2].column->module->isNull( aStack[2].column,
                                             aStack[2].value ) == ID_TRUE ) ||
         ( aStack[3].column->module->isNull( aStack[3].column,
                                             aStack[3].value ) == ID_TRUE ) )
    {
        IDE_RAISE( ERR_ARGUMENT_NOT_APPLICABLE );
    }
    else
    {
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

        // table name
        sTableName = (mtdCharType*)aStack[2].value;

        IDE_TEST_RAISE( sTableName->length > QC_MAX_OBJECT_NAME_LEN,
                        ERR_SHARD_TABLE_NAME_TOO_LONG );
        idlOS::strncpy( sTableNameStr,
                        (SChar*)sTableName->value,
                        sTableName->length );
        sTableNameStr[sTableName->length] = '\0';

        // partition node list
        sPartitionNodeName = (mtdCharType*)aStack[3].value;
        
        IDE_TEST_RAISE( sTableName->length > 32000,
                        ERR_SHARD_TABLE_NAME_TOO_LONG );
        idlOS::strncpy( sPartitionNodeNameStr,
                        (SChar*)sPartitionNodeName->value,
                        sPartitionNodeName->length );
        sPartitionNodeNameStr[sPartitionNodeName->length] = '\0';

        // method_for_irregular
        sIrregularOpt = (mtdCharType*)aStack[4].value;

        // error add
        idlOS::strncpy( sIrregularOptStr,
                        (SChar*)sIrregularOpt->value,
                        sIrregularOpt->length );
        sIrregularOptStr[sIrregularOpt->length] = '\0';        


        // replication_parallel_count
        sReplParallelCnt = *(mtdIntegerType*)aStack[5].value;
        
        // split method
        /* NULL 이면 partition 정보를 가져와서 자동으로 설정한다. */
        IDE_TEST( sdm::getSplitMethodByPartition( QC_SMI_STMT( sStatement ), 
                                                  sUserNameStr, 
                                                  sTableNameStr, 
                                                  &sSdSplitMethod ) != IDE_SUCCESS );

        IDE_TEST( sdi::getSplitMethodCharByType(sSdSplitMethod, &(sSplitMethodStr[0]))
                  != IDE_SUCCESS );
        sSplitMethodStr[1] = '\0';

        //shard key
        /* NULL 이면 partition 정보를 가져와서 자동으로 설정한다. */
        IDE_TEST( sdm::getShardKeyStrByPartition( QC_SMI_STMT( sStatement ), 
                                                  sUserNameStr,
                                                  sTableNameStr,
                                                  QC_MAX_OBJECT_NAME_LEN + 1,
                                                  sKeyColumnNameStr )
                  != IDE_SUCCESS );

        // BUG-49002 Default partition name을 얻어와서 argument list에 포함하는지 검사한다.
        /* Null String을 입력하면 Default Partition 정보를 Return 한다 */
        IDE_TEST( sdm::getPartitionNameByValue( QC_SMI_STMT( sStatement ),
                                                sUserNameStr,
                                                sTableNameStr,
                                                (SChar *)"",
                                                &sTablePartitionType,
                                                sDefaultPartitionNameStr ) != IDE_SUCCESS );

        //---------------------------------
        // Check Session Property
        //---------------------------------
        IDE_TEST_RAISE( QCG_GET_SESSION_IS_AUTOCOMMIT( sStatement ) == ID_TRUE,
                        ERR_COMMIT_STATE );

        IDE_TEST_RAISE( QCG_GET_SESSION_GTX_LEVEL( sStatement ) < 2,
                        ERR_GTX_LEVEL );
        
        IDE_TEST_RAISE ( QCG_GET_SESSION_TRANSACTIONAL_DDL( sStatement) != ID_TRUE,
                         ERR_DDL_TRANSACTION );
        
        /* PROJ-2757 Advanced Global DDL */
        IDE_TEST_RAISE ( QCG_GET_SESSION_GLOBAL_DDL( sStatement) == ID_TRUE,
                         ERR_GLOBAL_DDL );
        
        //---------------------------------
        // check table name and option
        //---------------------------------
        IDE_TEST_RAISE(( sTableName->length +
                         idlOS::strlen(SDI_BACKUP_TABLE_PREFIX) ) > QC_MAX_OBJECT_NAME_LEN,
                       ERR_SHARD_TABLE_NAME_TOO_LONG );

        IDE_TEST_RAISE(( sIrregularOptStr[0] != 'E' ) &&
                       ( sIrregularOptStr[0] != 'R' ) &&
                       ( sIrregularOptStr[0] != 'T' ),
                       ERR_IRREGULAR_OPTION );
        
        //---------------------------------
        // Insert Meta to All Node
        //---------------------------------
        IDE_TEST( sdi::compareDataAndSessionSMN( sStatement )
                  != IDE_SUCCESS );

        IDE_TEST( sdiZookeeper::getShardMetaLock( QC_SMI_STMT(sStatement)->getTrans()->getTransID() )
                  != IDE_SUCCESS );

        sdi::setShardMetaTouched( sStatement->session );

        sdf::setProcedureJobType();
    
        IDE_TEST( sdiZookeeper::checkAllNodeAlive( &sIsAlive )
                  != IDE_SUCCESS );

        IDE_TEST_RAISE( sIsAlive != ID_TRUE, ERR_CLUSTER_STATE );

        IDE_TEST( sdiZookeeper::getAllNodeInfoList( &sNodeInfoList,
                                                    &sNodeCount )
                  != IDE_SUCCESS );
        
        IDE_TEST( sdi::getIncreasedSMNForMetaNode( ( QC_SMI_STMT( sStatement ) )->getTrans() , 
                                                   &sSMN ) 
                  != IDE_SUCCESS );

        IDE_TEST( sdi::checkShardLinker( sStatement ) != IDE_SUCCESS );
        
        //---------------------------------
        // make partition and node name list
        //---------------------------------
        IDE_TEST( sdf::makeArgumentList( sStatement,
                                         sPartitionNodeNameStr,
                                         &sPartAndNodeList,
                                         &sPartitionCnt,
                                         sDefaultPartitionNameStr )
                  != IDE_SUCCESS );

        //---------------------------------
        // check partition cnt, pk, view, node
        //---------------------------------        
        IDE_TEST( sdm::validateShardObjectTable( QC_SMI_STMT(sStatement),
                                                 sUserNameStr,
                                                 sTableNameStr,
                                                 sPartAndNodeList,
                                                 sSdSplitMethod,
                                                 sPartitionCnt,
                                                 &sExistIndex )
                  != IDE_SUCCESS );

        IDE_TEST( sdf::validateNode( sStatement,
                                     sPartAndNodeList,
                                     NULL,
                                     sSdSplitMethod )
                  != IDE_SUCCESS );
        
        //---------------------------------
        // check schema
        //---------------------------------
        // table
        idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                         "SELECT DIGEST( DBMS_METADATA.GET_DDL('TABLE', "
                         "'%s', '%s'), 'SHA-256') A FROM DUAL ",
                         sTableNameStr,
                         sUserNameStr );
        
        IDE_TEST( sdf::checkShardObjectSchema( sStatement,
                                               sSqlStr )
                  != IDE_SUCCESS );
        
        // index
        if ( sExistIndex == ID_TRUE )
        {
            idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                             "SELECT DIGEST( DBMS_METADATA.GET_DEPENDENT_DDL('INDEX', "
                             "'%s', '%s'), 'SHA-256') A FROM DUAL ",
                             sTableNameStr,
                             sUserNameStr );

            IDE_TEST( sdf::checkShardObjectSchema( sStatement,
                                                   sSqlStr )
                      != IDE_SUCCESS );
        }
        else
        {
            idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                             "SELECT COUNT(*) FROM SYSTEM_.SYS_INDICES_ "
                               "WHERE TABLE_ID = ( SELECT TBL.TABLE_ID FROM SYSTEM_.SYS_TABLES_ TBL, "
                                                                           "SYSTEM_.SYS_USERS_ USR "
                                                    "WHERE TBL.TABLE_NAME = '%s' "
                                                      "AND USR.USER_NAME = '%s' "
                                                      "AND TBL.USER_ID = USR.USER_ID )",
                             sTableNameStr,
                             sUserNameStr );

            IDE_TEST( sdf::checkShardObjectSchema( sStatement,
                                                   sSqlStr )
                      != IDE_SUCCESS );
        }

        //---------------------------------
        // irregular option E
        //---------------------------------
        IDE_TEST( irregularOptionError( sStatement,
                                        sPartAndNodeList,
                                        sUserNameStr,
                                        sTableNameStr,
                                        sIrregularOptStr )
                  != IDE_SUCCESS );

        //---------------------------------
        // SET_SHARD_TABLE
        //---------------------------------
        idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                         "exec dbms_shard.set_shard_table_local( '"
                         QCM_SQL_STRING_SKIP_FMT"', '"
                         QCM_SQL_STRING_SKIP_FMT"', "
                         QCM_SQL_VARCHAR_FMT", '"
                         QCM_SQL_STRING_SKIP_FMT"', "
                         "'','','' )",
                         sUserNameStr,
                         sTableNameStr,
                         sSplitMethodStr,
                         sKeyColumnNameStr );
        
        IDE_TEST( sdf::runRemoteQuery( sStatement,
                                       sSqlStr )
                  != IDE_SUCCESS );
        
        //---------------------------------
        // irregular option R
        //---------------------------------
        IDE_TEST( irregularOptionRemain( sStatement,
                                         sNodeInfoList,
                                         sPartAndNodeList,
                                         sUserNameStr,
                                         sTableNameStr,
                                         sReplParallelCnt )
                  != IDE_SUCCESS );
        
        //---------------------------------
        // SET_SHARD_PARTITION_NODE
        //---------------------------------
        for ( sPartAndNodeIterator = sPartAndNodeList;
              sPartAndNodeIterator != NULL;
              sPartAndNodeIterator = sPartAndNodeIterator->mNext )
        {    
            IDE_TEST( executeSetShardPartitionNode( sStatement,
                                                    sUserNameStr,
                                                    sTableNameStr,
                                                    sPartAndNodeIterator->mArgument1,
                                                    sPartAndNodeIterator->mArgument2 )
                      != IDE_SUCCESS );
        }
        
        //---------------------------------
        // REPLICATION FLUSH FOR NODE
        //---------------------------------
        IDE_TEST( replicationFlushForNode( sStatement,
                                           sUserNameStr,
                                           sPartAndNodeList )
                  != IDE_SUCCESS );

        //---------------------------------
        //  irregular option T
        //---------------------------------
        for ( sPartAndNodeIterator = sPartAndNodeList;
              sPartAndNodeIterator != NULL;
              sPartAndNodeIterator = sPartAndNodeIterator->mNext )
        {
            IDE_TEST( irregularOptionTruncate( sStatement,
                                               sPartAndNodeList,
                                               sUserNameStr,
                                               sTableNameStr,
                                               sPartAndNodeIterator->mArgument2,
                                               sIrregularOptStr )
                      != IDE_SUCCESS );
        }
    }
    
    idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                     "COMMIT " );

    IDE_TEST( qciMisc::runDCLforInternal( sStatement,
                                          sSqlStr,
                                          sStatement->session->mMmSession )
              != IDE_SUCCESS );
    
    *(mtdIntegerType*)aStack[0].value = 0;
    
    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_CLUSTER_STATE )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_ZKC_DEADNODE_EXIST ) );
    }
    IDE_EXCEPTION( ERR_COMMIT_STATE )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDF_CANNOT_EXECUTE_IN_AUTOCOMMIT_MODE ) );
    }
    IDE_EXCEPTION( ERR_GTX_LEVEL )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDF_INVALID_GTX_LEVEL ) );
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
    IDE_EXCEPTION( ERR_ARGUMENT_NOT_APPLICABLE );
    {
        IDE_SET(ideSetErrorCode(mtERR_ABORT_ARGUMENT_NOT_APPLICABLE));
    }
    IDE_EXCEPTION( ERR_NO_GRANT )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QRC_NO_GRANT ) );
    }
    IDE_EXCEPTION( ERR_DDL_TRANSACTION );
    {
        IDE_SET(ideSetErrorCode( sdERR_ABORT_SDC_INSUFFICIENT_ATTRIBUTE,
                                 "TANSACTIONAL_DDL = 1" ));
    }
    IDE_EXCEPTION( ERR_GLOBAL_DDL );
    {
        IDE_SET(ideSetErrorCode( sdERR_ABORT_SDC_INSUFFICIENT_ATTRIBUTE,
                                 "GLOBAL_DDL = 0" ));
    }
    IDE_EXCEPTION( ERR_IRREGULAR_OPTION )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDF_INVALID_OPTION,
                                  sIrregularOptStr ) );
    }
    IDE_EXCEPTION_END;

    IDE_PUSH();
    
    idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                     "ROLLBACK " );

    (void) qciMisc::runDCLforInternal( sStatement,
                                       sSqlStr,
                                       sStatement->session->mMmSession );
    IDE_POP();
    
    return IDE_FAILURE;
}

IDE_RC executeSetShardPartitionNode( qcStatement * aStatement,
                                     SChar       * aUserNameStr,
                                     SChar       * aTableNameStr,
                                     SChar       * aPartitionNameStr,
                                     SChar       * aNodeNameStr )
{
    SChar sSqlStr[SDF_QUERY_LEN];
    
    idlOS::snprintf( sSqlStr, SDF_QUERY_LEN,
                     "exec dbms_shard.set_shard_partition_node_local( '"
                     QCM_SQL_STRING_SKIP_FMT"', '"
                     QCM_SQL_STRING_SKIP_FMT"', '"
                     QCM_SQL_STRING_SKIP_FMT"', '"
                     QCM_SQL_STRING_SKIP_FMT"' )",
                     aUserNameStr,
                     aTableNameStr,
                     aPartitionNameStr,
                     aNodeNameStr );

    IDE_TEST( sdf::runRemoteQuery( aStatement,
                                   sSqlStr )
              != IDE_SUCCESS );    
        
    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC executeRemainDataToBackupTableSync( qcStatement    * aStatement,
                                           iduList        * aNodeInfoList,
                                           sdfArgument    * aArgumentList,
                                           SChar          * aUserNameStr,
                                           SChar          * aTableNameStr,
                                           SInt             aReplParallelCnt )
{
    smiStatement   * sOldStmt = NULL;
    smiStatement     sSmiStmt;
    UInt             sSmiStmtFlag = 0;
    SInt             sState = 0;    

    sSmiStmtFlag = SMI_STATEMENT_NORMAL | SMI_STATEMENT_MEMORY_CURSOR;
    sOldStmt                = QC_SMI_STMT(aStatement);
    QC_SMI_STMT(aStatement) = &sSmiStmt;
    sState = 1;

    IDE_TEST( sSmiStmt.begin( aStatement->mStatistics,
                              sOldStmt,
                              sSmiStmtFlag )
              != IDE_SUCCESS );
    sState = 2;

    //---------------------------------
    // CREATE REPLICATION
    //---------------------------------    
    IDE_TEST( executeReplicationFunction( aStatement,
                                          aNodeInfoList,
                                          aArgumentList,
                                          aUserNameStr,
                                          aTableNameStr,
                                          aReplParallelCnt,
                                          SDF_REPL_CREATE )
              != IDE_SUCCESS );

    //---------------------------------
    // ADD REPLICATION
    //---------------------------------    
    IDE_TEST( executeReplicationFunction( aStatement,
                                          aNodeInfoList,
                                          aArgumentList,
                                          aUserNameStr,
                                          aTableNameStr,
                                          aReplParallelCnt,
                                          SDF_REPL_ADD )
              != IDE_SUCCESS );

    //---------------------------------
    // SYNC ONLY OPTION PARALLEL
    //---------------------------------    
    IDE_TEST( executeReplicationFunction( aStatement,
                                          aNodeInfoList,
                                          aArgumentList,
                                          aUserNameStr,
                                          aTableNameStr,
                                          aReplParallelCnt,
                                          SDF_REPL_START )
              != IDE_SUCCESS );
    
    //---------------------------------
    // DROP REPLICATION
    //---------------------------------    
    IDE_TEST( executeReplicationFunction( aStatement,
                                          aNodeInfoList,
                                          aArgumentList,
                                          aUserNameStr,
                                          aTableNameStr,
                                          aReplParallelCnt,
                                          SDF_REPL_DROP )
              != IDE_SUCCESS );
        
    sState = 1;
    IDE_TEST( sSmiStmt.end( SMI_STATEMENT_RESULT_SUCCESS ) != IDE_SUCCESS );

    sState = 0;
    QC_SMI_STMT(aStatement) = sOldStmt;

    // drop replication 되어 revert job에 등록된 replication revert query제거
    sdiZookeeper::removeRevertJob();
                    
    return IDE_SUCCESS;
    
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
            QC_SMI_STMT(aStatement) = sOldStmt;
        default:
            break;
    }
    
    IDE_POP();
    
    return IDE_FAILURE;
}

IDE_RC executeReplicationFunction( qcStatement            * aStatement,
                                   iduList                * aNodeInfoList,
                                   sdfArgument            * aArgumentList,
                                   SChar                  * aUserNameStr,
                                   SChar                  * aTableNameStr,
                                   SInt                     aReplParallelCnt,
                                   sdfReplicationFunction   aReplicationFunction )
{
    ULong               sSMN = ID_ULONG(0);
    sdiGlobalMetaInfo   sMetaNodeInfo = { ID_ULONG(0) };
    sdiReplicaSetInfo   sReplicaSetInfo;
    sdiLocalMetaInfo    sLocalMetaInfo;
    sdiReplicaSetInfo   sSendFoundReplicaSetInfo;
    sdiReplicaSetInfo   sRecvFoundReplicaSetInfo;
    sdfArgument       * sPartAndNodeIterator = NULL;
    UInt                sCnt = 0 ;
    UInt                i = 0 ;
                    
    IDE_TEST( sdm::getLocalMetaInfo( &sLocalMetaInfo ) != IDE_SUCCESS );

    IDE_TEST( sdm::getGlobalMetaInfoCore( QC_SMI_STMT( aStatement ),
                                          &sMetaNodeInfo )
              != IDE_SUCCESS );

    sSMN = sMetaNodeInfo.mShardMetaNumber;
    
    IDE_TEST( sdm::getAllReplicaSetInfoSortedByPName( QC_SMI_STMT( aStatement ),
                                                      &sReplicaSetInfo,
                                                      sSMN ) 
              != IDE_SUCCESS );

    for ( sCnt = 0; sCnt < sReplicaSetInfo.mCount; sCnt++ )
    {
        for ( i = 0 ; i < sLocalMetaInfo.mKSafety; i++ )
        {
            // sender
            IDE_TEST( sdm::findSendReplInfoFromReplicaSet( &sReplicaSetInfo,
                                                           sReplicaSetInfo.mReplicaSets[sCnt].mPrimaryNodeName,
                                                           &sSendFoundReplicaSetInfo )
                      != IDE_SUCCESS );

            // receiver
            IDE_TEST( sdm::findRecvReplInfoFromReplicaSet( &sReplicaSetInfo,
                                                           sReplicaSetInfo.mReplicaSets[sCnt].mPrimaryNodeName,
                                                           i,
                                                           &sRecvFoundReplicaSetInfo )
                      != IDE_SUCCESS );
            

            switch ( aReplicationFunction )
            {
                case SDF_REPL_CREATE:
                case SDF_REPL_DROP:
                    // COMMON
                    IDE_TEST( sdf::searchReplicaSet( aStatement,
                                                     aNodeInfoList,
                                                     &sSendFoundReplicaSetInfo,
                                                     SDM_NA_STR,
                                                     aUserNameStr,
                                                     aTableNameStr,
                                                     NULL,
                                                     aReplParallelCnt,
                                                     aReplicationFunction,
                                                     SDF_REPL_SENDER,
                                                     i )
                              != IDE_SUCCESS );
     
                    IDE_TEST( sdf::searchReplicaSet( aStatement,
                                                     aNodeInfoList,
                                                     &sRecvFoundReplicaSetInfo,
                                                     sReplicaSetInfo.mReplicaSets[sCnt].mPrimaryNodeName,
                                                     aUserNameStr,
                                                     aTableNameStr,
                                                     NULL,
                                                     aReplParallelCnt,
                                                     aReplicationFunction,
                                                     SDF_REPL_RECEIVER,
                                                     i )
                              != IDE_SUCCESS );

                    break;
                case SDF_REPL_ADD:
                    if ( aArgumentList == NULL )
                    {
                        // SET_SHARD_TABLE_SOLO, SET_SHARD_TABLE_CLONE
                        IDE_TEST( sdf::searchReplicaSet( aStatement,
                                                         aNodeInfoList,
                                                         &sSendFoundReplicaSetInfo,
                                                         SDM_NA_STR,
                                                         aUserNameStr,
                                                         aTableNameStr,
                                                         SDM_NA_STR,
                                                         aReplParallelCnt,
                                                         aReplicationFunction,
                                                         SDF_REPL_SENDER,
                                                         i )
                                  != IDE_SUCCESS );

                        IDE_TEST( sdf::searchReplicaSet( aStatement,
                                                         aNodeInfoList,
                                                         &sRecvFoundReplicaSetInfo,
                                                         sReplicaSetInfo.mReplicaSets[sCnt].mPrimaryNodeName,
                                                         aUserNameStr,
                                                         aTableNameStr,
                                                         SDM_NA_STR,
                                                         aReplParallelCnt,
                                                         aReplicationFunction,
                                                         SDF_REPL_RECEIVER,
                                                         i )
                                  != IDE_SUCCESS );
                    }
                    else
                    {
                        // SET_SHARD_TABLE_SHARDKEY
                        for ( sPartAndNodeIterator = aArgumentList;
                              sPartAndNodeIterator != NULL;
                              sPartAndNodeIterator = sPartAndNodeIterator->mNext )
                        {                
                            if ( ( sSendFoundReplicaSetInfo.mCount > 0 ) &&
                                 ( idlOS::strMatch(
                                     sPartAndNodeIterator->mArgument2,
                                     idlOS::strlen(sPartAndNodeIterator->mArgument2),
                                     sSendFoundReplicaSetInfo.mReplicaSets[0].mPrimaryNodeName,
                                     idlOS::strlen(sSendFoundReplicaSetInfo.mReplicaSets[0].mPrimaryNodeName)) == 0 ) )
                            {
                                IDE_TEST( sdf::searchReplicaSet( aStatement,
                                                                 aNodeInfoList,
                                                                 &sSendFoundReplicaSetInfo,
                                                                 SDM_NA_STR,
                                                                 aUserNameStr,
                                                                 aTableNameStr,
                                                                 sPartAndNodeIterator->mArgument1,
                                                                 aReplParallelCnt,
                                                                 aReplicationFunction,
                                                                 SDF_REPL_SENDER,
                                                                 i )
                                          != IDE_SUCCESS );
                            }
                        }

                        for ( sPartAndNodeIterator = aArgumentList;
                              sPartAndNodeIterator != NULL;
                              sPartAndNodeIterator = sPartAndNodeIterator->mNext )
                        {                
                            if ( ( sRecvFoundReplicaSetInfo.mCount > 0 ) &&
                                 ( idlOS::strMatch(
                                     sPartAndNodeIterator->mArgument2,
                                     idlOS::strlen(sPartAndNodeIterator->mArgument2),
                                     sRecvFoundReplicaSetInfo.mReplicaSets[0].mPrimaryNodeName,
                                     idlOS::strlen(sRecvFoundReplicaSetInfo.mReplicaSets[0].mPrimaryNodeName)) == 0 ) )
                            {
                                IDE_TEST( sdf::searchReplicaSet( aStatement,
                                                                 aNodeInfoList,
                                                                 &sRecvFoundReplicaSetInfo,
                                                                 sReplicaSetInfo.mReplicaSets[sCnt].mPrimaryNodeName,
                                                                 aUserNameStr,
                                                                 aTableNameStr,
                                                                 sPartAndNodeIterator->mArgument1,
                                                                 aReplParallelCnt,
                                                                 aReplicationFunction,
                                                                 SDF_REPL_RECEIVER,
                                                                 i )
                                          != IDE_SUCCESS );
                            }
                        }
                    }
                    break;
                case SDF_REPL_START:
                case SDF_REPL_STOP:
                    // COMMON
                    IDE_TEST( sdf::searchReplicaSet( aStatement,
                                                     aNodeInfoList,
                                                     &sSendFoundReplicaSetInfo,
                                                     SDM_NA_STR,
                                                     aUserNameStr,
                                                     aTableNameStr,
                                                     NULL,
                                                     aReplParallelCnt,
                                                     aReplicationFunction,
                                                     SDF_REPL_SENDER,
                                                     i )
                              != IDE_SUCCESS );
                    break;
                default:
                    IDE_DASSERT(0);
                    break;
            }
        }                    
    }
      
    return IDE_SUCCESS;
    
    IDE_EXCEPTION_END;
    
    return IDE_FAILURE;
}

IDE_RC irregularOptionError( qcStatement * aStatement,
                             sdfArgument * aArgumentList,
                             SChar       * aUserNameStr,
                             SChar       * aTableNameStr,
                             SChar       * aIrregularOptStr )
{
    sdiClientInfo  * sClientInfo  = NULL;
    sdiConnectInfo * sConnectInfo = NULL;
    sdfArgument    * sIterator = NULL;
    sdfArgument    * sIterator1 = NULL;
    SChar            sSqlStr[SDF_QUERY_LEN];
    idBool           sFetchResult = ID_FALSE;
    idBool           sIsRangeNode = ID_TRUE;
    UInt             sExecCount = 0;
    ULong            sNumResult = 0;
    ULong            sNodeTotalRow = 0;
    ULong            sNodePartitionRow = 0;
    ULong            sNodeAddPartitionTotalRow = 0;
    UInt             i = 0;

    /*
      argument list에 분산 정의 다음과 같이 설정
      p1 node1, p2 node2, p3 node1, p4 node1

      node1 total row => 40
      node1 각 p1,p2,p3,p4 row => 10
      분산 정의는 p1,p3,p4로 row수의 합은 => 30

      SET_SHARD_TABLE_SHARDKEY()에서 분산 정의에 해당 되지 않는 PARTITION에 데이터가 있으면 에러
      node1 total row (40) - node1(p1+p3+p4)(30) = 10 
      분산 정의 이외의 patition에 데이터가 존재한다 
    */
    if ( aIrregularOptStr[0] == 'E' )
    {
        // 분산 정의에 맞게 데이터 존재 여부 체크
        for ( sIterator = aArgumentList;
              sIterator != NULL;
              sIterator = sIterator->mNext )
        {
            sNodeAddPartitionTotalRow = 0;
                
            // node의 total row 를 구한다.
            idlOS::snprintf( sSqlStr, SDF_QUERY_LEN,
                             "SELECT COUNT(*) FROM "QCM_SQL_STRING_SKIP_FMT"."QCM_SQL_STRING_SKIP_FMT" ",
                             aUserNameStr,
                             aTableNameStr );
                            
            sClientInfo = aStatement->session->mQPSpecific.mClientInfo;
            
            if ( sClientInfo != NULL )
            {
                IDE_TEST( sdi::shardExecDirect( aStatement,
                                                sIterator->mArgument2,
                                                (SChar*)sSqlStr,
                                                (UInt) idlOS::strlen(sSqlStr),
                                                SDI_INTERNAL_OP_NORMAL,
                                                &sExecCount,
                                                &sNodeTotalRow,
                                                NULL,
                                                ID_SIZEOF(sNodeTotalRow),
                                                &sFetchResult )
                          != IDE_SUCCESS );

                IDE_TEST_RAISE( sFetchResult == ID_FALSE, ERR_NO_DATA_FOUND );
            }
            else
            {
                IDE_RAISE( ERR_NODE_NOT_EXIST );
            }

            for ( sIterator1 = aArgumentList;
                  sIterator1 != NULL;
                  sIterator1 = sIterator1->mNext )
            {
                // argument list에 동일한 node의 분산 정의 된 partition의 row를 얻는다.
                if ( idlOS::strMatch( sIterator->mArgument2,
                                      idlOS::strlen(sIterator->mArgument2),
                                      sIterator1->mArgument2,
                                      idlOS::strlen(sIterator1->mArgument2)) == 0 )
                {
                    sIterator1->mIsCheck = ID_TRUE;
                    
                    idlOS::snprintf( sSqlStr, SDF_QUERY_LEN,
                                     "SELECT COUNT(*) I1 FROM "QCM_SQL_STRING_SKIP_FMT"."QCM_SQL_STRING_SKIP_FMT" PARTITION("QCM_SQL_STRING_SKIP_FMT") ",
                                     aUserNameStr,
                                     aTableNameStr,
                                     sIterator1->mArgument1 );
                    
                    IDE_TEST( sdi::shardExecDirect( aStatement,
                                                    sIterator1->mArgument2,
                                                    (SChar*)sSqlStr,
                                                    (UInt) idlOS::strlen(sSqlStr),
                                                    SDI_INTERNAL_OP_NORMAL,
                                                    &sExecCount,
                                                    &sNodePartitionRow,
                                                    NULL,
                                                    ID_SIZEOF(sNodePartitionRow),
                                                    &sFetchResult )
                              != IDE_SUCCESS );

                    IDE_TEST_RAISE( sFetchResult == ID_FALSE, ERR_NO_DATA_FOUND );

                    // 분산정의 된 partition의 total row를 얻는다.
                    sNodeAddPartitionTotalRow += sNodePartitionRow;
                }
                else
                {
                    // nothig to do
                }
            }

            IDE_TEST_RAISE( sNodeTotalRow < sNodeAddPartitionTotalRow,
                            ERR_INVALID_DATA );
            
            IDE_TEST_RAISE( ( sNodeTotalRow - sNodeAddPartitionTotalRow ) > 0,
                            ERR_PARTITION_RECORD_EXIST );
        }
        
        // 분산 정의에 포함 되지 않는 노드의 데이터 존재 여부
        sClientInfo = aStatement->session->mQPSpecific.mClientInfo;
            
        if ( sClientInfo != NULL )
        {
            sConnectInfo = sClientInfo->mConnectInfo;
            
            for ( i = 0; i < sClientInfo->mCount; i++, sConnectInfo++ )
            {
                for ( sIterator = aArgumentList;
                      sIterator != NULL;
                      sIterator = sIterator->mNext )
                {
                    if ( idlOS::strMatch( sConnectInfo->mNodeName,
                                          idlOS::strlen( sConnectInfo->mNodeName ),
                                          sIterator->mArgument2,
                                          idlOS::strlen( sIterator->mArgument2 ) ) == 0 )
                    {
                        sIsRangeNode = ID_TRUE;
                        break;
                    }
                    else
                    {
                        sIsRangeNode = ID_FALSE;
                    }
                }

                if ( sIsRangeNode == ID_FALSE )
                {
                    idlOS::snprintf( sSqlStr, SDF_QUERY_LEN,
                                     "SELECT COUNT(*) FROM "QCM_SQL_STRING_SKIP_FMT"."QCM_SQL_STRING_SKIP_FMT" ",
                                     aUserNameStr,
                                     aTableNameStr );
                    
                    IDE_TEST( sdi::shardExecDirect( aStatement,
                                                    sConnectInfo->mNodeName,
                                                    (SChar*)sSqlStr,
                                                    (UInt) idlOS::strlen(sSqlStr),
                                                    SDI_INTERNAL_OP_NORMAL,
                                                    &sExecCount,
                                                    &sNumResult,
                                                    NULL,
                                                    ID_SIZEOF(sNumResult),
                                                    &sFetchResult )
                              != IDE_SUCCESS );

                    IDE_TEST_RAISE( sFetchResult == ID_FALSE, ERR_NO_DATA_FOUND );

                    IDE_TEST_RAISE( sNumResult > 0, ERR_TABLE_RECORD_EXIST );
                }
            }
        }
        else
        {
            IDE_RAISE( ERR_NODE_NOT_EXIST );
        }
    }

    return IDE_SUCCESS;
    
    IDE_EXCEPTION( ERR_NODE_NOT_EXIST )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_SHARD_NODE_NOT_EXIST ) );
    }
    IDE_EXCEPTION( ERR_PARTITION_RECORD_EXIST )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDF_CHECK_RECORD_EXIST,
                                  sIterator->mArgument2 ));
    }
    IDE_EXCEPTION( ERR_TABLE_RECORD_EXIST )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDF_CHECK_RECORD_EXIST,
                                  sConnectInfo->mNodeName ));
    }
    IDE_EXCEPTION( ERR_NO_DATA_FOUND )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDC_UNEXPECTED_ERROR,
                                  "sdfCalculate_SetShardTableShardKey",
                                  "get row count fetch no data found" ) );
    }
    IDE_EXCEPTION( ERR_INVALID_DATA )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDC_UNEXPECTED_ERROR,
                                  "sdfCalculate_SetShardTableShardKey",
                                  "invaild data" ) );
    }
    IDE_EXCEPTION_END;
    
    return IDE_FAILURE;
}

IDE_RC irregularOptionRemain( qcStatement * aStatement,
                              iduList     * aNodeInfoList,
                              sdfArgument * aArgumentList,
                              SChar       * aUserNameStr,
                              SChar       * aTableNameStr,
                              SInt          aReplParallelCnt )
{
    SChar sSqlStr[SDF_QUERY_LEN];
    UInt  sAccessMode = 0;
    SInt  sAccessState = 0;

    // Truncate, Error Option 모두 분산정의 된 table partition은
    // backup table과 sync
    
    //---------------------------------
    // ACCESS READ ONLY
    //---------------------------------
    IDE_TEST( sdf::getCurrentAccessMode( aStatement,
                                         aTableNameStr,
                                         &sAccessMode )
              != IDE_SUCCESS );

    idlOS::snprintf( sSqlStr, SDF_QUERY_LEN,
                     "ALTER TABLE "QCM_SQL_STRING_SKIP_FMT"."QCM_SQL_STRING_SKIP_FMT"  ACCESS READ ONLY ",
                     aUserNameStr,
                     aTableNameStr );

    IDE_TEST( sdf::remoteSQLWithNewTrans( aStatement,
                                          sSqlStr )
              != IDE_SUCCESS );
    sAccessState = 1;
    
    // sync data and bak table
    IDE_TEST( executeRemainDataToBackupTableSync( aStatement,
                                                  aNodeInfoList,
                                                  aArgumentList,
                                                  aUserNameStr,
                                                  aTableNameStr,
                                                  aReplParallelCnt )
              != IDE_SUCCESS );
            
    //---------------------------------
    // REVERT ACCESS MODE
    //---------------------------------
    switch ( sAccessMode )
    {
        case 1: // Write
            idlOS::snprintf( sSqlStr, SDF_QUERY_LEN,
                             "ALTER TABLE  "QCM_SQL_STRING_SKIP_FMT"."QCM_SQL_STRING_SKIP_FMT"  ACCESS READ WRITE ",
                             aUserNameStr,
                             aTableNameStr );
            break;
        case 2: // Read
            idlOS::snprintf( sSqlStr, SDF_QUERY_LEN,
                             "ALTER TABLE "QCM_SQL_STRING_SKIP_FMT"."QCM_SQL_STRING_SKIP_FMT"  ACCESS READ READ ",
                             aUserNameStr,
                             aTableNameStr );
            break;
        case 3: // Append
            idlOS::snprintf( sSqlStr, SDF_QUERY_LEN,
                             "ALTER TABLE "QCM_SQL_STRING_SKIP_FMT"."QCM_SQL_STRING_SKIP_FMT"  ACCESS READ APPEND ",
                             aUserNameStr,
                             aTableNameStr );
            break;
        default:
            IDE_DASSERT(0);
            break;
    }
            
    IDE_TEST( sdf::remoteSQLWithNewTrans( aStatement,
                                          sSqlStr )
              != IDE_SUCCESS );
    sAccessState = 0;
    
    return IDE_SUCCESS;
    
    IDE_EXCEPTION_END;

    IDE_PUSH();
        
    if ( sAccessState == 1 )
    {
        switch ( sAccessMode )
        {
            case 1: // Write
                idlOS::snprintf( sSqlStr, SDF_QUERY_LEN,
                                 "ALTER TABLE "QCM_SQL_STRING_SKIP_FMT"."QCM_SQL_STRING_SKIP_FMT"  ACCESS READ WRITE ",
                                 aUserNameStr,
                                 aTableNameStr );
                break;
            case 2: // Read
                idlOS::snprintf( sSqlStr, SDF_QUERY_LEN,
                                 "ALTER TABLE "QCM_SQL_STRING_SKIP_FMT"."QCM_SQL_STRING_SKIP_FMT"  ACCESS READ READ ",
                                 aUserNameStr,
                                 aTableNameStr );
                break;
            case 3: // Append
                idlOS::snprintf( sSqlStr, SDF_QUERY_LEN,
                                 "ALTER TABLE "QCM_SQL_STRING_SKIP_FMT"."QCM_SQL_STRING_SKIP_FMT"  ACCESS READ APPEND ",
                                 aUserNameStr,
                                 aTableNameStr );
                break;
            default:
                IDE_DASSERT(0);
                break;
        }
            
        (void)sdf::remoteSQLWithNewTrans( aStatement,
                                          sSqlStr );
    }

    IDE_POP();
    
    return IDE_FAILURE;
}

IDE_RC irregularOptionTruncate( qcStatement * aStatement,
                                sdfArgument * aArgumentList,
                                SChar       * aUserNameStr,
                                SChar       * aTableNameStr,
                                SChar       * aNodeNameStr,
                                SChar       * aIrregularOptStr )
{
    sdiClientInfo  * sClientInfo  = NULL;
    sdiConnectInfo * sConnectInfo = NULL;
    sdfArgument    * sIterator = NULL;
    SChar            sSqlStr[SDF_QUERY_LEN];
    UInt             sExecCount = 0;
    smiStatement   * sOldStmt = NULL;
    smiStatement     sSmiStmt;
    UInt             sSmiStmtFlag = 0;
    SInt             sState = 0;    
    idBool           sIsMyNode = ID_FALSE;
    idBool           sIsRangeNode = ID_FALSE;
    idBool           sIsSameNode = ID_FALSE;
    sdiLocalMetaInfo sLocalMetaInfo;
    UInt             i = 0;
    
    IDE_TEST( sdi::getLocalMetaInfo( &sLocalMetaInfo ) != IDE_SUCCESS );
    
    if ( aIrregularOptStr[0] == 'T' )
    {
        sSmiStmtFlag = SMI_STATEMENT_NORMAL | SMI_STATEMENT_MEMORY_CURSOR;
        sOldStmt                = QC_SMI_STMT(aStatement);
        QC_SMI_STMT(aStatement) = &sSmiStmt;
        sState = 1;

        IDE_TEST( sSmiStmt.begin( aStatement->mStatistics,
                                  sOldStmt,
                                  sSmiStmtFlag )
                  != IDE_SUCCESS );
        sState = 2;

        // 분산 정의에 맞게 DATA TRUNCATE
        for ( sIterator = aArgumentList;
              sIterator != NULL;
              sIterator = sIterator->mNext )
        {
            sIsSameNode = sdi::isSameNode( aNodeNameStr,
                                           sIterator->mArgument2);
            if ( sIsSameNode != ID_TRUE )
            {
                sIsMyNode = sdi::isSameNode(sLocalMetaInfo.mNodeName, aNodeNameStr);

                idlOS::snprintf( sSqlStr, SDF_QUERY_LEN,
                                 "ALTER TABLE "QCM_SQL_STRING_SKIP_FMT" TRUNCATE PARTITION "QCM_SQL_STRING_SKIP_FMT" ",
                                 aTableNameStr,
                                 sIterator->mArgument1 );
                
                if ( sIsMyNode == ID_TRUE )
                {
                    IDE_TEST( qciMisc::runRollbackableInternalDDL( aStatement,
                                                                   QC_SMI_STMT( aStatement ),
                                                                   QCG_GET_SESSION_USER_ID( aStatement ),
                                                                   sSqlStr )
                              != IDE_SUCCESS );
                }
                else
                {
                    sClientInfo = aStatement->session->mQPSpecific.mClientInfo;
            
                    if ( sClientInfo != NULL )
                    {
                        IDE_TEST( sdi::shardExecDirect( aStatement,
                                                        aNodeNameStr,
                                                        (SChar*)sSqlStr,
                                                        (UInt) idlOS::strlen( sSqlStr ),
                                                        SDI_INTERNAL_OP_NORMAL,
                                                        &sExecCount,
                                                        NULL,
                                                        NULL,
                                                        0,
                                                        NULL )
                                  != IDE_SUCCESS );

                        IDE_TEST_RAISE( sExecCount == 0,
                                        ERR_INVALID_SHARD_NODE );
                    }
                    else
                    {
                        IDE_RAISE( ERR_NODE_NOT_EXIST );
                    }
                }
            }
        }
        
        // 분산 정의에 포함 되지 않는 노드의 DATA TRUNCATE
        sClientInfo = aStatement->session->mQPSpecific.mClientInfo;
            
        if ( sClientInfo != NULL )
        {
            sConnectInfo = sClientInfo->mConnectInfo;
            
            for ( i = 0; i < sClientInfo->mCount; i++, sConnectInfo++ )
            {
                for ( sIterator = aArgumentList;
                      sIterator != NULL;
                      sIterator = sIterator->mNext )
                {
                    if ( idlOS::strMatch( sConnectInfo->mNodeName,
                                          idlOS::strlen( sConnectInfo->mNodeName ),
                                          sIterator->mArgument2,
                                          idlOS::strlen( sIterator->mArgument2 ) ) == 0 )
                    {
                        sIsRangeNode = ID_TRUE;
                        break;
                    }
                    else
                    {
                        sIsRangeNode = ID_FALSE;
                    }
                }

                if ( sIsRangeNode == ID_FALSE )
                {
                    sIsMyNode = sdi::isSameNode( sLocalMetaInfo.mNodeName,
                                                 sConnectInfo->mNodeName );
                    
                    idlOS::snprintf( sSqlStr, SDF_QUERY_LEN,
                                     "TRUNCATE TABLE "QCM_SQL_STRING_SKIP_FMT"."QCM_SQL_STRING_SKIP_FMT" ",
                                     aUserNameStr,
                                     aTableNameStr );
                                        
                    if ( sIsMyNode == ID_TRUE )
                    {
                        IDE_TEST( qciMisc::runRollbackableInternalDDL( aStatement,
                                                                       QC_SMI_STMT( aStatement ),
                                                                       QCG_GET_SESSION_USER_ID( aStatement ),
                                                                       sSqlStr )
                                  != IDE_SUCCESS );
                    }
                    else
                    {
                        IDE_TEST( sdi::shardExecDirect( aStatement,
                                                        sConnectInfo->mNodeName,
                                                        (SChar*)sSqlStr,
                                                        (UInt) idlOS::strlen( sSqlStr ),
                                                        SDI_INTERNAL_OP_NORMAL,
                                                        &sExecCount,
                                                        NULL,
                                                        NULL,
                                                        0,
                                                        NULL )
                                  != IDE_SUCCESS );

                        IDE_TEST_RAISE( sExecCount == 0,
                                        ERR_INVALID_SHARD_NODE );
                    }
                }
            }
        }
        else
        {
            IDE_RAISE( ERR_NODE_NOT_EXIST );
        }
        
        sState = 1;
        IDE_TEST( sSmiStmt.end( SMI_STATEMENT_RESULT_SUCCESS ) != IDE_SUCCESS );

        sState = 0;
        QC_SMI_STMT(aStatement) = sOldStmt;
    }
        
    return IDE_SUCCESS;
    
    IDE_EXCEPTION( ERR_NODE_NOT_EXIST )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_SHARD_NODE_NOT_EXIST ) );
    }
    IDE_EXCEPTION( ERR_INVALID_SHARD_NODE );
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDF_INVALID_SHARD_NODE ) );
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
            QC_SMI_STMT(aStatement) = sOldStmt;
        default:
            break;
    }

    IDE_POP();
        
    return IDE_FAILURE;
}

IDE_RC replicationFlushForNode( qcStatement * aStatement,
                                SChar       * aUserNameStr,
                                sdfArgument * aArgumentList )
{
    smiStatement      * sOldStmt = NULL;
    sdfArgument       * sIterator = NULL;
    sdfArgument       * sIterator1 = NULL;
    sdiLocalMetaInfo    sLocalMetaInfo;
    sdiReplicaSetInfo   sReplicaSetInfo;
    sdiReplicaSetInfo   sFoundReplicaSetInfo;
    smiStatement        sSmiStmt;
    UInt                sSmiStmtFlag;
    UInt                i = 0;
    SInt                sState = 0;
    ULong               sSMN = ID_ULONG(0);
    
    IDE_TEST( sdm::getLocalMetaInfo( &sLocalMetaInfo ) != IDE_SUCCESS );

    //---------------------------------
    // Begin Statement for meta
    //---------------------------------
    IDE_TEST( sdi::getIncreasedSMNForMetaNode( ( QC_SMI_STMT( aStatement ) )->getTrans(), 
                                               &sSMN ) 
              != IDE_SUCCESS );

    sSmiStmtFlag = SMI_STATEMENT_NORMAL | SMI_STATEMENT_MEMORY_CURSOR;
    sOldStmt                = QC_SMI_STMT(aStatement);
    QC_SMI_STMT(aStatement) = &sSmiStmt;
    sState = 1;

    IDE_TEST( sSmiStmt.begin( aStatement->mStatistics,
                              sOldStmt,
                              sSmiStmtFlag )
              != IDE_SUCCESS );
    sState = 2;

    IDE_TEST( sdm::getAllReplicaSetInfoSortedByPName( QC_SMI_STMT( aStatement ),
                                                      &sReplicaSetInfo,
                                                      sSMN ) 
              != IDE_SUCCESS );

    //---------------------------------
    // End Statement
    //---------------------------------

    sState = 1;
    IDE_TEST( sSmiStmt.end( SMI_STATEMENT_RESULT_SUCCESS ) != IDE_SUCCESS );

    sState = 0;
    QC_SMI_STMT(aStatement) = sOldStmt;
    
    for ( sIterator = aArgumentList;
          sIterator != NULL;
          sIterator = sIterator->mNext )
    {        
        for ( sIterator1 = aArgumentList;
              sIterator1 != NULL;
              sIterator1 = sIterator1->mNext )
        {
            if ( idlOS::strMatch( sIterator->mArgument2,
                                  idlOS::strlen(sIterator->mArgument2),
                                  sIterator1->mArgument2,
                                  idlOS::strlen(sIterator1->mArgument2)) == 0 )
            {
                if ( sIterator1->mIsFlush == ID_TRUE )
                {
                    sIterator->mIsFlush = ID_TRUE;
                    break;
                }
                else
                {
                    sIterator1->mIsFlush = ID_TRUE;
                    
                    // replication flush
                    for ( i = 0 ; i < sLocalMetaInfo.mKSafety; i++ )
                    {
                        sSmiStmtFlag = SMI_STATEMENT_NORMAL | SMI_STATEMENT_MEMORY_CURSOR;
                        sOldStmt                = QC_SMI_STMT(aStatement);
                        QC_SMI_STMT(aStatement) = &sSmiStmt;
                        sState = 1;

                        IDE_TEST( sSmiStmt.begin( aStatement->mStatistics,
                                                  sOldStmt,
                                                  sSmiStmtFlag )
                                  != IDE_SUCCESS );
                        sState = 2;

                        /* 내가 보내는 ReplName을 찾아야 한다. */
                        IDE_TEST( sdm::findSendReplInfoFromReplicaSet( &sReplicaSetInfo,
                                                                       sIterator1->mArgument2,
                                                                       &sFoundReplicaSetInfo )
                                  != IDE_SUCCESS );

                        IDE_TEST( sdm::flushReplicationItemUsingRPSets( aStatement,
                                                                        &sFoundReplicaSetInfo,
                                                                        aUserNameStr,
                                                                        sIterator1->mArgument2,
                                                                        i,
                                                                        SDM_REPL_SENDER )
                                  != IDE_SUCCESS );

                        sState = 1;
                        IDE_TEST( sSmiStmt.end( SMI_STATEMENT_RESULT_SUCCESS ) != IDE_SUCCESS );

                        sState = 0;
                        QC_SMI_STMT(aStatement) = sOldStmt;
                    }
                    break;                    
                }
            }                            
        }
    }
        
    return IDE_SUCCESS;

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
            QC_SMI_STMT(aStatement) = sOldStmt;
        default:
            break;
    }
    
    IDE_POP();    
    
    return IDE_FAILURE;
}

