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
 * $Id: utAtbLob.cpp 82687 2018-04-03 05:47:24Z bethy $
 ******************************************************************************/

#include <utAtb.h>
#include <sqlcli.h>

IDE_RC utAtbLob::initialize(utAtbQuery *aQuery,
                            SInt        aSqlType,
                            SQLUBIGINT  aLobLoc)
{
    mQuery = aQuery;
    mOffset = 0;
    mTotalLen = 0;
    mCurrLen = 0;
    mLobLoc = aLobLoc;

    if(aSqlType == SQL_BLOB)
    {
        mLocatorCType = SQL_C_BLOB_LOCATOR;
        mSourceCType = SQL_C_BINARY;
    }
    else if(aSqlType == SQL_CLOB)
    {
        mLocatorCType = SQL_C_CLOB_LOCATOR;
        mSourceCType = SQL_C_CHAR;
    }

    if (mLobLoc == 0)
    {
        IDE_CONT(RET_POS);
    }

    /* LOBLength for GET */
    IDE_TEST(SQLGetLobLength(mQuery->_stmt,
                mLobLoc,
                mLocatorCType,
                &mTotalLen) != SQL_SUCCESS);

    IDE_EXCEPTION_CONT(RET_POS);

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    mQuery->printError();

    return IDE_FAILURE;
}

IDE_RC utAtbLob::finalize( )
{
    if (mLobLoc != 0)
    {
        SQLFreeLob(mQuery->_stmt, mLobLoc);
    }
    return IDE_SUCCESS;
}

IDE_RC utAtbLob::next(SQLUINTEGER *aLen)
{
    SQLUINTEGER sForLen;

    sForLen = (mTotalLen - mOffset <= BUF_LEN) ?
              (mTotalLen - mOffset) : BUF_LEN;

    IDE_TEST(SQLGetLob(mQuery->_stmt,
                mLocatorCType,
                mLobLoc,
                mOffset,
                sForLen,
                mSourceCType,
                mBuf,
                BUF_LEN,
                &mCurrLen) != SQL_SUCCESS);

    mOffset += mCurrLen;
    *aLen = mCurrLen;

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    mQuery->printError();

    return IDE_FAILURE;
}

bool utAtbLob::equals(utAtbLob *aLob)
{
    if(mCurrLen != aLob->getCurrLen())
    {
        return false;
    }
    if (idlOS::memcmp(mBuf, aLob->getValue(), mCurrLen) != 0)
    {
        return false;
    }
    return true;
}


