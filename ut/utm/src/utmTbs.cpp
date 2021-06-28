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

#include <smDef.h>
#include <utm.h>
#include <utmExtern.h>
#include <utmDbmsMeta.h>

#define GET_TBSID_QUERY                                              \
    "select A.id, A.type, A.name "                                   \
    "from x$tablespaces_header A "                                   \
    "where A.type between ? and ? "                                  \
    "and A.state_bitset != ? "                                       \
    "order by 1"

#define GET_TBS_4_USER                                               \
    "SELECT NAME, type "                                             \
    " FROM V$TABLESPACES "                                           \
    " WHERE ID = (SELECT TEMP_TBS_ID "                               \
    "             FROM SYSTEM_.SYS_USERS_ WHERE USER_NAME = ?) "     \
    "UNION ALL "                                                     \
    "SELECT NAME, type "                                             \
    " FROM V$TABLESPACES "                                           \
    " WHERE ID = (SELECT DEFAULT_TBS_ID "                            \
    "             FROM SYSTEM_.SYS_USERS_ WHERE USER_NAME = ?) "     \
    "UNION ALL "                                                     \
    "SELECT B.NAME, b.type "                                         \
    " FROM SYSTEM_.SYS_TBS_USERS_ A, V$TABLESPACES B "               \
    " WHERE A.TBS_ID = B.ID AND A.USER_ID = ("                       \
    "   SELECT USER_ID FROM SYSTEM_.SYS_USERS_ WHERE USER_NAME = ?)"

