/*
 * Copyright (c) 1999~2017, Altibase Corp. and/or its affiliates. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License, version 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

package Altibase.jdbc.driver.cm;

import Altibase.jdbc.driver.sharding.core.ShardTransactionLevel;

public class CmProtocolContextShardStmt extends CmProtocolContext
{
    private CmProtocolContextShardConnect mShardContextConnect;
    private CmShardAnalyzeResult          mShardAnalyzeResult;
    private CmPrepareResult               mShardPrepareResult;

    /* BUG-46513 Node 제거 이후에 getUpdateCount()를 수행해도 결과를 감소시키지 않기 위해
       row count를 Statement shard context에 보관한다.  */
    private int                           mUpdateRowcount;

    public CmProtocolContextShardStmt(CmProtocolContextShardConnect aShardContextConnect)
    {
        mShardAnalyzeResult = new CmShardAnalyzeResult();
        mShardPrepareResult = new CmPrepareResult();
        mShardContextConnect = aShardContextConnect;
    }

    public CmShardAnalyzeResult getShardAnalyzeResult()
    {
        return mShardAnalyzeResult;
    }

    public CmPrepareResult getShardPrepareResult()
    {
        return mShardPrepareResult;
    }

    public CmProtocolContextShardConnect getShardContextConnect()
    {
        return mShardContextConnect;
    }

    public ShardTransactionLevel getShardTransactionLevel()
    {
        return mShardContextConnect.getShardTransactionLevel();
    }

    public boolean isAutoCommitMode()
    {
        return mShardContextConnect.isAutoCommitMode();
    }

    public int getUpdateRowcount()
    {
        return mUpdateRowcount;
    }

    public void setUpdateRowcount(int aUpdateRowcount)
    {
        mUpdateRowcount = aUpdateRowcount;
    }
}
