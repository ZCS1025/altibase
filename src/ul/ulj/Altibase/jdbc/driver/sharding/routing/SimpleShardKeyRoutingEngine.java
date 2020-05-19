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

import Altibase.jdbc.driver.cm.CmProtocolContextShardStmt;
import Altibase.jdbc.driver.cm.CmShardAnalyzeResult;
import Altibase.jdbc.driver.datatype.Column;
import Altibase.jdbc.driver.sharding.algorithm.ListShardingValue;
import Altibase.jdbc.driver.sharding.algorithm.ShardingValue;
import Altibase.jdbc.driver.sharding.core.DataNode;
import Altibase.jdbc.driver.sharding.strategy.StandardShardingStrategy;

import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;

public class SimpleShardKeyRoutingEngine extends ShardKeyBaseRoutingEngine
{
    public SimpleShardKeyRoutingEngine(CmProtocolContextShardStmt aShardStmtCtx)
    {
        super(aShardStmtCtx);
        mShardingStrategy = new StandardShardingStrategy(mShardRangeList, mShardSplitMethod,
                                                         getDefaultNode());
    }

    public SQLRouteResult route(String aSql, List<Column> aParameters) throws SQLException
    {
        List<ShardingValue> sShardingValues = new ArrayList<ShardingValue>();
        CmShardAnalyzeResult sShardAnalyzeResult = mShardStmtCtx.getShardAnalyzeResult();
        List<Comparable<?>> sShardValues = getShardValues(aParameters,
                                                          sShardAnalyzeResult.getShardValueInfoList(),
                                                          mShardSplitMethod,
                                                          mShardKeyDataType,
                                                          false);
        sShardingValues.add(new ListShardingValue<Comparable<?>>(mShardParamIdx, sShardValues));

        SQLRouteResult sResult = new SQLRouteResult();
        Set<DataNode> sShardResult = mShardingStrategy.doSharding(sShardingValues);

        for (DataNode sEach : sShardResult)
        {
            sResult.getExecutionUnits().add(new SQLExecutionUnit(sEach, aSql));
        }

        return sResult;
    }

    @Override
    public String toString()
    {
        final StringBuilder sSb = new StringBuilder("SimpleShardKeyRoutingEngine{");
        sSb.append("mShardParamIdx=").append(mShardParamIdx);
        sSb.append(", mShardSplitMethod=").append(mShardSplitMethod);
        sSb.append(", mShardKeyDataType=").append(mShardKeyDataType);
        sSb.append('}');
        return sSb.toString();
    }
}
