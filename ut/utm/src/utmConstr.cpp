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
 * $Id: utmConstr.cpp $
 **********************************************************************/

#include <smDef.h>
#include <utm.h>
#include <utmExtern.h>
#include <utmDbStats.h>

/* BUG-45518 trigger_oid -> trigger_name */
#define GET_TRIGGER_QUERY "SELECT a.user_name, a.trigger_name, b.seqno, b.substring "       \
                           "from system_.sys_triggers_ a, system_.sys_trigger_strings_ b " \
                           "where b.trigger_oid = a.trigger_oid "                          \
                           "and a.USER_NAME=? "                                            \
                           "order by 1, 2, 3 "

#define GET_TRIGGER_SYS_QUERY "SELECT a.user_name, a.trigger_name, b.seqno, b.substring "   \
                           "from system_.sys_triggers_ a, system_.sys_trigger_strings_ b " \
                           "where b.trigger_oid = a.trigger_oid "                          \
                           "order by 1, 2, 3 "

SQLRETURN getIndexQuery( SChar * a_user,
                         SChar * a_puser,
                         SChar * a_table,
                         FILE  * a_IndexFp )
{
#define IDE_FN "getIndexQuery()"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY(""));

    SInt   i = 0;
    idBool sConnectStrFlag = ID_FALSE;
    static SChar  sPuserName[UTM_NAME_LEN+1];
    SChar s_passwd[STR_LEN];
    SChar *sDdl = NULL;
    const SChar *sObjTypes[] = { UTM_OBJ_TYPE_CONSTRAINT,
                                 UTM_OBJ_TYPE_INDEX };

    if (( idlOS::strcmp( a_user, a_puser ) != 0) &&
            ( gProgOption.m_bExist_OBJECT != ID_TRUE ))
    {
        idlOS::strcpy( sPuserName, a_puser );
    }
    
    if (( idlOS::strcmp( a_user, sPuserName ) != 0) &&
            /* BUG-49356 User mode인 경우에만 connect 문 생성,
             * DB mode는 SYS/manager로 고정 */
            ( mExportModeType == UTM_EXPORT_USER_MODE ))
    {
        IDE_TEST(getPasswd( a_user, s_passwd) != SQL_SUCCESS);
        sConnectStrFlag = ID_TRUE;
    }

    /* BUG-47159 Using DBMS_METADATA package in aexport */
    for (i=0; i<2; i++)
    {
        IDE_TEST(gMeta->getDdl(DEPENDENT_DDL,
                    sObjTypes[i],
                    a_table, a_user) != IDE_SUCCESS);

        sDdl = gMeta->getDdlStr();
        if (sDdl[0] != '\0')
        {
#ifdef DEBUG
            idlOS::fprintf(stderr, "%s\n\n", sDdl);
#endif
            if (sConnectStrFlag == ID_TRUE)
            {
                idlOS::fprintf(a_IndexFp, "connect \"%s\" / \"%s\"\n", a_user, s_passwd);
                sConnectStrFlag = ID_FALSE;
                idlOS::strcpy( sPuserName, a_user );
            }
            idlOS::fprintf(a_IndexFp, "%s\n\n", sDdl);
            idlOS::fflush(a_IndexFp);
        }
    }

    return SQL_SUCCESS;

    IDE_EXCEPTION_END;

    return SQL_ERROR;
#undef IDE_FN
}

