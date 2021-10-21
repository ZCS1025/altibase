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
 
/*******************************************************************************
 * $Id: utAtbQuery.cpp 91790 2021-10-05 01:14:45Z chkim $
 ******************************************************************************/

#include <uto.h>
#include <utAtb.h>

IDE_RC  utAtbQuery::prepare()
{
    //IDE_TEST(mSQL[0] == '\0');
    if(mIsPrepared == ID_TRUE)
    {
        return IDE_SUCCESS;
    }

    IDE_TEST(reset() != IDE_SUCCESS);
    IDE_TEST_RAISE(SQL_ERROR == SQLPrepare(_stmt, (SQLCHAR *)mSQL, SQL_NTS),
                   SQLPREPARE_ERR);

    mIsPrepared = ID_TRUE;

    return IDE_SUCCESS;

    IDE_EXCEPTION(SQLPREPARE_ERR)
    {
        mErrNo            = SQL_ERROR;
        _conn->error_stmt = _stmt;
        _conn->error(_stmt);
    }
    IDE_EXCEPTION_END;

    mIsPrepared = ID_FALSE;

    return IDE_FAILURE;
}


Row * utAtbQuery::fetch(dba_t, bool aFileMode)
{
    SInt  ret = SQL_INVALID_HANDLE;

    /* BUG-32569 The string with null character should be processed in Audit */
    SInt  sI;
    UInt  sValueLength;
    Field *sField;

    IDE_TEST(mRow == NULL);
    IDE_TEST(mRow->reset() != IDE_SUCCESS);
    ret = SQLFetch(_stmt);

    /* BUG-32569 The string with null character should be processed in Audit */
    for( sI = 1, sField = mRow->getField(1); sField != NULL; sField = mRow->getField(++sI) )
    {
        sValueLength = (UInt)(*((utAtbField *)sField)->getValueInd());
        sField->setValueLength(sValueLength);
    }

    IDE_TEST(!SQL_SUCCEEDED(ret));

    /* TASK-4212: audit툴의 대용량 처리시 개선 */
    if ( aFileMode == true )
    {
        // do nothing
    }
    else
    {
        ++_rows;
    }

    return mRow;

    IDE_EXCEPTION_END;

    /* BUG-39193 'Unable to retrieve error information from CLI driver' is written
     * as a error message instead of exact one in log file. */
    if (ret != SQL_NO_DATA_FOUND)
    {
        mErrNo            = SQL_ERROR;
        _conn->error_stmt = _stmt;
        _conn->error(_stmt);
    }

    return NULL;
}

