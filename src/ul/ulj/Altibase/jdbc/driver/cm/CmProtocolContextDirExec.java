/*
 *  Copyright (c) 1999~2017, Altibase Corp. and/or its affiliates. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License, version 3,
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

package Altibase.jdbc.driver.cm;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import Altibase.jdbc.driver.sharding.core.DistTxInfo;
import Altibase.jdbc.driver.cm.CmProtocolContextShardStmt.ShardPartialExecType;

public class CmProtocolContextDirExec extends CmProtocolContext
{
    private final List<Map<String,Object>> mDeferredRequests;   // BUG-48431 deferred된 prepare 요청 목록
    private DistTxInfo                     mDistTxInfo;         // PROJ-2733

    // BUG-49296 : BUG-48315
    private List<Integer>                  mClientTouchedNodes;
    private short                          mClientTouchedNodeCount = 0;

    // TASK-7219 Non-shard DML
    private int                            mStmtExecSeqForShardTx;
    private ShardPartialExecType           mPartialExecType;

    public CmProtocolContextDirExec(CmChannel aChannel)
    {
        super(aChannel);
        mDeferredRequests   = new ArrayList<>();
        mDistTxInfo         = new DistTxInfo();
        mClientTouchedNodes = new ArrayList<Integer>();
    }
    
    public CmPrepareResult getPrepareResult()
    {
        return (CmPrepareResult)getCmResult(CmPrepareResult.MY_OP);
    }

    public CmGetColumnInfoResult getGetColumnInfoResult()
    {
        return (CmGetColumnInfoResult)getCmResult(CmGetColumnInfoResult.MY_OP);
    }

    public CmExecutionResult getExecutionResult()
    {
        return (CmExecutionResult)getCmResult(CmExecutionResult.MY_OP);
    }
    
    public CmFetchResult getFetchResult()
    {
        return (CmFetchResult)getCmResult(CmFetchResult.MY_OP);
    }

    public CmGetPlanResult getGetPlanResult()
    {
        return (CmGetPlanResult) getCmResult(CmGetPlanResult.MY_OP);
    }
    
    public CmGetBindParamInfoResult getGetBindParamInfoResult()
    {
        return (CmGetBindParamInfoResult) getCmResult(CmGetBindParamInfoResult.MY_OP);
    }
    
    int getStatementId()
    {
        return getPrepareResult().getStatementId();
    }
    
    short getResultSetId()
    {
        return getFetchResult().getResultSetId();
    }
    
    public void setResultSetId(short aResultSetId)
    {
        getFetchResult().setResultSetId(aResultSetId);
    }

    public void addDeferredRequest(Map<String, Object> aMethodInfo)
    {
        mDeferredRequests.add(aMethodInfo);
    }

    public List<Map<String, Object>> getDeferredRequests()
    {
        return mDeferredRequests;
    }

    public void clearDeferredRequests()
    {
        mDeferredRequests.clear();
    }

    public DistTxInfo getDistTxInfo()
    {
        return mDistTxInfo;
    }

    public short getClientTouchedNodeCount()
    {
        return mClientTouchedNodeCount;
    }

    public void setClientTouchedNodeCount(short aClientTouchedNodeCount)
    {
        mClientTouchedNodeCount = aClientTouchedNodeCount;
    }

    public List<Integer> getClientTouchNodes()
    {
        return mClientTouchedNodes;
    }

    public void setClientTouchedNodes(int aNodeId)
    {
        mClientTouchedNodes.add(aNodeId);
    }

    public void clearClientTouchedNodes()
    {
        mClientTouchedNodeCount = 0;
        mClientTouchedNodes.clear();
    }

    public int getStmtExecSeqForShardTx()
    {
        return mStmtExecSeqForShardTx;
    }
    
    public void setStmtExecSeqForShardTx(int aStmtExecSeqForShardTx)
    {
       mStmtExecSeqForShardTx = aStmtExecSeqForShardTx;
    }
    
    ShardPartialExecType getPartialExecType()
    {
        return mPartialExecType;
    }

    void setPartialExecType(ShardPartialExecType aPartialExecType)
    {
       mPartialExecType = aPartialExecType;
    }
}
