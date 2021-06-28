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
 * $Id: utmTable.cpp $
 **********************************************************************/

#include <smDef.h>
#include <utm.h>
#include <utmManager.h>
#include <utmExtern.h>
#include <utmDbStats.h>
#include <utmScript4Ilo.h>

#define GET_COLUMN_COMMENTS_QUERY                                   \
    "SELECT COLUMN_NAME, COMMENTS "                                 \
    "FROM SYSTEM_.SYS_COMMENTS_ "                                   \
    "WHERE USER_NAME = ? AND TABLE_NAME = ?"

/* BUG-32114 aexport must support the import/export of partition tables. */
#define GET_TABLE_CHECK_PARTITION_QUERY                     \
    " select COUNT(*) "                                     \
    " from "                                                \
    " SYSTEM_.SYS_TABLE_PARTITIONS_ A, "                    \
    " ( "                                                   \
    " select A.USER_ID, B.TABLE_ID "                        \
    " from "                                                \
    " SYSTEM_.SYS_USERS_ A, SYSTEM_.SYS_TABLES_ B "         \
    " where "                                               \
    " A.USER_NAME='%s' and B.TABLE_NAME='%s' "              \
    " )CHARTONUM "                                          \
    " where "                                               \
    " A.USER_ID = CHARTONUM.USER_ID "                       \
    " and A.TABLE_ID = CHARTONUM.TABLE_ID "                 \
    " and A.PARTITION_NAME IS NOT NULL" 

#define GET_TABLE_PARTITION_QUERY                           \
    " select A.PARTITION_NAME "                             \
    " from "                                                \
    " SYSTEM_.SYS_TABLE_PARTITIONS_ A, "                    \
    " ( "                                                   \
    " select A.USER_ID, B.TABLE_ID "                        \
    " from "                                                \
    " SYSTEM_.SYS_USERS_ A, SYSTEM_.SYS_TABLES_ B "         \
    " where "                                               \
    " A.USER_NAME='%s' and B.TABLE_NAME='%s' "              \
    " )CHARTONUM "                                          \
    " where "                                               \
    " A.USER_ID = CHARTONUM.USER_ID "                       \
    " and A.TABLE_ID = CHARTONUM.TABLE_ID "                 \
    " and A.PARTITION_NAME IS NOT NULL" 

