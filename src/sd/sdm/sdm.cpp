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

#include <idl.h>
#include <sdi.h>
#include <sdm.h>
#include <mtc.h>
#include <qcmTableInfo.h>
#include <qcmUser.h>
#include <qcg.h>
#include <qcmProc.h>
#include <sduVersion.h>
#include <sdmShardPin.h>

IDE_RC sdm::createMeta( qcStatement * aStatement )
{
    SChar        * sCrtMetaSql[] = {
        (SChar*) "CREATE USER SYS_SHARD IDENTIFIED BY MANAGER"
        ,
        (SChar*) "CREATE SEQUENCE SYS_SHARD.NEXT_NODE_ID MAXVALUE "
QCM_META_SEQ_MAXVALUE_STR" CYCLE"
        ,
        (SChar*) "CREATE SEQUENCE SYS_SHARD.NEXT_SHARD_ID MAXVALUE "
QCM_META_SEQ_MAXVALUE_STR" CYCLE"
        ,
        (SChar*) "CREATE TABLE SYS_SHARD.VERSION_ ( "
"MAJOR_VER          INTEGER, "
"MINOR_VER          INTEGER, "
"PATCH_VER          INTEGER )"
        ,
        (SChar*) "CREATE TABLE SYS_SHARD.LOCAL_META_INFO_ ( "
"META_NODE_ID       INTEGER )"
        ,
        (SChar*) "CREATE TABLE SYS_SHARD.GLOBAL_META_INFO_ ( "
"ID                 INTEGER, "
"SHARD_META_NUMBER  BIGINT )"
        ,
        (SChar*) "CREATE TABLE SYS_SHARD.NODES_ ( "
"NODE_ID            INTEGER                             NOT NULL, "
"NODE_NAME          VARCHAR("QCM_META_NAME_LEN")  FIXED NOT NULL, "
"HOST_IP            VARCHAR(64)                   FIXED NOT NULL, "
"PORT_NO            INTEGER                             NOT NULL, "
"ALTERNATE_HOST_IP  VARCHAR(64)                   FIXED, "
"ALTERNATE_PORT_NO  INTEGER, "
"INTERNAL_HOST_IP            VARCHAR(64)          FIXED NOT NULL, "
"INTERNAL_PORT_NO            INTEGER                    NOT NULL, "
"INTERNAL_ALTERNATE_HOST_IP  VARCHAR(64)          FIXED, "
"INTERNAL_ALTERNATE_PORT_NO  INTEGER, "
"INTERNAL_CONN_TYPE          INTEGER, "
"SMN                BIGINT                              NOT NULL )"
        ,
        (SChar*) "CREATE TABLE SYS_SHARD.OBJECTS_ ( "
"SHARD_ID           INTEGER  NOT NULL, "
"USER_NAME          VARCHAR("QCM_META_OBJECT_NAME_LEN") FIXED NOT NULL, "
"OBJECT_NAME        VARCHAR("QCM_META_OBJECT_NAME_LEN") FIXED NOT NULL, "
"OBJECT_TYPE        CHAR(1)  NOT NULL, "
"SPLIT_METHOD       CHAR(1)  NOT NULL, "
"KEY_COLUMN_NAME    VARCHAR("QCM_META_OBJECT_NAME_LEN") FIXED, "
"SUB_SPLIT_METHOD   CHAR(1), "
"SUB_KEY_COLUMN_NAME VARCHAR("QCM_META_OBJECT_NAME_LEN") FIXED, "
"DEFAULT_NODE_ID    INTEGER, "
"SMN                BIGINT   NOT NULL )"
        ,
        (SChar*) "CREATE TABLE SYS_SHARD.RANGES_ ( "
"SHARD_ID           INTEGER  NOT NULL, "
"VALUE              VARCHAR("SDI_RANGE_VARCHAR_MAX_PRECISION_STR") FIXED NOT NULL, "
"SUB_VALUE          VARCHAR("SDI_RANGE_VARCHAR_MAX_PRECISION_STR") FIXED NOT NULL, "
"NODE_ID            INTEGER  NOT NULL, "
"SMN                BIGINT   NOT NULL )"
        ,
        (SChar*) "CREATE TABLE SYS_SHARD.CLONES_ ( "
"SHARD_ID           INTEGER  NOT NULL, "
"NODE_ID            INTEGER  NOT NULL, "
"SMN                BIGINT   NOT NULL )"
                ,
        (SChar*) "CREATE TABLE SYS_SHARD.SOLOS_ ( "
"SHARD_ID           INTEGER  NOT NULL, "
"NODE_ID            INTEGER  NOT NULL, "
"SMN                BIGINT   NOT NULL )"
        ,
        (SChar*) "CREATE TABLE SYS_SHARD.REBUILD_STATE_ ( VALUE INTEGER )"
        ,
        (SChar*) "ALTER TABLE SYS_SHARD.GLOBAL_META_INFO_ ADD CONSTRAINT GLOBAL_META_INFO_PK "
"PRIMARY KEY ( ID )"
        ,
        (SChar*) "ALTER TABLE SYS_SHARD.NODES_ ADD CONSTRAINT NODES_PK "
"PRIMARY KEY ( NODE_ID, SMN )"
        ,
        (SChar*) "CREATE UNIQUE INDEX SYS_SHARD.NODES_INDEX1 ON "
"SYS_SHARD.NODES_ ( NODE_NAME, SMN )"
        ,
                (SChar*) "CREATE UNIQUE INDEX SYS_SHARD.NODES_INDEX2 ON "
"SYS_SHARD.NODES_ ( SMN, NODE_ID )"
        ,
        (SChar*) "CREATE UNIQUE INDEX SYS_SHARD.NODES_INDEX3 ON "   
"SYS_SHARD.NODES_ ( HOST_IP, PORT_NO, SMN )"                             
        ,
/* PROJ-2701 Online data rebuild
 * nullable인 alternate_... columns를 위하여 function-based index를 구성한다.
 */     
        (SChar*) "CREATE UNIQUE INDEX SYS_SHARD.NODES_INDEX4 ON "   
"SYS_SHARD.NODES_ ( CASE WHEN ALTERNATE_HOST_IP IS NOT NULL "
"                         AND ALTERNATE_PORT_NO IS NOT NULL "
"                        THEN ALTERNATE_HOST_IP||','||ALTERNATE_PORT_NO||','||SMN END )"
        ,
        (SChar*) "CREATE UNIQUE INDEX SYS_SHARD.NODES_INDEX5 ON "   
"SYS_SHARD.NODES_ ( INTERNAL_HOST_IP, INTERNAL_PORT_NO, SMN )"                             
        ,
        (SChar*) "CREATE UNIQUE INDEX SYS_SHARD.NODES_INDEX6 ON "   
"SYS_SHARD.NODES_ ( CASE WHEN INTERNAL_ALTERNATE_HOST_IP IS NOT NULL "
"                         AND INTERNAL_ALTERNATE_PORT_NO IS NOT NULL "
"                        THEN INTERNAL_ALTERNATE_HOST_IP||','||INTERNAL_ALTERNATE_PORT_NO||','||SMN END )"
        ,
        (SChar*) "ALTER TABLE SYS_SHARD.OBJECTS_ ADD CONSTRAINT OBJECTS_PK "
"PRIMARY KEY ( SHARD_ID, SMN )"
        ,
        (SChar*) "CREATE UNIQUE INDEX SYS_SHARD.OBJECTS_INDEX1 ON "
"SYS_SHARD.OBJECTS_ ( USER_NAME, OBJECT_NAME, SMN )"
        ,
        (SChar*) "ALTER TABLE SYS_SHARD.RANGES_ ADD CONSTRAINT RANGES_PK "
"PRIMARY KEY ( SHARD_ID, SMN, VALUE, SUB_VALUE )"
        ,
        (SChar*) "ALTER TABLE SYS_SHARD.CLONES_ ADD CONSTRAINT CLONES_PK "
"PRIMARY KEY ( SHARD_ID, SMN, NODE_ID )"
        ,
        (SChar*) "ALTER TABLE SYS_SHARD.SOLOS_ ADD CONSTRAINT SOLOS_PK "
"PRIMARY KEY ( SHARD_ID, SMN )"
        ,
        (SChar*) "INSERT INTO SYS_SHARD.VERSION_ VALUES ("
SHARD_MAJOR_VERSION_STR", "
SHARD_MINOR_VERSION_STR", "
SHARD_PATCH_VERSION_STR")"
        ,
        (SChar*) "INSERT INTO SYS_SHARD.GLOBAL_META_INFO_ VALUES ( INTEGER'0', BIGINT'1' )"
        ,
/* The last item should be NULL */
        NULL    };

    smiTrans       sTrans;
    smiStatement   sSmiStmt;
    smiStatement * sDummySmiStmt;
    smSCN          sDummySCN;
    SChar        * sSqlStr;
    vSLong         sRowCnt;
    UInt           sStage = 0;
    UInt           i;
    UInt           sShardUserID = ID_UINT_MAX;

    IDE_TEST( STRUCT_ALLOC_WITH_SIZE( aStatement->qmxMem,
                                      SChar,
                                      QD_MAX_SQL_LENGTH,
                                      &sSqlStr )
              != IDE_SUCCESS );

    IDE_TEST( sTrans.initialize() != IDE_SUCCESS );
    sStage = 1;

    for (i = 0; sCrtMetaSql[i] != NULL; i++)
    {
        IDE_TEST( sTrans.begin( &sDummySmiStmt,
                                NULL,
                                (SMI_ISOLATION_NO_PHANTOM     |
                                 SMI_TRANSACTION_NORMAL       |
                                 SMI_TRANSACTION_REPL_DEFAULT |
                                 SMI_COMMIT_WRITE_NOWAIT))
                  != IDE_SUCCESS );
        sStage = 2;

        IDE_TEST( sSmiStmt.begin( NULL,
                                  sDummySmiStmt,
                                  (SMI_STATEMENT_NORMAL |
                                   SMI_STATEMENT_MEMORY_CURSOR) )
                  != IDE_SUCCESS );
        sStage = 3;

        idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                         "%s",
                         sCrtMetaSql[i] );

        IDE_TEST( qciMisc::runSQLforShardMeta( &sSmiStmt,
                                               sSqlStr,
                                               &sRowCnt )
                  != IDE_SUCCESS );

        sStage = 2;
        IDE_TEST( sSmiStmt.end( SMI_STATEMENT_RESULT_SUCCESS )
                  != IDE_SUCCESS );

        sStage = 1;
        IDE_TEST( sTrans.commit( &sDummySCN ) != IDE_SUCCESS );

        ideLog::log( IDE_SD_0, "[SHARD META] %s\n", sCrtMetaSql[i] );
    }

    sStage = 0;
    IDE_TEST( sTrans.destroy( NULL ) != IDE_SUCCESS );

    /* ShardUserID를 세팅한다. */
    IDE_TEST( getShardUserID( &sShardUserID ) != IDE_SUCCESS );
    sdi::setShardUserID( sShardUserID );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    switch ( sStage )
    {
        case 3:
            ( void )sSmiStmt.end( SMI_STATEMENT_RESULT_FAILURE );
        case 2:
            ( void )sTrans.commit( &sDummySCN );
        case 1:
            ( void )sTrans.destroy( NULL );
        default:
            break;
    }

    ideLog::log( IDE_SD_0, "[SHARD META : FAILURE] errorcode 0x%05"ID_XINT32_FMT" %s\n",
                           E_ERROR_CODE(ideGetErrorCode()),
                           ideGetErrorMsg(ideGetErrorCode()));
    return IDE_FAILURE;
}

IDE_RC sdm::checkMetaVersion( smiStatement * aSmiStmt )
{
    UInt                 sStage = 0;
    smiTableCursor       sCursor;
    const void         * sRow;
    UInt                 sFlag = QCM_META_CURSOR_FLAG;
    const void         * sSdmVersion;
    mtcColumn          * sSdmVersionMajorVerColumn;
    mtcColumn          * sSdmVersionMinorVerColumn;
    mtcColumn          * sSdmVersionPatchVerColumn;
    mtdIntegerType       sMajorVer;
    mtdIntegerType       sMinorVer;
    mtdIntegerType       sPatchVer;

    scGRID               sRid; // Disk Table을 위한 Record IDentifier
    smiCursorProperties  sCursorProperty;

    IDE_TEST( getMetaTableAndIndex( aSmiStmt,
                                    SDM_VERSION,
                                    & sSdmVersion,
                                    NULL )
              != IDE_SUCCESS );

    IDE_TEST( smiGetTableColumns( sSdmVersion,
                                  SDM_VERSION_MAJOR_VER_COL_ORDER,
                                  (const smiColumn**)&sSdmVersionMajorVerColumn )
              != IDE_SUCCESS );

    IDE_TEST( smiGetTableColumns( sSdmVersion,
                                  SDM_VERSION_MINOR_VER_COL_ORDER,
                                  (const smiColumn**)&sSdmVersionMinorVerColumn )
              != IDE_SUCCESS );

    IDE_TEST( smiGetTableColumns( sSdmVersion,
                                  SDM_VERSION_PATCH_VER_COL_ORDER,
                                  (const smiColumn**)&sSdmVersionPatchVerColumn )
              != IDE_SUCCESS );

    sCursor.initialize();

    SMI_CURSOR_PROP_INIT_FOR_META_FULL_SCAN(&sCursorProperty, NULL);

    IDE_TEST(sCursor.open(
                 aSmiStmt,
                 sSdmVersion,
                 NULL,
                 smiGetRowSCN(sSdmVersion),
                 NULL,
                 smiGetDefaultKeyRange(),
                 smiGetDefaultKeyRange(),
                 smiGetDefaultFilter(),
                 sFlag,
                 SMI_SELECT_CURSOR,
                 &sCursorProperty) != IDE_SUCCESS);
    sStage = 1;

    IDE_TEST(sCursor.beforeFirst() != IDE_SUCCESS);

    IDE_TEST(sCursor.readRow(&sRow, &sRid, SMI_FIND_NEXT) != IDE_SUCCESS);
    // 1건도 없으면 에러
    IDE_TEST_RAISE( sRow == NULL, ERR_CHECK_META_VERSION );

    sMajorVer = *(mtdIntegerType*)((SChar *)sRow + sSdmVersionMajorVerColumn->column.offset );
    sMinorVer = *(mtdIntegerType*)((SChar *)sRow + sSdmVersionMinorVerColumn->column.offset );
    sPatchVer = *(mtdIntegerType*)((SChar *)sRow + sSdmVersionPatchVerColumn->column.offset );

    IDE_TEST(sCursor.readRow(&sRow, &sRid, SMI_FIND_NEXT) != IDE_SUCCESS);
    // 1건이어야 한다.
    IDE_TEST_RAISE( sRow != NULL, ERR_CHECK_META_VERSION );

    sStage = 0;
    IDE_TEST(sCursor.close() != IDE_SUCCESS);

    IDE_TEST_RAISE( ( sMajorVer != SHARD_MAJOR_VERSION ) ||
                    ( sMinorVer != SHARD_MINOR_VERSION ) ||
                    ( sPatchVer != SHARD_PATCH_VERSION ),
                    ERR_MISMATCH_META_VERSION );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_CHECK_META_VERSION )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_CHECK_META_VERSION ) );
    }
    IDE_EXCEPTION( ERR_MISMATCH_META_VERSION )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_MISMATCH_META_VERSION ) );
    }
    IDE_EXCEPTION_END;

    if ( sStage == 1 )
    {
        (void)sCursor.close();
    }
    else
    {
        /* Nothing to do */
    }

    return IDE_FAILURE;
}

IDE_RC sdm::resetMetaNodeId( qcStatement * aStatement, sdiLocalMetaNodeInfo * aMetaNodeInfo )
{
    smiTrans       sTrans;
    smiStatement   sSmiStmt;
    smiStatement * sDummySmiStmt = NULL;
    smSCN          sDummySCN;
    SChar        * sSqlStr       = NULL;
    vSLong         sRowCnt       = 0;
    UInt           sStage        = 0;

    IDE_TEST( STRUCT_ALLOC_WITH_SIZE( aStatement->qmxMem,
                                      SChar,
                                      QD_MAX_SQL_LENGTH,
                                      &sSqlStr )
              != IDE_SUCCESS );

    IDE_TEST( sTrans.initialize() != IDE_SUCCESS );
    sStage = 1;

    IDE_TEST( sTrans.begin( &sDummySmiStmt,
                            NULL,
                            ( SMI_ISOLATION_NO_PHANTOM     |
                              SMI_TRANSACTION_NORMAL       |
                              SMI_TRANSACTION_REPL_DEFAULT |
                              SMI_COMMIT_WRITE_NOWAIT ) )
              != IDE_SUCCESS );
    sStage = 2;

    IDE_TEST( sSmiStmt.begin( NULL,
                              sDummySmiStmt,
                              ( SMI_STATEMENT_NORMAL |
                                SMI_STATEMENT_MEMORY_CURSOR ) )
              != IDE_SUCCESS );
    sStage = 3;

    IDE_TEST( checkMetaVersion( &sSmiStmt )
              != IDE_SUCCESS );

    idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                     "DELETE FROM SYS_SHARD.LOCAL_META_INFO_" );

    IDE_TEST( qciMisc::runDMLforDDL( &sSmiStmt,
                                     sSqlStr,
                                     &sRowCnt )
              != IDE_SUCCESS );

    idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                     "INSERT INTO SYS_SHARD.LOCAL_META_INFO_ "
                     "VALUES ( "
                     QCM_SQL_INT32_FMT" )",
                     aMetaNodeInfo->mMetaNodeId );

    IDE_TEST( qciMisc::runDMLforDDL( &sSmiStmt,
                                     sSqlStr,
                                     &sRowCnt )
              != IDE_SUCCESS );

    IDE_TEST_RAISE( sRowCnt == 0, ERR_INSERT );

    IDE_TEST( sSmiStmt.end( SMI_STATEMENT_RESULT_SUCCESS )
              != IDE_SUCCESS );
    sStage = 2;

    IDE_TEST( sTrans.commit( &sDummySCN ) != IDE_SUCCESS );
    sStage = 1;

    sStage = 0;
    IDE_TEST( sTrans.destroy( NULL ) != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_INSERT );
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QCM_INTERNAL_ARG,
                                  "[sdm::resetMetaNodeId] ERR_INSERTED_COUNT_IS_NOT_1" ) ); 
    }
    IDE_EXCEPTION_END;

    IDE_PUSH();

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

    IDE_POP();

    ideLog::log( IDE_SD_0, "[SHARD META : FAILURE] errorcode 0x%05"ID_XINT32_FMT" %s\n",
                           E_ERROR_CODE(ideGetErrorCode()),
                           ideGetErrorMsg(ideGetErrorCode()));
    return IDE_FAILURE;
}

IDE_RC sdm::getMetaTableAndIndex( smiStatement * aSmiStmt,
                                  const SChar  * aMetaTableName,
                                  const void  ** aTableHandle,
                                  const void  ** aIndexHandle )
{
    UInt        sUserID;
    smSCN       sTableSCN;
    //mtcColumn * sColumn;
    //UInt        sColumnCnt;
    UInt        sIndexCnt;
    UInt        i;

    IDE_TEST_RAISE( qcmUser::getUserID( NULL,
                                        aSmiStmt,
                                        SDM_USER,
                                        idlOS::strlen(SDM_USER),
                                        & sUserID ) != IDE_SUCCESS,
                    ERR_META_HANDLE );

    IDE_TEST_RAISE( qcm::getTableHandleByName( aSmiStmt,
                                               sUserID,
                                               (UChar *)aMetaTableName,
                                               idlOS::strlen(aMetaTableName),
                                               (void**)aTableHandle,
                                               &sTableSCN ) != IDE_SUCCESS,
                    ERR_META_HANDLE );

    if ( aIndexHandle != NULL )
    {
        sIndexCnt = smiGetTableIndexCount( *aTableHandle );

        for ( i = 0; i < sIndexCnt; i++ )
        {
            aIndexHandle[i] = smiGetTableIndexes( *aTableHandle,
                                                  i );
        }
        for ( ; i < SDM_MAX_META_INDICES; i++ )
        {
            aIndexHandle[i] = NULL;
        }
    }
    else
    {
        /* Nothing to do */
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_META_HANDLE )
    {
        if ( ideGetErrorCode() == qpERR_ABORT_QCM_NOT_EXIST_USER )
        {
            IDE_SET( ideSetErrorCode(sdERR_ABORT_SDM_SHARD_META_NOT_CREATED,
                                     SDM_USER) );
        }
        else if ( ideGetErrorCode() == qpERR_ABORT_QCM_NOT_EXIST_TABLE )
        {
            IDE_SET( ideSetErrorCode(sdERR_ABORT_SDM_SHARD_META_NOT_CREATED,
                                     aMetaTableName) );
        }
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdm::getNextNodeID( qcStatement * aStatement,
                           UInt        * aNodeID,
                           ULong         aSMN )
{
    UInt     sUserID;
    void   * sSequenceHandle = NULL;
    idBool   sExist;
    idBool   sFirst;
    SLong    sSeqVal = 0;
    SLong    sSeqValFirst = 0;

    sExist = ID_TRUE;
    sFirst = ID_TRUE;

    IDE_TEST_RAISE( qcmUser::getUserID( QC_STATISTICS( aStatement ),
                                        QC_SMI_STMT( aStatement ),
                                        SDM_USER,
                                        idlOS::strlen(SDM_USER),
                                        & sUserID ) != IDE_SUCCESS,
                    ERR_META_HANDLE );

    IDE_TEST_RAISE( qciMisc::getSequenceHandleByName(
                        QC_SMI_STMT( aStatement ),
                        sUserID,
                        (UChar*)SDM_SEQUENCE_NODE_NAME,
                        idlOS::strlen(SDM_SEQUENCE_NODE_NAME),
                        &sSequenceHandle ) != IDE_SUCCESS,
                    ERR_META_HANDLE );

    while ( sExist == ID_TRUE )
    {
        IDE_TEST( smiTable::readSequence( QC_SMI_STMT( aStatement ),
                                          sSequenceHandle,
                                          SMI_SEQUENCE_NEXT,
                                          &sSeqVal,
                                          NULL )
                 != IDE_SUCCESS );

        // sSeqVal은 비록 SLong이지만, sequence를 생성할 때
        // max를 integer max를 안넘도록 하였기 때문에
        // 여기서 overflow체크는 하지 않는다.
        IDE_TEST( searchNodeID( QC_SMI_STMT( aStatement ),
                                (SInt)sSeqVal,
                                aSMN,
                                &sExist )
                  != IDE_SUCCESS );

        if ( sFirst == ID_TRUE )
        {
            sSeqValFirst = sSeqVal;
            sFirst = ID_FALSE;
        }
        else
        {
            // 찾다찾다 한바퀴 돈 경우.
            // 이는 object가 꽉 찬 것을 의미함.
            IDE_TEST_RAISE( sSeqVal == sSeqValFirst, ERR_OBJECTS_OVERFLOW );
        }
    }

    *aNodeID = sSeqVal;

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_META_HANDLE )
    {
        if ( ideGetErrorCode() == qpERR_ABORT_QCM_NOT_EXIST_USER )
        {
            IDE_SET( ideSetErrorCode(sdERR_ABORT_SDM_SHARD_META_NOT_CREATED,
                                     SDM_USER) );
        }
        else if ( ideGetErrorCode() == qpERR_ABORT_QCM_NOT_EXIST_SEQUENCE )
        {
            IDE_SET( ideSetErrorCode(sdERR_ABORT_SDM_SHARD_META_NOT_CREATED,
                                     SDM_SEQUENCE_NODE_NAME) );
        }
    }
    IDE_EXCEPTION( ERR_OBJECTS_OVERFLOW )
    {
        IDE_SET(ideSetErrorCode(qpERR_ABORT_QCM_MAXIMUM_OBJECT_NUMBER_EXCEED,
                                "shard node",
                                QCM_META_SEQ_MAXVALUE) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdm::searchNodeID( smiStatement * aSmiStmt,
                          SInt           aNodeID,
                          ULong          aSMN,
                          idBool       * aExist )
{
    UInt                 sStage = 0;
    smiRange             sRange;
    smiTableCursor       sCursor;
    const void         * sRow;
    UInt                 sFlag = QCM_META_CURSOR_FLAG;
    const void         * sSdmNodes;
    const void         * sSdmNodesIndex[SDM_MAX_META_INDICES];
    mtcColumn          * sSdmNodeIDColumn;
    qtcMetaRangeColumn   sNodeIDRange;
    mtcColumn          * sSdmSMNColumn;
    qtcMetaRangeColumn   sSMNRange;

    scGRID              sRid; // Disk Table을 위한 Record IDentifier
    smiCursorProperties sCursorProperty;

    IDE_TEST( getMetaTableAndIndex( aSmiStmt,
                                    SDM_NODES,
                                    & sSdmNodes,
                                    sSdmNodesIndex )
              != IDE_SUCCESS );

    IDE_TEST_RAISE( sSdmNodesIndex[SDM_NODES_IDX1_ORDER] == NULL,
                    ERR_META_HANDLE );

    IDE_TEST( smiGetTableColumns( sSdmNodes,
                                  SDM_NODES_NODE_ID_COL_ORDER,
                                  (const smiColumn**)&sSdmNodeIDColumn )
              != IDE_SUCCESS );

    IDE_TEST( smiGetTableColumns( sSdmNodes,
                                  SDM_NODES_SMN_COL_ORDER,
                                  (const smiColumn**)&sSdmSMNColumn )
              != IDE_SUCCESS );

    // mtdModule 설정
    IDE_TEST( mtd::moduleById( &(sSdmNodeIDColumn->module),
                               sSdmNodeIDColumn->type.dataTypeId )
              != IDE_SUCCESS );

    // mtlModule 설정
    IDE_TEST( mtl::moduleById( &sSdmNodeIDColumn->language,
                               sSdmNodeIDColumn->type.languageId )
              != IDE_SUCCESS );

    // mtdModule 설정
    IDE_TEST( mtd::moduleById( &(sSdmSMNColumn->module),
                               sSdmSMNColumn->type.dataTypeId )
              != IDE_SUCCESS );

    // mtlModule 설정
    IDE_TEST( mtl::moduleById( &sSdmSMNColumn->language,
                               sSdmSMNColumn->type.languageId )
              != IDE_SUCCESS );

    qciMisc::makeMetaRangeDoubleColumn(
        &sNodeIDRange,
        &sSMNRange,
        (const mtcColumn *) sSdmNodeIDColumn,
        (const void *) &aNodeID,
        (const mtcColumn *) sSdmSMNColumn,
        (const void *) &aSMN,        
        &sRange);

    sCursor.initialize();

    SMI_CURSOR_PROP_INIT_FOR_META_INDEX_SCAN(&sCursorProperty, NULL);

    IDE_TEST(sCursor.open(
                 aSmiStmt,
                 sSdmNodes,
                 sSdmNodesIndex[SDM_NODES_IDX1_ORDER],
                 smiGetRowSCN(sSdmNodes),
                 NULL,
                 &sRange,
                 smiGetDefaultKeyRange(),
                 smiGetDefaultFilter(),
                 sFlag,
                 SMI_SELECT_CURSOR,
                 &sCursorProperty) != IDE_SUCCESS);
    sStage = 1;

    IDE_TEST(sCursor.beforeFirst() != IDE_SUCCESS);

    IDE_TEST(sCursor.readRow(&sRow, &sRid, SMI_FIND_NEXT) != IDE_SUCCESS);

    if ( sRow == NULL )
    {
        *aExist = ID_FALSE;
    }
    else
    {
        *aExist = ID_TRUE;
    }

    sStage = 0;
    IDE_TEST(sCursor.close() != IDE_SUCCESS);

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_META_HANDLE )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_SHARD_META_NOT_CREATED,
                                  SDM_NODES ) );
    }
    IDE_EXCEPTION_END;

    if ( sStage == 1 )
    {
        (void)sCursor.close();
    }
    else
    {
        /* Nothing to do */
    }

    return IDE_FAILURE;
}

