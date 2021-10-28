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
 * $Id: utmDbmsMeta.cpp 91918 2021-10-28 02:52:21Z chkim $
 **********************************************************************/

#include <utmDbmsMeta.h>

#define GET_DDL_QUERY       "exec ? := dbms_metadata.get_ddl(?, ?, ?)"
#define GET_DEP_DDL_QUERY   "exec ? := dbms_metadata.get_dependent_ddl(?, ?, ?)"
#define GET_USER_DDL_QUERY  "exec ? := dbms_metadata.get_user_ddl(?, ?)"
#define GET_STATS_DDL_QUERY "exec ? := dbms_metadata.get_stats_ddl(?, ?)"

#define SET_TRANSFORM_PARAM_QUERY                                      \
    "begin "                                                           \
    "  dbms_metadata.set_transform_param('SQLTERMINATOR', 'T'); "      \
    "  dbms_metadata.set_transform_param('CONSTRAINTS', 'F'); "        \
    "  dbms_metadata.set_transform_param('REF_CONSTRAINTS', 'F'); "    \
    "  dbms_metadata.set_transform_param('NOTNULL_CONSTRAINTS', 'F'); "\
    "  dbms_metadata.set_transform_param('ACCESS_MODE', 'F'); "        \
    "  dbms_metadata.set_transform_param('SCHEMA', 'F'); "             \
    "end"

#define SET_EXPORT_DB_MODE_QUERY                                       \
    "begin "                                                           \
    "  dbms_metadata.set_transform_param('EXPORT_DB_MODE', 'T'); "     \
    "end"

extern uteErrorMgr    gErrorMgr;

utmDbmsMeta::utmDbmsMeta()
{
    mStmt = SQL_NULL_HSTMT;
    mStmt4User = SQL_NULL_HSTMT;
    mStmt4Dep = SQL_NULL_HSTMT;
    mStmt4Stats = SQL_NULL_HSTMT;
}

utmDbmsMeta::~utmDbmsMeta()
{
}

