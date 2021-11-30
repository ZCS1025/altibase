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

import Altibase.jdbc.driver.AltibaseStatement;
import Altibase.jdbc.driver.sharding.core.AltibaseShardingConnection;
import Altibase.jdbc.driver.sharding.core.GlobalTransactionLevel;

public class CmProtocolContextShardStmt extends CmProtocolContext
{
    /* TASK-7219 Non-shard DML
     * sdiShardPartialCoordType과 동일
     */
    public enum ShardPartialExecType
    {
        SHARD_PARTIAL_EXEC_TYPE_NONE,
        SHARD_PARTIAL_EXEC_TYPE_COORD,
        SHARD_PARTIAL_EXEC_TYPE_QUERY
    }

    private AltibaseShardingConnection    mMetaConn;
    private CmShardAnalyzeResult          mShardAnalyzeResult;
    private CmPrepareResult               mShardPrepareResult;

    /* BUG-46513 Node 제거 이후에 getUpdateCount()를 수행해도 결과를 감소시키지 않기 위해
       row count를 Statement shard context에 보관한다.  */
    private int                           mUpdateRowcount;
    private ShardPartialExecType          mShardPartialExecType;    // TASK-7219 Non-shard DML

    public CmProtocolContextShardStmt(AltibaseShardingConnection aMetaConn, AltibaseStatement aStmt)
    {
        mShardPrepareResult = new CmPrepareResult();
        initialize(aMetaConn, aStmt);
    }

    public CmProtocolContextShardStmt(AltibaseShardingConnection aMetaConn, AltibaseStatement aStmt,
                                      CmPrepareResult aPrepareResult)
    {
        mShardPrepareResult = aPrepareResult;
        initialize(aMetaConn, aStmt);
    }
    
    private void initialize(AltibaseShardingConnection aMetaConn, AltibaseStatement aStmt)
    {
        mShardAnalyzeResult = new CmShardAnalyzeResult();
        aStmt.setPrepareResult(mShardPrepareResult); // BUG-47274 Analyze용으로 생성한 statement에 CmPrepareResult객체를 주입한다.
        /* BUG-46790 failover시 shard context객체가 새로 생성되기때문에 context객체를 가지고 있는
           AltibaseShardingConnection 객체를 주입한다. */
        mMetaConn = aMetaConn;
        mShardPartialExecType = ShardPartialExecType.SHARD_PARTIAL_EXEC_TYPE_NONE;
    }

    public CmShardAnalyzeResult getShardAnalyzeResult()
    {
        return mShardAnalyzeResult;
    }

    public CmPrepareResult getShardPrepareResult()
    {
        return mShardPrepareResult;
    }

    public void setShardPrepareResult(CmPrepareResult aShardPrepareResult)
    {
        this.mShardPrepareResult = aShardPrepareResult;
    }

    public CmProtocolContextShardConnect getShardContextConnect()
    {
        return mMetaConn.getShardContextConnect();
    }

    public GlobalTransactionLevel getGlobalTransactionLevel()
    {
        return mMetaConn.getGlobalTransactionLevel();
    }

    public boolean isAutoCommitMode()
    {
        return mMetaConn.getShardContextConnect().isAutoCommitMode();
    }

    public int getUpdateRowcount()
    {
        return mUpdateRowcount;
    }

    public void setUpdateRowcount(int aUpdateRowcount)
    {
        mUpdateRowcount = aUpdateRowcount;
    }

    public ShardPartialExecType getShardPartialExecType()
    {
        return mShardPartialExecType;
    }

    public void setShardPartialExecType(ShardPartialExecType aShardPartialExecType)
    {
        mShardPartialExecType = aShardPartialExecType;
    }
}