IDE_RC sdm::getNextShardID( qcStatement * aStatement,
                            UInt        * aShardID,
                            ULong         aSMN )
{
    UInt     sUserID;
    void   * sSequenceHandle = NULL;
    idBool   sExist;
    idBool   sFirst;
    SLong    sSeqVal = 0;
    SLong    sSeqValFirst = 0;

    sExist = ID_TRUE;
    sFirst = ID_TRUE;

    IDE_TEST_RAISE( qcmUser::getUserID( QC_STATISTICS( aStatement ),
                                        QC_SMI_STMT( aStatement ),
                                        SDM_USER,
                                        idlOS::strlen(SDM_USER),
                                        & sUserID ) != IDE_SUCCESS,
                    ERR_META_HANDLE );

    IDE_TEST_RAISE( qciMisc::getSequenceHandleByName(
                        QC_SMI_STMT( aStatement ),
                        sUserID,
                        (UChar*)SDM_SEQUENCE_SHARD_NAME,
                        idlOS::strlen(SDM_SEQUENCE_SHARD_NAME),
                        &sSequenceHandle ) != IDE_SUCCESS,
                    ERR_META_HANDLE );

    while ( sExist == ID_TRUE )
    {
        IDE_TEST( smiTable::readSequence( QC_SMI_STMT( aStatement ),
                                          sSequenceHandle,
                                          SMI_SEQUENCE_NEXT,
                                          &sSeqVal,
                                          NULL )
                 != IDE_SUCCESS );

        // sSeqVal은 비록 SLong이지만, sequence를 생성할 때
        // max를 integer max를 안넘도록 하였기 때문에
        // 여기서 overflow체크는 하지 않는다.
        IDE_TEST( searchShardID( QC_SMI_STMT( aStatement ),
                                 (SInt)sSeqVal,
                                 aSMN,
                                 &sExist )
                  != IDE_SUCCESS );

        if ( sFirst == ID_TRUE )
        {
            sSeqValFirst = sSeqVal;
            sFirst = ID_FALSE;
        }
        else
        {
            // 찾다찾다 한바퀴 돈 경우.
            // 이는 object가 꽉 찬 것을 의미함.
            IDE_TEST_RAISE( sSeqVal == sSeqValFirst, ERR_OBJECTS_OVERFLOW );
        }
    }

    *aShardID = sSeqVal;

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_META_HANDLE )
    {
        if ( ideGetErrorCode() == qpERR_ABORT_QCM_NOT_EXIST_USER )
        {
            IDE_SET( ideSetErrorCode(sdERR_ABORT_SDM_SHARD_META_NOT_CREATED,
                                     SDM_USER) );
        }
        else if ( ideGetErrorCode() == qpERR_ABORT_QCM_NOT_EXIST_SEQUENCE )
        {
            IDE_SET( ideSetErrorCode(sdERR_ABORT_SDM_SHARD_META_NOT_CREATED,
                                     SDM_SEQUENCE_SHARD_NAME) );
        }
    }
    IDE_EXCEPTION( ERR_OBJECTS_OVERFLOW )
    {
        IDE_SET(ideSetErrorCode(qpERR_ABORT_QCM_MAXIMUM_OBJECT_NUMBER_EXCEED,
                                "shard object",
                                QCM_META_SEQ_MAXVALUE) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdm::searchShardID( smiStatement * aSmiStmt,
                           SInt           aShardID,
                           ULong          aSMN,
                           idBool       * aExist )
{
    UInt                 sStage = 0;
    smiRange             sRange;
    smiTableCursor       sCursor;
    const void         * sRow;
    UInt                 sFlag = QCM_META_CURSOR_FLAG;
    const void         * sSdmObjects;
    const void         * sSdmObjectsIndex[SDM_MAX_META_INDICES];
    mtcColumn          * sSdmShardIDColumn;
    qtcMetaRangeColumn   sShardIDRange;
    mtcColumn          * sSdmSMNColumn;
    qtcMetaRangeColumn   sSMNRange;

    scGRID              sRid; // Disk Table을 위한 Record IDentifier
    smiCursorProperties sCursorProperty;

    IDE_TEST( getMetaTableAndIndex( aSmiStmt,
                                    SDM_OBJECTS,
                                    & sSdmObjects,
                                    sSdmObjectsIndex )
              != IDE_SUCCESS );

    IDE_TEST_RAISE( sSdmObjectsIndex[SDM_OBJECTS_IDX1_ORDER] == NULL,
                    ERR_META_HANDLE );

    IDE_TEST( smiGetTableColumns( sSdmObjects,
                                  SDM_OBJECTS_SHARD_ID_COL_ORDER,
                                  (const smiColumn**)&sSdmShardIDColumn )
              != IDE_SUCCESS );
    IDE_TEST( smiGetTableColumns( sSdmObjects,
                                  SDM_OBJECTS_SMN_COL_ORDER,
                                  (const smiColumn**)&sSdmSMNColumn )
              != IDE_SUCCESS );

    // mtdModule 설정
    IDE_TEST( mtd::moduleById( &(sSdmShardIDColumn->module),
                               sSdmShardIDColumn->type.dataTypeId )
              != IDE_SUCCESS );

    // mtlModule 설정
    IDE_TEST( mtl::moduleById( &sSdmShardIDColumn->language,
                               sSdmShardIDColumn->type.languageId )
              != IDE_SUCCESS );

    // mtdModule 설정
    IDE_TEST( mtd::moduleById( &(sSdmSMNColumn->module),
                               sSdmSMNColumn->type.dataTypeId )
              != IDE_SUCCESS );

    // mtlModule 설정
    IDE_TEST( mtl::moduleById( &sSdmSMNColumn->language,
                               sSdmSMNColumn->type.languageId )
              != IDE_SUCCESS );

    qciMisc::makeMetaRangeDoubleColumn(
        &sShardIDRange,
        &sSMNRange,
        (const mtcColumn *) sSdmShardIDColumn,
        (const void *) &aShardID,
        (const mtcColumn *) sSdmSMNColumn,
        (const void *) &aSMN,
        &sRange);

    sCursor.initialize();

    SMI_CURSOR_PROP_INIT_FOR_META_INDEX_SCAN(&sCursorProperty, NULL);

    IDE_TEST(sCursor.open(
                 aSmiStmt,
                 sSdmObjects,
                 sSdmObjectsIndex[SDM_OBJECTS_IDX1_ORDER],
                 smiGetRowSCN(sSdmObjects),
                 NULL,
                 &sRange,
                 smiGetDefaultKeyRange(),
                 smiGetDefaultFilter(),
                 sFlag,
                 SMI_SELECT_CURSOR,
                 &sCursorProperty) != IDE_SUCCESS);
    sStage = 1;

    IDE_TEST(sCursor.beforeFirst() != IDE_SUCCESS);

    IDE_TEST(sCursor.readRow(&sRow, &sRid, SMI_FIND_NEXT) != IDE_SUCCESS);

    if ( sRow == NULL )
    {
        *aExist = ID_FALSE;
    }
    else
    {
        *aExist = ID_TRUE;
    }

    sStage = 0;
    IDE_TEST(sCursor.close() != IDE_SUCCESS);

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_META_HANDLE )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_SHARD_META_NOT_CREATED,
                                  SDM_OBJECTS ) );
    }
    IDE_EXCEPTION_END;

    if ( sStage == 1 )
    {
        (void)sCursor.close();
    }
    else
    {
        /* Nothing to do */
    }

    return IDE_FAILURE;
}

// insert node
IDE_RC sdm::insertNode( qcStatement * aStatement,
                        SChar       * aNodeName,
                        UInt          aPortNo,
                        SChar       * aHostIP,
                        UInt          aAlternatePortNo,
                        SChar       * aAlternateHostIP,
                        UInt          aConnType,
                        UInt        * aRowCnt )
{
    SChar      * sSqlStr;
    vSLong       sRowCnt;
    UInt         sNodeID;
    sdiGlobalMetaNodeInfo sMetaNodeInfo = { ID_ULONG(0) };

    IDE_TEST( checkMetaVersion( QC_SMI_STMT( aStatement ) )
              != IDE_SUCCESS );

    /* PROJ-2701 Online data rebuild */
    IDE_TEST( makeShardMeta4NewSMN(aStatement) != IDE_SUCCESS );

    IDE_TEST( getGlobalMetaNodeInfoCore( QC_SMI_STMT(aStatement),
                                         &sMetaNodeInfo ) != IDE_SUCCESS );

    IDE_TEST( STRUCT_ALLOC_WITH_SIZE( aStatement->qmxMem,
                                      SChar,
                                      QD_MAX_SQL_LENGTH,
                                      &sSqlStr )
              != IDE_SUCCESS);

    IDE_TEST( getNextNodeID( aStatement,
                             &sNodeID,
                             sMetaNodeInfo.mShardMetaNumber )
              != IDE_SUCCESS );


    if ( ( aAlternatePortNo == 0 ) || ( aAlternateHostIP[0] == '\0' ) )
    {
        idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                         "INSERT INTO SYS_SHARD.NODES_ "
                         "VALUES ( "
                         QCM_SQL_INT32_FMT", "     /* NODE_ID                    */
                         QCM_SQL_VARCHAR_FMT", "   /* NODE_NAME                  */
                         QCM_SQL_VARCHAR_FMT", "   /* HOST_IP                    */
                         QCM_SQL_INT32_FMT", "     /* PORT_NO                    */
                         "NULL, "                  /* ALTERNATE_HOST_IP          */
                         "NULL, "                  /* ALTERNATE_PORT_NO          */
                         QCM_SQL_VARCHAR_FMT", "   /* INTERNAL_HOST_IP           */
                         QCM_SQL_INT32_FMT", "     /* INTERNAL_PORT_NO           */
                         "NULL, "                  /* INTERNAL_ALTERNATE_HOST_IP */
                         "NULL, "                  /* INTERNAL_ALTERNATE_PORT_NO */
                         QCM_SQL_INT32_FMT", "     /* INTERNAL_CONN_TYPE         */
                         QCM_SQL_BIGINT_FMT" ) ",  /* SMN */
                         sNodeID,
                         aNodeName,
                         aHostIP,
                         aPortNo,
                         aHostIP,
                         aPortNo,
                         aConnType,
                         sMetaNodeInfo.mShardMetaNumber );
    }
    else
    {
        idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                         "INSERT INTO SYS_SHARD.NODES_ "
                         "VALUES ( "
                         QCM_SQL_INT32_FMT", "     /* NODE_ID                    */
                         QCM_SQL_VARCHAR_FMT", "   /* NODE_NAME                  */
                         QCM_SQL_VARCHAR_FMT", "   /* HOST_IP                    */
                         QCM_SQL_INT32_FMT", "     /* PORT_NO                    */
                         QCM_SQL_VARCHAR_FMT", "   /* ALTERNATE_HOST_IP          */
                         QCM_SQL_INT32_FMT", "     /* ALTERNATE_PORT_NO          */
                         QCM_SQL_VARCHAR_FMT", "   /* INTERNAL_HOST_IP           */
                         QCM_SQL_INT32_FMT", "     /* INTERNAL_PORT_NO           */
                         QCM_SQL_VARCHAR_FMT", "   /* INTERNAL_ALTERNATE_HOST_IP */
                         QCM_SQL_INT32_FMT", "     /* INTERNAL_ALTERNATE_PORT_NO */
                         QCM_SQL_INT32_FMT", "     /* INTERNAL_CONN_TYPE         */
                         QCM_SQL_BIGINT_FMT" ) ",  /* SMN */
                         sNodeID,
                         aNodeName,
                         aHostIP,
                         aPortNo,
                         aAlternateHostIP,
                         aAlternatePortNo,
                         aHostIP,
                         aPortNo,
                         aAlternateHostIP,
                         aAlternatePortNo,
                         aConnType,
                         sMetaNodeInfo.mShardMetaNumber );
    }

    IDE_TEST( qciMisc::runDMLforDDL( QC_SMI_STMT( aStatement ),
                                     sSqlStr,
                                     & sRowCnt )
              != IDE_SUCCESS );

    if ( aRowCnt != NULL )
    {
        *aRowCnt = (UInt)sRowCnt;
    }
    else
    {
        /* Nothing to do */
    }

    sdi::setShardMetaTouched( aStatement->session );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdm::updateNodeInternal( qcStatement * aStatement,
                                SChar       * aNodeName,
                                SChar       * aHostIP,
                                UInt          aPortNo,
                                SChar       * aAlternateHostIP,
                                UInt          aAlternatePortNo,
                                UInt          aConnType,
                                UInt        * aRowCnt )
{
    SChar      * sSqlStr   = NULL;
    vSLong       sRowCnt   = 0;
    sdiGlobalMetaNodeInfo sMetaNodeInfo = { ID_ULONG(0) };

    IDE_TEST( checkMetaVersion( QC_SMI_STMT( aStatement ) )
              != IDE_SUCCESS );

    /* PROJ-2701 Online data rebuild */
    IDE_TEST( makeShardMeta4NewSMN(aStatement) != IDE_SUCCESS );

    IDE_TEST( getGlobalMetaNodeInfoCore( QC_SMI_STMT(aStatement),
                                         &sMetaNodeInfo ) != IDE_SUCCESS );

    IDE_TEST( STRUCT_ALLOC_WITH_SIZE( aStatement->qmxMem,
                                      SChar,
                                      QD_MAX_SQL_LENGTH,
                                      &sSqlStr )
              != IDE_SUCCESS );

    if ( ( aAlternateHostIP[0] == '\0' ) || ( aAlternatePortNo == 0 ) )
    {
        idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                         "UPDATE SYS_SHARD.NODES_ SET "
                         "INTERNAL_HOST_IP = "QCM_SQL_VARCHAR_FMT", "
                         "INTERNAL_PORT_NO = "QCM_SQL_INT32_FMT", "
                         "INTERNAL_ALTERNATE_HOST_IP = NULL, "
                         "INTERNAL_ALTERNATE_PORT_NO = NULL, "
                         "INTERNAL_CONN_TYPE = "QCM_SQL_INT32_FMT" "
                         "WHERE NODE_NAME = "QCM_SQL_VARCHAR_FMT" "
                         "AND SMN = "QCM_SQL_BIGINT_FMT,
                         aHostIP,
                         aPortNo,
                         aConnType,
                         aNodeName,
                         sMetaNodeInfo.mShardMetaNumber );
    }
    else
    {
        idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                         "UPDATE SYS_SHARD.NODES_ SET "
                         "INTERNAL_HOST_IP = "QCM_SQL_VARCHAR_FMT", "
                         "INTERNAL_PORT_NO = "QCM_SQL_INT32_FMT", "
                         "INTERNAL_ALTERNATE_HOST_IP = "QCM_SQL_VARCHAR_FMT", "
                         "INTERNAL_ALTERNATE_PORT_NO = "QCM_SQL_INT32_FMT", "
                         "INTERNAL_CONN_TYPE = "QCM_SQL_INT32_FMT" "
                         "WHERE NODE_NAME = "QCM_SQL_VARCHAR_FMT" "
                         "AND SMN = "QCM_SQL_BIGINT_FMT,
                         aHostIP,
                         aPortNo,
                         aAlternateHostIP,
                         aAlternatePortNo,
                         aConnType,
                         aNodeName,
                         sMetaNodeInfo.mShardMetaNumber );
    }

    IDE_TEST( qciMisc::runDMLforDDL( QC_SMI_STMT( aStatement ),
                                     sSqlStr,
                                     & sRowCnt )
              != IDE_SUCCESS );

    if ( aRowCnt != NULL )
    {
        *aRowCnt = (UInt)sRowCnt;
    }
    else
    {
        /* Nothing to do */
    }

    sdi::setShardMetaTouched( aStatement->session );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdm::updateNodeExternal( qcStatement * aStatement,
                                SChar       * aNodeName,
                                SChar       * aHostIP,
                                UInt          aPortNo,
                                SChar       * aAlternateHostIP,
                                UInt          aAlternatePortNo,
                                UInt        * aRowCnt )
{
    SChar      * sSqlStr   = NULL;
    vSLong       sRowCnt   = 0;
    sdiGlobalMetaNodeInfo sMetaNodeInfo = { ID_ULONG(0) };

    IDE_TEST( checkMetaVersion( QC_SMI_STMT( aStatement ) )
              != IDE_SUCCESS );

    /* PROJ-2701 Online data rebuild */
    IDE_TEST( makeShardMeta4NewSMN(aStatement) != IDE_SUCCESS );

    IDE_TEST( getGlobalMetaNodeInfoCore( QC_SMI_STMT(aStatement),
                                         &sMetaNodeInfo ) != IDE_SUCCESS );

    IDE_TEST( STRUCT_ALLOC_WITH_SIZE( aStatement->qmxMem,
                                      SChar,
                                      QD_MAX_SQL_LENGTH,
                                      &sSqlStr )
              != IDE_SUCCESS );

    if ( ( aAlternateHostIP[0] == '\0' ) || ( aAlternatePortNo == 0 ) )
    {
        idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                         "UPDATE SYS_SHARD.NODES_ SET "
                         "HOST_IP = "QCM_SQL_VARCHAR_FMT", "
                         "PORT_NO = "QCM_SQL_INT32_FMT", "
                         "ALTERNATE_HOST_IP = NULL, "
                         "ALTERNATE_PORT_NO = NULL "
                         "WHERE NODE_NAME = "QCM_SQL_VARCHAR_FMT" "
                         "AND SMN = "QCM_SQL_BIGINT_FMT,
                         aHostIP,
                         aPortNo,
                         aNodeName,
                         sMetaNodeInfo.mShardMetaNumber );
    }
    else
    {
        idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                         "UPDATE SYS_SHARD.NODES_ SET "
                         "HOST_IP = "QCM_SQL_VARCHAR_FMT", "
                         "PORT_NO = "QCM_SQL_INT32_FMT", "
                         "ALTERNATE_HOST_IP = "QCM_SQL_VARCHAR_FMT", "
                         "ALTERNATE_PORT_NO = "QCM_SQL_INT32_FMT" "
                         "WHERE NODE_NAME = "QCM_SQL_VARCHAR_FMT" "
                         "AND SMN = "QCM_SQL_BIGINT_FMT,
                         aHostIP,
                         aPortNo,
                         aAlternateHostIP,
                         aAlternatePortNo,
                         aNodeName,
                         sMetaNodeInfo.mShardMetaNumber );
    }

    IDE_TEST( qciMisc::runDMLforDDL( QC_SMI_STMT( aStatement ),
                                     sSqlStr,
                                     & sRowCnt )
              != IDE_SUCCESS );

    if ( aRowCnt != NULL )
    {
        *aRowCnt = (UInt)sRowCnt;
    }
    else
    {
        /* Nothing to do */
    }

    sdi::setShardMetaTouched( aStatement->session );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

// delete node
IDE_RC sdm::deleteNode( qcStatement * aStatement,
                        SChar       * aNodeName,
                        UInt        * aRowCnt )
{
    SChar      * sSqlStr;
    vSLong       sRowCnt;

    idBool        sRecordExist = ID_FALSE;
    mtdBigintType sResultCount = 0;

    sdiGlobalMetaNodeInfo sMetaNodeInfo = { ID_ULONG(0) };

    IDE_TEST( checkMetaVersion( QC_SMI_STMT( aStatement ) )
              != IDE_SUCCESS );

    /* PROJ-2701 Online data rebuild */
    IDE_TEST( makeShardMeta4NewSMN(aStatement) != IDE_SUCCESS );

    IDE_TEST( getGlobalMetaNodeInfoCore( QC_SMI_STMT(aStatement),
                                         &sMetaNodeInfo ) != IDE_SUCCESS );

    IDE_TEST( STRUCT_ALLOC_WITH_SIZE( aStatement->qmxMem,
                                      SChar,
                                      QD_MAX_SQL_LENGTH,
                                      &sSqlStr )
              != IDE_SUCCESS);

    // BUG-46294
    idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                     "SELECT NODE_ID "
                     "  FROM SYS_SHARD.NODES_ "
                     " WHERE NODE_NAME = "QCM_SQL_VARCHAR_FMT
                     "   AND SMN = "QCM_SQL_BIGINT_FMT
                     "   AND NODE_ID IN ( SELECT DEFAULT_NODE_ID FROM SYS_SHARD.OBJECTS_ WHERE SMN = "QCM_SQL_BIGINT_FMT
                     "          UNION ALL SELECT NODE_ID FROM SYS_SHARD.RANGES_ WHERE SMN = "QCM_SQL_BIGINT_FMT
                     "          UNION ALL SELECT NODE_ID FROM SYS_SHARD.CLONES_ WHERE SMN = "QCM_SQL_BIGINT_FMT
                     "          UNION ALL SELECT NODE_ID FROM SYS_SHARD.SOLOS_ WHERE SMN = "QCM_SQL_BIGINT_FMT" )",
                     aNodeName,
                     sMetaNodeInfo.mShardMetaNumber,
                     sMetaNodeInfo.mShardMetaNumber,
                     sMetaNodeInfo.mShardMetaNumber,
                     sMetaNodeInfo.mShardMetaNumber,
                     sMetaNodeInfo.mShardMetaNumber );

    IDE_TEST( qcg::runSelectOneRowforDDL( QC_SMI_STMT( aStatement ),
                                          sSqlStr,
                                          &sResultCount,
                                          &sRecordExist,
                                          ID_FALSE )
              != IDE_SUCCESS );

    IDE_TEST_RAISE( sRecordExist == ID_TRUE, ERR_EXIST_REFERENCES_NODE )

    idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                     "DELETE FROM SYS_SHARD.NODES_ WHERE NODE_NAME = "QCM_SQL_VARCHAR_FMT" "
                     "AND SMN = "QCM_SQL_BIGINT_FMT,
                     aNodeName,
                     sMetaNodeInfo.mShardMetaNumber);

    IDE_TEST( qciMisc::runDMLforDDL( QC_SMI_STMT( aStatement ),
                                     sSqlStr,
                                     & sRowCnt )
              != IDE_SUCCESS );

    if ( aRowCnt != NULL )
    {
        *aRowCnt = (UInt)sRowCnt;
    }
    else
    {
        /* Nothing to do */
    }

    sdi::setShardMetaTouched( aStatement->session );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_EXIST_REFERENCES_NODE )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_EXIST_REFERENCES_NODE ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/* insert shard procedure */
