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

package Altibase.jdbc.driver.sharding.strategy;

import Altibase.jdbc.driver.sharding.algorithm.*;
import Altibase.jdbc.driver.sharding.core.DataNode;
import Altibase.jdbc.driver.sharding.core.ShardRangeList;
import Altibase.jdbc.driver.sharding.core.ShardSplitMethod;

import java.sql.SQLException;
import java.util.*;
import java.util.logging.Level;

import static Altibase.jdbc.driver.sharding.util.ShardingTraceLogger.shard_log;

public class CompositeShardingStrategy implements ShardingStrategy
{
    private CompositeShardingAlgorithm mDefaultShardingAlgorithm;
    private DataNode                   mDefaultNode;
    private List<ShardSplitMethod>     mShardSplitMethodList;

    public CompositeShardingStrategy(ShardRangeList aShardRangeList, DataNode aDefaultNode,
                                     List<ShardSplitMethod> aShardSplitMethodList)
    {
        mDefaultShardingAlgorithm = new StandardCompositeShardingAlgorithm(aShardRangeList);
        mDefaultNode = aDefaultNode;
        mShardSplitMethodList = aShardSplitMethodList;
    }

    public Set<DataNode> doSharding(List<ShardingValue> aShardingValues) throws SQLException
    {
        List<PreciseShardingValue> sShardValues = new ArrayList<PreciseShardingValue>();

        for (ShardingValue sShardingValue : aShardingValues)
        {
            sShardValues.add(getPreciseShardingValue((ListShardingValue<?>)sShardingValue));
        }

        Set<DataNode> sResults;
        try
        {
            sResults = mDefaultShardingAlgorithm.doSharding(sShardValues, mShardSplitMethodList,
                                                            mDefaultNode);
        }
        catch (SQLException aEx)
        {
            shard_log(Level.SEVERE, aEx);
            throw aEx;
        }

        return sResults;
    }

    @SuppressWarnings("unchecked")
    private PreciseShardingValue getPreciseShardingValue(ListShardingValue<?> aShardingValue)
    {
        PreciseShardingValue sResult = null;

        for (Comparable sEach : aShardingValue.getValues())
        {
            sResult =  new PreciseShardingValue(aShardingValue.getColumnIdx(), sEach);
        }

        return sResult;
    }
}