SQLRETURN getTBSQuery( FILE *aTbsFp )
{
#define IDE_FN "getTBSQuery()"
    SQLHSTMT  s_tbs_stmt = SQL_NULL_HSTMT;
    SQLRETURN sRet=0;
    SChar     sQuery[QUERY_LEN];
    /* fix for BUG-12847 */
    SChar    *sDdl = NULL;
    SInt      s_tbs_id=-1;
    SInt      s_tbs_type=-1;
    SChar     s_tbs_name[40+1];
    idBool    sFirstFlag = ID_TRUE;

    SInt sMtbs  = SMI_MEMORY_USER_DATA;
    SInt sVtbs  = SMI_VOLATILE_USER_DATA;
    SInt sFileD = SMI_FILE_DROPPED;

    idlOS::fprintf(stdout, "\n##### TBS #####\n");

    /* fix BUG-23229 확장된 undo,temp,system tablespace 에 대한 스크립트 누락
     * 다음의 tablespace 정보를 갖고 옴
     * s_tbs_type = X$TABLESPACES.TYPE
     *
     * smiDef.h
     *     SMI_MEMORY_SYSTEM_DICTIONARY   0 - system dictionary table space
     *     SMI_MEMORY_SYSTEM_DATA         1 - memory db system table space
     *     SMI_MEMORY_USER_DATA           2 - memory db user table space
     *     SMI_DISK_SYSTEM_DATA           3 - disk db system  table space
     *     SMI_DISK_USER_DATA             4 - disk db user table space
     *     SMI_DISK_SYSTEM_TEMP           5 - disk db sys temporary table space
     *     SMI_DISK_USER_TEMP             6 - disk db user temporary table space
     *     SMI_DISK_SYSTEM_UNDO           7 - disk db undo table space
     *     SMI_VOLATILE_USER_DATA         8
     *     SMI_TABLESPACE_TYPE_MAX        9 - for function array
     */

    /* BUG-23979 x$tablespaces_header 가 삭제된 테이블스페이스 정보까지 가지고 있음 */
    
    IDE_TEST_RAISE(SQLAllocStmt(m_hdbc, &s_tbs_stmt) 
                   != SQL_SUCCESS,alloc_error );

    idlOS::sprintf(sQuery, GET_TBSID_QUERY);
    
    IDE_TEST(Prepare(sQuery, s_tbs_stmt) != SQL_SUCCESS);

    IDE_TEST_RAISE(
    SQLBindParameter(s_tbs_stmt, 1, SQL_PARAM_INPUT,
                     SQL_C_SLONG, SQL_INTEGER,4,0,
                     &sMtbs, sizeof(sMtbs), NULL)
                     != SQL_SUCCESS, tbs_error);
    
    IDE_TEST_RAISE(
    SQLBindParameter(s_tbs_stmt, 2, SQL_PARAM_INPUT,
                     SQL_C_SLONG, SQL_INTEGER,4,0,
                     &sVtbs, sizeof(sVtbs), NULL)
                     != SQL_SUCCESS, tbs_error);

    IDE_TEST_RAISE(
    SQLBindParameter(s_tbs_stmt, 3, SQL_PARAM_INPUT,
                     SQL_C_SLONG, SQL_INTEGER,4,0,
                     &sFileD, sizeof(sFileD), NULL)
                     != SQL_SUCCESS, tbs_error);

    IDE_TEST_RAISE(
        SQLBindCol(s_tbs_stmt, 1, SQL_C_SLONG, (SQLPOINTER)&s_tbs_id, 0, NULL)
        != SQL_SUCCESS, tbs_error);        
    IDE_TEST_RAISE(
        SQLBindCol(s_tbs_stmt, 2, SQL_C_SLONG, (SQLPOINTER)&s_tbs_type, 0, NULL)
        != SQL_SUCCESS, tbs_error);    
    IDE_TEST_RAISE(
        SQLBindCol(s_tbs_stmt, 3, SQL_C_CHAR, (SQLPOINTER)s_tbs_name, sizeof(s_tbs_name), NULL)
        != SQL_SUCCESS, tbs_error);    

    IDE_TEST(Execute(s_tbs_stmt) != SQL_SUCCESS);

    while ((sRet = SQLFetch(s_tbs_stmt)) != SQL_NO_DATA)
    {
        IDE_TEST_RAISE( sRet != SQL_SUCCESS, tbs_error );

        switch( s_tbs_type )
        {
            case SMI_MEMORY_USER_DATA:
            case SMI_DISK_USER_DATA:
            case SMI_DISK_USER_TEMP:
            case SMI_VOLATILE_USER_DATA:
                if ( gProgOption.mbExistDrop == ID_TRUE )
                {
                    idlOS::fprintf(aTbsFp,
                        "\nDROP TABLESPACE \"%s\" INCLUDING CONTENTS AND DATAFILES; \n",
                        s_tbs_name);
                }
                /* BUG-47159 Using DBMS_METADATA package in aexport */
                IDE_TEST(gMeta->getDdl(DDL, UTM_OBJ_TYPE_TABLESPACE, s_tbs_name, NULL)
                         != IDE_SUCCESS);
                break;
            case SMI_DISK_SYSTEM_DATA:
            case SMI_DISK_SYSTEM_TEMP:
            case SMI_DISK_SYSTEM_UNDO:
                /* BUG-47159 Using DBMS_METADATA package in aexport */
                /* fix BUG-23229 확장된 undo,temp,system tablespace 에 대한 스크립트 누락
                 * ALTER TABLESPACE 구문 생성
                 */
                IDE_TEST(gMeta->getDdl(DDL, UTM_OBJ_TYPE_TABLESPACE, s_tbs_name, NULL)
                         != IDE_SUCCESS);
                break;
            default :
                IDE_ASSERT(0);
        }
        
        /* BUG-47159 Using DBMS_METADATA package in aexport */
        sDdl = gMeta->getDdlStr();
        if ( sFirstFlag == ID_TRUE && idlOS::strcmp( sDdl, "\0" ) != 0 )
        {
            idlOS::fprintf( aTbsFp,
                            "connect \"%s\" / \"%s\"\n\n",
                            gProgOption.GetUserNameInSQL(),
                            gProgOption.GetPassword() );
            sFirstFlag = ID_FALSE; 
        }
        else
        {
            /* do nothing */
        }
        
#ifdef DEBUG
        idlOS::fprintf( stderr, "%s\n\n", sDdl );
#endif
        idlOS::fprintf( aTbsFp, "%s\n\n", sDdl );
    } // end while

    // BUG-33995 aexport have wrong free handle code
    FreeStmt( &s_tbs_stmt );

    return SQL_SUCCESS;
    
    IDE_EXCEPTION(alloc_error);
    {
        utmSetErrorMsgWithHandle(SQL_HANDLE_DBC, (SQLHANDLE)m_hdbc);
    }
    IDE_EXCEPTION(tbs_error);
    {
        utmSetErrorMsgWithHandle(SQL_HANDLE_STMT, (SQLHANDLE)s_tbs_stmt);
    }
    IDE_EXCEPTION_END;

    // BUG-33995 aexport have wrong free handle code
    if ( s_tbs_stmt != SQL_NULL_HSTMT )
    {
        FreeStmt( &s_tbs_stmt );
    }

    return SQL_ERROR;
#undef IDE_FN
}

