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

#include <utISPApi.h>
#include <isqlTypes.h>

extern iSQLProperty         gProperty;

isqlClob::isqlClob()
{
    mCType = SQL_C_BLOB_LOCATOR;
}

IDE_RC isqlClob::initBuffer()
{
    mValueSize = 0;
    mValue = NULL;

    return IDE_SUCCESS;
}

IDE_RC isqlClob::InitLobBuffer( SInt aSize )
{
    /* Clob은 Fetch할 때마다 SQLGetLobLength,SQLGetLob으로
     * 데이터를 가져오므로 필요한 버퍼 크기가 매번 달라질 수 있다.
     * aSize가 현재 버퍼 크기보다 클 때만 할당하도록 한다.
     */
    if ( aSize > mValueSize )
    {
        freeMem();
        mValueSize = aSize;

        mValue = (SChar *) idlOS::malloc(mValueSize);
        IDE_TEST(mValue == NULL);
    }
    else
    {
        /* Nothing to do */
    }

    return IDE_SUCCESS;

    IDE_EXCEPTION_END;

    return IDE_FAILURE;
}

void isqlClob::initDisplaySize()
{
    mDisplaySize = CLOB_DISPLAY_SIZE;
}

idBool isqlClob::IsNull()
{
    if ( mInd == SQL_NULL_DATA )
    {
        return ID_TRUE;
    }
    else
    {
        return ID_FALSE;
    }
}

void isqlClob::SetNull()
{
    mInd = SQL_NULL_DATA;
    mValue[0] = '\0';

    mCurr = mValue;
    mCurrLen = 0;
}

void isqlClob::Reformat()
{
    /*
     * BUG-49014
     *   이 함수는 아래와 같이 utISPApi::Fetch에서 호출된다.
     *     iSQLExecuteCommand::FetchSelectStmt
     *       utISPApi::Fetch
     *         isqlClob::Reformat
     *       iSQLExecuteCommand::printRow
     *         utISPApi::GetLobData
     *           isqlClob::InitLobBuffer
     *   다른 타입과 달리 isqlClob은 이 시점에 mValue를 할당전이므로
     *   여기서 할 것이 없다.
     */
    return;
}

void isqlClob::SetLobValue()
{
    mValue[mInd] = '\0';
    mCurr = mValue;
    mCurrLen = mInd;
}

