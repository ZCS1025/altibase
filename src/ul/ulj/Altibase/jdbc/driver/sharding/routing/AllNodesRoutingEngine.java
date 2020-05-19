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

package Altibase.jdbc.driver.sharding.routing;

import Altibase.jdbc.driver.datatype.Column;
import Altibase.jdbc.driver.sharding.core.DataNode;
import Altibase.jdbc.driver.sharding.core.ShardNodeConfig;
import Altibase.jdbc.driver.sharding.core.ShardRange;
import Altibase.jdbc.driver.sharding.core.ShardRangeList;
import Altibase.jdbc.driver.cm.CmProtocolContextShardStmt;
import Altibase.jdbc.driver.cm.CmShardAnalyzeResult;

import java.util.List;

/**
 * 샤드 키값이 없기때문에 shard range의 모든 노드가 수행대상이 된다.
 */
public class AllNodesRoutingEngine implements RoutingEngine
{
    private CmProtocolContextShardStmt mShardStmtCtx;

    public AllNodesRoutingEngine(CmProtocolContextShardStmt aShardStmtCtx)
    {
        mShardStmtCtx = aShardStmtCtx;
    }

    public SQLRouteResult route(String aSql, List<Column> aParameters)
    {
        CmShardAnalyzeResult sShardAnalyzeResult = mShardStmtCtx.getShardAnalyzeResult();
        ShardRangeList sShardRangeList = sShardAnalyzeResult.getShardRangeList();
        SQLRouteResult sResult = new SQLRouteResult();
        ShardNodeConfig sShardNodeConfig = mShardStmtCtx.getShardContextConnect().getShardNodeConfig();

        if (sShardRangeList == null)
        {
            // PROJ-2690 range 정보가 없는 경우엔 전체 데이터노드가 실행대상이 된다.
            for (DataNode sEach : sShardNodeConfig.getDataNodes())
            {
                sResult.getExecutionUnits().add(new SQLExecutionUnit(sEach, aSql));
            }
        }
        else
        {
            int sDefaultNodeId = sShardAnalyzeResult.getShardDefaultNodeID();
            boolean sDefaultNodeIncluded = false;
            for (ShardRange sEach : sShardRangeList.getRangeList())
            {
                if (sEach.getNode().getNodeId() == sDefaultNodeId)
                {
                    sDefaultNodeIncluded = true;
                }
                sResult.getExecutionUnits().add(new SQLExecutionUnit(sEach.getNode(), aSql));
            }
            if (sDefaultNodeId >= 0 && !sDefaultNodeIncluded)
            {
                DataNode sNode = null;
                for (DataNode sEach : sShardNodeConfig.getDataNodes())
                {
                    if (sDefaultNodeId == sEach.getNodeId())
                    {
                        sNode = sEach;
                        break;
                    }
                }
                sResult.getExecutionUnits().add(new SQLExecutionUnit(sNode, aSql));
            }
        }

        return sResult;
    }

    @Override
    public String toString()
    {
        final StringBuilder sSb = new StringBuilder("AllNodesRoutingEngine{");
        sSb.append("mShardStmtCtx=").append(mShardStmtCtx);
        sSb.append('}');
        return sSb.toString();
    }
}