IDE_RC sdm::insertProcedure( qcStatement * aStatement,
                             SChar       * aUserName,
                             SChar       * aProcName,
                             SChar       * aSplitMethod,
                             SChar       * aKeyParamName,
                             SChar       * aSubSplitMethod,
                             SChar       * aSubKeyParamName,
                             SChar       * aDefaultNodeName,
                             UInt        * aRowCnt )
{
    SChar         * sSqlStr;
    vSLong          sRowCnt;
    UInt            sShardID;
    UInt            sUserID;
    qsOID           sProcOID;
    sdiTableInfo    sTableInfo;
    sdiNode         sNode;
    idBool          sIsTableFound = ID_FALSE;
    sdiGlobalMetaNodeInfo sMetaNodeInfo = { ID_ULONG(0) };

    IDE_TEST( checkMetaVersion( QC_SMI_STMT( aStatement ) )
              != IDE_SUCCESS );

    /* PROJ-2701 Online data rebuild */
    IDE_TEST( makeShardMeta4NewSMN(aStatement) != IDE_SUCCESS );

    IDE_TEST( getGlobalMetaNodeInfoCore( QC_SMI_STMT(aStatement),
                                         &sMetaNodeInfo ) != IDE_SUCCESS );

    IDE_TEST( qcmUser::getUserID( NULL,
                                  QC_SMI_STMT( aStatement ),
                                  aUserName,
                                  idlOS::strlen( aUserName ),
                                  &sUserID )
              != IDE_SUCCESS );

    IDE_TEST_RAISE( sUserID == QC_SYSTEM_USER_ID,
                    ERR_SYSTEM_OBJECT );

    IDE_TEST( qcmProc::getProcExistWithEmptyByNamePtr( aStatement,
                                                       sUserID,
                                                       aProcName,
                                                       idlOS::strlen( aProcName ),
                                                       &sProcOID )
              != IDE_SUCCESS );

    IDE_TEST_RAISE( sProcOID == QS_EMPTY_OID, ERR_NOT_EXIST_OBJECT );

    IDE_TEST( getTableInfo( QC_SMI_STMT( aStatement ),
                            aUserName,
                            aProcName,
                            sMetaNodeInfo.mShardMetaNumber,
                            &sTableInfo,
                            &sIsTableFound )
              != IDE_SUCCESS );

    // 이미 동일한 테이블이 존재한다면 에러처리한다.
    IDE_TEST_RAISE( sIsTableFound == ID_TRUE,
                    ERR_EXIST_SHARD_OBJECT );

    // BUG-46301
    IDE_TEST( validateParamBeforeInsert( sProcOID,
                                         aUserName,
                                         aProcName,
                                         aSplitMethod,
                                         aKeyParamName,
                                         aSubSplitMethod,
                                         aSubKeyParamName )
              != IDE_SUCCESS );

    IDE_TEST( STRUCT_ALLOC_WITH_SIZE( aStatement->qmxMem,
                                      SChar,
                                      QD_MAX_SQL_LENGTH,
                                      &sSqlStr )
              != IDE_SUCCESS );

    IDE_TEST( getNextShardID( aStatement,
                              &sShardID,
                              sMetaNodeInfo.mShardMetaNumber )
              != IDE_SUCCESS );

    if ( aSplitMethod[0] != 'C' )
    {
        if ( aDefaultNodeName[0] != '\0' )
        {
            IDE_TEST( getNodeByName( QC_SMI_STMT( aStatement ),
                                     aDefaultNodeName,
                                     sMetaNodeInfo.mShardMetaNumber,
                                     &sNode )
                      != IDE_SUCCESS );

            idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                             "INSERT INTO SYS_SHARD.OBJECTS_ VALUES( "
                             QCM_SQL_INT32_FMT", "
                             QCM_SQL_VARCHAR_FMT", "
                             QCM_SQL_VARCHAR_FMT", "
                             "'P', "
                             QCM_SQL_VARCHAR_FMT", "
                             QCM_SQL_VARCHAR_FMT", "
                             QCM_SQL_VARCHAR_FMT", "
                             QCM_SQL_VARCHAR_FMT", "
                             QCM_SQL_INT32_FMT", "
                             QCM_SQL_BIGINT_FMT" )",
                             sShardID,
                             aUserName,
                             aProcName,
                             aSplitMethod,
                             aKeyParamName,
                             aSubSplitMethod,
                             aSubKeyParamName,
                             sNode.mNodeId,
                             sMetaNodeInfo.mShardMetaNumber);
        }
        else
        {
            idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                             "INSERT INTO SYS_SHARD.OBJECTS_ VALUES( "
                             QCM_SQL_INT32_FMT", "
                             QCM_SQL_VARCHAR_FMT", "
                             QCM_SQL_VARCHAR_FMT", "
                             "'P', "
                             QCM_SQL_VARCHAR_FMT", "
                             QCM_SQL_VARCHAR_FMT", "
                             QCM_SQL_VARCHAR_FMT", "
                             QCM_SQL_VARCHAR_FMT", "
                             "NULL, " // default node id
                             QCM_SQL_BIGINT_FMT" )",
                             sShardID,
                             aUserName,
                             aProcName,
                             aSplitMethod,
                             aKeyParamName,
                             aSubSplitMethod,
                             aSubKeyParamName,
                             sMetaNodeInfo.mShardMetaNumber);
        }
    }
    else
    {
        // Clone table의 경우 shard key column이 없기 때문에,
        // key column name을 NULL로 입력한다.
        idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                         "INSERT INTO SYS_SHARD.OBJECTS_ VALUES( "
                         QCM_SQL_INT32_FMT", "
                         QCM_SQL_VARCHAR_FMT", "
                         QCM_SQL_VARCHAR_FMT", "
                         "'P', "
                         QCM_SQL_VARCHAR_FMT", "
                         "NULL, "
                         "NULL, " // sub split method
                         "NULL, " // sub shard key
                         "NULL, " // default node id
                         QCM_SQL_BIGINT_FMT")",
                         sShardID,
                         aUserName,
                         aProcName,
                         aSplitMethod,
                         sMetaNodeInfo.mShardMetaNumber);
    }

    IDE_TEST( qciMisc::runDMLforDDL( QC_SMI_STMT( aStatement ),
                                     sSqlStr,
                                     & sRowCnt ) != IDE_SUCCESS );

    if ( aRowCnt != NULL )
    {
        *aRowCnt = (UInt)sRowCnt;
    }
    else
    {
        /* Nothing to do */
    }

    sdi::setShardMetaTouched( aStatement->session );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_SYSTEM_OBJECT )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_SYSTEM_OBJECT ) );
    }
    IDE_EXCEPTION( ERR_NOT_EXIST_OBJECT )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QSV_NOT_EXIST_PROC_SQLTEXT,
                                  aProcName ) );
    }
    IDE_EXCEPTION( ERR_EXIST_SHARD_OBJECT )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_AREADY_EXIST_SHARD_OBJECT ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/* insert shard table */
IDE_RC sdm::insertTable( qcStatement * aStatement,
                         SChar       * aUserName,
                         SChar       * aTableName,
                         SChar       * aSplitMethod,
                         SChar       * aKeyColumnName,
                         SChar       * aSubSplitMethod,
                         SChar       * aSubKeyColumnName,
                         SChar       * aDefaultNodeName,
                         UInt        * aRowCnt )
{
    SChar         * sSqlStr;
    vSLong          sRowCnt;
    UInt            sShardID;
    UInt            sUserID;
    void          * sTableHandle;
    smSCN           sTableSCN;
    sdiTableInfo    sTableInfo;
    sdiNode         sNode;
    idBool          sIsTableFound = ID_FALSE;
    sdiGlobalMetaNodeInfo sMetaNodeInfo = { ID_ULONG(0) };

    IDE_TEST( checkMetaVersion( QC_SMI_STMT( aStatement ) )
              != IDE_SUCCESS );

    /* PROJ-2701 Online data rebuild */
    IDE_TEST( makeShardMeta4NewSMN(aStatement) != IDE_SUCCESS );

    IDE_TEST( getGlobalMetaNodeInfoCore( QC_SMI_STMT(aStatement),
                                         &sMetaNodeInfo ) != IDE_SUCCESS );

    IDE_TEST( qcmUser::getUserID( NULL,
                                  QC_SMI_STMT( aStatement ),
                                  aUserName,
                                  idlOS::strlen( aUserName ),
                                  &sUserID )
              != IDE_SUCCESS );

    IDE_TEST_RAISE( sUserID == QC_SYSTEM_USER_ID,
                    ERR_SYSTEM_OBJECT );

    IDE_TEST( qcm::getTableHandleByName( QC_SMI_STMT(aStatement),
                                         sUserID,
                                         (UChar*)aTableName,
                                         idlOS::strlen( aTableName ),
                                         &sTableHandle,
                                         &sTableSCN )
              != IDE_SUCCESS );

    IDE_TEST( getTableInfo( QC_SMI_STMT( aStatement ),
                            aUserName,
                            aTableName,
                            sMetaNodeInfo.mShardMetaNumber,
                            &sTableInfo,
                            &sIsTableFound )
              != IDE_SUCCESS );

    // 이미 동일한 테이블이 존재한다면 에러처리한다.
    IDE_TEST_RAISE( sIsTableFound == ID_TRUE,
                    ERR_EXIST_SHARD_OBJECT );

    // BUG-46301
    IDE_TEST( validateColumnBeforeInsert( aStatement,
                                          sUserID,
                                          aTableName,
                                          aSplitMethod,
                                          aKeyColumnName,
                                          aSubSplitMethod,
                                          aSubKeyColumnName )
              != IDE_SUCCESS );

    IDE_TEST( STRUCT_ALLOC_WITH_SIZE( aStatement->qmxMem,
                                      SChar,
                                      QD_MAX_SQL_LENGTH,
                                      &sSqlStr )
              != IDE_SUCCESS );

    IDE_TEST( getNextShardID( aStatement,
                              &sShardID,
                              sMetaNodeInfo.mShardMetaNumber )
              != IDE_SUCCESS );

    if ( aSplitMethod[0] != 'C' )
    {
        if ( aDefaultNodeName[0] != '\0' )
        {
            IDE_TEST( getNodeByName( QC_SMI_STMT( aStatement ),
                                     aDefaultNodeName,
                                     sMetaNodeInfo.mShardMetaNumber,
                                     &sNode )
                      != IDE_SUCCESS );

              idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                             "INSERT INTO SYS_SHARD.OBJECTS_ VALUES( "
                             QCM_SQL_INT32_FMT", "
                             QCM_SQL_VARCHAR_FMT", "
                             QCM_SQL_VARCHAR_FMT", "
                             "'T', "
                             QCM_SQL_VARCHAR_FMT", "
                             QCM_SQL_VARCHAR_FMT", "
                             QCM_SQL_VARCHAR_FMT", " // sub split method
                             QCM_SQL_VARCHAR_FMT", " // sub shard key
                             QCM_SQL_INT32_FMT", "
                             QCM_SQL_BIGINT_FMT" )",
                             sShardID,
                             aUserName,
                             aTableName,
                             aSplitMethod,
                             aKeyColumnName,
                             aSubSplitMethod,
                             aSubKeyColumnName,
                             sNode.mNodeId,
                             sMetaNodeInfo.mShardMetaNumber );
        }
        else
        {
            idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                             "INSERT INTO SYS_SHARD.OBJECTS_ VALUES( "
                             QCM_SQL_INT32_FMT", "
                             QCM_SQL_VARCHAR_FMT", "
                             QCM_SQL_VARCHAR_FMT", "
                             "'T', "
                             QCM_SQL_VARCHAR_FMT", "
                             QCM_SQL_VARCHAR_FMT", "
                             QCM_SQL_VARCHAR_FMT", " // sub split method
                             QCM_SQL_VARCHAR_FMT", " // sub shard key
                             "NULL, " // default node id
                             QCM_SQL_BIGINT_FMT")",
                             sShardID,
                             aUserName,
                             aTableName,
                             aSplitMethod,
                             aKeyColumnName,
                             aSubSplitMethod,
                             aSubKeyColumnName,
                             sMetaNodeInfo.mShardMetaNumber);
        }
    }
    else
    {
        // Clone, solo table의 경우 shard key column이 없기 때문에,
        // key column name을 NULL로 입력한다.
        idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                         "INSERT INTO SYS_SHARD.OBJECTS_ VALUES( "
                         QCM_SQL_INT32_FMT", "
                         QCM_SQL_VARCHAR_FMT", "
                         QCM_SQL_VARCHAR_FMT", "
                         "'T', "
                         QCM_SQL_VARCHAR_FMT", "
                         "NULL, "
                         "NULL, " // sub split method
                         "NULL, " // sub shard key
                         "NULL, " // default node id
                         QCM_SQL_BIGINT_FMT")",
                         sShardID,
                         aUserName,
                         aTableName,
                         aSplitMethod,
                         sMetaNodeInfo.mShardMetaNumber);
    }

    IDE_TEST( qciMisc::runDMLforDDL( QC_SMI_STMT( aStatement ),
                                     sSqlStr,
                                     & sRowCnt ) != IDE_SUCCESS );

    if ( aRowCnt != NULL )
    {
        *aRowCnt = (UInt)sRowCnt;
    }
    else
    {
        /* Nothing to do */
    }

    sdi::setShardMetaTouched( aStatement->session );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_SYSTEM_OBJECT )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_SYSTEM_OBJECT ) );
    }
    IDE_EXCEPTION( ERR_EXIST_SHARD_OBJECT )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_AREADY_EXIST_SHARD_OBJECT ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

// delete shard table
IDE_RC sdm::deleteObject( qcStatement * aStatement,
                          SChar       * aUserName,
                          SChar       * aTableName,
                          UInt        * aRowCnt )
{
    sdiTableInfo    sTableInfo;
    SChar         * sSqlStr;
    vSLong          sRowCnt;
    idBool          sIsTableFound = ID_FALSE;
    sdiGlobalMetaNodeInfo sMetaNodeInfo = { ID_ULONG(0) };

    IDE_TEST( checkMetaVersion( QC_SMI_STMT( aStatement ) )
              != IDE_SUCCESS );

    /* PROJ-2701 Online data rebuild */
    IDE_TEST( makeShardMeta4NewSMN(aStatement) != IDE_SUCCESS );

    IDE_TEST( getGlobalMetaNodeInfoCore( QC_SMI_STMT(aStatement),
                                         &sMetaNodeInfo ) != IDE_SUCCESS );

    IDE_TEST( getTableInfo( QC_SMI_STMT( aStatement ),
                            aUserName,
                            aTableName,
                            sMetaNodeInfo.mShardMetaNumber,
                            &sTableInfo,
                            &sIsTableFound )
              != IDE_SUCCESS );

    IDE_TEST_RAISE( sIsTableFound == ID_FALSE,
                    ERR_NOT_EXIST_TABLE );

    IDE_TEST( STRUCT_ALLOC_WITH_SIZE( aStatement->qmxMem,
                                      SChar,
                                      QD_MAX_SQL_LENGTH,
                                      &sSqlStr )
              != IDE_SUCCESS);

    // remove shard distribution info
    idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                     "DELETE FROM SYS_SHARD.RANGES_ WHERE SHARD_ID = "
                     QCM_SQL_INT32_FMT" "
                     "AND SMN = "QCM_SQL_BIGINT_FMT,
                     sTableInfo.mShardID,
                     sMetaNodeInfo.mShardMetaNumber );

    IDE_TEST( qciMisc::runDMLforDDL( QC_SMI_STMT( aStatement ),
                                     sSqlStr,
                                     & sRowCnt )
              != IDE_SUCCESS );

    idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                     "DELETE FROM SYS_SHARD.CLONES_ WHERE SHARD_ID = "
                     QCM_SQL_INT32_FMT" "
                     "AND SMN = "QCM_SQL_BIGINT_FMT,
                     sTableInfo.mShardID,
                     sMetaNodeInfo.mShardMetaNumber );

    IDE_TEST( qciMisc::runDMLforDDL( QC_SMI_STMT( aStatement ),
                                     sSqlStr,
                                     & sRowCnt )
              != IDE_SUCCESS );

    idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                     "DELETE FROM SYS_SHARD.SOLOS_ WHERE SHARD_ID = "
                     QCM_SQL_INT32_FMT" "
                     "AND SMN = "QCM_SQL_BIGINT_FMT,
                     sTableInfo.mShardID,
                     sMetaNodeInfo.mShardMetaNumber);

    IDE_TEST( qciMisc::runDMLforDDL( QC_SMI_STMT( aStatement ),
                                     sSqlStr,
                                     & sRowCnt )
              != IDE_SUCCESS );

    idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                     "DELETE FROM SYS_SHARD.OBJECTS_ WHERE SHARD_ID = "
                     QCM_SQL_INT32_FMT" "
                     "AND SMN = "QCM_SQL_BIGINT_FMT,
                     sTableInfo.mShardID,
                     sMetaNodeInfo.mShardMetaNumber);

    IDE_TEST( qciMisc::runDMLforDDL( QC_SMI_STMT( aStatement ),
                                     sSqlStr,
                                     & sRowCnt )
              != IDE_SUCCESS );

    if ( aRowCnt != NULL )
    {
        *aRowCnt = (UInt)sRowCnt;
    }
    else
    {
        /* Nothing to do */
    }

    sdi::setShardMetaTouched( aStatement->session );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_NOT_EXIST_TABLE )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_SHARD_TABLE_NOT_EXIST ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdm::deleteObjectByID( qcStatement * aStatement,
                              UInt          aShardID,
                              UInt        * aRowCnt )
{
    SChar         * sSqlStr;
    vSLong          sRowCnt;
    sdiGlobalMetaNodeInfo sMetaNodeInfo = { ID_ULONG(0) };

    IDE_TEST( checkMetaVersion( QC_SMI_STMT( aStatement ) )
              != IDE_SUCCESS );

    /* PROJ-2701 Online data rebuild */
    IDE_TEST( makeShardMeta4NewSMN(aStatement) != IDE_SUCCESS );

    IDE_TEST( getGlobalMetaNodeInfoCore( QC_SMI_STMT(aStatement),
                                         &sMetaNodeInfo ) != IDE_SUCCESS );

    IDE_TEST( STRUCT_ALLOC_WITH_SIZE( aStatement->qmxMem,
                                      SChar,
                                      QD_MAX_SQL_LENGTH,
                                      &sSqlStr )
              != IDE_SUCCESS);

    // delete shard distribution info
    idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                     "DELETE FROM SYS_SHARD.RANGES_ WHERE SHARD_ID = "
                     QCM_SQL_INT32_FMT" "
                     " AND SMN = "QCM_SQL_BIGINT_FMT,
                     aShardID,
                     sMetaNodeInfo.mShardMetaNumber);

    IDE_TEST( qciMisc::runDMLforDDL( QC_SMI_STMT( aStatement ),
                                     sSqlStr,
                                     & sRowCnt )
              != IDE_SUCCESS );

    idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                     "DELETE FROM SYS_SHARD.CLONES_ WHERE SHARD_ID = "
                     QCM_SQL_INT32_FMT" "
                     " AND SMN = "QCM_SQL_BIGINT_FMT,
                     aShardID,
                     sMetaNodeInfo.mShardMetaNumber);

    IDE_TEST( qciMisc::runDMLforDDL( QC_SMI_STMT( aStatement ),
                                     sSqlStr,
                                     & sRowCnt )
              != IDE_SUCCESS );

    idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                     "DELETE FROM SYS_SHARD.SOLOS_ WHERE SHARD_ID = "
                     QCM_SQL_INT32_FMT" "
                     " AND SMN = "QCM_SQL_BIGINT_FMT,
                     aShardID,
                     sMetaNodeInfo.mShardMetaNumber);

    IDE_TEST( qciMisc::runDMLforDDL( QC_SMI_STMT( aStatement ),
                                     sSqlStr,
                                     & sRowCnt )
              != IDE_SUCCESS );

    idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                     "DELETE FROM SYS_SHARD.OBJECTS_ WHERE SHARD_ID = "
                     QCM_SQL_INT32_FMT" "
                     " AND SMN = "QCM_SQL_BIGINT_FMT,
                     aShardID,
                     sMetaNodeInfo.mShardMetaNumber);

    IDE_TEST( qciMisc::runDMLforDDL( QC_SMI_STMT( aStatement ),
                                     sSqlStr,
                                     & sRowCnt )
              != IDE_SUCCESS );

    if ( aRowCnt != NULL )
    {
        *aRowCnt = (UInt)sRowCnt;
    }
    else
    {
        /* Nothing to do */
    }

    sdi::setShardMetaTouched( aStatement->session );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

// insert range info
IDE_RC sdm::insertRange( qcStatement * aStatement,
                         SChar       * aUserName,
                         SChar       * aTableName,
                         SChar       * aValue,
                         UShort        aValueLength,
                         SChar       * aSubValue,
                         UShort        aSubValueLength,
                         SChar       * aNodeName,
                         SChar       * aSetFuncType,
                         UInt        * aRowCnt )
{
    SChar         * sSqlStr;
    vSLong          sRowCnt;

    sdiNode         sNode;
    sdiTableInfo    sTableInfo;

    qcNamePosition  sPosition;
    SLong           sLongVal;
    idBool          sIsTableFound = ID_FALSE;

    sdiGlobalMetaNodeInfo sMetaNodeInfo = { ID_ULONG(0) };

    IDE_TEST( checkMetaVersion( QC_SMI_STMT( aStatement ) )
              != IDE_SUCCESS );

    /* PROJ-2701 Online data rebuild */
    IDE_TEST( makeShardMeta4NewSMN(aStatement) != IDE_SUCCESS );

    IDE_TEST( getGlobalMetaNodeInfoCore( QC_SMI_STMT(aStatement),
                                         &sMetaNodeInfo ) != IDE_SUCCESS );

    IDE_TEST( getNodeByName( QC_SMI_STMT( aStatement ),
                             aNodeName,
                             sMetaNodeInfo.mShardMetaNumber,
                             &sNode )
              != IDE_SUCCESS );

    IDE_TEST( getTableInfo( QC_SMI_STMT( aStatement ),
                            aUserName,
                            aTableName,
                            sMetaNodeInfo.mShardMetaNumber,
                            &sTableInfo,
                            &sIsTableFound )
              != IDE_SUCCESS );

    IDE_TEST_RAISE( sIsTableFound == ID_FALSE,
                    ERR_NOT_EXIST_TABLE );

    if ( aSetFuncType[0] == 'H' ) // from DBMS_SHARD.SET_SHARD_HASH()
    {
        IDE_TEST_RAISE( ( ( sTableInfo.mSplitMethod != SDI_SPLIT_HASH ) ||
                          ( sTableInfo.mSubKeyExists != ID_FALSE ) ),
                        ERR_INVALID_RANGE_FUNCTION );
    }
    else if ( aSetFuncType[0] == 'R' ) // from DBMS_SHARD.SET_SHARD_RANGE()
    {
        IDE_TEST_RAISE( ( ( sTableInfo.mSplitMethod != SDI_SPLIT_RANGE ) ||
                          ( sTableInfo.mSubKeyExists != ID_FALSE ) ),
                        ERR_INVALID_RANGE_FUNCTION );
    }
    else if ( aSetFuncType[0] == 'L' ) // from DBMS_SHARD.SET_SHARD_LIST()
    {
        IDE_TEST_RAISE( ( ( sTableInfo.mSplitMethod != SDI_SPLIT_LIST ) ||
                          ( sTableInfo.mSubKeyExists != ID_FALSE ) ),
                        ERR_INVALID_RANGE_FUNCTION );
    }
    else if ( aSetFuncType[0] == 'P' ) // from DBMS_SHARD.SET_SHARD_COMP()
    {
        IDE_TEST_RAISE( sTableInfo.mSubKeyExists != ID_TRUE, ERR_INVALID_RANGE_FUNCTION );

        IDE_TEST_RAISE( !( ( sTableInfo.mSplitMethod == SDI_SPLIT_RANGE ) ||
                           ( sTableInfo.mSplitMethod == SDI_SPLIT_HASH )  ||
                           ( sTableInfo.mSplitMethod == SDI_SPLIT_LIST ) ),
                        ERR_INVALID_RANGE_FUNCTION );

        IDE_TEST_RAISE( !( ( sTableInfo.mSubSplitMethod == SDI_SPLIT_RANGE ) ||
                           ( sTableInfo.mSubSplitMethod == SDI_SPLIT_HASH )  ||
                           ( sTableInfo.mSubSplitMethod == SDI_SPLIT_LIST ) ),
                        ERR_INVALID_RANGE_FUNCTION );
    }
    else
    {
        IDE_RAISE( ERR_INVALID_RANGE_FUNCTION );
    }

    // BUG-46619
    IDE_TEST( validateRangeCountBeforeInsert( aStatement,
                                              &sTableInfo,
                                              sMetaNodeInfo.mShardMetaNumber )
              != IDE_SUCCESS );

    // hash max는 1~1000까지만 가능하다.
    if ( sTableInfo.mSplitMethod == SDI_SPLIT_HASH )
    {
        sPosition.stmtText = (SChar*)(aValue);
        sPosition.offset   = 0;
        sPosition.size     = aValueLength;

        IDE_TEST_RAISE( qtc::getBigint( sPosition.stmtText,
                                        &sLongVal,
                                        &sPosition ) != IDE_SUCCESS,
                        ERR_INVALID_SHARD_KEY_RANGE );

        IDE_TEST_RAISE( ( sLongVal <= 0 ) || ( sLongVal > SDI_HASH_MAX_VALUE ),
                        ERR_INVALID_SHARD_KEY_RANGE );
    }
    else
    {
        IDE_TEST_RAISE( aValueLength == 0, ERR_INVALID_SHARD_KEY_RANGE );
    }

    if ( sTableInfo.mSubKeyExists == ID_TRUE )
    {
        if ( sTableInfo.mSubSplitMethod == SDI_SPLIT_HASH )
        {
            sPosition.stmtText = (SChar*)(aSubValue);
            sPosition.offset   = 0;
            sPosition.size     = aSubValueLength;

            IDE_TEST_RAISE( qtc::getBigint( sPosition.stmtText,
                                            &sLongVal,
                                            &sPosition ) != IDE_SUCCESS,
                            ERR_INVALID_SHARD_KEY_RANGE );

            IDE_TEST_RAISE( ( sLongVal <= 0 ) || ( sLongVal > SDI_HASH_MAX_VALUE ),
                            ERR_INVALID_SHARD_KEY_RANGE );
        }
        else
        {
            IDE_TEST_RAISE( aSubValueLength == 0, ERR_INVALID_SHARD_KEY_RANGE );
        }
    }
    else
    {
        IDE_TEST_RAISE( aSubValue[0] != '\0', ERR_INVALID_SHARD_KEY_RANGE );
    }

    IDE_TEST( STRUCT_ALLOC_WITH_SIZE( aStatement->qmxMem,
                                      SChar,
                                      QD_MAX_SQL_LENGTH,
                                      &sSqlStr )
              != IDE_SUCCESS);

    if ( sTableInfo.mSubKeyExists == ID_FALSE )
    {
        idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                         "INSERT INTO SYS_SHARD.RANGES_ VALUES( "
                         QCM_SQL_INT32_FMT", "    //shardID
                         QCM_SQL_VARCHAR_FMT", "  //aValue
                         QCM_SQL_VARCHAR_FMT", "  //aSubValue
                         QCM_SQL_INT32_FMT", "    //aNodeId
                         QCM_SQL_BIGINT_FMT") ",
                         sTableInfo.mShardID,
                         aValue,
                         "$$N/A",
                         sNode.mNodeId,
                         sMetaNodeInfo.mShardMetaNumber);
    }
    else
    {
        idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                         "INSERT INTO SYS_SHARD.RANGES_ VALUES( "
                         QCM_SQL_INT32_FMT", "    //shardID
                         QCM_SQL_VARCHAR_FMT", "  //aValue
                         QCM_SQL_VARCHAR_FMT", "  //aSubValue
                         QCM_SQL_INT32_FMT", "    //aNodeId
                         QCM_SQL_BIGINT_FMT") ",
                         sTableInfo.mShardID,
                         aValue,
                         aSubValue,
                         sNode.mNodeId,
                         sMetaNodeInfo.mShardMetaNumber);
    }

    IDE_TEST( qciMisc::runDMLforDDL( QC_SMI_STMT( aStatement ),
                                     sSqlStr,
                                     & sRowCnt )
              != IDE_SUCCESS );

    if ( aRowCnt != NULL )
    {
        *aRowCnt = (UInt)sRowCnt;
    }
    else
    {
        /* Nothing to do */
    }

    sdi::setShardMetaTouched( aStatement->session );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_NOT_EXIST_TABLE )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_SHARD_TABLE_NOT_EXIST ) );
    }
    IDE_EXCEPTION( ERR_INVALID_RANGE_FUNCTION )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_INVALID_RANGE_FUNCTION ) );
    }
    IDE_EXCEPTION( ERR_INVALID_SHARD_KEY_RANGE )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDF_INVALID_RANGE_VALUE, "") );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