IDE_RC utmDbmsMeta::initialize(SQLHDBC aDbc)
{
    IDE_TEST(existsDbmsMetadata(aDbc) != IDE_SUCCESS);

    IDE_TEST(init4Ddl(aDbc) != IDE_SUCCESS);
    IDE_TEST(init4UserDdl(aDbc) != IDE_SUCCESS);
    IDE_TEST(init4DepDdl(aDbc) != IDE_SUCCESS);
    IDE_TEST(init4StatsDdl(aDbc) != IDE_SUCCESS);

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC utmDbmsMeta::existsDbmsMetadata(SQLHDBC aDbc)
{
    SInt     sCnt = 0;
    SQLHSTMT sStmt = SQL_NULL_HSTMT;

    SChar *sQry = (SChar *)"select count(*) "             \
                  "from system_.sys_packages_ "           \
                  "where package_name = 'DBMS_METADATA' " \
                  "and package_type in (6,7) "            \
                  "and status = 0 ";

    IDE_TEST_RAISE(SQLAllocStmt(aDbc, &sStmt) != SQL_SUCCESS,
                   alloc_error);

    IDE_TEST_RAISE(SQLExecDirect(sStmt, (SQLCHAR*)sQry, SQL_NTS) != SQL_SUCCESS,
                   stmt_error);

    IDE_TEST_RAISE(
            SQLBindCol(sStmt, 1, SQL_C_SLONG, &sCnt, 0, NULL)
            != SQL_SUCCESS, stmt_error);

    IDE_TEST_RAISE(SQLFetch(sStmt) == SQL_ERROR, stmt_error);

    IDE_TEST_RAISE(sCnt != 2, dbms_metadata_error);

    FreeStmt(&sStmt);

    return IDE_SUCCESS;

    IDE_EXCEPTION(alloc_error);
    {
        utmSetErrorMsgWithHandle(SQL_HANDLE_DBC, (SQLHANDLE)aDbc);
    }
    IDE_EXCEPTION(stmt_error);
    {
        utmSetErrorMsgWithHandle(SQL_HANDLE_STMT, (SQLHANDLE)sStmt);
    }
    IDE_EXCEPTION(dbms_metadata_error);
    {
        uteSetErrorCode(&gErrorMgr, utERR_ABORT_DbmsMetadata_No_Exist_Error);
    }
    IDE_EXCEPTION_END;

    if (sStmt != SQL_NULL_HSTMT)
    {
        FreeStmt(&sStmt);
        sStmt = SQL_NULL_HSTMT;
    }
    return IDE_FAILURE;
}

IDE_RC utmDbmsMeta::init4Ddl(SQLHDBC aDbc)
{
    int sParamIdx = 1;

    IDE_TEST_RAISE(SQLAllocStmt(aDbc, &mStmt) != SQL_SUCCESS,
                   alloc_error);

    IDE_TEST_RAISE(SQLExecDirect(mStmt,
                (SQLCHAR*)SET_TRANSFORM_PARAM_QUERY,
                SQL_NTS) != SQL_SUCCESS,
                   stmt_error);

    IDE_TEST(Prepare((SChar*)GET_DDL_QUERY, mStmt) != SQL_SUCCESS);

    IDE_TEST_RAISE(
            SQLBindParameter(mStmt, sParamIdx++, SQL_PARAM_OUTPUT,
                SQL_C_CHAR, SQL_VARCHAR, 65534, 0,
                mDdl, sizeof(mDdl), &mDdlInd)
            != SQL_SUCCESS, stmt_error);

    IDE_TEST_RAISE(
            SQLBindParameter(mStmt, sParamIdx++, SQL_PARAM_INPUT,
                SQL_C_CHAR, SQL_VARCHAR, 20, 0,
                mObjType, sizeof(mObjType), NULL)
            != SQL_SUCCESS, stmt_error);

    IDE_TEST_RAISE(
            SQLBindParameter(mStmt, sParamIdx++, SQL_PARAM_INPUT,
                SQL_C_CHAR, SQL_VARCHAR, 128, 0,
                mBindParam1, sizeof(mBindParam1), NULL)
            != SQL_SUCCESS, stmt_error);

    IDE_TEST_RAISE(
            SQLBindParameter(mStmt, sParamIdx++, SQL_PARAM_INPUT,
                SQL_C_CHAR, SQL_VARCHAR, 128, 0,
                mBindParam2, sizeof(mBindParam2), &mBindParam2Ind)
            != SQL_SUCCESS, stmt_error);

    /*
    IDE_TEST_RAISE(
            SQLBindCol(mStmt, 1,
                SQL_C_CHAR, mDdl, sizeof(mDdl), &mDdlInd)
            != SQL_SUCCESS, stmt_error);
            */

    return IDE_SUCCESS;

    IDE_EXCEPTION(alloc_error);
    {
        utmSetErrorMsgWithHandle(SQL_HANDLE_DBC, (SQLHANDLE)aDbc);
    }
    IDE_EXCEPTION(stmt_error);
    {
        utmSetErrorMsgWithHandle(SQL_HANDLE_STMT, (SQLHANDLE)mStmt);
    }
    IDE_EXCEPTION_END;

    if (mStmt != SQL_NULL_HSTMT)
    {
        FreeStmt(&mStmt);
        mStmt = SQL_NULL_HSTMT;
    }
    return IDE_FAILURE;
}

IDE_RC utmDbmsMeta::init4UserDdl(SQLHDBC aDbc)
{
    int sParamIdx = 1;

    IDE_TEST_RAISE(SQLAllocStmt(aDbc, &mStmt4User) != SQL_SUCCESS,
                   alloc_error);

    IDE_TEST(Prepare((SChar*)GET_USER_DDL_QUERY, mStmt4User) != SQL_SUCCESS);

    IDE_TEST_RAISE(
            SQLBindParameter(mStmt4User, sParamIdx++, SQL_PARAM_OUTPUT,
                SQL_C_CHAR, SQL_VARCHAR, 65534, 0,
                mDdl, sizeof(mDdl), &mDdlInd)
            != SQL_SUCCESS, stmt_error);

    IDE_TEST_RAISE(
            SQLBindParameter(mStmt4User, sParamIdx++, SQL_PARAM_INPUT,
                SQL_C_CHAR, SQL_VARCHAR, 128, 0,
                mBindParam1, sizeof(mBindParam1), NULL)
            != SQL_SUCCESS, stmt_error);

    IDE_TEST_RAISE(
            SQLBindParameter(mStmt4User, sParamIdx++, SQL_PARAM_INPUT,
                SQL_C_CHAR, SQL_VARCHAR, 128, 0,
                mBindParam2, sizeof(mBindParam2), &mBindParam2Ind)
            != SQL_SUCCESS, stmt_error);

    /*
    IDE_TEST_RAISE(
            SQLBindCol(mStmt4User, 1,
                SQL_C_CHAR, mDdl, sizeof(mDdl), &mDdlInd)
            != SQL_SUCCESS, stmt_error);
            */

    return IDE_SUCCESS;

    IDE_EXCEPTION(alloc_error);
    {
        utmSetErrorMsgWithHandle(SQL_HANDLE_DBC, (SQLHANDLE)aDbc);
    }
    IDE_EXCEPTION(stmt_error);
    {
        utmSetErrorMsgWithHandle(SQL_HANDLE_STMT, (SQLHANDLE)mStmt4User);
    }
    IDE_EXCEPTION_END;

    if (mStmt4User != SQL_NULL_HSTMT)
    {
        FreeStmt(&mStmt4User);
        mStmt4User = SQL_NULL_HSTMT;
    }
    return IDE_FAILURE;
}

IDE_RC utmDbmsMeta::init4DepDdl(SQLHDBC aDbc)
{
    int sParamIdx = 1;

    IDE_TEST_RAISE(SQLAllocStmt(aDbc, &mStmt4Dep) != SQL_SUCCESS,
                   alloc_error);

    IDE_TEST(Prepare((SChar*)GET_DEP_DDL_QUERY, mStmt4Dep) != SQL_SUCCESS);

    IDE_TEST_RAISE(
            SQLBindParameter(mStmt4Dep, sParamIdx++, SQL_PARAM_OUTPUT,
                SQL_C_CHAR, SQL_VARCHAR, 65534, 0,
                mDdl, sizeof(mDdl), &mDdlInd)
            != SQL_SUCCESS, stmt_error);

    IDE_TEST_RAISE(
            SQLBindParameter(mStmt4Dep, sParamIdx++, SQL_PARAM_INPUT,
                SQL_C_CHAR, SQL_VARCHAR, 20, 0,
                mObjType, sizeof(mObjType), NULL)
            != SQL_SUCCESS, stmt_error);

    IDE_TEST_RAISE(
            SQLBindParameter(mStmt4Dep, sParamIdx++, SQL_PARAM_INPUT,
                SQL_C_CHAR, SQL_VARCHAR, 128, 0,
                mBindParam1, sizeof(mBindParam1), NULL)
            != SQL_SUCCESS, stmt_error);

    IDE_TEST_RAISE(
            SQLBindParameter(mStmt4Dep, sParamIdx++, SQL_PARAM_INPUT,
                SQL_C_CHAR, SQL_VARCHAR, 128, 0,
                mBindParam2, sizeof(mBindParam2), &mBindParam2Ind)
            != SQL_SUCCESS, stmt_error);

    /*
    IDE_TEST_RAISE(
            SQLBindCol(mStmt4Dep, 1,
                SQL_C_CHAR, mDdl, sizeof(mDdl), &mDdlInd)
            != SQL_SUCCESS, stmt_error);
            */

    return IDE_SUCCESS;

    IDE_EXCEPTION(alloc_error);
    {
        utmSetErrorMsgWithHandle(SQL_HANDLE_DBC, (SQLHANDLE)aDbc);
    }
    IDE_EXCEPTION(stmt_error);
    {
        utmSetErrorMsgWithHandle(SQL_HANDLE_STMT, (SQLHANDLE)mStmt4Dep);
    }
    IDE_EXCEPTION_END;

    if (mStmt4Dep != SQL_NULL_HSTMT)
    {
        FreeStmt(&mStmt4Dep);
        mStmt4Dep = SQL_NULL_HSTMT;
    }
    return IDE_FAILURE;
}

IDE_RC utmDbmsMeta::init4StatsDdl(SQLHDBC aDbc)
{
    int sParamIdx = 1;

    IDE_TEST_RAISE(SQLAllocStmt(aDbc, &mStmt4Stats) != SQL_SUCCESS,
                   alloc_error);

    IDE_TEST(Prepare((SChar*)GET_STATS_DDL_QUERY, mStmt4Stats) != SQL_SUCCESS);

    IDE_TEST_RAISE(
            SQLBindParameter(mStmt4Stats, sParamIdx++, SQL_PARAM_OUTPUT,
                SQL_C_CHAR, SQL_VARCHAR, 65534, 0,
                mDdl, sizeof(mDdl), &mDdlInd)
            != SQL_SUCCESS, stmt_error);

    IDE_TEST_RAISE(
            SQLBindParameter(mStmt4Stats, sParamIdx++, SQL_PARAM_INPUT,
                SQL_C_CHAR, SQL_VARCHAR, 128, 0,
                mBindParam1, sizeof(mBindParam1), NULL)
            != SQL_SUCCESS, stmt_error);

    IDE_TEST_RAISE(
            SQLBindParameter(mStmt4Stats, sParamIdx++, SQL_PARAM_INPUT,
                SQL_C_CHAR, SQL_VARCHAR, 128, 0,
                mBindParam2, sizeof(mBindParam2), &mBindParam2Ind)
            != SQL_SUCCESS, stmt_error);

    /*
    IDE_TEST_RAISE(
            SQLBindCol(mStmt4Stats, 1,
                SQL_C_CHAR, mDdl, sizeof(mDdl), &mDdlInd)
            != SQL_SUCCESS, stmt_error);
            */

    return IDE_SUCCESS;

    IDE_EXCEPTION(alloc_error);
    {
        utmSetErrorMsgWithHandle(SQL_HANDLE_DBC, (SQLHANDLE)aDbc);
    }
    IDE_EXCEPTION(stmt_error);
    {
        utmSetErrorMsgWithHandle(SQL_HANDLE_STMT, (SQLHANDLE)mStmt4Stats);
    }
    IDE_EXCEPTION_END;

    if (mStmt4Stats != SQL_NULL_HSTMT)
    {
        FreeStmt(&mStmt4Stats);
        mStmt4Stats = SQL_NULL_HSTMT;
    }
    return IDE_FAILURE;
}

IDE_RC utmDbmsMeta::finalize()
{
    if (mStmt != SQL_NULL_HSTMT)
    {
        SQLFreeHandle(SQL_HANDLE_STMT, (SQLHANDLE)mStmt);
        mStmt = SQL_NULL_HSTMT;
    }
    if (mStmt4User != SQL_NULL_HSTMT)
    {
        SQLFreeHandle(SQL_HANDLE_STMT, (SQLHANDLE)mStmt4User);
        mStmt4User = SQL_NULL_HSTMT;
    }
    if (mStmt4Dep != SQL_NULL_HSTMT)
    {
        SQLFreeHandle(SQL_HANDLE_STMT, (SQLHANDLE)mStmt4Dep);
        mStmt4Dep = SQL_NULL_HSTMT;
    }
    return IDE_SUCCESS;
}

IDE_RC utmDbmsMeta::getDdl(
                       utmDdlType      aDdlType,
                       const SChar    *aObjType,
                       SChar      *aObjName,
                       SChar      *aSchema)
{
    SQLHSTMT sTmpStmt = SQL_NULL_HSTMT;

    if (aDdlType == DDL)
    {
        sTmpStmt = mStmt;
    }
    else if (aDdlType == DEPENDENT_DDL)
    {
        sTmpStmt = mStmt4Dep;
    }
    else if (aDdlType == STATS_DDL)
    {
        sTmpStmt = mStmt4Stats;
    }

    idlOS::strcpy(mObjType, aObjType);
    idlOS::strcpy(mBindParam1, aObjName);
    if (aSchema != NULL)
    {
        idlOS::strcpy(mBindParam2, aSchema);
        mBindParam2Ind = SQL_NTS;
    }
    else
    {
        mBindParam2Ind = SQL_NULL_DATA;
    }

    mDdl[0] = '\0';

    IDE_TEST(Execute(sTmpStmt) != SQL_SUCCESS);

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    if (utmGetErrorCODE() == 0xF1B3D) // NO DATA
    {
        return IDE_SUCCESS;
    }
    return IDE_FAILURE;
}

IDE_RC utmDbmsMeta::getUserDdl(
                       SChar      *aUserName,
                       SChar      *aUserPasswd)
{
    idlOS::strcpy(mBindParam1, aUserName);
    if (aUserPasswd == NULL)
    {
        mBindParam2Ind = SQL_NULL_DATA;
    }
    else
    {
        idlOS::strcpy(mBindParam2, aUserPasswd);
        mBindParam2Ind = SQL_NTS;
    }

    mDdl[0] = '\0';

    IDE_TEST(Execute(mStmt4User) != SQL_SUCCESS);

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    if (utmGetErrorCODE() == 0xF1B3D) // NO DATA
    {
        return IDE_SUCCESS;
    }
    return IDE_FAILURE;
}

/* BUG-49356 Cross-schema reference index */
IDE_RC utmDbmsMeta::setExportDbMode( SQLHDBC aDbc )
{
    SQLHSTMT sStmt  = SQL_NULL_HSTMT;

    IDE_TEST_RAISE( SQLAllocStmt( aDbc, &sStmt ) != SQL_SUCCESS,
                   allocError );

    IDE_TEST_RAISE( SQLExecDirect( sStmt, 
                    (SQLCHAR*)SET_EXPORT_DB_MODE_QUERY, 
                    SQL_NTS ) != SQL_SUCCESS,
                    stmtError );

    FreeStmt( &sStmt );

    return IDE_SUCCESS;

    IDE_EXCEPTION( allocError );
    {
        utmSetErrorMsgWithHandle( SQL_HANDLE_DBC, ( SQLHANDLE ) aDbc );
    }
    IDE_EXCEPTION( stmtError );
    {
        utmSetErrorMsgWithHandle( SQL_HANDLE_STMT, ( SQLHANDLE ) sStmt );
    }
    IDE_EXCEPTION_END;

    if ( sStmt != SQL_NULL_HSTMT )
    {
        FreeStmt( &sStmt );
        sStmt = SQL_NULL_HSTMT;
    }

    return IDE_FAILURE;
}
