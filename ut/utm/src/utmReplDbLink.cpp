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
 * $Id: utmReplDbLink.cpp $
 **********************************************************************/

#include <smDef.h>
#include <rp.h>
#include <utm.h>
#include <utmExtern.h>

/* BUG-39658 Aexport should support 'FOR ANALYSIS' syntax. */
/* BUG-39661 Aexport should support 'OPTIONS' syntax. */
/* BUG-45236 Local Replication 지원 */
#define GET_REPLOBJ_QUERY                                            \
    "select /*+ NO_PLAN_CACHE */ distinct "                          \
    "       REPLICATION_NAME "                                       \
    "from SYSTEM_.SYS_REPLICATIONS_  "                               \
    "order by REPLICATION_NAME "

#define GET_DBLINK_SYS_QUERY                                        \
    "SELECT "                                                       \
    " us.USER_NAME, "                                               \
    " lk.LINK_NAME, "                                               \
    " lk.USER_MODE  "                                               \
    "FROM SYSTEM_.SYS_DATABASE_LINKS_ lk "                          \
    "     LEFT OUTER JOIN "                                         \
    "     SYSTEM_.SYS_USERS_ us "                                   \
    "     ON lk.USER_ID = us.USER_ID "                              \
    "ORDER BY 1,2 "

#define GET_DBLINK_QUERY                                            \
    "SELECT "                                                       \
    " us.USER_NAME, "                                               \
    " lk.LINK_NAME, "                                               \
    " lk.USER_MODE  "                                               \
    "FROM SYSTEM_.SYS_DATABASE_LINKS_ lk,"                          \
    "     SYSTEM_.SYS_USERS_ us "                                   \
    "WHERE lk.USER_ID = us.USER_ID "                                \
    "      AND us.USER_NAME = ? "                                   \
    "ORDER BY 1,2 "

SQLRETURN getReplQuery( FILE *aReplFp )
{
#define IDE_FN "getReplQuery()"
    SQLHSTMT  sResultStmt               = SQL_NULL_HSTMT;
    SQLRETURN sRet;

    SChar     sQuery[QUERY_LEN];
    SQLLEN    sRepInd                   = 0;

    SChar     sRepName[UTM_NAME_LEN + 1] = {'\0',};
    
    idlOS::fprintf(stdout, "\n##### REPLICATION #####\n");

    IDE_TEST_RAISE(
        SQLAllocStmt( m_hdbc,
                      &sResultStmt) != SQL_SUCCESS, alloc_error );

    idlOS::sprintf( sQuery, GET_REPLOBJ_QUERY );

    IDE_TEST_RAISE(
        SQLBindCol( sResultStmt,
                    1,
                    SQL_C_CHAR,
                    (SQLPOINTER)sRepName,
                    (SQLLEN)ID_SIZEOF(sRepName),
                    &sRepInd ) != SQL_SUCCESS, stmt_error );
   
    IDE_TEST_RAISE(
        SQLExecDirect( sResultStmt,
                       (SQLCHAR *)sQuery,
                       SQL_NTS ) != SQL_SUCCESS, stmt_error );
    
    while (( sRet = SQLFetch(sResultStmt)) != SQL_NO_DATA )
    {
        IDE_TEST_RAISE( sRet != SQL_SUCCESS, stmt_error );

        // only print 'DROP' query when aexport property 'DROP' is 'ON'
        if ( gProgOption.mbExistDrop == ID_TRUE )
        {
            idlOS::fprintf( aReplFp,
                            "\ndrop replication \"%s\";\n\n", 
                            sRepName );
        }

        /* BUG-47159 Using DBMS_METADATA package in aexport */
        IDE_TEST(gMeta->getDdl(DDL, UTM_OBJ_TYPE_REPLICATION,
                 sRepName, (SChar*)NULL) != IDE_SUCCESS);
        idlOS::fprintf( aReplFp, "%s\n", gMeta->getDdlStr() );
    }
    
    // BUG-33995 aexport have wrong free handle code
    FreeStmt( &sResultStmt );

    idlOS::fflush( aReplFp );

    return SQL_SUCCESS;

    IDE_EXCEPTION(alloc_error);
    {
        utmSetErrorMsgWithHandle(SQL_HANDLE_DBC, (SQLHANDLE)m_hdbc);
    }
    IDE_EXCEPTION(stmt_error);
    {
        utmSetErrorMsgWithHandle(SQL_HANDLE_STMT, (SQLHANDLE)sResultStmt);
    }
    IDE_EXCEPTION_END;

    if ( sResultStmt != SQL_NULL_HSTMT )
    {
        // BUG-33995 aexport have wrong free handle code
        FreeStmt( &sResultStmt );
    }

    return SQL_ERROR;
#undef IDE_FN
}

/***************************************************
 * Description: BUG-37050
 * 모든 DB link의 메타데이터를 조회, 
 * 그 데이터를 조합하여 create DB link DDL을 생성한다.
 * 생성한 DDL문을 파일에 기입한다.
 * 
 *   a_user  (IN): string of UserID 
 *   aLinkFp (IN): file pointer of ALL_CRT_LINK.sql
 ***************************************************/