// update range info
IDE_RC sdm::updateRange( qcStatement * aStatement,
                         SChar       * aUserName,
                         SChar       * aTableName,
                         SChar       * aOldNodeName,
                         SChar       * aNewNodeName,
                         SChar       * aValue,
                         SChar       * aSubValue,
                         UInt        * aRowCnt )
{
    SChar         * sSqlStr;
    vSLong          sRowCnt;

    sdiNode         sNewNode;
    sdiNode         sOldNode;
    sdiTableInfo    sTableInfo;
    idBool          sIsTableFound = ID_FALSE;

    sdiGlobalMetaNodeInfo sMetaNodeInfo = { ID_ULONG(0) };

    IDE_TEST( STRUCT_ALLOC_WITH_SIZE( aStatement->qmxMem,
                                      SChar,
                                      QD_MAX_SQL_LENGTH,
                                      &sSqlStr )
              != IDE_SUCCESS);

    IDE_TEST( checkMetaVersion( QC_SMI_STMT( aStatement ) )
              != IDE_SUCCESS );

    /* PROJ-2701 Online data rebuild */
    IDE_TEST( makeShardMeta4NewSMN(aStatement) != IDE_SUCCESS );

    IDE_TEST( getGlobalMetaNodeInfoCore( QC_SMI_STMT(aStatement),
                                         &sMetaNodeInfo ) != IDE_SUCCESS );

    // get tableInfo
    IDE_TEST( getTableInfo( QC_SMI_STMT( aStatement ),
                            aUserName,
                            aTableName,
                            sMetaNodeInfo.mShardMetaNumber,
                            &sTableInfo,
                            &sIsTableFound )
              != IDE_SUCCESS );

    // 테이블이 존재하지 않는다면 에러
    IDE_TEST_RAISE( sIsTableFound == ID_FALSE,
                    ERR_SHARD_OBJECT_DOES_NOT_EXIST );

    // get oldNodeInfo
    IDE_TEST( getNodeByName( QC_SMI_STMT( aStatement ),
                             aOldNodeName,
                             sMetaNodeInfo.mShardMetaNumber,
                             &sOldNode )
              != IDE_SUCCESS );

    // get newNodeInfo
    if ( aNewNodeName[0] != '\0' )
    {
        IDE_TEST( getNodeByName( QC_SMI_STMT( aStatement ),
                                 aNewNodeName,
                                 sMetaNodeInfo.mShardMetaNumber,
                                 &sNewNode )
                  != IDE_SUCCESS );
    }
    else
    {
        // Nothing to do.
    }

    if ( ( sTableInfo.mSplitMethod == SDI_SPLIT_HASH )  ||
         ( sTableInfo.mSplitMethod == SDI_SPLIT_RANGE ) ||
         ( sTableInfo.mSplitMethod == SDI_SPLIT_LIST )  ||
         ( sTableInfo.mSubKeyExists == ID_TRUE ) )
    {
        // HASH, RANGE, LIST, COMPOSITE
        if ( aNewNodeName[0] == '\0' )
        {
            // NewNodeName이 없다.
            // 해당 거주노드를 제거
            if ( aSubValue[0] == '\0' )
            {
                // HASH, RANGE, LIST
                idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                                 "DELETE "
                                 "  FROM SYS_SHARD.RANGES_ "
                                 " WHERE VALUE = "
                                 QCM_SQL_VARCHAR_FMT //aValue
                                 "   AND SHARD_ID = "
                                 QCM_SQL_INT32_FMT   // shardId
                                 "   AND NODE_ID = "
                                 QCM_SQL_INT32_FMT  // oldNodeId
                                 "   AND SMN = "
                                 QCM_SQL_BIGINT_FMT, // SMN
                                 aValue,
                                 sTableInfo.mShardID,
                                 sOldNode.mNodeId,
                                 sMetaNodeInfo.mShardMetaNumber );
            }
            else
            {
                // COMPOSITE
                idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                                 "DELETE "
                                 "  FROM SYS_SHARD.RANGES_ "
                                 " WHERE VALUE = "
                                 QCM_SQL_VARCHAR_FMT // aValue
                                 "   AND SUB_VALUE = "
                                 QCM_SQL_VARCHAR_FMT // aSubValue
                                 "   AND SHARD_ID = "
                                 QCM_SQL_INT32_FMT   // shardId
                                 "   AND NODE_ID = "
                                 QCM_SQL_INT32_FMT   // oldNodeId
                                 "   AND SMN = "
                                 QCM_SQL_BIGINT_FMT, // SMN
                                 aValue,
                                 aSubValue,
                                 sTableInfo.mShardID,
                                 sOldNode.mNodeId,
                                 sMetaNodeInfo.mShardMetaNumber );
            }
        }
        else
        {
            // NewNodeName이 있다.
            // 해당 거주노드를 newNodeName으로 갱신
            if ( aSubValue[0] == '\0' )
            {
                // HASH, RANGE, LIST
                idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                                 "UPDATE SYS_SHARD.RANGES_ "
                                 "   SET NODE_ID = "
                                 QCM_SQL_INT32_FMT   // newNodeId
                                 " WHERE VALUE = "
                                 QCM_SQL_VARCHAR_FMT // aValue
                                 "   AND SHARD_ID = "
                                 QCM_SQL_INT32_FMT   // shardId
                                 "   AND NODE_ID = "
                                 QCM_SQL_INT32_FMT   // oldNodeId
                                 "   AND SMN = "
                                 QCM_SQL_BIGINT_FMT, // SMN
                                 sNewNode.mNodeId,
                                 aValue,
                                 sTableInfo.mShardID,
                                 sOldNode.mNodeId,
                                 sMetaNodeInfo.mShardMetaNumber );
            }
            else
            {
                // COMPOSITE
                idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                                 "UPDATE SYS_SHARD.RANGES_ "
                                 "   SET NODE_ID = "
                                 QCM_SQL_INT32_FMT   // newNodeId
                                 " WHERE VALUE = "
                                 QCM_SQL_VARCHAR_FMT // aValue
                                 "   AND SUB_VALUE = "
                                 QCM_SQL_VARCHAR_FMT // aSubValue
                                 "   AND SHARD_ID = "
                                 QCM_SQL_INT32_FMT   // shardId
                                 "   AND NODE_ID = "
                                 QCM_SQL_INT32_FMT   // oldNodeId
                                 "   AND SMN = "
                                 QCM_SQL_BIGINT_FMT, // SMN
                                 sNewNode.mNodeId,
                                 aValue,
                                 aSubValue,
                                 sTableInfo.mShardID,
                                 sOldNode.mNodeId,
                                 sMetaNodeInfo.mShardMetaNumber );
            }
        }
    }
    else if ( sTableInfo.mSplitMethod == SDI_SPLIT_CLONE )
    {
        // CLONE
        if ( aNewNodeName[0] == '\0' )
        {
            // NewNodeName이 없다.
            // 해당 거주노드를 제거
            idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                             "DELETE "
                             "  FROM SYS_SHARD.CLONES_ "
                             " WHERE SHARD_ID = "
                             QCM_SQL_INT32_FMT  // shardId
                             "   AND NODE_ID = "
                             QCM_SQL_INT32_FMT  // oldNodeId
                             "   AND SMN = "
                             QCM_SQL_BIGINT_FMT, // SMN
                             sTableInfo.mShardID,
                             sOldNode.mNodeId,
                             sMetaNodeInfo.mShardMetaNumber );
        }
        else
        {
            // NewNodeName이 있다.
            // 해당 거주노드를 newNodeName으로 갱신
            idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                             "UPDATE SYS_SHARD.CLONES_ "
                             "   SET NODE_ID = "
                             QCM_SQL_INT32_FMT   // nodeId
                             " WHERE SHARD_ID = "
                             QCM_SQL_INT32_FMT   // shardId
                             "   AND NODE_ID = "
                             QCM_SQL_INT32_FMT   // oldNodeId
                             "   AND SMN = "
                             QCM_SQL_BIGINT_FMT, // SMN
                             sNewNode.mNodeId,
                             sTableInfo.mShardID,
                             sOldNode.mNodeId,
                             sMetaNodeInfo.mShardMetaNumber );
        }
    }
    else
    {
        // SOLO
        if ( aNewNodeName[0] == '\0' )
        {
            // NewNodeName이 없다.
            // 해당 거주노드를 제거
            idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                             "DELETE "
                             "  FROM SYS_SHARD.SOLOS_ "
                             " WHERE SHARD_ID = "
                             QCM_SQL_INT32_FMT   // shardId
                             "   AND NODE_ID = "
                             QCM_SQL_INT32_FMT   // oldNodeId
                             "   AND SMN = "
                             QCM_SQL_BIGINT_FMT, // SMN
                             sTableInfo.mShardID,
                             sOldNode.mNodeId,
                             sMetaNodeInfo.mShardMetaNumber );

        }
        else
        {
            // NewNodeName이 있다.
            // 해당 거주노드를 newNodeName으로 갱신
            idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                             "UPDATE SYS_SHARD.SOLOS_ "
                             "   SET NODE_ID = "
                             QCM_SQL_INT32_FMT  // nodeId
                             " WHERE SHARD_ID = "
                             QCM_SQL_INT32_FMT  // shardId
                             "   AND NODE_ID = "
                             QCM_SQL_INT32_FMT  // oldNodeId
                             "   AND SMN = "
                             QCM_SQL_BIGINT_FMT, // SMN
                             sNewNode.mNodeId,
                             sTableInfo.mShardID,
                             sOldNode.mNodeId,
                             sMetaNodeInfo.mShardMetaNumber );
        }
    }

    IDE_TEST( qciMisc::runDMLforDDL( QC_SMI_STMT( aStatement ),
                                     sSqlStr,
                                     & sRowCnt )
              != IDE_SUCCESS );

    if ( aRowCnt != NULL )
    {
        *aRowCnt = (UInt)sRowCnt;
    }
    else
    {
        /* Nothing to do */
    }

    sdi::setShardMetaTouched( aStatement->session );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_SHARD_OBJECT_DOES_NOT_EXIST )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_SHARD_TABLE_NOT_EXIST ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

// insert clone info
IDE_RC sdm::insertClone( qcStatement * aStatement,
                         SChar       * aUserName,
                         SChar       * aTableName,
                         SChar       * aNodeName,
                         UInt        * aRowCnt )
{
    SChar         * sSqlStr;
    vSLong          sRowCnt;

    sdiNode         sNode;
    sdiTableInfo    sTableInfo;
    idBool          sIsTableFound = ID_FALSE;

    sdiGlobalMetaNodeInfo sMetaNodeInfo = { ID_ULONG(0) };

    IDE_TEST( checkMetaVersion( QC_SMI_STMT( aStatement ) )
              != IDE_SUCCESS );

    /* PROJ-2701 Online data rebuild */
    IDE_TEST( makeShardMeta4NewSMN(aStatement) != IDE_SUCCESS );

    IDE_TEST( getGlobalMetaNodeInfoCore( QC_SMI_STMT(aStatement),
                                         &sMetaNodeInfo ) != IDE_SUCCESS );

    IDE_TEST( getNodeByName( QC_SMI_STMT( aStatement ),
                             aNodeName,
                             sMetaNodeInfo.mShardMetaNumber,
                             &sNode )
              != IDE_SUCCESS );

    IDE_TEST( getTableInfo( QC_SMI_STMT( aStatement ),
                            aUserName,
                            aTableName,
                            sMetaNodeInfo.mShardMetaNumber,
                            &sTableInfo,
                            &sIsTableFound )
              != IDE_SUCCESS );

    IDE_TEST_RAISE( sIsTableFound == ID_FALSE,
                    ERR_NOT_EXIST_TABLE );

    IDE_TEST_RAISE( sTableInfo.mSplitMethod != SDI_SPLIT_CLONE,
                    ERR_INVALID_RANGE_FUNCTION );

    // BUG-46619
    IDE_TEST( validateRangeCountBeforeInsert( aStatement,
                                              &sTableInfo,
                                              sMetaNodeInfo.mShardMetaNumber )
              != IDE_SUCCESS );

    IDE_TEST( STRUCT_ALLOC_WITH_SIZE( aStatement->qmxMem,
                                      SChar,
                                      QD_MAX_SQL_LENGTH,
                                      &sSqlStr )
              != IDE_SUCCESS);

    idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                     "INSERT INTO SYS_SHARD.CLONES_ VALUES( "
                     QCM_SQL_INT32_FMT", "  //shardID
                     QCM_SQL_INT32_FMT", "  //aNodeId
                     QCM_SQL_BIGINT_FMT") ",
                     sTableInfo.mShardID,
                     sNode.mNodeId,
                     sMetaNodeInfo.mShardMetaNumber );

    IDE_TEST( qciMisc::runDMLforDDL( QC_SMI_STMT( aStatement ),
                                     sSqlStr,
                                     & sRowCnt )
              != IDE_SUCCESS );

    if ( aRowCnt != NULL )
    {
        *aRowCnt = (UInt)sRowCnt;
    }
    else
    {
        /* Nothing to do */
    }

    sdi::setShardMetaTouched( aStatement->session );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_NOT_EXIST_TABLE )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_SHARD_TABLE_NOT_EXIST ) );
    }
    IDE_EXCEPTION( ERR_INVALID_RANGE_FUNCTION )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_INVALID_RANGE_FUNCTION ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

// insert solo info
IDE_RC sdm::insertSolo( qcStatement * aStatement,
                        SChar       * aUserName,
                        SChar       * aTableName,
                        SChar       * aNodeName,
                        UInt        * aRowCnt )
{
    SChar         * sSqlStr;
    vSLong          sRowCnt;

    sdiNode         sNode;
    sdiTableInfo    sTableInfo;
    idBool          sIsTableFound = ID_FALSE;

    sdiGlobalMetaNodeInfo sMetaNodeInfo = { ID_ULONG(0) };

    /* PROJ-2701 Online data rebuild */
    IDE_TEST( makeShardMeta4NewSMN(aStatement) != IDE_SUCCESS );

    IDE_TEST( getGlobalMetaNodeInfoCore( QC_SMI_STMT(aStatement),
                                         &sMetaNodeInfo ) != IDE_SUCCESS );

    IDE_TEST( getNodeByName( QC_SMI_STMT( aStatement ),
                             aNodeName,
                             sMetaNodeInfo.mShardMetaNumber,
                             &sNode )
              != IDE_SUCCESS );

    IDE_TEST( getTableInfo( QC_SMI_STMT( aStatement ),
                            aUserName,
                            aTableName,
                            sMetaNodeInfo.mShardMetaNumber,
                            &sTableInfo,
                            &sIsTableFound )
              != IDE_SUCCESS );

    IDE_TEST_RAISE( sIsTableFound == ID_FALSE,
                    ERR_NOT_EXIST_TABLE );

    IDE_TEST_RAISE( sTableInfo.mSplitMethod != SDI_SPLIT_SOLO,
                    ERR_INVALID_RANGE_FUNCTION );

    // BUG-46619
    IDE_TEST( validateRangeCountBeforeInsert( aStatement,
                                              &sTableInfo,
                                              sMetaNodeInfo.mShardMetaNumber )
              != IDE_SUCCESS );

    IDE_TEST( STRUCT_ALLOC_WITH_SIZE( aStatement->qmxMem,
                                      SChar,
                                      QD_MAX_SQL_LENGTH,
                                      &sSqlStr )
              != IDE_SUCCESS);

    idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                     "INSERT INTO SYS_SHARD.SOLOS_ VALUES( "
                     QCM_SQL_INT32_FMT", "  //shardID
                     QCM_SQL_INT32_FMT", "  //aNodeId
                     QCM_SQL_BIGINT_FMT") ",
                     sTableInfo.mShardID,
                     sNode.mNodeId,
                     sMetaNodeInfo.mShardMetaNumber );

    IDE_TEST( qciMisc::runDMLforDDL( QC_SMI_STMT( aStatement ),
                                     sSqlStr,
                                     & sRowCnt )
              != IDE_SUCCESS );

    if ( aRowCnt != NULL )
    {
        *aRowCnt = (UInt)sRowCnt;
    }
    else
    {
        /* Nothing to do */
    }

    sdi::setShardMetaTouched( aStatement->session );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_NOT_EXIST_TABLE )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_SHARD_TABLE_NOT_EXIST ) );
    }
    IDE_EXCEPTION( ERR_INVALID_RANGE_FUNCTION )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_INVALID_RANGE_FUNCTION ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdm::getNodeByName( smiStatement * aSmiStmt,
                           SChar        * aNodeName,
                           ULong          aSMN,
                           sdiNode      * aNode )
{
    idBool            sIsCursorOpen = ID_FALSE;
    const void      * sRow          = NULL;
    scGRID            sRid;
    const void      * sSdmNodes;
    const void      * sSdmNodesIndex[SDM_MAX_META_INDICES];
    mtcColumn       * sNodeIDColumn;
    mtcColumn       * sNodeNameColumn;
    mtcColumn       * sPortColumn;
    mtcColumn       * sHostIPColumn;
    mtcColumn       * sAlternatePortColumn;
    mtcColumn       * sAlternateHostIPColumn;
    mtcColumn       * sSMNColumn;

    qcNameCharBuffer  sNameValueBuffer;
    mtdCharType     * sNameValue = ( mtdCharType * ) & sNameValueBuffer;

    mtdIntegerType    sID;
    mtdCharType     * sName;
    mtdIntegerType    sPort;
    mtdCharType     * sHostIP;
    mtdIntegerType    sAlternatePort;
    mtdCharType     * sAlternateHostIP;

    qtcMetaRangeColumn sNodeNameRange;
    qtcMetaRangeColumn sSMNRange;
    smiRange           sRange;

    smiTableCursor       sCursor;
    smiCursorProperties  sCursorProperty;

    IDE_TEST_RAISE( idlOS::strlen(aNodeName) > QC_MAX_OBJECT_NAME_LEN,
                    ERR_NOT_EXIST_NODE );

    IDE_TEST( getMetaTableAndIndex( aSmiStmt,
                                    SDM_NODES,
                                    & sSdmNodes,
                                    sSdmNodesIndex )
              != IDE_SUCCESS );

    IDE_TEST_RAISE( sSdmNodesIndex[SDM_NODES_IDX2_ORDER] == NULL,
                    ERR_META_HANDLE );

    IDE_TEST( smiGetTableColumns( sSdmNodes,
                                  SDM_NODES_NODE_ID_COL_ORDER,
                                  (const smiColumn**)&sNodeIDColumn )
              != IDE_SUCCESS );
    IDE_TEST( smiGetTableColumns( sSdmNodes,
                                  SDM_NODES_NODE_NAME_COL_ORDER,
                                  (const smiColumn**)&sNodeNameColumn )
              != IDE_SUCCESS );
    IDE_TEST( smiGetTableColumns( sSdmNodes,
                                  SDM_NODES_PORT_NO_COL_ORDER,
                                  (const smiColumn**)&sPortColumn )
              != IDE_SUCCESS );
    IDE_TEST( smiGetTableColumns( sSdmNodes,
                                  SDM_NODES_HOST_IP_COL_ORDER,
                                  (const smiColumn**)&sHostIPColumn )
              != IDE_SUCCESS );
    IDE_TEST( smiGetTableColumns( sSdmNodes,
                                  SDM_NODES_ALTERNATE_PORT_NO_COL_ORDER,
                                  (const smiColumn**)&sAlternatePortColumn )
              != IDE_SUCCESS );
    IDE_TEST( smiGetTableColumns( sSdmNodes,
                                  SDM_NODES_ALTERNATE_HOST_IP_COL_ORDER,
                                  (const smiColumn**)&sAlternateHostIPColumn )
              != IDE_SUCCESS );
    IDE_TEST( smiGetTableColumns( sSdmNodes,
                                  SDM_NODES_SMN_COL_ORDER,
                                  (const smiColumn**)&sSMNColumn )
              != IDE_SUCCESS );

    // mtdModule 설정
    IDE_TEST( mtd::moduleById( &(sNodeNameColumn->module),
                               sNodeNameColumn->type.dataTypeId )
              != IDE_SUCCESS );

    // mtlModule 설정
    IDE_TEST( mtl::moduleById( &sNodeNameColumn->language,
                               sNodeNameColumn->type.languageId )
              != IDE_SUCCESS );
    // mtdModule 설정
    IDE_TEST( mtd::moduleById( &(sSMNColumn->module),
                               sSMNColumn->type.dataTypeId )
              != IDE_SUCCESS );

    // mtlModule 설정
    IDE_TEST( mtl::moduleById( &sSMNColumn->language,
                               sSMNColumn->type.languageId )
              != IDE_SUCCESS );

    qciMisc::setVarcharValue( sNameValue,
                              NULL,
                              aNodeName,
                              idlOS::strlen(aNodeName) );

    qciMisc::makeMetaRangeDoubleColumn(
        &sNodeNameRange,
        &sSMNRange,
        sNodeNameColumn,
        (const void *) sNameValue,
        sSMNColumn,
        (const void *) &aSMN,
        &sRange );

    sCursor.initialize();

    /* PROJ-2622 */
    SMI_CURSOR_PROP_INIT_FOR_META_INDEX_SCAN( &sCursorProperty, NULL );

    IDE_TEST( sCursor.open(
                  aSmiStmt,
                  sSdmNodes,
                  sSdmNodesIndex[SDM_NODES_IDX2_ORDER],
                  smiGetRowSCN( sSdmNodes ),
                  NULL,
                  &sRange,
                  smiGetDefaultKeyRange(),
                  smiGetDefaultFilter(),
                  QCM_META_CURSOR_FLAG,
                  SMI_SELECT_CURSOR,
                  &sCursorProperty )
              != IDE_SUCCESS );

    sIsCursorOpen = ID_TRUE;

    IDE_TEST( sCursor.beforeFirst() != IDE_SUCCESS );

    IDE_TEST( sCursor.readRow( &sRow, &sRid, SMI_FIND_NEXT )
              != IDE_SUCCESS );

    IDE_TEST_RAISE( sRow == NULL, ERR_NOT_EXIST_NODE );

    sID = *(mtdIntegerType*)((SChar *)sRow + sNodeIDColumn->column.offset );

    /* PROJ-2638 */
    sName = (mtdCharType*)((SChar *)sRow + sNodeNameColumn->column.offset );
    sPort = *(mtdIntegerType*)((SChar *)sRow + sPortColumn->column.offset );
    sHostIP = (mtdCharType*)((SChar *)sRow + sHostIPColumn->column.offset );
    sAlternatePort = *(mtdIntegerType*)((SChar *)sRow + sAlternatePortColumn->column.offset );
    sAlternateHostIP = (mtdCharType*)((SChar *)sRow + sAlternateHostIPColumn->column.offset );

    aNode->mNodeId = sID;

    if ( sHostIP->length <= 15 )
    {
        idlOS::memcpy( aNode->mServerIP,
                       sHostIP->value,
                       sHostIP->length );
        aNode->mServerIP[sHostIP->length] = '\0';
    }
    else
    {
        IDE_RAISE( IP_STR_OVERFLOW );
    }

    if ( sAlternateHostIP->length <= 15 )
    {
        if ( sAlternateHostIP->length > 0 )
        {
            idlOS::memcpy( aNode->mAlternateServerIP,
                           sAlternateHostIP->value,
                           sAlternateHostIP->length );
            aNode->mAlternateServerIP[sAlternateHostIP->length] = '\0';
        }
        else
        {
            aNode->mAlternateServerIP[0] = '\0';
        }
    }
    else
    {
        IDE_RAISE( IP_STR_OVERFLOW );
    }

    if ( sName->length <= 40 )
    {
        idlOS::memcpy( aNode->mNodeName,
                       sName->value,
                       sName->length );
        aNode->mNodeName[sName->length] = '\0';
    }
    else
    {
        IDE_RAISE( NODE_NAME_OVERFLOW );
    }

    aNode->mPortNo = (UShort)sPort;
    if ( sAlternatePort == MTD_INTEGER_NULL )
    {
        aNode->mAlternatePortNo = 0;
    }
    else
    {
        aNode->mAlternatePortNo = (UShort)sAlternatePort;
    }

    sIsCursorOpen = ID_FALSE;
    IDE_TEST( sCursor.close() != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_META_HANDLE )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_SHARD_META_NOT_CREATED,
                                  SDM_NODES ) );
    }
    IDE_EXCEPTION( ERR_NOT_EXIST_NODE )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_SHARD_NODE_NOT_EXIST ) );
    }
    IDE_EXCEPTION( IP_STR_OVERFLOW )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QRC_INVALID_HOST_IP_PORT ) );
    }
    IDE_EXCEPTION( NODE_NAME_OVERFLOW )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDF_SHARD_NODE_NAME_TOO_LONG ) );
    }
    IDE_EXCEPTION_END;

    if ( sIsCursorOpen == ID_TRUE )
    {
        (void)sCursor.close();
    }
    else
    {
        /* Nothing to do */
    }

    return IDE_FAILURE;
}