SQLRETURN getTableInfo( SChar *a_user,
                        FILE  *aIlOutFp,
                        FILE  *aIlInFp,
                        FILE  *aTblFp,
                        FILE  *aIndexFp,
                        FILE  *aAltTblFp,  /* PROJ-2359 Table/Partition Access Option */
                        FILE  *aDbStatsFp )
{
#define IDE_FN "getTableInfo()"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY(""));
    SQLHSTMT s_tblStmt = SQL_NULL_HSTMT;
    SQLRETURN sRet;

    SChar       sQuotedUserName[UTM_NAME_LEN+3];
    SChar       s_file_name[UTM_NAME_LEN*2+10];
    SChar       s_user_name[UTM_NAME_LEN+1];
    SChar       s_puser_name[UTM_NAME_LEN+1];
    SChar       s_table_name[UTM_NAME_LEN+1];
    SChar       s_table_type[STR_LEN];
    SChar       s_passwd[STR_LEN];
    SQLLEN      s_user_ind;
    SQLLEN      s_table_ind;
    SQLLEN      sMaxRowInd;
#if (SIZEOF_LONG == 8) || defined(HAVE_LONG_LONG) || defined(VC_WIN32)
    SQLBIGINT   sMaxRow   = (SQLBIGINT)ID_LONG(0);
#else
    SQLBIGINT   sMaxRow   = {0, 0};
#endif
    SChar       sTbsName[41];
    SQLLEN      sTbsName_ind;
    SInt        sTbsType  = 0;
    SQLLEN      sTbsType_ind;
    SInt        sPctFree = 0;
    SQLLEN      sPctFree_ind;
    SInt        sPctUsed = 0;
    SQLLEN      sPctUsed_ind;

    s_file_name[0] = '\0';
    s_puser_name[0] = '\0';

    idlOS::fprintf(stdout, "\n##### TABLE #####\n");

    IDE_TEST_RAISE(SQLAllocStmt(m_hdbc, &s_tblStmt) != SQL_SUCCESS,
                   alloc_error);

    if (idlOS::strcasecmp(a_user, "SYS") == 0)
    {
        IDE_TEST_RAISE(SQLTables(s_tblStmt, NULL, 0, NULL, 0, NULL, 0,
                                 (SQLCHAR *)"TABLE,MATERIALIZED VIEW,GLOBAL TEMPORARY", SQL_NTS)
                       != SQL_SUCCESS, tbl_error);
    }
    else
    {
        /* BUG-36593 소문자,공백이 포함된 이름(quoted identifier) 처리 */
        utString::makeQuotedName(sQuotedUserName, a_user, idlOS::strlen(a_user));

        IDE_TEST_RAISE(SQLTables(s_tblStmt,
                             NULL, 0,
                             (SQLCHAR *)sQuotedUserName, SQL_NTS,
                             NULL, 0,
                             (SQLCHAR *)"TABLE,MATERIALIZED VIEW,GLOBAL TEMPORARY", SQL_NTS)
                       != SQL_SUCCESS, tbl_error);
    }

    IDE_TEST_RAISE(
        SQLBindCol(s_tblStmt, 2, SQL_C_CHAR, (SQLPOINTER)s_user_name,
                   (SQLLEN)ID_SIZEOF(s_user_name), &s_user_ind)
        != SQL_SUCCESS, tbl_error);
    IDE_TEST_RAISE(
        SQLBindCol(s_tblStmt, 3, SQL_C_CHAR, (SQLPOINTER)s_table_name,
                   (SQLLEN)ID_SIZEOF(s_table_name), &s_table_ind)
        != SQL_SUCCESS, tbl_error);
    IDE_TEST_RAISE(
        SQLBindCol(s_tblStmt, 4, SQL_C_CHAR, (SQLPOINTER)s_table_type,
                   (SQLLEN)ID_SIZEOF(s_table_type), NULL)
        != SQL_SUCCESS, tbl_error);
    IDE_TEST_RAISE(
        SQLBindCol(s_tblStmt, 6, SQL_C_SBIGINT, (SQLPOINTER)&sMaxRow, 0,
               &sMaxRowInd)
        != SQL_SUCCESS, tbl_error);

    /* PROJ-1349
     * 7  TABLESPACE tbs_name
     * 8  TABLESPACE tbs_type
     * 9  PCTFREE (default : 10)
     * 10 PCTUSED (default : 40)
     */

    IDE_TEST_RAISE(
        SQLBindCol(s_tblStmt, 7, SQL_C_CHAR, (SQLPOINTER)sTbsName,
                   (SQLLEN)ID_SIZEOF(sTbsName), &sTbsName_ind)
        != SQL_SUCCESS, tbl_error);        
    IDE_TEST_RAISE(
        SQLBindCol(s_tblStmt, 8, SQL_C_SLONG, (SQLPOINTER)&sTbsType, 0,
                   &sTbsType_ind)
        != SQL_SUCCESS, tbl_error);

    /* BUG-23652
     * [5.3.1 SD] insert high limit, insert low limit 을 pctfree 와 pctused 로 변경 */
    IDE_TEST_RAISE(    
        SQLBindCol(s_tblStmt, 9, SQL_C_SLONG, (SQLPOINTER)&sPctFree, 0,
                   &sPctFree_ind) != SQL_SUCCESS, tbl_error);

    IDE_TEST_RAISE(
        SQLBindCol(s_tblStmt, 10, SQL_C_SLONG, (SQLPOINTER)&sPctUsed, 0,
                   &sPctUsed_ind) != SQL_SUCCESS, tbl_error);

    while ((sRet = SQLFetch(s_tblStmt)) != SQL_NO_DATA)
    {
        IDE_TEST_RAISE( sRet != SQL_SUCCESS, tbl_error );

        if ( idlOS::strncmp( s_user_name, "SYSTEM_", 7 ) == 0 )
        {
            continue;
        }

        /* BUG-29495
           The temp table for checking the relation of views and procedures is exported
           when two or more the aexports are executed simultaneously. */
        if ( idlOS::strcmp( s_table_type, "TABLE" ) == 0
             && idlOS::strcmp( s_table_name, "__TEMP_TABLE__FOR__DEPENDENCY__" ) != 0 )
        {
            idlOS::fprintf(stdout, "** \"%s\".\"%s\"\n", s_user_name, s_table_name);
            IDE_TEST(getPasswd(s_user_name, s_passwd) != SQL_SUCCESS);

            idlOS::fprintf(aTblFp, "\n");
            idlOS::fprintf(aTblFp, "connect \"%s\" / \"%s\"\n",s_user_name, s_passwd);

            /* PROJ-2359 Table/Partition Access Option */
            idlOS::fprintf( aAltTblFp, "\nconnect \"%s\" / \"%s\"\n", s_user_name, s_passwd );

            idlOS::sprintf(s_file_name, "%s_%s.fmt", s_user_name, s_table_name);

            /* BUG-32114 aexport must support the import/export of partition tables. */
            IDE_TEST( createRunIlScript( aIlOutFp, aIlInFp, s_user_name,
                                         s_table_name,s_passwd ) != SQL_SUCCESS );

            /* BUG-40174 Support export and import DBMS Stats */
            if ( gProgOption.mbCollectDbStats == ID_TRUE )
            {
                IDE_TEST( getTableStats( s_user_name, 
                                         s_table_name, 
                                         aDbStatsFp ) 
                          != SQL_SUCCESS );
            }

            IDE_TEST( getTableQuery( s_user_name,
                                     s_table_name,
                                     aTblFp,
                                     aAltTblFp )
                      != SQL_SUCCESS );

            IDE_TEST( getIndexQuery( s_user_name, s_puser_name, s_table_name, aIndexFp )
                      != SQL_SUCCESS );

            /* BUG-26236 comment 쿼리문의 유틸리티 지원 */
            IDE_TEST( getComment( s_user_name,
                                  s_table_name,
                                  aTblFp ) != SQL_SUCCESS );

            idlOS::fprintf(stdout, "\n");
        }
        else if ( idlOS::strcmp( s_table_type, "GLOBAL TEMPORARY" ) == 0 )
        {
            idlOS::fprintf(stdout, "** \"%s\".\"%s\"\n", s_user_name, s_table_name);
            IDE_TEST(getPasswd(s_user_name, s_passwd) != SQL_SUCCESS);

            idlOS::fprintf(aTblFp, "\n");
            idlOS::fprintf(aTblFp, "connect \"%s\" / \"%s\"\n",s_user_name, s_passwd);

            IDE_TEST( getTempTableQuery( s_user_name,
                                         s_table_name,
                                         aTblFp )  != SQL_SUCCESS );

            idlOS::fprintf(stdout, "\n");
        }
        else if ( idlOS::strcmp( s_table_type, "MATERIALIZED VIEW" ) == 0 )
        {
            IDE_TEST( getIndexQuery( s_user_name, s_puser_name, s_table_name, aIndexFp )
                      != SQL_SUCCESS );
        }
        else
        {
            continue;
        }

        idlOS::strcpy(s_puser_name, s_user_name);
    } // end while

    // BUG-33995 aexport have wrong free handle code
    FreeStmt( &s_tblStmt );

    return SQL_SUCCESS;

    IDE_EXCEPTION(alloc_error);
    {
        utmSetErrorMsgWithHandle(SQL_HANDLE_DBC, (SQLHANDLE)m_hdbc);
    }
    IDE_EXCEPTION(tbl_error);
    {
        utmSetErrorMsgWithHandle(SQL_HANDLE_STMT, (SQLHANDLE)s_tblStmt);
    }
    IDE_EXCEPTION_END;

    if ( s_tblStmt != SQL_NULL_HSTMT )
    {
        // BUG-33995 aexport have wrong free handle code
        FreeStmt( &s_tblStmt );
    }

    return SQL_ERROR;
#undef IDE_FN
}


