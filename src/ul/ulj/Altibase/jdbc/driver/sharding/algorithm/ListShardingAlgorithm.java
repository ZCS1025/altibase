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

package Altibase.jdbc.driver.sharding.algorithm;

import Altibase.jdbc.driver.ex.Error;
import Altibase.jdbc.driver.ex.ErrorDef;
import Altibase.jdbc.driver.sharding.core.DataNode;
import Altibase.jdbc.driver.sharding.core.ShardRange;
import Altibase.jdbc.driver.sharding.core.ShardRangeList;
import Altibase.jdbc.driver.sharding.util.Range;

import java.sql.SQLException;
import java.util.HashSet;
import java.util.Set;

import static Altibase.jdbc.driver.sharding.util.ShardingTraceLogger.shard_log;

public class ListShardingAlgorithm <T extends Comparable<?>> implements RangeShardingAlgorithm<T>
{
    private ShardRangeList mShardRangeList;

    public ListShardingAlgorithm(ShardRangeList aShardRangeList)
    {
        this.mShardRangeList = aShardRangeList;
    }

    @SuppressWarnings("unchecked")
    public Set<DataNode> doSharding(PreciseShardingValue<T> aShardingValue, DataNode aDefaultNode)
            throws SQLException
    {
        if (mShardRangeList.getRangeList().isEmpty())
        {
            Error.throwSQLException(ErrorDef.SHARD_RANGE_NOT_FOUNDED);
        }
        Set<DataNode> sResult = new HashSet<DataNode>();
        for (ShardRange sShardRange : mShardRangeList.getRangeList())
        {
            Range sRange = sShardRange.getRange();
            if (sRange.isEndedBy(aShardingValue.getValue()))
            {
                sResult.add(sShardRange.getNode());
            }
        }

        if (sResult.size() == 0)
        {
            if (aDefaultNode != null)
            {
                sResult.add(aDefaultNode);
            }
            else
            {
                Error.throwSQLException(ErrorDef.SHARD_NODE_NOT_FOUNDED);
            }
        }

        shard_log("(LIST SHARDING RESULT) shardValue={0}, nodeId={1}",
                  new Object[] { aShardingValue, sResult });

        return sResult;
    }
}