IDE_RC sdm::getExternalNodeInfo( smiStatement * aSmiStmt,
                                 sdiNodeInfo  * aNodeInfo,
                                 ULong          aSMN )
{
    idBool            sIsCursorOpen = ID_FALSE;
    const void      * sRow          = NULL;
    scGRID            sRid;
    const void      * sSdmNodes;
    const void      * sSdmNodesIndex[SDM_MAX_META_INDICES];
    mtcColumn       * sNodeIDColumn;
    mtcColumn       * sNodeNameColumn;
    mtcColumn       * sPortColumn;
    mtcColumn       * sHostIPColumn;
    mtcColumn       * sAlternatePortColumn;
    mtcColumn       * sAlternateHostIPColumn;
    mtcColumn       * sSMNColumn;

    UShort            sCount = 0;

    mtdIntegerType    sID;
    mtdIntegerType    sPort;
    mtdCharType     * sHostIP;
    mtdIntegerType    sAlternatePort;
    mtdCharType     * sAlternateHostIP;

    /* PROJ-2638 */
    mtdCharType     * sName;

    smiTableCursor       sCursor;
    smiCursorProperties  sCursorProperty;

    qtcMetaRangeColumn sSMNRange;
    smiRange           sRange;

    /* init */
    aNodeInfo->mCount = 0;

    IDE_TEST( checkMetaVersion( aSmiStmt )
              != IDE_SUCCESS );

    IDE_TEST( getMetaTableAndIndex( aSmiStmt,
                                    SDM_NODES,
                                    & sSdmNodes,
                                    sSdmNodesIndex )
              != IDE_SUCCESS );

    IDE_TEST_RAISE( sSdmNodesIndex[SDM_NODES_IDX3_ORDER] == NULL,
                    ERR_META_HANDLE );

    IDE_TEST( smiGetTableColumns( sSdmNodes,
                                  SDM_NODES_NODE_ID_COL_ORDER,
                                  (const smiColumn**)&sNodeIDColumn )
              != IDE_SUCCESS );
    IDE_TEST( smiGetTableColumns( sSdmNodes,
                                  SDM_NODES_NODE_NAME_COL_ORDER,
                                  (const smiColumn**)&sNodeNameColumn )
              != IDE_SUCCESS );
    IDE_TEST( smiGetTableColumns( sSdmNodes,
                                  SDM_NODES_PORT_NO_COL_ORDER,
                                  (const smiColumn**)&sPortColumn )
              != IDE_SUCCESS );
    IDE_TEST( smiGetTableColumns( sSdmNodes,
                                  SDM_NODES_HOST_IP_COL_ORDER,
                                  (const smiColumn**)&sHostIPColumn )
              != IDE_SUCCESS );
    IDE_TEST( smiGetTableColumns( sSdmNodes,
                                  SDM_NODES_ALTERNATE_PORT_NO_COL_ORDER,
                                  (const smiColumn**)&sAlternatePortColumn )
              != IDE_SUCCESS );
    IDE_TEST( smiGetTableColumns( sSdmNodes,
                                  SDM_NODES_ALTERNATE_HOST_IP_COL_ORDER,
                                  (const smiColumn**)&sAlternateHostIPColumn )
              != IDE_SUCCESS );
    IDE_TEST( smiGetTableColumns( sSdmNodes,
                                  SDM_NODES_SMN_COL_ORDER,
                                  (const smiColumn**)&sSMNColumn )
              != IDE_SUCCESS );

    // mtdModule 설정
    IDE_TEST( mtd::moduleById( &(sSMNColumn->module),
                               sSMNColumn->type.dataTypeId )
              != IDE_SUCCESS );

    // mtlModule 설정
    IDE_TEST( mtl::moduleById( &sSMNColumn->language,
                               sSMNColumn->type.languageId )
              != IDE_SUCCESS );

    qciMisc::makeMetaRangeSingleColumn(
        &sSMNRange,
        (const mtcColumn *) sSMNColumn,
        (const void *) &aSMN,
        &sRange );

    sCursor.initialize();

    /* PROJ-2622 */
    SMI_CURSOR_PROP_INIT_FOR_META_INDEX_SCAN( &sCursorProperty, NULL );

    IDE_TEST( sCursor.open(
                  aSmiStmt,
                  sSdmNodes,
                  sSdmNodesIndex[SDM_NODES_IDX3_ORDER],
                  smiGetRowSCN( sSdmNodes ),
                  NULL,
                  &sRange,
                  smiGetDefaultKeyRange(),
                  smiGetDefaultFilter(),
                  QCM_META_CURSOR_FLAG,
                  SMI_SELECT_CURSOR,
                  &sCursorProperty )
              != IDE_SUCCESS );

    sIsCursorOpen = ID_TRUE;

    IDE_TEST( sCursor.beforeFirst() != IDE_SUCCESS );

    IDE_TEST( sCursor.readRow( &sRow, &sRid, SMI_FIND_NEXT )
              != IDE_SUCCESS );

    while ( sRow != NULL )
    {
        sID = *(mtdIntegerType*)((SChar *)sRow + sNodeIDColumn->column.offset );

        /* PROJ-2638 */
        sName = (mtdCharType*)((SChar *)sRow + sNodeNameColumn->column.offset );
        sPort = *(mtdIntegerType*)((SChar *)sRow + sPortColumn->column.offset );
        sHostIP = (mtdCharType*)((SChar *)sRow + sHostIPColumn->column.offset );
        sAlternatePort = *(mtdIntegerType*)((SChar *)sRow + sAlternatePortColumn->column.offset );
        sAlternateHostIP = (mtdCharType*)((SChar *)sRow + sAlternateHostIPColumn->column.offset );

        aNodeInfo->mNodes[sCount].mNodeId = sID;

        if ( sHostIP->length <= 15 )
        {
            idlOS::memcpy( aNodeInfo->mNodes[sCount].mServerIP,
                           sHostIP->value,
                           sHostIP->length );
            aNodeInfo->mNodes[sCount].mServerIP[sHostIP->length] = '\0';
        }
        else
        {
            IDE_RAISE( IP_STR_OVERFLOW );
        }

        if ( sAlternateHostIP->length <= 15 )
        {
            if ( sAlternateHostIP->length > 0 )
            {
                idlOS::memcpy( aNodeInfo->mNodes[sCount].mAlternateServerIP,
                               sAlternateHostIP->value,
                               sAlternateHostIP->length );
                aNodeInfo->mNodes[sCount].mAlternateServerIP[sAlternateHostIP->length] = '\0';
            }
            else
            {
                aNodeInfo->mNodes[sCount].mAlternateServerIP[0] = '\0';
            }
        }
        else
        {
            IDE_RAISE( IP_STR_OVERFLOW );
        }

        if ( sName->length <= SDI_NODE_NAME_MAX_SIZE )
        {
            idlOS::memcpy( aNodeInfo->mNodes[sCount].mNodeName,
                           sName->value,
                           sName->length );
            aNodeInfo->mNodes[sCount].mNodeName[sName->length] = '\0';
        }
        else
        {
            IDE_RAISE( NODE_NAME_OVERFLOW );
        }

        aNodeInfo->mNodes[sCount].mPortNo = (UShort)sPort;
        if ( sAlternatePort == MTD_INTEGER_NULL )
        {
            aNodeInfo->mNodes[sCount].mAlternatePortNo = 0;
        }
        else
        {
            aNodeInfo->mNodes[sCount].mAlternatePortNo = (UShort)sAlternatePort;
        }

        aNodeInfo->mNodes[sCount].mConnectType = SDI_DATA_NODE_CONNECT_TYPE_DEFAULT;

        sCount++;

        if ( sCount >= SDI_NODE_MAX_COUNT )
        {
            IDE_RAISE( BUFFER_OVERFLOW );
        }
        else
        {
            /* Nothing to do */
        }

        IDE_TEST( sCursor.readRow( &sRow, &sRid, SMI_FIND_NEXT )
                  != IDE_SUCCESS );
    }

    sIsCursorOpen = ID_FALSE;
    IDE_TEST( sCursor.close() != IDE_SUCCESS );

    aNodeInfo->mCount = sCount;

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_META_HANDLE )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_SHARD_META_NOT_CREATED,
                                  SDM_NODES ) );
    }
    IDE_EXCEPTION( BUFFER_OVERFLOW )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_SHARD_NODE_OVERFLOW ) );
    }
    IDE_EXCEPTION( IP_STR_OVERFLOW )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QRC_INVALID_HOST_IP_PORT ) );
    }
    IDE_EXCEPTION( NODE_NAME_OVERFLOW )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDF_SHARD_NODE_NAME_TOO_LONG ) );
    }
    IDE_EXCEPTION_END;

    if ( sIsCursorOpen == ID_TRUE )
    {
        (void)sCursor.close();
    }
    else
    {
        /* Nothing to do */
    }

    return IDE_FAILURE;
}

IDE_RC sdm::getInternalNodeInfo( smiStatement * aSmiStmt,
                                 sdiNodeInfo  * aNodeInfo,
                                 ULong          aSMN )
{
    idBool            sIsCursorOpen = ID_FALSE;
    const void      * sRow          = NULL;
    scGRID            sRid;
    const void      * sSdmNodes;
    const void      * sSdmNodesIndex[SDM_MAX_META_INDICES];
    mtcColumn       * sNodeIDColumn;
    mtcColumn       * sNodeNameColumn;
    mtcColumn       * sPortColumn;
    mtcColumn       * sHostIPColumn;
    mtcColumn       * sAlternatePortColumn;
    mtcColumn       * sAlternateHostIPColumn;
    mtcColumn       * sConnectTypeColumn;
    mtcColumn       * sSMNColumn;

    UShort            sCount = 0;

    mtdIntegerType    sID;
    mtdIntegerType    sPort;
    mtdCharType     * sHostIP;
    mtdIntegerType    sAlternatePort;
    mtdCharType     * sAlternateHostIP;
    mtdIntegerType    sConnectType;

    /* PROJ-2638 */
    mtdCharType     * sName;

    smiTableCursor       sCursor;
    smiCursorProperties  sCursorProperty;

    qtcMetaRangeColumn sSMNRange;
    smiRange           sRange;

    /* init */
    aNodeInfo->mCount = 0;

    IDE_TEST( checkMetaVersion( aSmiStmt )
              != IDE_SUCCESS );

    IDE_TEST( getMetaTableAndIndex( aSmiStmt,
                                    SDM_NODES,
                                    & sSdmNodes,
                                    sSdmNodesIndex )
              != IDE_SUCCESS );

    IDE_TEST_RAISE( sSdmNodesIndex[SDM_NODES_IDX3_ORDER] == NULL,
                    ERR_META_HANDLE );

    IDE_TEST( smiGetTableColumns( sSdmNodes,
                                  SDM_NODES_NODE_ID_COL_ORDER,
                                  (const smiColumn**)&sNodeIDColumn )
              != IDE_SUCCESS );
    IDE_TEST( smiGetTableColumns( sSdmNodes,
                                  SDM_NODES_NODE_NAME_COL_ORDER,
                                  (const smiColumn**)&sNodeNameColumn )
              != IDE_SUCCESS );
    IDE_TEST( smiGetTableColumns( sSdmNodes,
                                  SDM_NODES_INTERNAL_PORT_NO_COL_ORDER,
                                  (const smiColumn**)&sPortColumn )
              != IDE_SUCCESS );
    IDE_TEST( smiGetTableColumns( sSdmNodes,
                                  SDM_NODES_INTERNAL_HOST_IP_COL_ORDER,
                                  (const smiColumn**)&sHostIPColumn )
              != IDE_SUCCESS );
    IDE_TEST( smiGetTableColumns( sSdmNodes,
                                  SDM_NODES_INTERNAL_ALTERNATE_PORT_NO_COL_ORDER,
                                  (const smiColumn**)&sAlternatePortColumn )
              != IDE_SUCCESS );
    IDE_TEST( smiGetTableColumns( sSdmNodes,
                                  SDM_NODES_INTERNAL_ALTERNATE_HOST_IP_COL_ORDER,
                                  (const smiColumn**)&sAlternateHostIPColumn )
              != IDE_SUCCESS );
    IDE_TEST( smiGetTableColumns( sSdmNodes,
                                  SDM_NODES_INTERNAL_CONNECT_TYPE,
                                  (const smiColumn**)&sConnectTypeColumn )
              != IDE_SUCCESS );
    IDE_TEST( smiGetTableColumns( sSdmNodes,
                                  SDM_NODES_SMN_COL_ORDER,
                                  (const smiColumn**)&sSMNColumn )
              != IDE_SUCCESS );

    // mtdModule 설정
    IDE_TEST( mtd::moduleById( &(sSMNColumn->module),
                               sSMNColumn->type.dataTypeId )
              != IDE_SUCCESS );

    // mtlModule 설정
    IDE_TEST( mtl::moduleById( &sSMNColumn->language,
                               sSMNColumn->type.languageId )
              != IDE_SUCCESS );

    qciMisc::makeMetaRangeSingleColumn(
        &sSMNRange,
        (const mtcColumn *) sSMNColumn,
        (const void *) &aSMN,
        &sRange );

    sCursor.initialize();

    /* PROJ-2622 */
    SMI_CURSOR_PROP_INIT_FOR_META_INDEX_SCAN( &sCursorProperty, NULL );

    IDE_TEST( sCursor.open(
                  aSmiStmt,
                  sSdmNodes,
                  sSdmNodesIndex[SDM_NODES_IDX3_ORDER],
                  smiGetRowSCN( sSdmNodes ),
                  NULL,
                  &sRange,
                  smiGetDefaultKeyRange(),
                  smiGetDefaultFilter(),
                  QCM_META_CURSOR_FLAG,
                  SMI_SELECT_CURSOR,
                  &sCursorProperty )
              != IDE_SUCCESS );

    sIsCursorOpen = ID_TRUE;

    IDE_TEST( sCursor.beforeFirst() != IDE_SUCCESS );

    IDE_TEST( sCursor.readRow( &sRow, &sRid, SMI_FIND_NEXT )
              != IDE_SUCCESS );

    while ( sRow != NULL )
    {
        sID = *(mtdIntegerType*)((SChar *)sRow + sNodeIDColumn->column.offset );

        /* PROJ-2638 */
        sName = (mtdCharType*)((SChar *)sRow + sNodeNameColumn->column.offset );
        sPort = *(mtdIntegerType*)((SChar *)sRow + sPortColumn->column.offset );
        sHostIP = (mtdCharType*)((SChar *)sRow + sHostIPColumn->column.offset );
        sAlternatePort = *(mtdIntegerType*)((SChar *)sRow + sAlternatePortColumn->column.offset );
        sAlternateHostIP = (mtdCharType*)((SChar *)sRow + sAlternateHostIPColumn->column.offset );
        sConnectType = *(mtdIntegerType*)((SChar *)sRow + sConnectTypeColumn->column.offset );

        aNodeInfo->mNodes[sCount].mNodeId = sID;

        if ( sHostIP->length <= 15 )
        {
            idlOS::memcpy( aNodeInfo->mNodes[sCount].mServerIP,
                           sHostIP->value,
                           sHostIP->length );
            aNodeInfo->mNodes[sCount].mServerIP[sHostIP->length] = '\0';
        }
        else
        {
            IDE_RAISE( IP_STR_OVERFLOW );
        }

        if ( sAlternateHostIP->length <= 15 )
        {
            if ( sAlternateHostIP->length > 0 )
            {
                idlOS::memcpy( aNodeInfo->mNodes[sCount].mAlternateServerIP,
                               sAlternateHostIP->value,
                               sAlternateHostIP->length );
                aNodeInfo->mNodes[sCount].mAlternateServerIP[sAlternateHostIP->length] = '\0';
            }
            else
            {
                aNodeInfo->mNodes[sCount].mAlternateServerIP[0] = '\0';
            }
        }
        else
        {
            IDE_RAISE( IP_STR_OVERFLOW );
        }

        if ( sName->length <= SDI_NODE_NAME_MAX_SIZE )
        {
            idlOS::memcpy( aNodeInfo->mNodes[sCount].mNodeName,
                           sName->value,
                           sName->length );
            aNodeInfo->mNodes[sCount].mNodeName[sName->length] = '\0';
        }
        else
        {
            // BUGBUG 이후 올바른 message 로 변경해야 함
            IDE_RAISE( NODE_NAME_OVERFLOW );
        }

        aNodeInfo->mNodes[sCount].mPortNo = (UShort)sPort;
        if ( sAlternatePort == MTD_INTEGER_NULL )
        {
            aNodeInfo->mNodes[sCount].mAlternatePortNo = 0;
        }
        else
        {
            aNodeInfo->mNodes[sCount].mAlternatePortNo = (UShort)sAlternatePort;
        }

        aNodeInfo->mNodes[sCount].mConnectType = (sdiDataNodeConnectType)sConnectType;

        sCount++;

        if ( sCount >= SDI_NODE_MAX_COUNT )
        {
            IDE_RAISE( BUFFER_OVERFLOW );
        }
        else
        {
            /* Nothing to do */
        }

        IDE_TEST( sCursor.readRow( &sRow, &sRid, SMI_FIND_NEXT )
                  != IDE_SUCCESS );
    }

    sIsCursorOpen = ID_FALSE;
    IDE_TEST( sCursor.close() != IDE_SUCCESS );

    aNodeInfo->mCount = sCount;

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_META_HANDLE )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_SHARD_META_NOT_CREATED,
                                  SDM_NODES ) );
    }
    IDE_EXCEPTION( BUFFER_OVERFLOW )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_SHARD_NODE_OVERFLOW ) );
    }
    IDE_EXCEPTION( IP_STR_OVERFLOW )
    {
        IDE_SET( ideSetErrorCode( qpERR_ABORT_QRC_INVALID_HOST_IP_PORT ) );
    }
    IDE_EXCEPTION( NODE_NAME_OVERFLOW )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDF_SHARD_NODE_NAME_TOO_LONG ) );
    }
    IDE_EXCEPTION_END;

    if ( sIsCursorOpen == ID_TRUE )
    {
        (void)sCursor.close();
    }
    else
    {
        /* Nothing to do */
    }

    return IDE_FAILURE;
}

IDE_RC sdm::getTableInfo( smiStatement * aSmiStmt,
                          SChar        * aUserName,
                          SChar        * aTableName,
                          ULong          aSMN,
                          sdiTableInfo * aTableInfo,
                          idBool       * aIsTableFound )
{
    idBool            sIsCursorOpen = ID_FALSE;
    idBool            sIsTableFound = ID_FALSE;
    const void      * sRow          = NULL;
    scGRID            sRid;
    const void      * sSdmObjects;
    const void      * sSdmObjectsIndex[SDM_MAX_META_INDICES];
    mtcColumn       * sShardIDColumn;
    mtcColumn       * sUserNameColumn;
    mtcColumn       * sObjectNameColumn;
    mtcColumn       * sObjectTypeColumn;
    mtcColumn       * sSplitMethodColumn;
    mtcColumn       * sKeyColumnNameColumn;
    mtcColumn       * sSubSplitMethodColumn;
    mtcColumn       * sSubKeyColumnNameColumn;
    mtcColumn       * sDefaultNodeIDColumn;
    mtcColumn       * sSMNColumn;

    mtdCharType     * sUserName;
    mtdCharType     * sObjectName;
    mtdCharType     * sObjectType;
    mtdCharType     * sSplitMethod;
    mtdCharType     * sKeyColumnName;
    mtdCharType     * sSubSplitMethod;
    mtdCharType     * sSubKeyColumnName;
    mtdIntegerType    sDefaultNodeID;

    qcNameCharBuffer   sUserNameBuffer;
    mtdCharType      * sUserNameBuf = ( mtdCharType * ) & sUserNameBuffer;
    qcNameCharBuffer   sObjectNameBuffer;
    mtdCharType      * sObjectNameBuf = ( mtdCharType * ) & sObjectNameBuffer;

    qtcMetaRangeColumn sUserNameRange;
    qtcMetaRangeColumn sTableNameRange;
    qtcMetaRangeColumn sSMNRange;
    smiRange           sRange;

    smiTableCursor       sCursor;
    smiCursorProperties  sCursorProperty;

    /* init */
    SDI_INIT_TABLE_INFO( aTableInfo );

    IDE_TEST_CONT( idlOS::strlen(aUserName) > QC_MAX_OBJECT_NAME_LEN,
                   LABEL_TABLE_NOT_FOUND );
    IDE_TEST_CONT( idlOS::strlen(aTableName) > QC_MAX_OBJECT_NAME_LEN,
                   LABEL_TABLE_NOT_FOUND );

    IDE_TEST( checkMetaVersion( aSmiStmt )
              != IDE_SUCCESS );

    IDE_TEST( getMetaTableAndIndex( aSmiStmt,
                                    SDM_OBJECTS,
                                    & sSdmObjects,
                                    sSdmObjectsIndex )
              != IDE_SUCCESS );

    IDE_TEST_RAISE( sSdmObjectsIndex[SDM_OBJECTS_IDX2_ORDER] == NULL,
                    ERR_META_HANDLE );

    IDE_TEST( smiGetTableColumns( sSdmObjects,
                                  SDM_OBJECTS_SHARD_ID_COL_ORDER,
                                  (const smiColumn**)&sShardIDColumn )
              != IDE_SUCCESS );
    IDE_TEST( smiGetTableColumns( sSdmObjects,
                                  SDM_OBJECTS_USER_NAME_COL_ORDER,
                                  (const smiColumn**)&sUserNameColumn )
              != IDE_SUCCESS );
    IDE_TEST( smiGetTableColumns( sSdmObjects,
                                  SDM_OBJECTS_OBJECT_NAME_COL_ORDER,
                                  (const smiColumn**)&sObjectNameColumn )
              != IDE_SUCCESS );
    IDE_TEST( smiGetTableColumns( sSdmObjects,
                                  SDM_OBJECTS_OBJECT_TYPE_COL_ORDER,
                                  (const smiColumn**)&sObjectTypeColumn )
              != IDE_SUCCESS );
    IDE_TEST( smiGetTableColumns( sSdmObjects,
                                  SDM_OBJECTS_SPLIT_METHOD_COL_ORDER,
                                  (const smiColumn**)&sSplitMethodColumn )
              != IDE_SUCCESS );
    IDE_TEST( smiGetTableColumns( sSdmObjects,
                                  SDM_OBJECTS_KEY_COLUMN_NAME_COL_ORDER,
                                  (const smiColumn**)&sKeyColumnNameColumn )
              != IDE_SUCCESS );
    IDE_TEST( smiGetTableColumns( sSdmObjects,
                                  SDM_OBJECTS_SUB_SPLIT_METHOD_COL_ORDER,
                                  (const smiColumn**)&sSubSplitMethodColumn )
              != IDE_SUCCESS );
    IDE_TEST( smiGetTableColumns( sSdmObjects,
                                  SDM_OBJECTS_SUB_KEY_COLUMN_NAME_COL_ORDER,
                                  (const smiColumn**)&sSubKeyColumnNameColumn )
              != IDE_SUCCESS );
    IDE_TEST( smiGetTableColumns( sSdmObjects,
                                  SDM_OBJECTS_DEFAULT_NODE_ID_COL_ORDER,
                                  (const smiColumn**)&sDefaultNodeIDColumn )
              != IDE_SUCCESS );
    IDE_TEST( smiGetTableColumns( sSdmObjects,
                                  SDM_OBJECTS_SMN_COL_ORDER,
                                  (const smiColumn**)&sSMNColumn )
              != IDE_SUCCESS );

    // mtdModule 설정
    IDE_TEST( mtd::moduleById( &(sUserNameColumn->module),
                               sUserNameColumn->type.dataTypeId )
              != IDE_SUCCESS );

    // mtlModule 설정
    IDE_TEST( mtl::moduleById( &sUserNameColumn->language,
                               sUserNameColumn->type.languageId )
              != IDE_SUCCESS );

    // mtdModule 설정
    IDE_TEST( mtd::moduleById( &(sObjectNameColumn->module),
                               sObjectNameColumn->type.dataTypeId )
              != IDE_SUCCESS );

    // mtlModule 설정
    IDE_TEST( mtl::moduleById( &sObjectNameColumn->language,
                               sObjectNameColumn->type.languageId )
              != IDE_SUCCESS );

    // mtdModule 설정
    IDE_TEST( mtd::moduleById( &(sSMNColumn->module),
                               sSMNColumn->type.dataTypeId )
              != IDE_SUCCESS );

    // mtlModule 설정
    IDE_TEST( mtl::moduleById( &sSMNColumn->language,
                               sSMNColumn->type.languageId )
              != IDE_SUCCESS );

    qtc::setVarcharValue( sUserNameBuf,
                          NULL,
                          (SChar*)aUserName,
                          idlOS::strlen(aUserName) );

    qtc::setVarcharValue( sObjectNameBuf,
                          NULL,
                          (SChar*)aTableName,
                          idlOS::strlen(aTableName) );

    qcm::makeMetaRangeTripleColumn(
        &sUserNameRange,
        &sTableNameRange,
        &sSMNRange,
        (const mtcColumn *) sUserNameColumn,
        (const void *) sUserNameBuf,
        (const mtcColumn *) sObjectNameColumn,
        (const void *) sObjectNameBuf,
        (const mtcColumn *) sSMNColumn,
        (const void *) &aSMN,
        &sRange );

    sCursor.initialize();

    SMI_CURSOR_PROP_INIT_FOR_META_INDEX_SCAN( &sCursorProperty, NULL );

    IDE_TEST( sCursor.open(
                  aSmiStmt,
                  sSdmObjects,
                  sSdmObjectsIndex[SDM_OBJECTS_IDX2_ORDER],
                  smiGetRowSCN( sSdmObjects ),
                  NULL,
                  &sRange,
                  smiGetDefaultKeyRange(),
                  smiGetDefaultFilter(),
                  QCM_META_CURSOR_FLAG,
                  SMI_SELECT_CURSOR,
                  &sCursorProperty )
              != IDE_SUCCESS );

    sIsCursorOpen = ID_TRUE;

    IDE_TEST( sCursor.beforeFirst() != IDE_SUCCESS );

    IDE_TEST( sCursor.readRow( &sRow, &sRid, SMI_FIND_NEXT )
              != IDE_SUCCESS );

    IDE_TEST_CONT( sRow == NULL, LABEL_TABLE_NOT_FOUND );

    sIsTableFound = ID_TRUE;

    aTableInfo->mShardID =
        *(mtdIntegerType*)((SChar *)sRow + sShardIDColumn->column.offset );

    sUserName = (mtdCharType*)((SChar *)sRow + sUserNameColumn->column.offset );
    idlOS::memcpy( aTableInfo->mUserName, sUserName->value, sUserName->length );
    aTableInfo->mUserName[sUserName->length] = '\0';

    sObjectName = (mtdCharType*)((SChar *)sRow + sObjectNameColumn->column.offset );
    idlOS::memcpy( aTableInfo->mObjectName, sObjectName->value, sObjectName->length );
    aTableInfo->mObjectName[sObjectName->length] = '\0';

    sObjectType = (mtdCharType*)((SChar *)sRow + sObjectTypeColumn->column.offset );
    if ( sObjectType->length == 1 )
    {
        aTableInfo->mObjectType = sObjectType->value[0];
    }
    else
    {
        aTableInfo->mObjectType = 0;
    }

    sSplitMethod = (mtdCharType*)((SChar *)sRow + sSplitMethodColumn->column.offset );
    if ( idlOS::strMatch( (SChar*)sSplitMethod->value, sSplitMethod->length,
                          "H", 1 ) == 0 )
    {
        aTableInfo->mSplitMethod = SDI_SPLIT_HASH;
    }
    else if ( idlOS::strMatch( (SChar*)sSplitMethod->value, sSplitMethod->length,
                               "R", 1 ) == 0 )
    {
        aTableInfo->mSplitMethod = SDI_SPLIT_RANGE;
    }
    else if ( idlOS::strMatch( (SChar*)sSplitMethod->value, sSplitMethod->length,
                               "L", 1 ) == 0 )
    {
        aTableInfo->mSplitMethod = SDI_SPLIT_LIST;
    }
    else if ( idlOS::strMatch( (SChar*)sSplitMethod->value, sSplitMethod->length,
                               "C", 1 ) == 0 )
    {
        aTableInfo->mSplitMethod = SDI_SPLIT_CLONE;
    }
    else if ( idlOS::strMatch( (SChar*)sSplitMethod->value, sSplitMethod->length,
                               "S", 1 ) == 0 )
    {
        aTableInfo->mSplitMethod = SDI_SPLIT_SOLO;
    }
    else
    {
        ideLog::log( IDE_SD_0, "[FAILURE] getTableInfo : unknown SPLIT_METHOD !!!\n");
        IDE_RAISE( ERR_META_CRASH );
    }

    sKeyColumnName = (mtdCharType*)((SChar *)sRow + sKeyColumnNameColumn->column.offset );
    idlOS::memcpy( aTableInfo->mKeyColumnName, sKeyColumnName->value, sKeyColumnName->length );
    aTableInfo->mKeyColumnName[sKeyColumnName->length] = '\0';

    /* PROJ-2655 Composite shard key */
    sSubSplitMethod = (mtdCharType*)((SChar *)sRow + sSubSplitMethodColumn->column.offset );

    if ( sSubSplitMethod->length == 1 )
    {
        if ( idlOS::strMatch( (SChar*)sSubSplitMethod->value, sSubSplitMethod->length,
                              "H", 1 ) == 0 )
        {
            aTableInfo->mSubSplitMethod = SDI_SPLIT_HASH;
        }
        else if ( idlOS::strMatch( (SChar*)sSubSplitMethod->value, sSubSplitMethod->length,
                                   "R", 1 ) == 0 )
        {
            aTableInfo->mSubSplitMethod = SDI_SPLIT_RANGE;
        }
        else if ( idlOS::strMatch( (SChar*)sSubSplitMethod->value, sSubSplitMethod->length,
                                   "L", 1 ) == 0 )
        {
            aTableInfo->mSubSplitMethod = SDI_SPLIT_LIST;
        }
        else if ( idlOS::strMatch( (SChar*)sSubSplitMethod->value, sSubSplitMethod->length,
                                   "C", 1 ) == 0 )
        {
            aTableInfo->mSubSplitMethod = SDI_SPLIT_CLONE;
        }
        else if ( idlOS::strMatch( (SChar*)sSubSplitMethod->value, sSubSplitMethod->length,
                                   "S", 1 ) == 0 )
        {
            aTableInfo->mSubSplitMethod = SDI_SPLIT_SOLO;
        }
        else
        {
            ideLog::log( IDE_SD_0, "[FAILURE] getTableInfo : unknown SPLIT_METHOD !!!\n");
            IDE_RAISE( ERR_META_CRASH );
        }

        aTableInfo->mSubKeyExists = ID_TRUE;
    }
    else if ( sSubSplitMethod->length > 1 )
    {
        ideLog::log( IDE_SD_0, "[FAILURE] getTableInfo : unknown SPLIT_METHOD !!!\n");
        IDE_RAISE( ERR_META_CRASH );
    }
    else // sSubSplitMethod->length == 0
    {
        /* SDI_INIT_TABLE_INFO()에서 초기화 되어있지만, 다시 설정 */
        aTableInfo->mSubKeyExists   = ID_FALSE;
        aTableInfo->mSubSplitMethod = SDI_SPLIT_NONE;
    }

    sSubKeyColumnName = (mtdCharType*)((SChar *)sRow + sSubKeyColumnNameColumn->column.offset );
    idlOS::memcpy( aTableInfo->mSubKeyColumnName, sSubKeyColumnName->value, sSubKeyColumnName->length );
    aTableInfo->mSubKeyColumnName[sSubKeyColumnName->length] = '\0';

    sDefaultNodeID =
        *(mtdIntegerType*)((SChar *)sRow + sDefaultNodeIDColumn->column.offset );
    if ( sDefaultNodeID == MTD_INTEGER_NULL )
    {
        aTableInfo->mDefaultNodeId = ID_UINT_MAX;
    }
    else
    {
        aTableInfo->mDefaultNodeId = (UInt) sDefaultNodeID;
    }

    IDE_EXCEPTION_CONT( LABEL_TABLE_NOT_FOUND )

    if ( sIsCursorOpen == ID_TRUE )
    {
        sIsCursorOpen = ID_FALSE;
        IDE_TEST( sCursor.close() != IDE_SUCCESS );
    }
    else
    {
        /* Nothing to do */
    }

    *aIsTableFound = sIsTableFound;

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_META_HANDLE )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_SHARD_META_NOT_CREATED,
                                  SDM_OBJECTS ) );
    }
    IDE_EXCEPTION( ERR_META_CRASH )
    {
        IDE_SET(ideSetErrorCode(qpERR_ABORT_QCM_META_CRASH));
    }
    IDE_EXCEPTION_END;

    if ( sIsCursorOpen == ID_TRUE )
    {
        (void)sCursor.close();
    }
    else
    {
        /* Nothing to do */
    }

    *aIsTableFound = sIsTableFound;

    return IDE_FAILURE;
}