/* BUG-40038 [ux-aexport] Needs to consider temporary table in aexport codes */
SQLRETURN getTempTableQuery( SChar *a_user,
                             SChar *a_table,
                             FILE  *a_crt_fp )
{
#define IDE_FN "getTempTableQuery()"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY(""));

    idlOS::fprintf(a_crt_fp, "\n--############################\n");
    idlOS::fprintf(a_crt_fp, "--  \"%s\".\"%s\"  \n", a_user, a_table);
    idlOS::fprintf(a_crt_fp, "--############################\n");

    if ( gProgOption.mbExistDrop == ID_TRUE )
    {
        // BUG-20943 drop 구문에서 user 가 명시되지 않아 drop 이 실패합니다.
        idlOS::fprintf(a_crt_fp, "drop table \"%s\".\"%s\";\n", a_user, a_table);
    }
    
    /* BUG-47159 Using DBMS_METADATA package in aexport */
    IDE_TEST(gMeta->getDdl(DDL, UTM_OBJ_TYPE_TABLE, a_table, a_user)
             != IDE_SUCCESS);
    
    /* Table 구문을 FILE WRITE */
#ifdef DEBUG
    idlOS::fprintf( stderr, "%s\n", gMeta->getDdlStr() );
#endif
    idlOS::fprintf( a_crt_fp, "%s\n", gMeta->getDdlStr() );

    return SQL_SUCCESS;

    IDE_EXCEPTION_END;

    return SQL_ERROR;
#undef IDE_FN
}