SQLRETURN getTBSInfo4UserMode( FILE  *aTbsFp,
                               SChar *aUserName )
{
#define IDE_FN "getTBSInfo4UserMode()"
    SQLHSTMT  s_tbs_stmt = SQL_NULL_HSTMT;
    SQLRETURN sRet=0;

    SChar       sTbsName[STR_LEN];
    SInt        sTbsType     = -1;
    SQLLEN      sTbsInd            = 0;

    SChar     sQuery[QUERY_LEN];
    SChar    *s_file_query = NULL;

    IDE_TEST_RAISE(SQLAllocStmt(m_hdbc, &s_tbs_stmt) 
                   != SQL_SUCCESS,alloc_error );

    idlOS::sprintf(sQuery, GET_TBS_4_USER);
    
    IDE_TEST(Prepare(sQuery, s_tbs_stmt) != SQL_SUCCESS);

    IDE_TEST_RAISE(
    SQLBindParameter(s_tbs_stmt, 1, SQL_PARAM_INPUT,
                     SQL_C_CHAR, SQL_VARCHAR,UTM_NAME_LEN,0,
                     aUserName, UTM_NAME_LEN+1, NULL)
                     != SQL_SUCCESS, tbs_error);
    
    IDE_TEST_RAISE(
    SQLBindParameter(s_tbs_stmt, 2, SQL_PARAM_INPUT,
                     SQL_C_CHAR, SQL_VARCHAR,UTM_NAME_LEN,0,
                     aUserName, UTM_NAME_LEN+1, NULL)
                     != SQL_SUCCESS, tbs_error);

    IDE_TEST_RAISE(
    SQLBindParameter(s_tbs_stmt, 3, SQL_PARAM_INPUT,
                     SQL_C_CHAR, SQL_VARCHAR,UTM_NAME_LEN,0,
                     aUserName, UTM_NAME_LEN+1, NULL)
                     != SQL_SUCCESS, tbs_error);

    IDE_TEST_RAISE(
    SQLBindCol(s_tbs_stmt, 1, SQL_C_CHAR, (SQLPOINTER)sTbsName,
               (SQLLEN)ID_SIZEOF(sTbsName), &sTbsInd)
               != SQL_SUCCESS, tbs_error);                   
    
    IDE_TEST_RAISE(
        SQLBindCol(s_tbs_stmt, 2, SQL_C_SLONG, (SQLPOINTER)&sTbsType,
                   0, NULL) != SQL_SUCCESS, tbs_error);    

    IDE_TEST(Execute(s_tbs_stmt) != SQL_SUCCESS);

    while ((sRet = SQLFetch(s_tbs_stmt)) != SQL_NO_DATA)
    {
        IDE_TEST_RAISE( sRet != SQL_SUCCESS, tbs_error );

       switch(sTbsType)
       {
       case SMI_MEMORY_SYSTEM_DATA:
           return SQL_SUCCESS;
           break;
       case SMI_MEMORY_USER_DATA:
       case SMI_DISK_USER_DATA:
       case SMI_DISK_USER_TEMP:
       case SMI_VOLATILE_USER_DATA:
           if ( gProgOption.mbExistDrop == ID_TRUE )
           {
               idlOS::fprintf(aTbsFp,
                   "\nDROP TABLESPACE \"%s\" INCLUDING CONTENTS AND DATAFILES; \n",
                   sTbsName);
           }
       case SMI_DISK_SYSTEM_DATA:
       case SMI_DISK_SYSTEM_TEMP:
       case SMI_DISK_SYSTEM_UNDO:
           /* BUG-47159 Using DBMS_METADATA package in aexport */
           IDE_TEST(gMeta->getDdl(DDL, UTM_OBJ_TYPE_TABLESPACE, sTbsName, NULL) != IDE_SUCCESS);
           s_file_query = gMeta->getDdlStr();
           if( idlOS::strcmp( s_file_query, "\0" ) != 0 )
           {
               idlOS::fprintf( aTbsFp, "%s\n\n", s_file_query );
           }
           break;
       default :
           IDE_ASSERT(0);
       }
    }

    FreeStmt( &s_tbs_stmt );

    idlOS::fflush(aTbsFp);

    return SQL_SUCCESS;
    
    IDE_EXCEPTION(alloc_error);
    {
        utmSetErrorMsgWithHandle(SQL_HANDLE_DBC, (SQLHANDLE)m_hdbc);
    }
    IDE_EXCEPTION(tbs_error);
    {
        utmSetErrorMsgWithHandle(SQL_HANDLE_STMT, (SQLHANDLE)s_tbs_stmt);
    }
    IDE_EXCEPTION_END;

    idlOS::fflush(aTbsFp);

    if ( s_tbs_stmt != SQL_NULL_HSTMT )
    {
        // BUG-33995 aexport have wrong free handle code
        FreeStmt( &s_tbs_stmt );
    }
    return SQL_ERROR;
#undef IDE_FN
}