IDE_RC sdm::getRangeInfo( qcStatement  * aStatement,
                          smiStatement * aSmiStmt,
                          ULong          aSMN,
                          sdiTableInfo * aTableInfo,
                          sdiRangeInfo * aRangeInfo )
{
    switch ( aTableInfo->mSplitMethod )
    {
        case SDI_SPLIT_HASH:
        case SDI_SPLIT_RANGE:
        case SDI_SPLIT_LIST:
            IDE_TEST( getRange( aStatement,
                                aSmiStmt,
                                aSMN,
                                aTableInfo,
                                aRangeInfo )
                      != IDE_SUCCESS );
            break;
        case SDI_SPLIT_CLONE:
            IDE_TEST( getClone( aStatement,
                                aSmiStmt,
                                aSMN,
                                aTableInfo,
                                aRangeInfo )
                      != IDE_SUCCESS );
            break;
        case SDI_SPLIT_SOLO:
            IDE_TEST( getSolo( aStatement,
                               aSmiStmt,
                               aSMN,
                               aTableInfo,
                               aRangeInfo )
                      != IDE_SUCCESS );
            break;
        default:
            IDE_DASSERT( 0 );
            break;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdm::getRange( qcStatement  * aStatement,
                      smiStatement * aSmiStmt,
                      ULong          aSMN,
                      sdiTableInfo * aTableInfo,
                      sdiRangeInfo * aRangeInfo )
{
    idBool            sIsCursorOpen = ID_FALSE;
    const void      * sRow          = NULL;
    scGRID            sRid;
    const void      * sSdmRanges;    
    const void      * sSdmRangesIndex[SDM_MAX_META_INDICES];
    mtcColumn       * sShardIDColumn;
    mtcColumn       * sValueColumn;
    mtcColumn       * sSubValueColumn;
    mtcColumn       * sNodeIDColumn;
    mtcColumn       * sSMNColumn;

    qtcMetaRangeColumn sShardIDRange;
    qtcMetaRangeColumn sSMNRange;
    smiRange           sRange;

    UInt              sCount = 0;

    mtdCharType     * sValue;
    mtdCharType     * sSubValue;
    mtdIntegerType    sNodeID;

    smiTableCursor       sCursor;
    smiCursorProperties  sCursorProperty;

    /* init */
    aRangeInfo->mCount = 0;
    aRangeInfo->mRanges = NULL;

    IDE_TEST( checkMetaVersion( aSmiStmt )
              != IDE_SUCCESS );

    IDE_TEST( getMetaTableAndIndex( aSmiStmt,
                                    SDM_RANGES,
                                    & sSdmRanges,
                                    sSdmRangesIndex )
              != IDE_SUCCESS );

    IDE_TEST_RAISE( sSdmRangesIndex[SDM_RANGES_IDX1_ORDER] == NULL,
                    ERR_META_HANDLE );

    IDE_TEST( smiGetTableColumns( sSdmRanges,
                                  SDM_RANGES_SHARD_ID_COL_ORDER,
                                  (const smiColumn**)&sShardIDColumn )
              != IDE_SUCCESS );
    IDE_TEST( smiGetTableColumns( sSdmRanges,
                                  SDM_RANGES_VALUE_ID_COL_ORDER,
                                  (const smiColumn**)&sValueColumn )
              != IDE_SUCCESS );
    IDE_TEST( smiGetTableColumns( sSdmRanges,
                                  SDM_RANGES_SUB_VALUE_ID_COL_ORDER,
                                  (const smiColumn**)&sSubValueColumn )
              != IDE_SUCCESS );
    IDE_TEST( smiGetTableColumns( sSdmRanges,
                                  SDM_RANGES_NODE_ID_COL_ORDER,
                                  (const smiColumn**)&sNodeIDColumn )
              != IDE_SUCCESS );
    IDE_TEST( smiGetTableColumns( sSdmRanges,
                                  SDM_RANGES_SMN_COL_ORDER,
                                  (const smiColumn**)&sSMNColumn )
              != IDE_SUCCESS );

    // mtdModule 설정
    IDE_TEST( mtd::moduleById( &(sShardIDColumn->module),
                               sShardIDColumn->type.dataTypeId )
              != IDE_SUCCESS );

    // mtlModule 설정
    IDE_TEST( mtl::moduleById( &sShardIDColumn->language,
                               sShardIDColumn->type.languageId )
              != IDE_SUCCESS );

    // mtdModule 설정
    IDE_TEST( mtd::moduleById( &(sSMNColumn->module),
                               sSMNColumn->type.dataTypeId )
              != IDE_SUCCESS );

    // mtlModule 설정
    IDE_TEST( mtl::moduleById( &sSMNColumn->language,
                               sSMNColumn->type.languageId )
              != IDE_SUCCESS );

    qciMisc::makeMetaRangeDoubleColumn(
        &sShardIDRange,
        &sSMNRange,
        sShardIDColumn,
        (const void *)&(aTableInfo->mShardID),
        sSMNColumn,
        (const void *)&aSMN,
        &sRange );

    sCursor.initialize();

    SMI_CURSOR_PROP_INIT_FOR_META_INDEX_SCAN( &sCursorProperty, NULL );

    IDE_TEST( sCursor.open(
                  aSmiStmt,
                  sSdmRanges,
                  sSdmRangesIndex[SDM_RANGES_IDX1_ORDER],
                  smiGetRowSCN( sSdmRanges ),
                  NULL,
                  &sRange,
                  smiGetDefaultKeyRange(),
                  smiGetDefaultFilter(),
                  QCM_META_CURSOR_FLAG,
                  SMI_SELECT_CURSOR,
                  &sCursorProperty )
              != IDE_SUCCESS );

    sIsCursorOpen = ID_TRUE;

    IDE_TEST( sCursor.beforeFirst() != IDE_SUCCESS );

    IDE_TEST( sCursor.readRow( &sRow, &sRid, SMI_FIND_NEXT )
              != IDE_SUCCESS );

    // BUG-45718
    // Get record count
    while ( sRow != NULL )
    {
        sCount++;

        IDE_TEST( sCursor.readRow( &sRow, &sRid, SMI_FIND_NEXT )
                  != IDE_SUCCESS );
    }

    // Alloc memory as much as record count
    if ( sCount > 0 )
    {
        IDE_TEST_RAISE( sCount > SDI_RANGE_MAX_COUNT, RANGE_COUNT_OVERFLOW );

        IDE_TEST( QC_QMP_MEM(aStatement)->alloc( ID_SIZEOF(sdiRange) * sCount,
                                                 (void**) & aRangeInfo->mRanges )
                  != IDE_SUCCESS );

        IDE_TEST( sCursor.beforeFirst() != IDE_SUCCESS );

        IDE_TEST( sCursor.readRow( &sRow, &sRid, SMI_FIND_NEXT )
                  != IDE_SUCCESS );

        // Set ranges
        while ( sRow != NULL )
        {
            sValue = (mtdCharType*)((SChar *)sRow + sValueColumn->column.offset );
            sSubValue = (mtdCharType*)((SChar *)sRow + sSubValueColumn->column.offset );
            sNodeID  = *(mtdIntegerType*)((SChar *)sRow + sNodeIDColumn->column.offset );

            aRangeInfo->mRanges[aRangeInfo->mCount].mNodeId = (UInt)sNodeID;

            // shard key의 range value string을 알맞은 data type으로 변환
            if ( aTableInfo->mSplitMethod == SDI_SPLIT_HASH )
            {
                // hash는 integer로 변환
                IDE_TEST( convertRangeValue( (SChar*)sValue->value,
                                             sValue->length,
                                             MTD_INTEGER_ID,
                                             &(aRangeInfo->mRanges[aRangeInfo->mCount].mValue) )
                          != IDE_SUCCESS );
            }
            else
            {
                // range, list는 해당 key type으로 변환
                IDE_TEST( convertRangeValue( (SChar*)sValue->value,
                                             sValue->length,
                                             aTableInfo->mKeyDataType,
                                             &(aRangeInfo->mRanges[aRangeInfo->mCount].mValue) )
                          != IDE_SUCCESS );
            }

            // sub-shard key의 range value string을 알맞은 data type으로 변환
            if ( aTableInfo->mSubKeyExists == ID_TRUE )
            {
                if ( aTableInfo->mSubSplitMethod == SDI_SPLIT_HASH )
                {
                    // hash는 integer로 변환
                    IDE_TEST( convertRangeValue( (SChar*)sSubValue->value,
                                                 sSubValue->length,
                                                 MTD_INTEGER_ID,
                                                 &(aRangeInfo->mRanges[aRangeInfo->mCount].mSubValue) )
                              != IDE_SUCCESS );
                }
                else
                {
                    // range, list는 해당 key type으로 변환
                    IDE_TEST( convertRangeValue( (SChar*)sSubValue->value,
                                                 sSubValue->length,
                                                 aTableInfo->mSubKeyDataType,
                                                 &(aRangeInfo->mRanges[aRangeInfo->mCount].mSubValue) )
                              != IDE_SUCCESS );
                }
            }
            else
            {
                /* Nothing to do. */
            }

            aRangeInfo->mCount++;

            IDE_TEST( sCursor.readRow( &sRow, &sRid, SMI_FIND_NEXT )
                      != IDE_SUCCESS );
        }
    }
    else
    {
        // Nothing to do.
    }

    sIsCursorOpen = ID_FALSE;
    IDE_TEST( sCursor.close() != IDE_SUCCESS );

    // 숫자 타입으로 변환된 string의 경우
    // index를 이용한 정렬이 올바르지 않아 추가 정렬이 필요하다.
    IDE_TEST( shardRangeSort( aTableInfo->mSplitMethod,
                              aTableInfo->mKeyDataType,
                              aTableInfo->mSubKeyExists,
                              aTableInfo->mSubSplitMethod,
                              aTableInfo->mSubKeyDataType,
                              aRangeInfo )
              != IDE_SUCCESS );

    // 정렬 후 중복된 분산 정의를 합쳐준다.
    IDE_TEST( shardEliminateDuplication( aTableInfo,
                                         aRangeInfo )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_META_HANDLE )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_SHARD_META_NOT_CREATED,
                                  SDM_RANGES ) );
    }
    IDE_EXCEPTION( RANGE_COUNT_OVERFLOW )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_SHARD_RANGE_OVERFLOW ) );
    }
    IDE_EXCEPTION_END;

    if ( sIsCursorOpen == ID_TRUE )
    {
        (void)sCursor.close();
    }
    else
    {
        /* Nothing to do */
    }

    return IDE_FAILURE;
}

IDE_RC sdm::getClone( qcStatement  * aStatement,
                      smiStatement * aSmiStmt,
                      ULong          aSMN,
                      sdiTableInfo * aTableInfo,
                      sdiRangeInfo * aRangeInfo )
{
    idBool               sIsCursorOpen = ID_FALSE;
    const void         * sRow          = NULL;
    scGRID               sRid;
    const void         * sSdmClone;
    const void         * sSdmCloneIndex[SDM_MAX_META_INDICES];
    mtcColumn          * sShardIDColumn;

    mtcColumn          * sNodeIDColumn;
    mtcColumn          * sSMNColumn;

    qtcMetaRangeColumn   sShardIDRange;
    qtcMetaRangeColumn   sSMNRange;
    smiRange             sRange;

    UInt                 sCount    = 0;

    mtdIntegerType       sNodeID;

    smiTableCursor       sCursor;
    smiCursorProperties  sCursorProperty;

    /* init */
    aRangeInfo->mCount = 0;
    aRangeInfo->mRanges = NULL;

    IDE_TEST( checkMetaVersion( aSmiStmt )
              != IDE_SUCCESS );

    IDE_TEST( getMetaTableAndIndex( aSmiStmt,
                                    SDM_CLONES,
                                    & sSdmClone,
                                    sSdmCloneIndex )
              != IDE_SUCCESS );

    IDE_TEST_RAISE( sSdmCloneIndex[SDM_CLONES_IDX1_ORDER] == NULL,
                    ERR_META_HANDLE );

    IDE_TEST( smiGetTableColumns( sSdmClone,
                                  SDM_CLONES_SHARD_ID_COL_ORDER,
                                  (const smiColumn**)&sShardIDColumn )
              != IDE_SUCCESS );

    IDE_TEST( smiGetTableColumns( sSdmClone,
                                  SDM_CLONES_NODE_ID_COL_ORDER,
                                  (const smiColumn**)&sNodeIDColumn )
              != IDE_SUCCESS );

    IDE_TEST( smiGetTableColumns( sSdmClone,
                                  SDM_CLONES_SMN_COL_ORDER,
                                  (const smiColumn**)&sSMNColumn )
              != IDE_SUCCESS );

    // mtdModule 설정
    IDE_TEST( mtd::moduleById( &(sShardIDColumn->module),
                               sShardIDColumn->type.dataTypeId )
              != IDE_SUCCESS );

    // mtlModule 설정
    IDE_TEST( mtl::moduleById( &sShardIDColumn->language,
                               sShardIDColumn->type.languageId )
              != IDE_SUCCESS );

    // mtdModule 설정
    IDE_TEST( mtd::moduleById( &(sSMNColumn->module),
                               sSMNColumn->type.dataTypeId )
              != IDE_SUCCESS );

    // mtlModule 설정
    IDE_TEST( mtl::moduleById( &sSMNColumn->language,
                               sSMNColumn->type.languageId )
              != IDE_SUCCESS );

    qciMisc::makeMetaRangeDoubleColumn(
        &sShardIDRange,
        &sSMNRange,
        sShardIDColumn,
        (const void *)&(aTableInfo->mShardID),
        sSMNColumn,
        (const void *)&aSMN,
        &sRange );

    sCursor.initialize();

    SMI_CURSOR_PROP_INIT_FOR_META_INDEX_SCAN( &sCursorProperty, NULL );

    IDE_TEST( sCursor.open(
                  aSmiStmt,
                  sSdmClone,
                  sSdmCloneIndex[SDM_CLONES_IDX1_ORDER],
                  smiGetRowSCN( sSdmClone ),
                  NULL,
                  &sRange,
                  smiGetDefaultKeyRange(),
                  smiGetDefaultFilter(),
                  QCM_META_CURSOR_FLAG,
                  SMI_SELECT_CURSOR,
                  &sCursorProperty )
              != IDE_SUCCESS );

    sIsCursorOpen = ID_TRUE;

    IDE_TEST( sCursor.beforeFirst() != IDE_SUCCESS );

    IDE_TEST( sCursor.readRow( &sRow, &sRid, SMI_FIND_NEXT )
              != IDE_SUCCESS );

    // BUG-45718
    // Get record count
    while ( sRow != NULL )
    {
        sCount++;

        IDE_TEST( sCursor.readRow( &sRow, &sRid, SMI_FIND_NEXT )
                  != IDE_SUCCESS );
    }

    if ( sCount > 0 )
    {
        IDE_TEST_RAISE( sCount > SDI_RANGE_MAX_COUNT, RANGE_COUNT_OVERFLOW );

        // Alloc memory as much as record count
        IDE_TEST( QC_QMP_MEM(aStatement)->alloc( ID_SIZEOF(sdiRange) * sCount,
                                                 (void**) & aRangeInfo->mRanges )
                  != IDE_SUCCESS );

        IDE_TEST( sCursor.beforeFirst() != IDE_SUCCESS );

        IDE_TEST( sCursor.readRow( &sRow, &sRid, SMI_FIND_NEXT )
                  != IDE_SUCCESS );

        // Set ranges
        while ( sRow != NULL )
        {
            sNodeID  = *(mtdIntegerType*)((SChar *)sRow + sNodeIDColumn->column.offset );

            aRangeInfo->mRanges[aRangeInfo->mCount].mNodeId = (UInt)sNodeID;
            // Clone table 에서 hash value는 의미 없다. max로 설정
            aRangeInfo->mRanges[aRangeInfo->mCount].mValue.mHashMax = (UInt)SDI_RANGE_MAX_COUNT;

            aRangeInfo->mCount++;

            IDE_TEST( sCursor.readRow( &sRow, &sRid, SMI_FIND_NEXT )
                      != IDE_SUCCESS );
        }
    }
    else
    {
        // Nothing to do.
    }

    sIsCursorOpen = ID_FALSE;
    IDE_TEST( sCursor.close() != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_META_HANDLE )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_SHARD_META_NOT_CREATED,
                                  SDM_CLONES ) );
    }
    IDE_EXCEPTION( RANGE_COUNT_OVERFLOW )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_SHARD_RANGE_OVERFLOW ) );
    }
    IDE_EXCEPTION_END;

    if ( sIsCursorOpen == ID_TRUE )
    {
        (void)sCursor.close();
    }
    else
    {
        /* Nothing to do */
    }

    return IDE_FAILURE;
}

IDE_RC sdm::getSolo( qcStatement  * aStatement,
                     smiStatement * aSmiStmt,
                     ULong          aSMN,
                     sdiTableInfo * aTableInfo,
                     sdiRangeInfo * aRangeInfo )
{
    idBool               sIsCursorOpen = ID_FALSE;
    const void         * sRow          = NULL;
    scGRID               sRid;
    const void         * sSdmSolo;
    const void         * sSdmSoloIndex[SDM_MAX_META_INDICES];
    mtcColumn          * sShardIDColumn;

    mtcColumn          * sNodeIDColumn;
    mtcColumn          * sSMNColumn;

    qtcMetaRangeColumn   sShardIDRange;
    qtcMetaRangeColumn   sSMNRange;
    smiRange             sRange;

    UInt                 sCount    = 0;

    mtdIntegerType       sNodeID;

    smiTableCursor       sCursor;
    smiCursorProperties  sCursorProperty;

    /* init */
    aRangeInfo->mCount = 0;
    aRangeInfo->mRanges = NULL;

    IDE_TEST( getMetaTableAndIndex( aSmiStmt,
                                    SDM_SOLOS,
                                    & sSdmSolo,
                                    sSdmSoloIndex )
              != IDE_SUCCESS );

    IDE_TEST_RAISE( sSdmSoloIndex[SDM_SOLOS_IDX1_ORDER] == NULL,
                    ERR_META_HANDLE );

    IDE_TEST( smiGetTableColumns( sSdmSolo,
                                  SDM_SOLOS_SHARD_ID_COL_ORDER,
                                  (const smiColumn**)&sShardIDColumn )
              != IDE_SUCCESS );

    IDE_TEST( smiGetTableColumns( sSdmSolo,
                                  SDM_SOLOS_NODE_ID_COL_ORDER,
                                  (const smiColumn**)&sNodeIDColumn )
              != IDE_SUCCESS );

    IDE_TEST( smiGetTableColumns( sSdmSolo,
                                  SDM_SOLOS_SMN_COL_ORDER,
                                  (const smiColumn**)&sSMNColumn )
              != IDE_SUCCESS );

    // mtdModule 설정
    IDE_TEST( mtd::moduleById( &(sShardIDColumn->module),
                               sShardIDColumn->type.dataTypeId )
              != IDE_SUCCESS );

    // mtlModule 설정
    IDE_TEST( mtl::moduleById( &sShardIDColumn->language,
                               sShardIDColumn->type.languageId )
              != IDE_SUCCESS );

    // mtdModule 설정
    IDE_TEST( mtd::moduleById( &(sSMNColumn->module),
                               sSMNColumn->type.dataTypeId )
              != IDE_SUCCESS );

    // mtlModule 설정
    IDE_TEST( mtl::moduleById( &sSMNColumn->language,
                               sSMNColumn->type.languageId )
              != IDE_SUCCESS );

    qciMisc::makeMetaRangeDoubleColumn(
        &sShardIDRange,
        &sSMNRange,
        sShardIDColumn,
        (const void *)&(aTableInfo->mShardID),
        sSMNColumn,
        (const void *)&aSMN,
        &sRange );

    sCursor.initialize();

    SMI_CURSOR_PROP_INIT_FOR_META_INDEX_SCAN( &sCursorProperty, NULL );

    IDE_TEST( sCursor.open(
                  aSmiStmt,
                  sSdmSolo,
                  sSdmSoloIndex[SDM_SOLOS_IDX1_ORDER],
                  smiGetRowSCN( sSdmSolo ),
                  NULL,
                  &sRange,
                  smiGetDefaultKeyRange(),
                  smiGetDefaultFilter(),
                  QCM_META_CURSOR_FLAG,
                  SMI_SELECT_CURSOR,
                  &sCursorProperty )
              != IDE_SUCCESS );

    sIsCursorOpen = ID_TRUE;

    IDE_TEST( sCursor.beforeFirst() != IDE_SUCCESS );

    IDE_TEST( sCursor.readRow( &sRow, &sRid, SMI_FIND_NEXT )
              != IDE_SUCCESS );

    // BUG-45718
    // Get record count
    while ( sRow != NULL )
    {
        sCount++;

        IDE_TEST( sCursor.readRow( &sRow, &sRid, SMI_FIND_NEXT )
                  != IDE_SUCCESS );
    }

    if ( sCount > 0 )
    {
        IDE_TEST_RAISE( sCount > 1, RANGE_COUNT_OVERFLOW );

        // Alloc memory as much as record count
        IDE_TEST( QC_QMP_MEM(aStatement)->alloc( ID_SIZEOF(sdiRange) * sCount,
                                                 (void**) & aRangeInfo->mRanges )
                  != IDE_SUCCESS );

        IDE_TEST( sCursor.beforeFirst() != IDE_SUCCESS );

        IDE_TEST( sCursor.readRow( &sRow, &sRid, SMI_FIND_NEXT )
                  != IDE_SUCCESS );

        // Set ranges
        while ( sRow != NULL )
        {
            sNodeID  = *(mtdIntegerType*)((SChar *)sRow + sNodeIDColumn->column.offset );

            aRangeInfo->mRanges[aRangeInfo->mCount].mNodeId = (UInt)sNodeID;
            // Solo table 에서 value는 의미 없다. max로 설정
            aRangeInfo->mRanges[aRangeInfo->mCount].mValue.mHashMax = (UInt)SDI_RANGE_MAX_COUNT;

            aRangeInfo->mCount++;

            IDE_TEST( sCursor.readRow( &sRow, &sRid, SMI_FIND_NEXT )
                      != IDE_SUCCESS );
        }
    }
    else
    {
        // Nothing to do.
    }

    sIsCursorOpen = ID_FALSE;
    IDE_TEST( sCursor.close() != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_META_HANDLE )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_SHARD_META_NOT_CREATED,
                                  SDM_SOLOS ) );
    }
    IDE_EXCEPTION( RANGE_COUNT_OVERFLOW )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_SHARD_RANGE_OVERFLOW ) );
    }
    IDE_EXCEPTION_END;

    if ( sIsCursorOpen == ID_TRUE )
    {
        (void)sCursor.close();
    }
    else
    {
        /* Nothing to do */
    }

    return IDE_FAILURE;
}