SQLRETURN getTableQuery( SChar *a_user,
                         SChar *a_table,
                         FILE  *a_crt_fp,
                         FILE  *aAltTblFp )
{
#define IDE_FN "getTableQuery()"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY(""));
    SChar *sAlterStr = NULL;

    idlOS::fprintf(a_crt_fp, "\n--############################\n");
    idlOS::fprintf(a_crt_fp, "--  \"%s\".\"%s\"  \n", a_user, a_table);
    idlOS::fprintf(a_crt_fp, "--############################\n");
    if ( gProgOption.mbExistDrop == ID_TRUE )
    {
        // BUG-20943 drop 구문에서 user 가 명시되지 않아 drop 이 실패합니다.
        idlOS::fprintf(a_crt_fp, "drop table \"%s\".\"%s\";\n", a_user, a_table);
    }

    /* BUG-47159 Using DBMS_METADATA package in aexport */
    IDE_TEST(gMeta->getDdl(DDL, UTM_OBJ_TYPE_TABLE, a_table, a_user)
             != IDE_SUCCESS);
    
    /* Table 구문을 FILE WRITE */
#ifdef DEBUG
    idlOS::fprintf( stderr, "%s\n", gMeta->getDdlStr() );
#endif
    idlOS::fprintf( a_crt_fp, "%s\n", gMeta->getDdlStr() );

    /* TABLE EXPORT 될 때 해당 TABLE과 연관된 OBJECT PRIVILEGE 함께 EXPORT */
    IDE_TEST( getObjPrivQuery( a_crt_fp, UTM_TABLE, a_user, a_table )
                               != SQL_SUCCESS );

    idlOS::fflush(a_crt_fp);

    /* BUG-47159 Using DBMS_METADATA package in aexport */
    /* PROJ-2359 Table/Partition Access Option */
    IDE_TEST(gMeta->getDdl(DEPENDENT_DDL, UTM_OBJ_TYPE_ACCESS_MODE, a_table, a_user)
             != IDE_SUCCESS);
    sAlterStr = gMeta->getDdlStr();
    if (sAlterStr[0] != '\0')
    {
        idlOS::fprintf(aAltTblFp, "%s\n", sAlterStr);
        idlOS::fflush( aAltTblFp );
    }

    return SQL_SUCCESS;

    IDE_EXCEPTION_END;

    return SQL_ERROR;
#undef IDE_FN
}

