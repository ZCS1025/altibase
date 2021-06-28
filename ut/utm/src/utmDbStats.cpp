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
 * $Id: utmGatheringObjStats.cpp $
 **********************************************************************/
#include <utm.h>
#include <utmExtern.h>
#include <utmDbStats.h>

#define GET_SYSTEM_STATS_QUERY                              \
    "EXEC GET_SYSTEM_STATS(?, ?)"

/* BUG-40174 Support export and import DBMS Stats */
SQLRETURN getSystemStats( FILE  *a_DbStatsFp )
{
    SQLHSTMT  sStmt             = SQL_NULL_HSTMT;
    double    sSystemStatsValue = 0;
    SQLLEN    sSystemStatsValueInd;
    // MAX LENGTH SYSTEM PARAMETER IS MREAD_PAGE_COUNT (LENGTH 16)
    SChar     sParameters[][20] = // 16 + alpha
    { 
        "SREAD_TIME", "MREAD_TIME", "MREAD_PAGE_COUNT", "HASH_TIME", "COMPARE_TIME", 
        "STORE_TIME" 
    };

    SChar     sParameter[20] = "";
    idBool    sIsSysStat = ID_FALSE;

    IDE_TEST_RAISE(SQLAllocStmt(m_hdbc, &sStmt) != SQL_SUCCESS,
                   alloc_error);

#ifdef DEBUG
    idlOS::fprintf(stderr, "%s\n", m_collectDbStatsStr);
#endif

    IDE_TEST(Prepare((SChar *)GET_SYSTEM_STATS_QUERY, sStmt) != SQL_SUCCESS);

    IDE_TEST_RAISE(
            SQLBindParameter(sStmt, 1, SQL_PARAM_INPUT,
                SQL_C_CHAR, SQL_VARCHAR, UTM_NAME_LEN, 0,
                sParameter, UTM_NAME_LEN+1, NULL)
            != SQL_SUCCESS, stmt_error);

    IDE_TEST_RAISE(
            SQLBindParameter(sStmt, 2, SQL_PARAM_OUTPUT,
                SQL_C_DOUBLE, SQL_NUMERIC, 38, 16,
                &sSystemStatsValue, 0, &sSystemStatsValueInd)
            != SQL_SUCCESS, stmt_error);

    for(UInt i = 0; i < ID_SIZEOF(sParameters)/ID_SIZEOF(sParameters[0]); i++)
    {
        idlOS::sprintf(sParameter, sParameters[i]);
        IDE_TEST(Execute(sStmt) != SQL_SUCCESS );

        if ( sSystemStatsValueInd != SQL_NULL_DATA ) /* BUG-44813 */
        {
            sIsSysStat = ID_TRUE;
            idlOS::snprintf(m_collectDbStatsStr,
                            ID_SIZEOF(m_collectDbStatsStr),
                            "EXEC SET_SYSTEM_STATS('%s', %f);",
                            sParameters[i], sSystemStatsValue);

            idlOS::fprintf(a_DbStatsFp, "%s\n", m_collectDbStatsStr);
        }
        else
        {
            idlOS::fprintf(a_DbStatsFp,
                           "-- %s has never been gathered.\n",
                           sParameters[i]);
        }
    }

    /* BUG-44813 */
    if ( sIsSysStat == ID_FALSE )
    {
        idlOS::fprintf(a_DbStatsFp,
                       "/* System statistics have never been gathered. */\n");
    }

    // BUG-33995 aexport have wrong free handle code
    FreeStmt( &sStmt );

    return SQL_SUCCESS;

    IDE_EXCEPTION(alloc_error);
    {
        utmSetErrorMsgWithHandle(SQL_HANDLE_DBC, (SQLHANDLE)m_hdbc);
    }
    IDE_EXCEPTION(stmt_error);
    {
        utmSetErrorMsgWithHandle(SQL_HANDLE_STMT, (SQLHANDLE)sStmt);
    }
    IDE_EXCEPTION_END;

    if ( sStmt != SQL_NULL_HSTMT )
    {
        // BUG-33995 aexport have wrong free handle code
        FreeStmt( &sStmt );
    }

    return SQL_ERROR;
}

/* BUG-40174 Support export and import DBMS Stats */
SQLRETURN getTableStats( SChar *a_user,
                         SChar *a_table,
                         FILE  *a_DbStatsFp )
{
    /* BUG-47159 Using DBMS_METADATA package in aexport */
    IDE_TEST(gMeta->getDdl(STATS_DDL, UTM_OBJ_TYPE_TABLE,
                a_table, a_user) != IDE_SUCCESS);

    idlOS::fprintf(a_DbStatsFp, "%s\n", gMeta->getDdlStr());

    return SQL_SUCCESS;

    IDE_EXCEPTION_END;

    return SQL_ERROR;
}