IDE_RC sdm::convertRangeValue( SChar       * aValue,
                               UInt          aLength,
                               UInt          aKeyType,
                               sdiValue    * aRangeValue )
{
    SChar           sBuf[SDI_RANGE_VARCHAR_MAX_PRECISION + 1];
    qcNamePosition  sPosition;
    SLong           sLongVal;

    IDE_DASSERT( aLength <= SDI_RANGE_VARCHAR_MAX_PRECISION );

    if ( ( aKeyType == MTD_SMALLINT_ID ) ||
         ( aKeyType == MTD_INTEGER_ID ) ||
         ( aKeyType == MTD_BIGINT_ID ) )
    {
        sPosition.stmtText = aValue;
        sPosition.offset   = 0;
        sPosition.size     = aLength;

        IDE_TEST_RAISE( qtc::getBigint( sPosition.stmtText,
                                        &sLongVal,
                                        &sPosition ) != IDE_SUCCESS,
                        ERR_INVALID_RANGE_VALUE );

        if ( aKeyType == MTD_SMALLINT_ID )
        {
            IDE_TEST_RAISE( ( sLongVal < MTD_SMALLINT_MINIMUM ) ||
                            ( sLongVal > MTD_SMALLINT_MAXIMUM ),
                            ERR_INVALID_RANGE_VALUE );

            aRangeValue->mSmallintMax = (SShort) sLongVal;
        }
        else if ( aKeyType == MTD_INTEGER_ID )
        {
            IDE_TEST_RAISE( ( sLongVal < MTD_INTEGER_MINIMUM ) ||
                            ( sLongVal > MTD_INTEGER_MAXIMUM ),
                            ERR_INVALID_RANGE_VALUE );

            aRangeValue->mIntegerMax = (SInt) sLongVal;
        }
        else
        {
            IDE_TEST_RAISE( sLongVal == MTD_BIGINT_NULL,
                            ERR_INVALID_RANGE_VALUE );

            aRangeValue->mBigintMax = sLongVal;
        }
    }
    else if ( ( aKeyType == MTD_CHAR_ID ) ||
              ( aKeyType == MTD_VARCHAR_ID ) )
    {
        aRangeValue->mCharMax.length = (UShort)aLength;
        idlOS::memcpy( (void*) aRangeValue->mCharMax.value,
                       (void*) aValue,
                       aLength );
    }
    else
    {
        IDE_DASSERT(0);
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_INVALID_RANGE_VALUE )
    {
        idlOS::memcpy( sBuf, aValue, aLength );
        sBuf[aLength] = '\0';

        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDF_INVALID_RANGE_VALUE, sBuf ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdm::compareKeyData( UInt       aKeyDataType,
                            sdiValue * aValue1,
                            sdiValue * aValue2,
                            SShort   * aResult )
{
    const mtdModule * sKeyModule   = NULL;

    mtdValueInfo      sValueInfo1;
    mtdValueInfo      sValueInfo2;

    SInt              sCompResult = ID_SINT_MAX;

    IDE_TEST( mtd::moduleById( &sKeyModule,
                               aKeyDataType )
              != IDE_SUCCESS );

    switch ( aKeyDataType )
    {
        case MTD_SMALLINT_ID :
        {
            sValueInfo1.column = NULL;
            sValueInfo1.value  = &(aValue1->mSmallintMax);
            sValueInfo1.flag   = MTD_OFFSET_USELESS;

            sValueInfo2.column = NULL;
            sValueInfo2.value  = &(aValue2->mSmallintMax);
            sValueInfo2.flag   = MTD_OFFSET_USELESS;
            break;
        }
        case MTD_INTEGER_ID :
        {
            sValueInfo1.column = NULL;
            sValueInfo1.value  = &(aValue1->mIntegerMax);
            sValueInfo1.flag   = MTD_OFFSET_USELESS;

            sValueInfo2.column = NULL;
            sValueInfo2.value  = &(aValue2->mIntegerMax);
            sValueInfo2.flag   = MTD_OFFSET_USELESS;
            break;
        }
        case MTD_BIGINT_ID :
        {
            sValueInfo1.column = NULL;
            sValueInfo1.value  = &(aValue1->mBigintMax);
            sValueInfo1.flag   = MTD_OFFSET_USELESS;

            sValueInfo2.column = NULL;
            sValueInfo2.value  = &(aValue2->mBigintMax);
            sValueInfo2.flag   = MTD_OFFSET_USELESS;
            break;
        }
        case MTD_CHAR_ID :
        case MTD_VARCHAR_ID :
        {
            sValueInfo1.column = NULL;
            sValueInfo1.value  = &(aValue1->mCharMax);
            sValueInfo1.flag   = MTD_OFFSET_USELESS;

            sValueInfo2.column = NULL;
            sValueInfo2.value  = &(aValue2->mCharMax);
            sValueInfo2.flag   = MTD_OFFSET_USELESS;
            break;
        }
        default :
        {
            // unexpected error raise
            IDE_DASSERT(0);
            break;
        }
    }

    sCompResult = sKeyModule->logicalCompare[MTD_COMPARE_ASCENDING]( &sValueInfo1, &sValueInfo2 );

    if ( sCompResult == 0 )
    {
        *aResult = 0;
    }
    else if ( sCompResult > 0 )
    {
        *aResult = 1;
    }
    else // ( sCompResult < 0 )
    {
        *aResult = -1;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdm::shardRangeSort( sdiSplitMethod   aSplitMethod,
                            UInt             aKeyDataType,
                            idBool           aSubKeyExists,
                            sdiSplitMethod   aSubSplitMethod,
                            UInt             aSubKeyDataType,
                            sdiRangeInfo   * aRangeInfo )
{
    /*
     * PROJ-2655 Composite shard key
     * 
     * Index에 의해 보장 되지 않는
     * Data type conversion( e.x. string to integer ) 후 의 rangeInfo의 range들을 정렬한다.
     *
     */

    UInt     sKeyDataType;
    UInt     sSubKeyDataType;

    SInt     i = 0;
    SInt     j = 0;

    sdiRange sTmp;

    SShort   sCompare    = ID_SSHORT_MAX; // -1 : less, 0 : equal, 1 : greater

    /* Set key data type */
    if ( aSplitMethod == SDI_SPLIT_HASH )
    {
        sKeyDataType = MTD_INTEGER_ID;
    }
    else
    {
        sKeyDataType = aKeyDataType;
    }

    if ( aSubKeyExists == ID_TRUE )
    {
        if ( aSubSplitMethod == SDI_SPLIT_HASH )
        {
            sSubKeyDataType = MTD_INTEGER_ID;
        }
        else
        {
            sSubKeyDataType = aSubKeyDataType;
        }
    }
    else
    {
        /* Nothing to do. */
    }

    /* Insertion sort */
    for ( i = 0; i < ( aRangeInfo->mCount - 1 ); i++ )
    {
        idlOS::memcpy( (void*)&sTmp,
                       (void*)&aRangeInfo->mRanges[i + 1],
                       sizeof(sdiRange) );

        for ( j = i; j > -1; j-- )
        {
            // compare shard key
            IDE_TEST( compareKeyData( sKeyDataType,
                                      &aRangeInfo->mRanges[j].mValue, // A
                                      &sTmp.mValue,                   // B
                                      &sCompare )                     // A = B : 0, A > B : 1, A < B : -1
                      != IDE_SUCCESS );

            if ( sCompare == 0 )
            {
                if ( aSubKeyExists == ID_TRUE )
                {
                    // compare sub-shard key
                    IDE_TEST( compareKeyData( sSubKeyDataType,
                                              &aRangeInfo->mRanges[j].mSubValue,
                                              &sTmp.mSubValue,
                                              &sCompare )
                              != IDE_SUCCESS );

                    if ( sCompare == 1 )
                    {
                        idlOS::memcpy( (void*)&aRangeInfo->mRanges[j + 1],
                                       (void*)&aRangeInfo->mRanges[j],
                                       sizeof(sdiRange) );
                        if ( j == 0 )
                        {
                            idlOS::memcpy( (void*)&aRangeInfo->mRanges[j],
                                           (void*)&sTmp,
                                           sizeof(sdiRange) );
                        }
                        else
                        {
                            /* Nothing to do. */
                        }
                    }
                    else if ( sCompare == -1 )
                    {
                        idlOS::memcpy( (void*)&aRangeInfo->mRanges[j + 1],
                                       (void*)&sTmp,
                                       sizeof(sdiRange) );
                        break;
                    }
                    else
                    {
                        // Sub-shard key 까지 equal 일 수 없다. ( unique index )
                        IDE_RAISE( ERR_DUPLICATED );
                    }
                }
                else
                {
                    // shard key가 equal일 수 없다. ( unique index )
                    IDE_RAISE( ERR_DUPLICATED );
                }
            }
            else if ( sCompare == 1 )
            {
                idlOS::memcpy( (void*)&aRangeInfo->mRanges[j + 1],
                               (void*)&aRangeInfo->mRanges[j],
                               sizeof(sdiRange) );

                if ( j == 0 )
                {
                    idlOS::memcpy( (void*)&aRangeInfo->mRanges[j],
                                   (void*)&sTmp,
                                   sizeof(sdiRange) );
                }
                else
                {
                    /* Nothing to do. */
                }
            }
            else
            {
                IDE_DASSERT( sCompare == -1 );

                idlOS::memcpy( (void*)&aRangeInfo->mRanges[j + 1],
                               (void*)&sTmp,
                               sizeof(sdiRange) );
                break;
            }
        }
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_DUPLICATED )
    {
        IDE_SET(ideSetErrorCode(sdERR_ABORT_SDM_DUPLICATED_RANGE_VALUE));
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdm::shardEliminateDuplication( sdiTableInfo * aTableInfo,
                                       sdiRangeInfo * aRangeInfo )
{
    UShort   i;

    UShort   sNewCount = 0;
    UShort   sSplitCombination = 0;

    SShort   sCompare;
    UInt     sKeyDataType;

    /*
     * PROJ-2655 Composite shard key
     *
     * Value 에 대해 sort가 된 상태의 range value에 대해
     * 중첩 정의 된 range value를 합쳐준다.
     *
     * e.x ) RANGE [100][200][300][400]
     *       NODE  [ A ][ B ][ B ][ C ]
     *
     *   ->  RANGE [100][300][400]
     *       NODE  [ A ][ B ][ C ]
     *
     * + 합치는 기준 +
     *
     * sSplitCombination | split method  | sub split method | process ( 정렬되어 있는 상태에서~ )
     *--------------------------------------------------------------------------------------------------------
     *         1         | RANGE or HASH |        -         | 노드가 같으면 합칠 수 있다.
     *         2         | LIST          | RANGE or HASH    | Value가 같으면서 노드가 같으면 합칠 수 있다.
     *         3         | RANGE or HASH | RANGE or HASH    | Value가 같으면서 노드가 같으면 합칠 수 있다.
     *--------------------------------------------------------------------------------------------------------
     *
     * + 목록에 없는 조합은 중복제거 하면 안된다.
     *
     *    e.x. ) RANGE & LIST etc...
     *
     */

    /* Set split combination type */
    if ( aTableInfo->mSplitMethod == SDI_SPLIT_LIST )
    {
        // SDI_SPLIT_LIST
        if ( aTableInfo->mSubKeyExists == ID_TRUE )
        {
            if ( ( aTableInfo->mSubSplitMethod == SDI_SPLIT_HASH ) ||
                 ( aTableInfo->mSubSplitMethod == SDI_SPLIT_RANGE ) )
            {
                sSplitCombination = 2;
            }
            else if ( aTableInfo->mSubSplitMethod == SDI_SPLIT_LIST )
            {
                /* Nothing to do. */
            }
            else
            {
                // SDI_SPLIT_NONE or SDI_SPLIT_CLONE or SDI_SPLIT_SOLO etc...
                ideLog::log( IDE_SD_0, "[FAILURE] shardEliminateDuplication : invalid SPLIT_METHOD !!!\n");
                IDE_RAISE( ERR_META_CRASH );
            }
        }
        else
        {
            /* Nothing to do. */
        }
    }
    else if ( ( aTableInfo->mSplitMethod == SDI_SPLIT_HASH ) ||
              ( aTableInfo->mSplitMethod == SDI_SPLIT_RANGE ) )
    {
        if ( aTableInfo->mSubKeyExists == ID_TRUE )
        {
            if ( ( aTableInfo->mSubSplitMethod == SDI_SPLIT_HASH ) ||
                 ( aTableInfo->mSubSplitMethod == SDI_SPLIT_RANGE ) )
            {
                sSplitCombination = 3;
            }
            else if ( aTableInfo->mSubSplitMethod == SDI_SPLIT_LIST )                
            {
                /* Nothing to do. */
            }
            else
            {
                // SDI_SPLIT_NONE or SDI_SPLIT_CLONE or SDI_SPLIT_SOLO or etc...
                ideLog::log( IDE_SD_0, "[FAILURE] shardEliminateDuplication : invalid SPLIT_METHOD !!!\n");
                IDE_RAISE( ERR_META_CRASH );
            }
        }
        else
        {
            sSplitCombination = 1;
        }
    }
    else
    {
        // SDI_SPLIT_NONE or SDI_SPLIT_CLONE or SDI_SPLIT_SOLO
        IDE_DASSERT(0);
    }

    /* Eliminate duplicated range */
    if ( ( aRangeInfo->mCount > 1 ) && ( sSplitCombination > 0 ) )
    {   
        for ( i = 1; i < aRangeInfo->mCount; i++ )
        {
            if ( sSplitCombination == 1 )
            {
                if ( aRangeInfo->mRanges[i].mNodeId != aRangeInfo->mRanges[sNewCount].mNodeId )
                {
                    sNewCount++;
                }
                else
                {
                    /* Nothing to do. */
                }
            }
            else if ( ( sSplitCombination == 2  ) || ( sSplitCombination == 3 ) )
            {

                /* Set key data type */
                if ( aTableInfo->mSplitMethod == SDI_SPLIT_HASH )
                {
                    sKeyDataType = MTD_INTEGER_ID;
                }
                else
                {
                    sKeyDataType = aTableInfo->mKeyDataType;
                }
                
                // compare first shard key
                IDE_TEST( compareKeyData( sKeyDataType,
                                          &aRangeInfo->mRanges[i].mValue,
                                          &aRangeInfo->mRanges[sNewCount].mValue,
                                          &sCompare )
                          != IDE_SUCCESS );

                if ( sCompare != 0 )
                {
                    sNewCount++;
                }
                else
                {
                    if ( aRangeInfo->mRanges[i].mNodeId != aRangeInfo->mRanges[sNewCount].mNodeId )
                    {
                        sNewCount++;
                    }
                    else
                    {
                        /* Nothing to do. */
                    }
                }
            }
            else
            {
                ideLog::log( IDE_SD_0, "[FAILURE] shardEliminateDuplication : invalid SPLIT_METHOD !!!\n");
                IDE_RAISE( ERR_META_CRASH );
            }

            aRangeInfo->mRanges[sNewCount].mNodeId = aRangeInfo->mRanges[i].mNodeId;
            aRangeInfo->mRanges[sNewCount].mValue  = aRangeInfo->mRanges[i].mValue;

            if ( aTableInfo->mSubKeyExists == ID_TRUE )
            {
                aRangeInfo->mRanges[sNewCount].mSubValue = aRangeInfo->mRanges[i].mSubValue;
            }
            else
            {
                /* Nothing to do. */
            }
        }

        aRangeInfo->mCount = ( sNewCount + 1 );
    }
    else
    {
        /* Nothing to do. */
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_META_CRASH )
    {
        IDE_SET(ideSetErrorCode(qpERR_ABORT_QCM_META_CRASH));
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdm::getLocalMetaNodeInfo( sdiLocalMetaNodeInfo * aMetaNodeInfo )
{
    smiTrans       sTrans;
    smiStatement   sSmiStmt;
    smiStatement * sDummySmiStmt = NULL;
    smSCN          sDummySCN;
    UInt           sStage        = 0;

    IDE_DASSERT( aMetaNodeInfo != NULL );

    IDE_TEST( sTrans.initialize() != IDE_SUCCESS );
    sStage = 1;

    IDE_TEST( sTrans.begin( &sDummySmiStmt, NULL )
              != IDE_SUCCESS );
    sStage = 2;

    IDE_TEST( sSmiStmt.begin( NULL,
                              sDummySmiStmt,
                              ( SMI_STATEMENT_UNTOUCHABLE |
                                SMI_STATEMENT_MEMORY_CURSOR ) )
              != IDE_SUCCESS );
    sStage = 3;

    IDE_TEST( getLocalMetaNodeInfoCore( &sSmiStmt, aMetaNodeInfo ) != IDE_SUCCESS );

    IDE_TEST( sSmiStmt.end( SMI_STATEMENT_RESULT_SUCCESS ) != IDE_SUCCESS );
    sStage = 2;

    IDE_TEST( sTrans.commit( &sDummySCN ) != IDE_SUCCESS );
    sStage = 1;

    sStage = 0;
    IDE_TEST( sTrans.destroy( NULL ) != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    IDE_PUSH();

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

    IDE_POP();

    ideLog::log( IDE_SD_0, "[SHARD META : FAILURE] errorcode 0x%05"ID_XINT32_FMT" %s\n",
                           E_ERROR_CODE(ideGetErrorCode()),
                           ideGetErrorMsg(ideGetErrorCode()));

    return IDE_FAILURE;
}

IDE_RC sdm::getLocalMetaNodeInfoCore( smiStatement         * aSmiStmt,
                                      sdiLocalMetaNodeInfo * aMetaNodeInfo )
{
    idBool            sIsCursorOpen     = ID_FALSE;
    const void      * sRow              = NULL;
    scGRID            sRid;
    const void      * sSdmMetaNodeInfo  = NULL;
    mtcColumn       * sMetaNodeIdColumn = NULL;
    mtdIntegerType    sMetaNodeId;

    smiTableCursor       sCursor;
    smiCursorProperties  sCursorProperty;

    IDE_TEST( checkMetaVersion( aSmiStmt )
              != IDE_SUCCESS );

    IDE_TEST( getMetaTableAndIndex( aSmiStmt,
                                    SDM_LOCAL_META_INFO,
                                    &sSdmMetaNodeInfo,
                                    NULL )
              != IDE_SUCCESS );

    IDE_TEST( smiGetTableColumns( sSdmMetaNodeInfo,
                                  SDM_LOCAL_META_INFO_META_NODE_ID_COL_ORDER,
                                  (const smiColumn**)&sMetaNodeIdColumn )
              != IDE_SUCCESS );

    sCursor.initialize();

    SMI_CURSOR_PROP_INIT_FOR_META_FULL_SCAN( &sCursorProperty, NULL );

    IDE_TEST( sCursor.open( aSmiStmt,
                            sSdmMetaNodeInfo,
                            NULL,
                            smiGetRowSCN( sSdmMetaNodeInfo ),
                            NULL,
                            smiGetDefaultKeyRange(),
                            smiGetDefaultKeyRange(),
                            smiGetDefaultFilter(),
                            QCM_META_CURSOR_FLAG,
                            SMI_SELECT_CURSOR,
                            &sCursorProperty )
              != IDE_SUCCESS );

    sIsCursorOpen = ID_TRUE;

    IDE_TEST( sCursor.beforeFirst() != IDE_SUCCESS );

    IDE_TEST( sCursor.readRow( &sRow, &sRid, SMI_FIND_NEXT )
              != IDE_SUCCESS );

    // 1건도 없으면 에러
    IDE_TEST_RAISE( sRow == NULL, ERR_CHECK_META_NODE_INFO );

    sMetaNodeId = *(mtdIntegerType*)( (SChar *)sRow + sMetaNodeIdColumn->column.offset );

    IDE_TEST(sCursor.readRow(&sRow, &sRid, SMI_FIND_NEXT) != IDE_SUCCESS);

    // 1건이어야 한다.
    IDE_TEST_RAISE( sRow != NULL, ERR_CHECK_META_NODE_INFO );

    sIsCursorOpen = ID_FALSE;
    IDE_TEST( sCursor.close() != IDE_SUCCESS );

    aMetaNodeInfo->mMetaNodeId = sMetaNodeId;

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_CHECK_META_NODE_INFO )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_INVALID_META_NODE_INFO ) );
    }
    IDE_EXCEPTION_END;

    IDE_PUSH();

    if ( sIsCursorOpen == ID_TRUE )
    {
        (void)sCursor.close();
    }
    else
    {
        /* Nothing to do */
    }

    IDE_POP();

    return IDE_FAILURE;
}

idBool sdm::isShardMetaCreated( smiStatement * aSmiStmt )
{
    const void * sSdmVersion;

    IDE_TEST( getMetaTableAndIndex( aSmiStmt,
                                    SDM_VERSION,
                                    &sSdmVersion,
                                    NULL )
              != IDE_SUCCESS );

    return ID_TRUE;

    IDE_EXCEPTION_END;

    IDE_CLEAR();

    return ID_FALSE;
}

IDE_RC sdm::getGlobalMetaNodeInfo( sdiGlobalMetaNodeInfo * aMetaNodeInfo )
{
    smiTrans       sTrans;
    smiStatement   sSmiStmt;
    smiStatement * sDummySmiStmt = NULL;
    smSCN          sDummySCN;
    UInt           sStage        = 0;

    IDE_DASSERT( aMetaNodeInfo != NULL );

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

    IDE_TEST( getGlobalMetaNodeInfoCore( &sSmiStmt, aMetaNodeInfo ) != IDE_SUCCESS );

    IDE_TEST( sSmiStmt.end( SMI_STATEMENT_RESULT_SUCCESS ) != IDE_SUCCESS );
    sStage = 2;

    IDE_TEST( sTrans.commit( &sDummySCN ) != IDE_SUCCESS );
    sStage = 1;

    sStage = 0;
    IDE_TEST( sTrans.destroy( NULL ) != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    IDE_PUSH();

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

    IDE_POP();

    ideLog::log( IDE_SD_0, "[SHARD META : FAILURE] errorcode 0x%05"ID_XINT32_FMT" %s\n",
                           E_ERROR_CODE( ideGetErrorCode() ),
                           ideGetErrorMsg( ideGetErrorCode() ) );

    return IDE_FAILURE;
}

IDE_RC sdm::getGlobalMetaNodeInfoCore( smiStatement          * aSmiStmt,
                                       sdiGlobalMetaNodeInfo * aMetaNodeInfo )
{
    const void          * sRow                   = NULL;
    scGRID                sRid;
    const void          * sSdmMetaNodeInfo       = NULL;
    idBool                sIsCursorOpen          = ID_FALSE;

    mtcColumn           * sShardMetaNumberColumn = NULL;
    mtdBigintType         sShardMetaNumber       = ID_LONG(0);

    smiTableCursor        sCursor;
    smiCursorProperties   sCursorProperty;

    IDE_TEST( checkMetaVersion( aSmiStmt ) != IDE_SUCCESS );

    IDE_TEST( getMetaTableAndIndex( aSmiStmt,
                                    SDM_GLOBAL_META_INFO,
                                    &sSdmMetaNodeInfo,
                                    NULL )
              != IDE_SUCCESS );

    IDE_TEST( smiGetTableColumns( sSdmMetaNodeInfo,
                                  SDM_GLOBAL_META_INFO_SHARD_META_NUMBER_COL_ORDER,
                                  (const smiColumn **)&sShardMetaNumberColumn )
              != IDE_SUCCESS );

    sCursor.initialize();

    SMI_CURSOR_PROP_INIT_FOR_META_FULL_SCAN( &sCursorProperty, NULL );

    IDE_TEST( sCursor.open( aSmiStmt,
                            sSdmMetaNodeInfo,
                            NULL,
                            smiGetRowSCN( sSdmMetaNodeInfo ),
                            NULL,
                            smiGetDefaultKeyRange(),
                            smiGetDefaultKeyRange(),
                            smiGetDefaultFilter(),
                            QCM_META_CURSOR_FLAG,
                            SMI_SELECT_CURSOR,
                            &sCursorProperty )
              != IDE_SUCCESS );
    sIsCursorOpen = ID_TRUE;

    IDE_TEST( sCursor.beforeFirst() != IDE_SUCCESS );

    IDE_TEST( sCursor.readRow( &sRow, &sRid, SMI_FIND_NEXT ) != IDE_SUCCESS );

    IDE_TEST_RAISE( sRow == NULL, ERR_META_NODE_INFO_ROW_COUNT );

    sShardMetaNumber = *(mtdBigintType *)( (SChar *)sRow + sShardMetaNumberColumn->column.offset );

    IDE_TEST( sCursor.readRow( &sRow, &sRid, SMI_FIND_NEXT ) != IDE_SUCCESS );

    IDE_TEST_RAISE( sRow != NULL, ERR_META_NODE_INFO_ROW_COUNT );

    sIsCursorOpen = ID_FALSE;
    IDE_TEST( sCursor.close() != IDE_SUCCESS );

    aMetaNodeInfo->mShardMetaNumber = (ULong)sShardMetaNumber;

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_META_NODE_INFO_ROW_COUNT )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDC_UNEXPECTED_ERROR,
                                  "sdm::getGlobalMetaNodeInfoCore",
                                  "row count is not 1" ) );
    }
    IDE_EXCEPTION_END;

    IDE_PUSH();

    if ( sIsCursorOpen == ID_TRUE )
    {
        (void)sCursor.close();
    }
    else
    {
        /* Nothing to do */
    }

    IDE_POP();

    return IDE_FAILURE;
}

/*
 * PROJ-2701 Online data rebuild
 */
IDE_RC sdm::makeShardMeta4NewSMN( qcStatement * aStatement )
{
    SChar                 * sSqlStr;
    vSLong                  sRowCnt;
    sdiGlobalMetaNodeInfo   sMetaNodeInfo = { ID_ULONG(0) };
    sdiGlobalMetaNodeInfo   sMetaNodeInfoWithAnotherTx = { ID_ULONG(0) };

    IDE_TEST( checkMetaVersion( QC_SMI_STMT( aStatement ) )
              != IDE_SUCCESS );

    IDE_TEST( STRUCT_ALLOC_WITH_SIZE( aStatement->qmxMem,
                                      SChar,
                                      QD_MAX_SQL_LENGTH,
                                      &sSqlStr )
              != IDE_SUCCESS);

    /* Acquire record lock for checking SMN */
    idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                     "UPDATE SYS_SHARD.GLOBAL_META_INFO_ "
                     "   SET SHARD_META_NUMBER = SHARD_META_NUMBER" );

    IDE_TEST( qciMisc::runDMLforDDL( QC_SMI_STMT( aStatement ),
                                     sSqlStr,
                                     & sRowCnt )
              != IDE_SUCCESS );

    /* Meta SMN of current transaction */
    IDE_TEST ( getGlobalMetaNodeInfoCore( QC_SMI_STMT(aStatement),
                                          &sMetaNodeInfo ) != IDE_SUCCESS );

    /* Meta SMN of another transaction */
    IDE_TEST ( getGlobalMetaNodeInfo ( &sMetaNodeInfoWithAnotherTx ) != IDE_SUCCESS );

    /* BUG-46884
     * SMN의 lock을 획득한 후에 currentTx의 SMN과 newTx의 SMN을 비교하여 같으면,
     * 내가 currentTx에서 shard meta를 건드리는 첫 statement 수행이다.
     */
    if ( sMetaNodeInfo.mShardMetaNumber == sMetaNodeInfoWithAnotherTx.mShardMetaNumber )
    {
        /* increase shard meta number */
        idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                         "UPDATE SYS_SHARD.GLOBAL_META_INFO_ "
                         "   SET SHARD_META_NUMBER = SHARD_META_NUMBER + 1" );

        IDE_TEST( qciMisc::runDMLforDDL( QC_SMI_STMT( aStatement ),
                                         sSqlStr,
                                         & sRowCnt )
                  != IDE_SUCCESS );

        /* get increased shard meta number */
        IDE_TEST( getGlobalMetaNodeInfoCore( QC_SMI_STMT(aStatement),
                                             &sMetaNodeInfo ) != IDE_SUCCESS );

        sMetaNodeInfo.mShardMetaNumber--;

        /* copy nodes_ with increased SMN */
        idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                         "INSERT INTO SYS_SHARD.NODES_ "
                         "     ( NODE_ID, NODE_NAME, HOST_IP, PORT_NO, ALTERNATE_HOST_IP, ALTERNATE_PORT_NO, "
                         "       INTERNAL_HOST_IP, INTERNAL_PORT_NO, "
                         "       INTERNAL_ALTERNATE_HOST_IP, INTERNAL_ALTERNATE_PORT_NO, INTERNAL_CONN_TYPE, SMN ) "
                         "SELECT NODE_ID, NODE_NAME, HOST_IP, PORT_NO, ALTERNATE_HOST_IP, ALTERNATE_PORT_NO, "
                         "       INTERNAL_HOST_IP, INTERNAL_PORT_NO, "
                         "       INTERNAL_ALTERNATE_HOST_IP, INTERNAL_ALTERNATE_PORT_NO, INTERNAL_CONN_TYPE, SMN + 1 "
                         "  FROM SYS_SHARD.NODES_ "
                         " WHERE SMN = "QCM_SQL_BIGINT_FMT,
                         sMetaNodeInfo.mShardMetaNumber );

        IDE_TEST( qciMisc::runDMLforDDL( QC_SMI_STMT( aStatement ),
                                         sSqlStr,
                                         & sRowCnt )
                  != IDE_SUCCESS );

        /* copy objects_ with increased SMN */
        idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                         "INSERT INTO SYS_SHARD.OBJECTS_ "
                         "      ( SHARD_ID, USER_NAME, OBJECT_NAME, OBJECT_TYPE, SPLIT_METHOD, KEY_COLUMN_NAME, "
                         "        SUB_SPLIT_METHOD, SUB_KEY_COLUMN_NAME, DEFAULT_NODE_ID, SMN ) "
                         "SELECT  SHARD_ID, USER_NAME, OBJECT_NAME, OBJECT_TYPE, SPLIT_METHOD, KEY_COLUMN_NAME, "
                         "        SUB_SPLIT_METHOD, SUB_KEY_COLUMN_NAME, DEFAULT_NODE_ID, SMN + 1 "
                         "  FROM SYS_SHARD.OBJECTS_ "
                         " WHERE SMN = "QCM_SQL_BIGINT_FMT,
                         sMetaNodeInfo.mShardMetaNumber );

        IDE_TEST( qciMisc::runDMLforDDL( QC_SMI_STMT( aStatement ),
                                         sSqlStr,
                                         & sRowCnt )
                  != IDE_SUCCESS );

        /* copy ranges_ with increased SMN */
        idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                         "INSERT INTO SYS_SHARD.RANGES_ "
                         "     ( SHARD_ID, VALUE, SUB_VALUE, NODE_ID, SMN )"
                         "SELECT SHARD_ID, VALUE, SUB_VALUE, NODE_ID, SMN + 1 "
                         "  FROM SYS_SHARD.RANGES_ "
                         " WHERE SMN = "QCM_SQL_BIGINT_FMT,
                         sMetaNodeInfo.mShardMetaNumber );

        IDE_TEST( qciMisc::runDMLforDDL( QC_SMI_STMT( aStatement ),
                                         sSqlStr,
                                         & sRowCnt )
                  != IDE_SUCCESS );

        /* copy ranges_ with increased SMN */
        idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                         "INSERT INTO SYS_SHARD.CLONES_ "
                         "     ( SHARD_ID, NODE_ID, SMN )"
                         "SELECT SHARD_ID, NODE_ID, SMN + 1 "
                         "  FROM SYS_SHARD.CLONES_ "
                         " WHERE SMN = "QCM_SQL_BIGINT_FMT,
                         sMetaNodeInfo.mShardMetaNumber );

        IDE_TEST( qciMisc::runDMLforDDL( QC_SMI_STMT( aStatement ),
                                         sSqlStr,
                                         & sRowCnt )
                  != IDE_SUCCESS );

        /* copy solos_ with increased SMN */
        idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                         "INSERT INTO SYS_SHARD.SOLOS_ "
                         "     ( SHARD_ID, NODE_ID, SMN )"
                         "SELECT SHARD_ID, NODE_ID, SMN + 1 "
                         "  FROM SYS_SHARD.SOLOS_ "
                         " WHERE SMN = "QCM_SQL_BIGINT_FMT,
                         sMetaNodeInfo.mShardMetaNumber );

        IDE_TEST( qciMisc::runDMLforDDL( QC_SMI_STMT( aStatement ),
                                         sSqlStr,
                                         & sRowCnt )
                  != IDE_SUCCESS );

        if ( SDU_SHARD_META_HISTORY_AUTO_PURGE_DISABLE == 0 )
        {
            /* delete the old SMN data of the shard meta tables */
            IDE_TEST( deleteOldSMN( aStatement,
                                    NULL ) // less then the current SMN
                      != IDE_SUCCESS );
        }
        else
        {
            // Nothing to do.
        }
    }
    else
    {
        // Current transactioin의 이전 statement가 newSMN을 이미 생성한 경우
        IDE_TEST_RAISE( sMetaNodeInfo.mShardMetaNumber < sMetaNodeInfoWithAnotherTx.mShardMetaNumber,
                        ERR_SHARD_META_CHANGE_PROCESS_CRASH );

    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_SHARD_META_CHANGE_PROCESS_CRASH )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDC_UNEXPECTED_ERROR,
                                  "sdm::makeShardMeta4NewSMN",
                                  "The shard meta modification was failed on the current transaction, please ROLLBACK." ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdm::deleteOldSMN( qcStatement * aStatement,
                          ULong       * aSMN )
{
    SChar                 * sSqlStr;
    vSLong                  sRowCnt;
    sdiGlobalMetaNodeInfo   sMetaNodeInfo = { ID_ULONG(0) };

    IDE_TEST( checkMetaVersion( QC_SMI_STMT( aStatement ) )
              != IDE_SUCCESS );

    IDE_TEST( getGlobalMetaNodeInfoCore( QC_SMI_STMT(aStatement),
                                         &sMetaNodeInfo ) != IDE_SUCCESS );

    if ( aSMN == NULL )
    {
        sMetaNodeInfo.mShardMetaNumber--;
    }
    else
    {
        // Current SMN은 삭제 할 수 없다.
        IDE_TEST_RAISE( *aSMN >= sMetaNodeInfo.mShardMetaNumber, ERR_CANNOT_DELETE_CURRENT_SMN );
        sMetaNodeInfo.mShardMetaNumber = *aSMN;
    }

    IDE_TEST( STRUCT_ALLOC_WITH_SIZE( aStatement->qmxMem,
                                      SChar,
                                      QD_MAX_SQL_LENGTH,
                                      &sSqlStr )
              != IDE_SUCCESS);

    idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                     "DELETE FROM SYS_SHARD.RANGES_ WHERE SMN <= "QCM_SQL_BIGINT_FMT,
                     sMetaNodeInfo.mShardMetaNumber);

    IDE_TEST( qciMisc::runDMLforDDL( QC_SMI_STMT( aStatement ),
                                     sSqlStr,
                                     & sRowCnt )
              != IDE_SUCCESS );

    idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                     "DELETE FROM SYS_SHARD.CLONES_ WHERE SMN <= "QCM_SQL_BIGINT_FMT,
                     sMetaNodeInfo.mShardMetaNumber);

    IDE_TEST( qciMisc::runDMLforDDL( QC_SMI_STMT( aStatement ),
                                     sSqlStr,
                                     & sRowCnt )
              != IDE_SUCCESS );

    idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                     "DELETE FROM SYS_SHARD.SOLOS_ WHERE SMN <= "QCM_SQL_BIGINT_FMT,
                     sMetaNodeInfo.mShardMetaNumber);

    IDE_TEST( qciMisc::runDMLforDDL( QC_SMI_STMT( aStatement ),
                                     sSqlStr,
                                     & sRowCnt )
              != IDE_SUCCESS );

    idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                     "DELETE FROM SYS_SHARD.OBJECTS_ WHERE SMN <= "QCM_SQL_BIGINT_FMT,
                     sMetaNodeInfo.mShardMetaNumber);

    IDE_TEST( qciMisc::runDMLforDDL( QC_SMI_STMT( aStatement ),
                                     sSqlStr,
                                     & sRowCnt )
              != IDE_SUCCESS );
    
    idlOS::snprintf( sSqlStr, QD_MAX_SQL_LENGTH,
                     "DELETE FROM SYS_SHARD.NODES_ WHERE SMN <= "QCM_SQL_BIGINT_FMT,
                     sMetaNodeInfo.mShardMetaNumber);

    IDE_TEST( qciMisc::runDMLforDDL( QC_SMI_STMT( aStatement ),
                                     sSqlStr,
                                     & sRowCnt )
              != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_CANNOT_DELETE_CURRENT_SMN )
    {
        IDE_SET( ideSetErrorCode(sdERR_ABORT_SDF_CANNOT_DELETE_CURRENT_SMN) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdm::getShardUserID( UInt * aShardUserID )
{
    smiTrans       sTrans;
    smiStatement   sSmiStmt;
    smiStatement * sDummySmiStmt = NULL;
    smSCN          sDummySCN;
    UInt           sStage = 0;

    IDE_DASSERT( aShardUserID != NULL );

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

    IDE_TEST( qcmUser::getUserID( NULL,
                                  &sSmiStmt,
                                  SDM_USER,
                                  idlOS::strlen(SDM_USER),
                                  aShardUserID )
              != IDE_SUCCESS );

    IDE_TEST( sSmiStmt.end( SMI_STATEMENT_RESULT_SUCCESS ) != IDE_SUCCESS );
    sStage = 2;

    IDE_TEST( sTrans.commit( &sDummySCN ) != IDE_SUCCESS );
    sStage = 1;

    sStage = 0;
    IDE_TEST( sTrans.destroy( NULL ) != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    IDE_PUSH();

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

    IDE_POP();

    ideLog::log( IDE_SD_0, "[SHARD META : FAILURE] errorcode 0x%05"ID_XINT32_FMT" %s\n",
                           E_ERROR_CODE( ideGetErrorCode() ),
                           ideGetErrorMsg( ideGetErrorCode() ) );

    return IDE_FAILURE;
}

IDE_RC sdm::validateRangeCountBeforeInsert( qcStatement  * aStatement,
                                            sdiTableInfo * aTableInfo,
                                            ULong          aSMN )
{
    const void         * sSdmRangesIndex[SDM_MAX_META_INDICES];
    const void         * sSdmRanges = NULL;
    mtcColumn          * sShardIDColumn = NULL;
    qtcMetaRangeColumn   sShardIDRange;
    mtcColumn          * sSMNColumn;
    qtcMetaRangeColumn   sSMNRange;
    smiRange             sRange;
    scGRID               sRid;
    const void         * sRow = NULL;
    smiTableCursor       sCursor;
    smiCursorProperties  sCursorProperty;
    idBool               sIsCursorOpen = ID_FALSE;
    UInt                 sExistCount = 0;

    IDE_TEST( getMetaTableAndIndex( QC_SMI_STMT( aStatement ),
                                    ( aTableInfo->mSplitMethod == SDI_SPLIT_SOLO  ) ? SDM_SOLOS:
                                    ( aTableInfo->mSplitMethod == SDI_SPLIT_CLONE ) ? SDM_CLONES:
                                    SDM_RANGES,
                                    & sSdmRanges,
                                    sSdmRangesIndex )
              != IDE_SUCCESS );

    IDE_TEST_RAISE( sSdmRangesIndex[ ( aTableInfo->mSplitMethod == SDI_SPLIT_SOLO  ) ? SDM_SOLOS_IDX1_ORDER:
                                     ( aTableInfo->mSplitMethod == SDI_SPLIT_CLONE ) ? SDM_CLONES_IDX1_ORDER:
                                     SDM_RANGES_IDX1_ORDER ] == NULL,
                    ERR_META_HANDLE );

    IDE_TEST( smiGetTableColumns( sSdmRanges,
                                  ( aTableInfo->mSplitMethod == SDI_SPLIT_SOLO  ) ? SDM_SOLOS_SHARD_ID_COL_ORDER:
                                  ( aTableInfo->mSplitMethod == SDI_SPLIT_CLONE ) ? SDM_CLONES_SHARD_ID_COL_ORDER:
                                  SDM_RANGES_SHARD_ID_COL_ORDER,
                                  (const smiColumn**)&sShardIDColumn )
              != IDE_SUCCESS );

    IDE_TEST( smiGetTableColumns( sSdmRanges,
                                  ( aTableInfo->mSplitMethod == SDI_SPLIT_SOLO  ) ? SDM_SOLOS_SMN_COL_ORDER:
                                  ( aTableInfo->mSplitMethod == SDI_SPLIT_CLONE ) ? SDM_CLONES_SMN_COL_ORDER:
                                  SDM_RANGES_SMN_COL_ORDER,
                                  (const smiColumn**)&sSMNColumn )
              != IDE_SUCCESS );

    // mtdModule 설정
    IDE_TEST( mtd::moduleById( &(sShardIDColumn->module),
                               sShardIDColumn->type.dataTypeId )
              != IDE_SUCCESS );

    // mtlModule 설정
    IDE_TEST( mtl::moduleById( &sShardIDColumn->language,
                               sShardIDColumn->type.languageId )
              != IDE_SUCCESS );

    // mtdModule 설정
    IDE_TEST( mtd::moduleById( &(sSMNColumn->module),
                               sSMNColumn->type.dataTypeId )
              != IDE_SUCCESS );

    // mtlModule 설정
    IDE_TEST( mtl::moduleById( &sSMNColumn->language,
                               sSMNColumn->type.languageId )
              != IDE_SUCCESS );

    qciMisc::makeMetaRangeDoubleColumn(
        &sShardIDRange,
        &sSMNRange,
        sShardIDColumn,
        (const void *)&(aTableInfo->mShardID),
        sSMNColumn,
        (const void *)&aSMN,
        &sRange );

    sCursor.initialize();

    SMI_CURSOR_PROP_INIT_FOR_META_INDEX_SCAN( &sCursorProperty, NULL );

    IDE_TEST( sCursor.open(
                  QC_SMI_STMT( aStatement ),
                  sSdmRanges,
                  sSdmRangesIndex[( aTableInfo->mSplitMethod == SDI_SPLIT_SOLO  ) ? SDM_SOLOS_IDX1_ORDER:
                                  ( aTableInfo->mSplitMethod == SDI_SPLIT_CLONE ) ? SDM_CLONES_IDX1_ORDER:
                                  SDM_RANGES_IDX1_ORDER],
                  smiGetRowSCN( sSdmRanges ),
                  NULL,
                  &sRange,
                  smiGetDefaultKeyRange(),
                  smiGetDefaultFilter(),
                  QCM_META_CURSOR_FLAG,
                  SMI_SELECT_CURSOR,
                  &sCursorProperty )
              != IDE_SUCCESS );

    sIsCursorOpen = ID_TRUE;

    IDE_TEST( sCursor.beforeFirst() != IDE_SUCCESS );
    IDE_TEST( sCursor.readRow( &sRow, &sRid, SMI_FIND_NEXT )
              != IDE_SUCCESS );

    while ( sRow != NULL )
    {
        sExistCount++;
        IDE_TEST( sCursor.readRow( &sRow, &sRid, SMI_FIND_NEXT )
                  != IDE_SUCCESS );
    }

    if ( aTableInfo->mSplitMethod == SDI_SPLIT_SOLO )
    {
        IDE_TEST_RAISE( sExistCount >= 1, RANGE_COUNT_OVERFLOW );
    }
    else
    {
        IDE_TEST_RAISE( sExistCount >= SDI_RANGE_MAX_COUNT, RANGE_COUNT_OVERFLOW );
    }

    sIsCursorOpen = ID_FALSE;
    IDE_TEST( sCursor.close() != IDE_SUCCESS );

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_META_HANDLE )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_SHARD_META_NOT_CREATED,
                                  ( aTableInfo->mSplitMethod == SDI_SPLIT_SOLO  ) ? SDM_SOLOS:
                                  ( aTableInfo->mSplitMethod == SDI_SPLIT_CLONE ) ? SDM_CLONES:
                                  SDM_RANGES ) );
    }
    IDE_EXCEPTION( RANGE_COUNT_OVERFLOW )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_SHARD_RANGE_OVERFLOW ) );
    }
    IDE_EXCEPTION_END;

    if ( sIsCursorOpen == ID_TRUE )
    {
        (void)sCursor.close();
    }

    return IDE_FAILURE;
}