/* BUG-26236 comment 쿼리문의 유틸리티 지원 */
SQLRETURN getComment( SChar * a_user,
                      SChar * a_table,
                      FILE  * a_fp )
{
    SChar *sDdl = NULL;

    /* BUG-47159 Using DBMS_METADATA package in aexport */
    IDE_TEST(gMeta->getDdl(DEPENDENT_DDL, UTM_OBJ_TYPE_COMMENT, a_table, a_user)
             != IDE_SUCCESS);
    
    sDdl = gMeta->getDdlStr();
    if (sDdl[0] != '\0')
    {
#ifdef DEBUG
        idlOS::fprintf( stderr, "%s\n", sDdl );
#endif
        idlOS::fprintf( a_fp, "%s\n", sDdl );
    }

    return SQL_SUCCESS;

    IDE_EXCEPTION_END;

    return SQL_ERROR;
}

/* BUG-32114 aexport must support the import/export of partition tables. */
SQLRETURN createRunIlScript( FILE *aIlOutFp,
                             FILE *aIlInFp,
                             SChar *aUser,
                             SChar *aTable,
                             SChar *aPasswd )
{
    SQLHSTMT    sPartitionStmt = SQL_NULL_HSTMT;
    SQLHSTMT    sCheckStmt = SQL_NULL_HSTMT;
    SQLRETURN   sRet;
    SChar       sQuery[QUERY_LEN];
    SChar       sCheckQuery[QUERY_LEN];
    SChar       sPartName[STR_LEN];
    SInt        sPartCnt = 0;
    SQLLEN      sPartNameInd;

    idBool sNeedQuote4User   = ID_FALSE;
    idBool sNeedQuote4Pwd = ID_FALSE;
    idBool sNeedQuote4Tbl    = ID_FALSE;
    SInt   sNeedQuote4File          = 0;
    
    /* CHECK PARTITION */
    if( gProgOption.mbExistIloaderPartition == ID_TRUE )
    {
        IDE_TEST_RAISE(SQLAllocStmt(m_hdbc, &sCheckStmt) != SQL_SUCCESS,
                alloc_error);

        idlOS::sprintf(sCheckQuery, GET_TABLE_CHECK_PARTITION_QUERY,
                aUser, aTable);

        IDE_TEST(Prepare(sCheckQuery, sCheckStmt) != SQL_SUCCESS);

        IDE_TEST_RAISE(
                SQLBindCol( sCheckStmt, 1, SQL_C_SLONG, (SQLPOINTER)&sPartCnt,
                    0, NULL ) != SQL_SUCCESS, stmt_error);

        IDE_TEST(Execute(sCheckStmt) != SQL_SUCCESS );

        sRet = SQLFetch(sCheckStmt);

        IDE_TEST_RAISE( sRet!= SQL_SUCCESS, stmt_error );        

        // BUG-33995 aexport have wrong free handle code
        FreeStmt( &sCheckStmt );
    }
   
    /* BUG-34502: handling quoted identifiers */
    sNeedQuote4User   = utString::needQuotationMarksForObject(aUser);
    sNeedQuote4Pwd = utString::needQuotationMarksForFile(aPasswd);
    sNeedQuote4Tbl    = utString::needQuotationMarksForObject(aTable, ID_TRUE);
    sNeedQuote4File          = utString::needQuotationMarksForFile(aUser) ||
                              utString::needQuotationMarksForFile(aTable);

    /* run_il_out.sh / run_il_in.sh */
    if ( sPartCnt == 0 )
    {
        /* formout in run_il_out.sh */
        printFormOutScript(aIlOutFp,
                           sNeedQuote4User,
                           sNeedQuote4Pwd,
                           sNeedQuote4Tbl,
                           sNeedQuote4File,
                           aUser,
                           aPasswd,
                           aTable,
                           ID_FALSE);

        /* data out in run_il_out.sh*/
        printOutScript(aIlOutFp,
                       sNeedQuote4User,
                       sNeedQuote4Pwd,
                       sNeedQuote4File,
                       aUser,
                       aPasswd,
                       aTable,
                       NULL );

        /* data in in run_il_in.sh*/
        printInScript(aIlInFp,
                      sNeedQuote4User,
                      sNeedQuote4Pwd,
                      sNeedQuote4File,
                      aUser,
                      aPasswd,
                      aTable,
                      NULL );
    }
    else  /* exist partition tables */
    {
        /* formout in run_il_out.sh */
        printFormOutScript(aIlOutFp,
                           sNeedQuote4User,
                           sNeedQuote4Pwd,
                           sNeedQuote4Tbl,
                           sNeedQuote4File,
                           aUser,
                           aPasswd,
                           aTable,
                           gProgOption.mbExistIloaderPartition);

        /* PARTITION NAME */
        IDE_TEST_RAISE(SQLAllocStmt(m_hdbc, &sPartitionStmt) != SQL_SUCCESS,
                alloc_error);

        idlOS::sprintf(sQuery, GET_TABLE_PARTITION_QUERY,
                aUser, aTable);

        IDE_TEST(Prepare(sQuery, sPartitionStmt) != SQL_SUCCESS);

        IDE_TEST_RAISE(
                SQLBindCol(sPartitionStmt, 1, SQL_C_CHAR, (SQLPOINTER)sPartName,
                    (SQLLEN)ID_SIZEOF(sPartName), &sPartNameInd)
                != SQL_SUCCESS, stmt_error);

        IDE_TEST(Execute(sPartitionStmt) != SQL_SUCCESS );

        while ((sRet = SQLFetch(sPartitionStmt)) != SQL_NO_DATA )
        {
            IDE_TEST_RAISE( sRet!= SQL_SUCCESS, stmt_error );

            sNeedQuote4File = utString::needQuotationMarksForFile(aUser)  ||
                             utString::needQuotationMarksForFile(aTable) ||
                             utString::needQuotationMarksForFile(sPartName);

            /* data out in run_il_out.sh */
            printOutScript(aIlOutFp,
                           sNeedQuote4User,
                           sNeedQuote4Pwd,
                           sNeedQuote4File,
                           aUser,
                           aPasswd,
                           aTable,
                           sPartName);

            /* data in in run_il_in.sh*/
            printInScript(aIlInFp,
                          sNeedQuote4User,
                          sNeedQuote4Pwd,
                          sNeedQuote4File,
                          aUser,
                          aPasswd,
                          aTable,
                          sPartName);
        }

        // BUG-33995 aexport have wrong free handle code
        FreeStmt( &sPartitionStmt );
    }

    return SQL_SUCCESS;

    IDE_EXCEPTION(alloc_error);
    {
        utmSetErrorMsgWithHandle(SQL_HANDLE_DBC, (SQLHANDLE)m_hdbc);
    }
    IDE_EXCEPTION(stmt_error);
    {
        utmSetErrorMsgWithHandle(SQL_HANDLE_STMT, (SQLHANDLE)sPartitionStmt);
    }
    IDE_EXCEPTION_END;

    // BUG-33995 aexport have wrong free handle code
    if ( sCheckStmt != SQL_NULL_HSTMT )
    {
        FreeStmt( &sCheckStmt );
    }

    if ( sPartitionStmt != SQL_NULL_HSTMT )
    {
        // BUG-33995 aexport have wrong free handle code
        FreeStmt( &sPartitionStmt );
    }

    return SQL_ERROR;
}