IDE_RC utAtbQuery::initialize(UInt)
{
    IDE_TEST(SQLAllocStmt(_conn->dbchp, &_stmt) != SQL_SUCCESS);

    mIsCursorOpened = ID_FALSE;
    // BUG-40205 insure++ warning 어떤 값으로 초기값 설정???
    lobCompareMode  = ID_FALSE;

    return Query::initialize();

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC utAtbQuery::close()
{
    //mSQL[0] = '\0';
    if(mIsCursorOpened == ID_TRUE || mIsPrepared == ID_TRUE)
    {
        IDE_TEST(SQLFreeStmt(_stmt, SQL_CLOSE) == SQL_ERROR);
        IDE_TEST(SQLFreeStmt(_stmt, SQL_UNBIND) == SQL_ERROR);
        IDE_TEST(SQLFreeStmt(_stmt, SQL_RESET_PARAMS) == SQL_ERROR);
        mIsPrepared = ID_FALSE;
        mIsCursorOpened = ID_FALSE;
    }

    return reset();

    IDE_EXCEPTION_END;

    reset();

    return IDE_FAILURE;
}


/* TASK-4212: audit툴의 대용량 처리시 개선 */
IDE_RC utAtbQuery::utaCloseCur(void)
{
/***********************************************************************
 *
 * Description :
 *    statement를 해제함. SQL_CLOSE, SQL_UNBIND, SQL_RESET_PARAMS 해준다.
 *
 ***********************************************************************/

    if(mIsCursorOpened == ID_TRUE || mIsPrepared == ID_TRUE)
    {
        IDE_TEST(SQLFreeStmt(_stmt, SQL_CLOSE)  == SQL_ERROR);
        IDE_TEST(SQLFreeStmt(_stmt, SQL_UNBIND) == SQL_ERROR);
        IDE_TEST(SQLFreeStmt(_stmt, SQL_RESET_PARAMS) == SQL_ERROR);
        mIsPrepared     = ID_FALSE;
        mIsCursorOpened = ID_FALSE;
    }

    return IDE_SUCCESS;
    IDE_EXCEPTION_END;
    return IDE_FAILURE;
}


IDE_RC utAtbQuery::clear()
{
    mSQL[0] = '\0';
    if(mIsPrepared == ID_TRUE)
    {
        IDE_TEST(SQLFreeStmt(_stmt, SQL_DROP) == SQL_ERROR);
        mIsPrepared = ID_FALSE;
    }

    return reset();

    IDE_EXCEPTION_END;

    reset();

    return IDE_FAILURE;
}


IDE_RC utAtbQuery::execute(bool)
{
    if(mIsPrepared != ID_TRUE)
    {
        IDE_TEST(prepare() != IDE_SUCCESS);
    }

    /* Execute process */
    IDE_TEST(SQLExecute(_stmt) == SQL_ERROR);

    if(_rescols == 0)
    {
        IDE_TEST(SQL_ERROR == SQLNumResultCols(_stmt,(SQLSMALLINT *)&_rescols));

        if(_rescols == 0)
        {
            _rescols = -1; //set NO RES ROWS
            _cols    =  0;
        }
        else
        {
            _cols = _rescols;
            if(mRow == NULL)
            {
                mRow = new utAtbRow(this,mErrNo);
                IDE_TEST(mRow == NULL);
                IDE_TEST(mRow->initialize() != IDE_SUCCESS);
            }
        }
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    if(mErrNo == SQL_SUCCESS)
    {
        mErrNo            = SQL_ERROR;
        _conn->error_stmt = _stmt;
        _conn->error(_stmt);
    }

    return IDE_FAILURE;
}

/*
 * BUG-45909 Improve LOB Processing
 *     It has been moved from execute()
 */
IDE_RC utAtbQuery::close4DML()
{
    SQLLEN sRowCount = 0;

    // BUG-18732 : close for DML
    IDE_TEST(SQLRowCount(_stmt, &sRowCount) == SQL_ERROR);
    if(sRowCount != -1) // PROJ-2396, 2370
    {
        IDE_TEST(close() != IDE_SUCCESS);
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    if(mErrNo == SQL_SUCCESS)
    {
        mErrNo            = SQL_ERROR;
        _conn->error_stmt = _stmt;
        _conn->error(_stmt);
    }

    return IDE_FAILURE;
}


IDE_RC utAtbQuery::execute(const SChar * aSQL, ...)
{
    va_list args;

    IDE_TEST(aSQL == NULL);
    IDE_TEST(close() != IDE_SUCCESS);

    va_start(args, aSQL);
#if defined(DEC_TRU64) || defined(VC_WIN32)
    IDE_TEST(vsprintf(mSQL, aSQL, args) < 0);
#else
    IDE_TEST(vsnprintf(mSQL, sizeof(mSQL)-1, aSQL, args) < 0);
#endif
    va_end(args);

    IDE_TEST(SQLExecDirect(_stmt, (SQLCHAR *)mSQL, SQL_NTS) == SQL_ERROR);

    IDE_TEST(SQL_ERROR == SQLNumResultCols (_stmt,(SQLSMALLINT *)&_rescols));

    if(_rescols)
    {
        _cols = _rescols;
        if(mRow == NULL)
        {
            mRow = new utAtbRow(this,mErrNo);
            IDE_TEST(mRow == NULL);
            IDE_TEST(mRow->initialize() != IDE_SUCCESS);
        }
    }
    if(mIsCursorOpened == ID_FALSE)
    {
        mIsCursorOpened = ID_TRUE;
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    mErrNo            = SQL_ERROR;
    _conn->error_stmt = _stmt;
    _conn->error(_stmt);

    return IDE_FAILURE;
}

/* BUG-45909 Improve LOB Processing */
void utAtbQuery::printError()
{
    _conn->setErrNo(SQL_ERROR);
    uteSetErrorCode(&gErrorMgr,
                    utERR_ABORT_AUDIT_DB_Error,
                    _conn->error(_stmt));
    utePrintfErrorCode(stderr, &gErrorMgr);
}

SQLUBIGINT utAtbQuery::getBindLobLoc(UShort aPos)
{
    binds_t    * sBind;
    SQLUBIGINT   sLobLoc = 0;

    for(sBind = _binds; sBind; sBind = sBind->next)
    {
        if (sBind->mPos == aPos)
        {
            sLobLoc = sBind->mLobLoc;
            break;
        }
    }

    return sLobLoc;
}

IDE_RC utAtbQuery::putLob(UShort aPos, Field *aField)
{
    SQLUINTEGER  sLen;
    SQLUINTEGER  sLobLen;
    SQLUINTEGER  sOffset;
    SQLSMALLINT  locatorCType;
    SQLSMALLINT  sourceCType;
    SQLUBIGINT   sLobLoc;
    utAtbLob    *sGetLob = NULL;
    utAtbField  *sGetF = (utAtbField*)aField;

    sGetF->initLob();

    sGetLob = sGetF->getLob();
    sLobLen = sGetLob->getLobLength();

    if(aField->getSQLType() == SQL_BLOB)
    {
        locatorCType = SQL_C_BLOB_LOCATOR;
        sourceCType = SQL_C_BINARY;
    }
    else // SQL_CLOB
    {
        locatorCType = SQL_C_CLOB_LOCATOR;
        sourceCType = SQL_C_CHAR;
    }
    sOffset = 0;
    sLobLoc = getBindLobLoc(aPos);
    IDE_TEST_RAISE(sLobLoc == 0, noLoc_err);

    while(sLobLen - sOffset > 0)
    {
        IDE_TEST(sGetLob->next(&sLen) != IDE_SUCCESS);

        if (sLen == 0) break;

        /* PUT LOB */
        IDE_TEST_RAISE(SQLPutLob(_stmt,
                           locatorCType,
                           sLobLoc,
                           sOffset,
                           0,
                           sourceCType,
                           sGetLob->getValue(),
                           sLen)
                 != SQL_SUCCESS, putLob_err);

        sOffset += sLen;
    }
    
    SQLFreeLob(_stmt, sLobLoc);

    return IDE_SUCCESS;

    IDE_EXCEPTION(noLoc_err);
    {
        /* cannot be here */
        idlOS::fprintf(stderr,
                "This column(%s) has not been bound.\n",
                aField->getName());
    }
    IDE_EXCEPTION(putLob_err);
    {
        mErrNo            = SQL_ERROR;
        _conn->error_stmt = _stmt;
        _conn->error(_stmt);
    }
    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

IDE_RC utAtbQuery::lobAtToAt(Query * aGetLob,
                             Query * aPutLob,
                             SChar * aTableNameA,
                             SChar * aTableNameB)
{
    UInt         i, lobColCount, fromPosition;
    SInt         tempCount, diff; 
    SChar        selectSql[QUERY_BUFSIZE], temp[BUF_LEN];
    SChar       *lobColName, *colName;
    metaColumns *sTCM;
    Field       *f;
    
    SQLHSTMT     getLobStmt = NULL;
    SQLHSTMT     putLobStmt = NULL;
    SQLHDBC      getConn;
    SQLHDBC      putConn;

    SQLPOINTER   buf[BUF_LEN], compareBuf[BUF_LEN];
    SQLUINTEGER  getForLength, putForLength;

    SInt         sSqlType[MAX_COL_CNT];
    SInt         locatorCType[MAX_COL_CNT];
    SQLSMALLINT  sourceCType[MAX_COL_CNT];
    SQLUBIGINT   getLobLoc[MAX_COL_CNT] = {0, };
    SQLUBIGINT   putLobLoc[MAX_COL_CNT] = {0, };
    SQLUINTEGER  getLobLength[MAX_COL_CNT], putLobLength[MAX_COL_CNT];
    SQLINTEGER   ALobLength[MAX_COL_CNT], BLobLength[MAX_COL_CNT];
    
    /* Get Meta Info */
    if(!(sTCM = aGetLob->getConn()->getTCM()))
    {
        sTCM = aPutLob->getConn()->getTCM();
    }
    
    if(sTCM == NULL)
    {
        return IDE_FAILURE;
    }

    lobColCount = sTCM->getCLSize(true);

    /**********************************************/
    /* GETLOB PREPARE                             */
    /**********************************************/

    /* Select Query */
    idlOS::strcpy(selectSql, "SELECT ");

    for(i = 1; i <= lobColCount; i ++)
    {
        idlOS::strcat(selectSql, sTCM->getCL(i, true));
        if(i != lobColCount)
        {
            idlOS::strcat(selectSql, ", ");
        }
    }
    strcat(selectSql, " FROM ");
    strcat(selectSql, aTableNameA);
    strcat(selectSql, " WHERE ");

    for(i = sTCM->getPKSize(); sTCM->getPK(i); --i)
    {
        idlOS::strcat(selectSql, sTCM->getPK(i));
        idlOS::strcat(selectSql," = ");
        
        IDE_ASSERT(aGetLob->getRow()->getField(i)->getSChar(temp, BUF_LEN) != -1);
        idlOS::strcat(selectSql, temp);
        if(i > 1)
        {
            idlOS::strcat(selectSql," AND ");
        }
    }
    
    if (aGetLob->lobCompareMode == false)
    {
        strcat(selectSql, " FOR UPDATE");
    }
    
    /* Fetch LOB */
    getConn = aGetLob->getConn()->getDbchp();

    IDE_TEST_RAISE(SQLAllocStmt(getConn, &getLobStmt) != SQL_SUCCESS,
                   AllocStmt_Err);

    IDE_TEST_RAISE(SQLExecDirect(getLobStmt, (SQLCHAR*)selectSql, SQL_NTS)
             != SQL_SUCCESS, getLobStmt_Err);

    for(tempCount = 0; tempCount < (SInt)lobColCount; tempCount ++)
    {
        sSqlType[tempCount] = 0;
        lobColName = sTCM->getCL(tempCount + 1, true);

        for(i = 1, f = aGetLob->getRow()->getField(1);
            f; f = aGetLob->getRow()->getField(++i))
        {
            colName = f->getName();
            if(idlOS::strcmp(lobColName, colName) == 0)
            {
                sSqlType[tempCount] = f->getSQLType();
                break;
            }
        }
        if(sSqlType[tempCount] == SQL_BLOB)
        {
            locatorCType[tempCount] = SQL_C_BLOB_LOCATOR;
            sourceCType[tempCount] = SQL_C_BINARY;
        }
        else if(sSqlType[tempCount] == SQL_CLOB)
        {
            locatorCType[tempCount] = SQL_C_CLOB_LOCATOR;
            sourceCType[tempCount] = SQL_C_CHAR;
        }
        else
        {
            /* cannot be here */
            idlOS::fprintf(stderr,
                    "The type of % column is not LOB.\n",
                    lobColName);
            return IDE_FAILURE;
        }
        IDE_TEST_RAISE(SQLBindCol(getLobStmt,
                            tempCount + 1,
                            locatorCType[tempCount],
                            &getLobLoc[tempCount],
                            0,
                            NULL)
                 != SQL_SUCCESS, getLobStmt_Err);
    }

    IDE_TEST_RAISE (SQLFetch(getLobStmt) != SQL_SUCCESS, getLobStmt_Err);
    
    /* LOBLength for GET */

    for(tempCount = 0; tempCount < (SInt)lobColCount; tempCount ++)
    {
        /* BUG-30301 */
        IDE_TEST_RAISE(SQLGetLobLength(getLobStmt,
                                 getLobLoc[tempCount],
                                 locatorCType[tempCount],
                                 &getLobLength[tempCount])
                       != SQL_SUCCESS, getLobStmt_Err);
    }

    /**********************************************/
    /* PUTLOB PREPARE                             */
    /**********************************************/

    /* LobLocator */

    /* Select Query */
    idlOS::strcpy(selectSql, "SELECT ");

    for(i = 1; i <= lobColCount; i ++)
    {
        idlOS::strcat(selectSql, sTCM->getCL(i, true));
        if(i != lobColCount)
        {
            idlOS::strcat(selectSql, ", ");
        }
    }
    strcat(selectSql, " FROM ");
    strcat(selectSql, aTableNameB);
    strcat(selectSql, " WHERE ");

    for(i = sTCM->getPKSize(); sTCM->getPK(i); --i)
    {
        idlOS::strcat(selectSql, sTCM->getPK(i));
        idlOS::strcat(selectSql," = ");
        
        /* 
         * BUG-32566
         *
         * Query가 잘못되어 LOB Column Select 안되는 문제 수정
         * 디버깅 중 실수로 주석처리 하지 않았을까????
         */
        IDE_ASSERT(aGetLob->getRow()->getField(i)->getSChar(temp, BUF_LEN) != -1);
        idlOS::strcat(selectSql, temp);
        if(i > 1)
        {
            idlOS::strcat(selectSql," AND ");
        }
    }
    
    if (aGetLob->lobCompareMode == false)
    {
        strcat(selectSql, " FOR UPDATE");
    }

    /* Fetch LOB */
    /* BUG-49274 Activate commit count */
    if ( aGetLob->lobCompareMode )
    {
        putConn = aPutLob->getConn()->getDbchp();
    }
    else
    {
        putConn = this->getConn()->getDbchp();
    }

    IDE_TEST_RAISE(SQLAllocStmt(putConn, &putLobStmt) != SQL_SUCCESS,
                   AllocStmt_Err);

    IDE_TEST_RAISE(SQLExecDirect(putLobStmt, (SQLCHAR*)selectSql, SQL_NTS)
                   != SQL_SUCCESS, putLobStmt_Err);

    for(tempCount = 0; tempCount < (SInt)lobColCount; tempCount ++)
    {
        lobColName = sTCM->getCL(tempCount + 1, true);
        for(i = 1, f = aPutLob->getRow()->getField(1); f;
            f = aPutLob->getRow()->getField(++i))
        {
            colName = f->getName();
            if(idlOS::strcmp(lobColName, colName) == 0)
            {
                sSqlType[tempCount] = f->getSQLType();
                break;
            }
        }
        if(sSqlType[tempCount] == SQL_BLOB)
        {
            locatorCType[tempCount] = SQL_C_BLOB_LOCATOR;
            sourceCType[tempCount] = SQL_C_BINARY;
        }
        else if(sSqlType[tempCount] == SQL_CLOB)
        {
            locatorCType[tempCount] = SQL_C_CLOB_LOCATOR;
            sourceCType[tempCount] = SQL_C_CHAR;
        }
        else
        {
            /* cannot be here */
            idlOS::fprintf(stderr,
                    "The type of % column is not LOB.\n",
                    lobColName);
            return IDE_FAILURE;
        }
        IDE_TEST_RAISE(SQLBindCol(putLobStmt,
                            tempCount + 1,
                            locatorCType[tempCount],
                            &putLobLoc[tempCount],
                            0,
                            NULL)
                       != SQL_SUCCESS, putLobStmt_Err);
    }

    IDE_TEST_RAISE (SQLFetch(putLobStmt) != SQL_SUCCESS, putLobStmt_Err);

    /* LOBLength for PUT */
    for(tempCount = 0; tempCount < (SInt)lobColCount; tempCount ++)
    {
        IDE_TEST_RAISE(SQLGetLobLength(putLobStmt,
                    putLobLoc[tempCount],
                    locatorCType[tempCount],
                    &putLobLength[tempCount])
                != SQL_SUCCESS, putLobStmt_Err);
    }

    //Lob Compare Mode
    if (aGetLob->lobCompareMode)
    {
        for(tempCount = 0; tempCount < (SInt)lobColCount; tempCount ++)
        {
            fromPosition = 0;
            getForLength = 0;
            if(getLobLength[tempCount] != putLobLength[tempCount])
            {
                aGetLob->setLobDiffCol(sTCM->getCL(tempCount + 1, true));
                aPutLob->setLobDiffCol(sTCM->getCL(tempCount + 1, true));
                break;
            }
            ALobLength[tempCount] = getLobLength[tempCount];
            BLobLength[tempCount] = putLobLength[tempCount];

            while((ALobLength[tempCount] > 0) && (BLobLength[tempCount] > 0))
            {
                idlOS::memset(buf, 0x00, sizeof(buf));
                idlOS::memset(compareBuf, 0x00, sizeof(compareBuf));

                getForLength = (ALobLength[tempCount] <= BUF_LEN)
                    ? ALobLength[tempCount] : BUF_LEN;

                putForLength = (BLobLength[tempCount] <= BUF_LEN)
                    ? BLobLength[tempCount] : BUF_LEN;

                /* GET LOB A */
                IDE_TEST_RAISE(SQLGetLob(getLobStmt,
                                   locatorCType[tempCount],
                                   getLobLoc[tempCount],
                                   fromPosition,
                                   getForLength,
                                   sourceCType[tempCount],
                                   buf,
                                   BUF_LEN,
                                   &getForLength)
                         != SQL_SUCCESS, getLobStmt_Err);
                /* GET LOB B */
                IDE_TEST_RAISE(SQLGetLob(putLobStmt,
                                   locatorCType[tempCount],
                                   putLobLoc[tempCount],
                                   fromPosition,
                                   putForLength,
                                   sourceCType[tempCount],
                                   compareBuf,
                                   BUF_LEN,
                                   &putForLength)
                         != SQL_SUCCESS, putLobStmt_Err);

                diff = idlOS::memcmp(buf,compareBuf, BUF_LEN);
                if(diff != 0)
                {
                    aGetLob->setLobDiffCol(sTCM->getCL(tempCount + 1, true));
                    aPutLob->setLobDiffCol(sTCM->getCL(tempCount + 1, true));
                    break;
                }
                fromPosition += getForLength;
                ALobLength[tempCount] -= getForLength;
                BLobLength[tempCount] -= putForLength;
            }
        }
    }
    //Lob Update Mode
    else
    {
        /* Initialize target */
        idlOS::memset(buf, 0x00, sizeof(buf));
        for(tempCount = 0; tempCount < (SInt)lobColCount; tempCount ++)
        {
            IDE_TEST_RAISE(SQLPutLob(putLobStmt,
                               locatorCType[tempCount],
                               putLobLoc[tempCount],
                               0,
                               putLobLength[tempCount],
                               sourceCType[tempCount],
                               buf,
                               0)
                     != SQL_SUCCESS, putLobStmt_Err);
        }

        /**********************************************/
        /* GETLOB   &   PUTLOB                        */
        /**********************************************/

        for(tempCount = 0; tempCount < (SInt)lobColCount; tempCount ++)
        {
            fromPosition = 0;
            getForLength = 0;

            while(getLobLength[tempCount] > 0)
            {
                getForLength = (getLobLength[tempCount] <= BUF_LEN)
                    ? getLobLength[tempCount] : BUF_LEN;

                putForLength = (putLobLength[tempCount] <= BUF_LEN)
                    ? putLobLength[tempCount] : BUF_LEN;

                /* GET LOB */
                IDE_TEST_RAISE(SQLGetLob(getLobStmt,
                                   locatorCType[tempCount],
                                   getLobLoc[tempCount],
                                   fromPosition,
                                   getForLength,
                                   sourceCType[tempCount],
                                   buf,
                                   BUF_LEN,
                                   &getForLength)
                         != SQL_SUCCESS, getLobStmt_Err);
                /* PUT LOB */
                IDE_TEST_RAISE(SQLPutLob(putLobStmt,
                                   locatorCType[tempCount],
                                   putLobLoc[tempCount],
                                   fromPosition,
                                   0,
                                   sourceCType[tempCount],
                                   buf,
                                   getForLength)
                         != SQL_SUCCESS, putLobStmt_Err);

                fromPosition += getForLength;
                getLobLength[tempCount] -= getForLength;
            }
        }
    }
    
    for(tempCount = 0; tempCount < (SInt)lobColCount; tempCount ++)
    {
        SQLFreeLob(getLobStmt, getLobLoc[tempCount]);
        SQLFreeLob(putLobStmt, putLobLoc[tempCount]);
    }
    IDE_TEST(SQLFreeStmt(getLobStmt, SQL_DROP) != SQL_SUCCESS);
    IDE_TEST(SQLFreeStmt(putLobStmt, SQL_DROP) != SQL_SUCCESS);

    return IDE_SUCCESS;

    IDE_EXCEPTION(AllocStmt_Err);
    {
        uteSetErrorCode(&gErrorMgr,
                        utERR_ABORT_AUDIT_Alloc_Handle_Error,
                        "SQLAllocStmt");
        utePrintfErrorCode(stderr, &gErrorMgr);
    }
    IDE_EXCEPTION(getLobStmt_Err);
    {
        aGetLob->getConn()->setErrNo(SQL_ERROR);
        uteSetErrorCode(&gErrorMgr,
                        utERR_ABORT_AUDIT_DB_Error,
                        aGetLob->getConn()->error(getLobStmt));
        utePrintfErrorCode(stderr, &gErrorMgr);
    }
    IDE_EXCEPTION(putLobStmt_Err);
    {
        aPutLob->getConn()->setErrNo(SQL_ERROR);
        uteSetErrorCode(&gErrorMgr,
                        utERR_ABORT_AUDIT_DB_Error,
                        aPutLob->getConn()->error(putLobStmt));
        utePrintfErrorCode(stderr, &gErrorMgr);
    }
    IDE_EXCEPTION_END;

    for(tempCount = 0; tempCount < (SInt)lobColCount; tempCount ++)
    {
        if (getLobLoc[tempCount] != 0)
        {
            SQLFreeLob(getLobStmt, getLobLoc[tempCount]);
        }
        if (putLobLoc[tempCount] != 0)
        {
            SQLFreeLob(putLobStmt, putLobLoc[tempCount]);
        }
    }
    if (getLobStmt != NULL)
    {
        SQLFreeStmt(getLobStmt, SQL_DROP);
    }
    if (putLobStmt != NULL)
    {
        SQLFreeStmt(putLobStmt, SQL_DROP);
    }

    return IDE_FAILURE;
}


IDE_RC utAtbQuery::reset()
{
    binds_t * binds;
    _rescols = 0;

    IDE_TEST(_stmt == NULL);

    for(binds = _binds; binds; binds = _binds)
    {
        _binds = binds->next;
        idlOS::free(binds);
    }

    //*** Delete Row ***/
    if(mRow)
    {
        IDE_TEST(mRow->finalize() != IDE_SUCCESS);
        delete mRow;
        mRow = NULL;
    }
    _rows = 0;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

/* BUG-32569 The string with null character should be processed in Audit */
IDE_RC utAtbQuery::bind(const UInt i, void *buf, UInt bufLen, SInt sqlType, bool isNull, UInt aValueLength)
{
    SQLSMALLINT sCType = sqlTypeToAltibase(sqlType);

    if(sqlType == SQL_TYPE_TIMESTAMP)
    {
        bufLen = sizeof(SQL_TIMESTAMP_STRUCT);
    }

    /* BUG-32569 The string with null character should be processed in Audit */
    return bind(i, buf, bufLen, sqlType, isNull, SQL_PARAM_INPUT, sCType, aValueLength);
}

/* BUG-32569 The string with null character should be processed in Audit */
IDE_RC utAtbQuery::bind(const UInt aPosition, void *aBuff,UInt aWidth, SInt sqlType,
                        bool isNull ,SQLSMALLINT pType ,SQLSMALLINT sCType, UInt aValueLength)
{
    SInt locatorType = 0;
    SInt locatorCType = 0;
    binds_t * sBinds   = NULL;
    
    if(mIsPrepared != ID_TRUE)
    {
        IDE_TEST(prepare() != IDE_SUCCESS);
    }

    sBinds = (binds_t*)idlOS::calloc(1,sizeof(binds_t));

    IDE_TEST(sBinds == NULL);
    sBinds->value         = aBuff  ;
    sBinds->pType         = pType  ;
    sBinds->sqlType       = sqlType;
    sBinds->scale         =  0     ;
    sBinds->columnSize    =  aWidth; // disable aWidth
    sBinds->mPos          =  aPosition;
    sBinds->mLobLoc       =  0     ;

    switch(sCType)
    {
        /* BUG-32569 The string with null character should be processed in Audit */
        case SQL_C_CHAR: sBinds->valueLength = aValueLength; break;
        default: sBinds->valueLength =  (SQLLEN)aWidth; break;
    }
    if (isNull)
    {
        sBinds->valueLength = SQL_NULL_DATA;
    }

    if(sqlType == SQL_BLOB || sqlType == SQL_CLOB)
    {
        if(sqlType == SQL_BLOB)
        {
            locatorType = SQL_BLOB_LOCATOR;
            locatorCType = SQL_C_BLOB_LOCATOR;        
        }
        else
        {
            locatorType = SQL_CLOB_LOCATOR;
            locatorCType = SQL_C_CLOB_LOCATOR;        
        }
        /* BUG-40205 insure++ warning
         * LOB의 경우 SQL_PARAM_OUTPUT으로 설정하여 locator를 받아온 후,
         * SQLPubLob에서 데이터를 입력해야 함: manual 참조
         * 여기에서 SQLBindParameter에 사용한 locator를 실제로는 사용하지 않으며,
         * utAtbQuery::lobAtToAt 함수에서 lob 칼럼을 별도로 처리하고 있음.
         */
        pType = SQL_PARAM_OUTPUT;
        mLobInd = 0;

        mErrNo = SQLBindParameter(_stmt,
                                  (SQLUSMALLINT)aPosition,
                                  (SQLSMALLINT) pType, //TODO get from statement ????
                                  (SQLSMALLINT) locatorCType, // C type parametr        ???
                                  (SQLSMALLINT) locatorType, // SQL TYPE
                                  0, // column size
                                  0, // Scale
                                  &(sBinds->mLobLoc), // BUG-45909
                                  0,
                                  &mLobInd); 
    }
    else
    {
        /* BUG-45958 Need to support BIT/VARBIT type */
        if(sqlType == SQL_BIT || sqlType == SQL_VARBIT)
        {
            sBinds->columnSize = ((bit_t *)aBuff)->mPrecision;
            sBinds->valueLength = aValueLength;
        }
        mErrNo = SQLBindParameter(_stmt,
                                  (SQLUSMALLINT)aPosition,
                                  (SQLSMALLINT) pType, //TODO get from statement ????
                                  (SQLSMALLINT) sCType, // C type parametr        ???
                                  (SQLSMALLINT) sqlType, // SQL TYPE
                                  (SQLULEN) sBinds->columnSize, // column size
                                  (SQLSMALLINT) sBinds->scale, // Scale
                                  (SQLPOINTER)  aBuff,
                                  (SQLLEN)  aWidth,
                                  &sBinds->valueLength);
    }

    IDE_TEST(mErrNo == SQL_ERROR);

    sBinds->next  =   _binds;
    _binds        =   sBinds;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    if(sBinds != NULL)
    {
        idlOS::free(sBinds);
    }

    return IDE_FAILURE;
}


IDE_RC utAtbQuery::finalize()
{
    if (_stmt)
    {
        IDE_TEST(reset() != IDE_SUCCESS);
        IDE_TEST(SQLFreeStmt(_stmt, SQL_DROP) == SQL_ERROR);
        _stmt = NULL;
    }

    return Query::finalize();

    IDE_EXCEPTION_END;

    _stmt = NULL;

    return IDE_FAILURE;
}


utAtbQuery::utAtbQuery(utAtbConnection * conn) : Query(conn)
{
    _conn     = conn;
    _stmt     = NULL;
    _binds    = NULL;
}