IDE_RC sdm::validateParamBeforeInsert( qsOID   aProcOID,
                                       SChar * aUserName,
                                       SChar * aProcName,
                                       SChar * aSplitMethod,
                                       SChar * aKeyName,
                                       SChar * aSubSplitMethod,
                                       SChar * aSubKeyName )
{
    qsxProcInfo     * sProcInfo = NULL;
    qsVariableItems * sParaDecls = NULL;

    idBool sKeyFound = ID_FALSE;
    idBool sSubKeyFound = ID_FALSE;
    UInt   i = 0;

    IDE_DASSERT( idlOS::strlen( aSplitMethod ) == 1 );

    IDE_TEST( qsxProc::getProcInfo( aProcOID, &sProcInfo ) != IDE_SUCCESS );

    IDE_DASSERT( sProcInfo != NULL );

    if ( ( aSplitMethod[0] == 'H' ) ||  
         ( aSplitMethod[0] == 'R' ) ||
         ( aSplitMethod[0] == 'L' ) )
    {
        for ( sParaDecls = sProcInfo->planTree->paraDecls, i = 0;
              sParaDecls != NULL;
              sParaDecls = sParaDecls->next, i++ )
        {
            if ( idlOS::strMatch( sParaDecls->name.stmtText + sParaDecls->name.offset,
                                  sParaDecls->name.size,
                                  aKeyName,
                                  idlOS::strlen( aKeyName ) ) == 0 )
            {
                sKeyFound = ID_TRUE;
                IDE_TEST_RAISE( sParaDecls->itemType != QS_VARIABLE, ERR_INVALID_SHARD_KEY_TYPE );
                IDE_TEST_RAISE( ((qsVariables*)sParaDecls)->inOutType != QS_IN, ERR_INVALID_SHARD_KEY_TYPE );
                IDE_TEST_RAISE( isSupportDataType( sProcInfo->planTree->paramModules[i]->id ) == ID_FALSE,
                                ERR_INVALID_SHARD_KEY_TYPE );
                break;
            }
        }

        IDE_TEST_RAISE( sKeyFound == ID_FALSE, ERR_NOT_EXIST_SHARD_KEY );

        if ( idlOS::strlen( aSubSplitMethod ) == 1 )
        {
            IDE_DASSERT( ( aSubSplitMethod[0] == 'H' ) ||  
                         ( aSubSplitMethod[0] == 'R' ) ||
                         ( aSubSplitMethod[0] == 'L' ) );

            for ( sParaDecls = sProcInfo->planTree->paraDecls, i = 0;
                  sParaDecls != NULL;
                  sParaDecls = sParaDecls->next, i++ )
            {
                if ( idlOS::strMatch( sParaDecls->name.stmtText + sParaDecls->name.offset,
                                      sParaDecls->name.size,
                                      aSubKeyName,
                                      idlOS::strlen( aSubKeyName ) ) == 0 )
                {
                    sSubKeyFound = ID_TRUE;
                    IDE_TEST_RAISE( sParaDecls->itemType != QS_VARIABLE, ERR_INVALID_SHARD_KEY_TYPE );
                    IDE_TEST_RAISE( ((qsVariables*)sParaDecls)->inOutType != QS_IN, ERR_INVALID_SHARD_KEY_TYPE );
                    IDE_TEST_RAISE( isSupportDataType( sProcInfo->planTree->paramModules[i]->id ) == ID_FALSE,
                                    ERR_INVALID_SHARD_KEY_TYPE );
                    break;
                }
            }

            IDE_TEST_RAISE( sSubKeyFound == ID_FALSE, ERR_NOT_EXIST_SHARD_KEY );
        }
        else
        {
            IDE_DASSERT( aSubSplitMethod[0] == '\0' );
        }
    }
    else
    {
        IDE_DASSERT( ( aSplitMethod[0] == 'C' ) || ( aSplitMethod[0] == 'S' ) );
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_NOT_EXIST_SHARD_KEY )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_SHARD_KEY_COLUMN_NOT_EXIST,
                                  aUserName,
                                  aProcName,
                                  ( sKeyFound == ID_FALSE )? aKeyName: aSubKeyName ) );
    }
    IDE_EXCEPTION( ERR_INVALID_SHARD_KEY_TYPE )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_UNSUPPORTED_SHARD_KEY_COLUMN_TYPE,
                                  aUserName,
                                  aProcName,
                                  ( sSubKeyFound == ID_TRUE )? aSubKeyName: aKeyName ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC sdm::validateColumnBeforeInsert( qcStatement  * aStatement,
                                        UInt           aUserID,
                                        SChar        * aTableName,
                                        SChar        * aSplitMethod,
                                        SChar        * aKeyName,
                                        SChar        * aSubSplitMethod,
                                        SChar        * aSubKeyName )
{
    smSCN          sTableSCN;
    void         * sTableHandle = NULL;
    qcmTableInfo * sTableInfo = NULL;
    idBool         sKeyFound = ID_FALSE;
    idBool         sSubKeyFound = ID_FALSE;
    UInt           i = 0;

    IDE_DASSERT( idlOS::strlen( aSplitMethod ) == 1 );

    IDE_TEST( qciMisc::getTableInfo( aStatement,
                                     aUserID,
                                     (UChar*)aTableName,
                                     idlOS::strlen( aTableName ),
                                     &sTableInfo,
                                     &sTableSCN,
                                     &sTableHandle )
              != IDE_SUCCESS );

    IDE_DASSERT( sTableInfo != NULL );

    if ( ( aSplitMethod[0] == 'H' ) ||  
         ( aSplitMethod[0] == 'R' ) ||
         ( aSplitMethod[0] == 'L' ) )
    {
        for ( i = 0; i < sTableInfo->columnCount; i++ )
        {
            if ( idlOS::strMatch( sTableInfo->columns[i].name,
                                  idlOS::strlen( sTableInfo->columns[i].name ),
                                  aKeyName,
                                  idlOS::strlen( aKeyName ) ) == 0 )
            {
                sKeyFound = ID_TRUE;
                IDE_TEST_RAISE( isSupportDataType( sTableInfo->columns[i].basicInfo->module->id ) == ID_FALSE,
                                ERR_INVALID_SHARD_KEY_TYPE );
                break;
            }
        }

        IDE_TEST_RAISE( sKeyFound == ID_FALSE, ERR_NOT_EXIST_SHARD_KEY );

        if ( idlOS::strlen( aSubSplitMethod ) == 1 )
        {
            IDE_DASSERT( ( aSubSplitMethod[0] == 'H' ) ||  
                         ( aSubSplitMethod[0] == 'R' ) ||
                         ( aSubSplitMethod[0] == 'L' ) );

            for ( i = 0; i < sTableInfo->columnCount; i++ )
            {
                if ( idlOS::strMatch( sTableInfo->columns[i].name,
                                      idlOS::strlen( sTableInfo->columns[i].name ),
                                      aSubKeyName,
                                      idlOS::strlen( aSubKeyName ) ) == 0 )
                {
                    sSubKeyFound = ID_TRUE;
                    IDE_TEST_RAISE( isSupportDataType( sTableInfo->columns[i].basicInfo->module->id ) == ID_FALSE,
                                    ERR_INVALID_SHARD_KEY_TYPE );
                    break;
                }
            }

            IDE_TEST_RAISE( sSubKeyFound == ID_FALSE, ERR_NOT_EXIST_SHARD_KEY );
        }
        else
        {
            IDE_DASSERT( aSubSplitMethod[0] == '\0' );
        }
    }
    else
    {
        IDE_DASSERT( ( aSplitMethod[0] == 'C' ) || ( aSplitMethod[0] == 'S' ) );
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION( ERR_NOT_EXIST_SHARD_KEY )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_SHARD_KEY_COLUMN_NOT_EXIST,
                                  sTableInfo->tableOwnerName,
                                  sTableInfo->name,
                                  ( sKeyFound == ID_FALSE )? aKeyName: aSubKeyName ) );
    }
    IDE_EXCEPTION( ERR_INVALID_SHARD_KEY_TYPE )
    {
        IDE_SET( ideSetErrorCode( sdERR_ABORT_SDM_UNSUPPORTED_SHARD_KEY_COLUMN_TYPE,
                                  sTableInfo->tableOwnerName,
                                  sTableInfo->name,
                                  ( sSubKeyFound == ID_TRUE )? aSubKeyName: aKeyName ) );
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

idBool sdm::isSupportDataType( UInt aModuleID )
{
    idBool sResult = ID_FALSE;

    if ( ( aModuleID == MTD_SMALLINT_ID ) ||
         ( aModuleID == MTD_INTEGER_ID  ) ||
         ( aModuleID == MTD_BIGINT_ID   ) ||
         ( aModuleID == MTD_CHAR_ID     ) ||
         ( aModuleID == MTD_VARCHAR_ID  ) )
    {
        sResult = ID_TRUE;
    }

    return sResult;
}