/* 전체 DB MODE일 경우 USER, TABLE NAME 구함 */
SQLRETURN getForeignKeys( SChar *a_user,
                          FILE  *aFkFp )
{
#define IDE_FN "getForeignKeys()"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY(""));

    SChar     sQuotedUserName[UTM_NAME_LEN+3];
    SQLHSTMT  s_tblStmt = SQL_NULL_HSTMT;
    SQLRETURN sRet;
    SChar     s_puser_name[UTM_NAME_LEN+1];
    SChar     s_user_name[UTM_NAME_LEN+1];
    SChar     s_table_name[UTM_NAME_LEN+1];
    SChar     s_table_type[STR_LEN];
    SQLLEN    s_user_ind;
    SQLLEN    s_table_ind;

    s_puser_name[0] = '\0';
    
    IDE_TEST_RAISE(SQLAllocStmt(m_hdbc, &s_tblStmt) != SQL_SUCCESS,
                   alloc_error);


    if (idlOS::strcasecmp(a_user, "SYS") == 0)
    {
        IDE_TEST_RAISE(SQLTables(s_tblStmt, NULL, 0, NULL, 0, NULL, 0,
                                 (SQLCHAR *)"TABLE,MATERIALIZED VIEW", SQL_NTS)
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
                            (SQLCHAR *)"TABLE,MATERIALIZED VIEW", SQL_NTS)
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

    while ((sRet = SQLFetch(s_tblStmt)) != SQL_NO_DATA)
    {
        IDE_TEST_RAISE( sRet != SQL_SUCCESS, tbl_error );

        if ( ( idlOS::strncmp(s_user_name, "SYSTEM_", 7) == 0 ) ||
             ( ( idlOS::strcmp(s_table_type, "TABLE") != 0 ) &&
               ( idlOS::strcmp(s_table_type, "MATERIALIZED VIEW") != 0 ) ) ) /* PROJ-2211 Materialized View */
        {
            continue;
        }

        IDE_TEST( resultForeignKeys( s_user_name, s_puser_name, s_table_name, aFkFp )
                                  != SQL_SUCCESS);

        idlOS::strcpy(s_puser_name, s_user_name);
    } //while

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

SQLRETURN resultForeignKeys( SChar *a_user,
                             SChar *a_puser,
                             SChar *a_table,
                             FILE  *a_fp )
{
#define IDE_FN "resultForeignKeys()"
    IDE_MSGLOG_FUNC(IDE_MSGLOG_BODY(""));

    idBool    sConnectStrFlag = ID_FALSE;
    SChar     s_passwd[STR_LEN];
    SChar    *sDdl = NULL;
    static SChar  sPuserName[UTM_NAME_LEN+1];

    if (( idlOS::strcmp( a_user, a_puser ) != 0) &&
            ( gProgOption.m_bExist_OBJECT != ID_TRUE ))
    {
        idlOS::strcpy( sPuserName, a_puser );
    }
    if (( idlOS::strcmp( a_user, sPuserName ) != 0) &&
            ( gProgOption.m_bExist_OBJECT != ID_TRUE ))
    {
        IDE_TEST(getPasswd( a_user, s_passwd) != SQL_SUCCESS);
        sConnectStrFlag = ID_TRUE;
    }

    /* BUG-47159 Using DBMS_METADATA package in aexport */
    IDE_TEST(gMeta->getDdl(DEPENDENT_DDL,
                UTM_OBJ_TYPE_REF_CONST,
                a_table, a_user) != IDE_SUCCESS);
    sDdl = gMeta->getDdlStr();
    if (sDdl[0] != '\0')
    {
        if (sConnectStrFlag == ID_TRUE)
        {
            idlOS::fprintf(a_fp, "connect \"%s\" / \"%s\"\n", a_user, s_passwd);
            idlOS::strcpy( sPuserName, a_user );
        }
#ifdef DEBUG
        idlOS::fprintf( stderr, "%s\n\n", sDdl );
#endif
        idlOS::fprintf( a_fp, "%s\n\n", sDdl );
        idlOS::fflush( a_fp );
    }

    return SQL_SUCCESS;

    IDE_EXCEPTION_END;

    return SQL_ERROR;
#undef IDE_FN
}

SQLRETURN getTrigQuery( SChar *a_user,
                         FILE *aTrigFp)
{

#define IDE_FN "getTrigQuery()"
    SQLHSTMT  s_trig_stmt = SQL_NULL_HSTMT;
    SQLRETURN sRet;    
    SChar     s_query[2048];    
    SChar     s_passwd[STR_LEN];
    SChar     s_puser_name[UTM_NAME_LEN+1];
    SChar     s_user_name[UTM_NAME_LEN+1];
    SChar     s_trig_name[UTM_NAME_LEN+1]; /* BUG-45518 */
    SChar     s_trig_parse[110];
    SInt      s_seq_no      = 0;
    SQLLEN    s_user_ind    = 0;
    SQLLEN    s_trig_ind    = 0;
    SQLLEN    s_parse_ind   = 0;
    idBool    sFirstFlag = ID_TRUE;

    s_puser_name[0] = '\0';

    idlOS::fprintf(stdout, "\n##### TRIGGER #####\n");    
    
    IDE_TEST_RAISE( SQLAllocStmt( m_hdbc, &s_trig_stmt ) 
                                  != SQL_SUCCESS, alloc_error);
    
    if (idlOS::strcasecmp(a_user, "SYS") != 0)
    {
        idlOS::sprintf(s_query, GET_TRIGGER_QUERY);    
    }
    else
    {
        idlOS::sprintf(s_query, GET_TRIGGER_SYS_QUERY);    
    }

    IDE_TEST(Prepare(s_query, s_trig_stmt) != SQL_SUCCESS);

    IDE_TEST_RAISE(
    SQLBindParameter(s_trig_stmt, 1, SQL_PARAM_INPUT,
                     SQL_C_CHAR, SQL_VARCHAR, UTM_NAME_LEN, 0,
                     a_user, UTM_NAME_LEN+1, NULL)
                     != SQL_SUCCESS, trig_error);

    IDE_TEST_RAISE(
    SQLBindCol( s_trig_stmt, 1, SQL_C_CHAR, 
                s_user_name, ID_SIZEOF(s_user_name), &s_user_ind )
                != SQL_SUCCESS, trig_error );

    IDE_TEST_RAISE(    
    SQLBindCol( s_trig_stmt, 2, SQL_C_CHAR,
                s_trig_name, ID_SIZEOF(s_trig_name), &s_trig_ind )
                != SQL_SUCCESS, trig_error );

    IDE_TEST_RAISE(    
    SQLBindCol( s_trig_stmt, 3, SQL_C_SLONG, 
                &s_seq_no,  0, NULL )
                != SQL_SUCCESS, trig_error );
   
    IDE_TEST_RAISE(    
    SQLBindCol( s_trig_stmt, 4, SQL_C_CHAR, 
                s_trig_parse, ID_SIZEOF(s_trig_parse), &s_parse_ind )
                != SQL_SUCCESS, trig_error );    
        
    IDE_TEST(Execute(s_trig_stmt) != SQL_SUCCESS );

    while ( ( sRet = SQLFetch(s_trig_stmt)) != SQL_NO_DATA )
    {
        IDE_TEST_RAISE( sRet != SQL_SUCCESS, trig_error );

        if ( idlOS::strcmp(s_user_name, "SYSTEM_") == 0 )
        {
            continue;
        }     
        
        if ( s_seq_no == 0 )
        {
            if ( sFirstFlag != ID_TRUE )
            {
#ifdef DEBUG
                idlOS::fprintf( stderr, ";\n/\n\n" );
#endif
                idlOS::fprintf( aTrigFp, ";\n/\n\n" );

            }

            if ( idlOS::strcasecmp( s_user_name, s_puser_name ) != 0 )
            {
                IDE_TEST(getPasswd(s_user_name, s_passwd) != SQL_SUCCESS);

                idlOS::fprintf( aTrigFp, "connect \"%s\" / \"%s\"\n\n", s_user_name, s_passwd);

            }
        }
        
        sFirstFlag = ID_FALSE;
        
        idlOS::fprintf( aTrigFp, "%s", s_trig_parse);
        idlOS::fflush( aTrigFp );
        idlOS::strcpy(s_puser_name, s_user_name);
    }
    
    if ( sFirstFlag == ID_FALSE )
    {
#ifdef DEBUG
        idlOS::fprintf( stderr, ";\n/\n\n" );
#endif
        idlOS::fprintf( aTrigFp, ";\n/\n\n" );
    }

    // BUG-33995 aexport have wrong free handle code
    FreeStmt( &s_trig_stmt );
    
    return SQL_SUCCESS;
        
    IDE_EXCEPTION(alloc_error);
    {
        utmSetErrorMsgWithHandle(SQL_HANDLE_DBC, (SQLHANDLE)m_hdbc);
    }
    IDE_EXCEPTION(trig_error);
    {
        utmSetErrorMsgWithHandle(SQL_HANDLE_STMT, (SQLHANDLE)s_trig_stmt);
    }
    IDE_EXCEPTION_END;

    // BUG-33995 aexport have wrong free handle code
    if ( s_trig_stmt != SQL_NULL_HSTMT )
    {
        FreeStmt( &s_trig_stmt );
    }

    return SQL_ERROR;
#undef IDE_FN
}