SQLRETURN getDBLinkQuery( SChar *a_user, FILE  *aLinkFp )
{
    SQLHSTMT  sDblinkStmt = SQL_NULL_HSTMT;
    SQLRETURN sRet;
    SChar     sQuery[QUERY_LEN] = { 0, };

    SChar     sOwnerName[UTM_NAME_LEN + 1]  = { 0, };
    SChar     sLinkName[UTM_NAME_LEN + 1]   = { 0, };
    SInt      sUserMode = 0;
    UInt      sState    = 1;

    SQLLEN    sOwnerNameInd    = 0;
    SQLLEN    sLinkNameInd     = 0;
    SChar     sPasswd[STR_LEN]            = { 0, };
    SChar     sPrevName[UTM_NAME_LEN + 1] = { 0, };
    
    IDE_TEST_RAISE( SQLAllocStmt( m_hdbc, &sDblinkStmt )
                                  != SQL_SUCCESS, alloc_error);
    sState = 1;

    idlOS::fprintf(stdout, "\n##### DATABASE LINK #####\n");

    /* 유저가 SYS가 아닐때는 해당 유저의 정보만 가져온다. */
    if( idlOS::strcasecmp( a_user, (SChar*)UTM_STR_SYS ) != 0 )
    {
        idlOS::sprintf( sQuery, GET_DBLINK_QUERY );
    }
    else
    {
        idlOS::sprintf( sQuery, GET_DBLINK_SYS_QUERY );
    }

    IDE_TEST( Prepare( sQuery, sDblinkStmt ) != SQL_SUCCESS );

    if( idlOS::strcasecmp( a_user, (SChar*)UTM_STR_SYS ) != 0 )
    {
        IDE_TEST_RAISE( SQLBindParameter( sDblinkStmt, 
                                          1, 
                                          SQL_PARAM_INPUT,
                                          SQL_C_CHAR, 
                                          SQL_VARCHAR,
                                          UTM_NAME_LEN,
                                          0,
                                          a_user, 
                                          UTM_NAME_LEN+1, 
                                          NULL ) 
                        != SQL_SUCCESS, execute_error);
    }

    IDE_TEST_RAISE( SQLBindCol( sDblinkStmt, 
                                1, 
                                SQL_C_CHAR, 
                                (SQLPOINTER)sOwnerName,
                                (SQLLEN)ID_SIZEOF(sOwnerName), 
                                &sOwnerNameInd ) 
                    != SQL_SUCCESS, execute_error);

    IDE_TEST_RAISE( SQLBindCol( sDblinkStmt, 
                                2, 
                                SQL_C_CHAR, 
                                (SQLPOINTER)sLinkName,
                                (SQLLEN)ID_SIZEOF(sLinkName), 
                                &sLinkNameInd )
                    != SQL_SUCCESS, execute_error);

    IDE_TEST_RAISE( SQLBindCol( sDblinkStmt, 
                                3, 
                                SQL_C_LONG, 
                                (SQLPOINTER)&sUserMode,
                                0, 
                                NULL ) 
                   != SQL_SUCCESS, execute_error);

    IDE_TEST( Execute( sDblinkStmt ) != SQL_SUCCESS );

    while( ( sRet = SQLFetch( sDblinkStmt ) ) != SQL_NO_DATA )
    {
        IDE_TEST_RAISE( sRet != SQL_SUCCESS, execute_error );

        // When OwnerNameInd is -1, sOwnerName is NULL.
        // It means that this database link is created with public mode.
        // Therefore, That 'sys' user create public database links.
        // Although some links have been be created by not 'sys' user,
        // database links that does not have ownership should be created by 'sys'.
        if( sOwnerNameInd == SQL_NULL_DATA )
        {
            idlOS::strcpy( sOwnerName, (SChar*)UTM_STR_SYS );
        }

        if (idlOS::strcasecmp( sOwnerName, sPrevName ) != 0 )
        {
            IDE_TEST( getPasswd( sOwnerName, sPasswd ) != SQL_SUCCESS );

            idlOS::fprintf( aLinkFp,
                            "connect \"%s\" / \"%s\"\n",
                            sOwnerName, 
                            sPasswd );

            idlOS::strcpy( sPrevName, sOwnerName );        
        }

        if ( gProgOption.mbExistDrop == ID_TRUE )
        {
            idlOS::fprintf( aLinkFp, "drop %s database link \"%s\";\n",
                            (sUserMode == 0) ? "public" : "private",
                            sLinkName );
        }

        /* BUG-47159 Using DBMS_METADATA package in aexport */
        if( sOwnerNameInd == SQL_NULL_DATA )
        {
            IDE_TEST(gMeta->getDdl(DDL, UTM_OBJ_TYPE_DB_LINK,
                     sLinkName, (SChar *)"PUBLIC") != IDE_SUCCESS);
        }
        else
        {
            IDE_TEST(gMeta->getDdl(DDL, UTM_OBJ_TYPE_DB_LINK,
                     sLinkName, sOwnerName) != IDE_SUCCESS);
        }
#ifdef DEBUG
        idlOS::fprintf( stderr, "%s\n", gMeta->getDdlStr() );
#endif
        idlOS::fprintf( aLinkFp, "%s\n\n", gMeta->getDdlStr() );

        idlOS::fflush( aLinkFp );
    }

    sState = 0;
    FreeStmt( &sDblinkStmt );

    return SQL_SUCCESS;

    IDE_EXCEPTION( alloc_error )
    {
        utmSetErrorMsgWithHandle( SQL_HANDLE_DBC, (SQLHANDLE)m_hdbc );
    }
    IDE_EXCEPTION( execute_error )
    {
        utmSetErrorMsgWithHandle( SQL_HANDLE_STMT, (SQLHANDLE)sDblinkStmt );
    }
    IDE_EXCEPTION_END;

    if( sState == 1 )
    {
        FreeStmt( &sDblinkStmt );
    }

    return SQL_ERROR;
#undef IDE_FN
}
