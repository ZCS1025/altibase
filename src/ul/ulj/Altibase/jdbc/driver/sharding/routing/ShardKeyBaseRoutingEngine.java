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

import Altibase.jdbc.driver.cm.CmProtocolContextShardConnect;
import Altibase.jdbc.driver.cm.CmProtocolContextShardStmt;
import Altibase.jdbc.driver.datatype.Column;
import Altibase.jdbc.driver.ex.Error;
import Altibase.jdbc.driver.ex.ErrorDef;
import Altibase.jdbc.driver.sharding.algorithm.HashGenerator;
import Altibase.jdbc.driver.sharding.core.*;
import Altibase.jdbc.driver.sharding.strategy.ShardingStrategy;

import java.io.UnsupportedEncodingException;
import java.sql.SQLException;
import java.util.LinkedList;
import java.util.List;

import static Altibase.jdbc.driver.sharding.util.ShardingTraceLogger.shard_log;

public abstract class ShardKeyBaseRoutingEngine implements RoutingEngine
{
    protected CmProtocolContextShardStmt mShardStmtCtx;
    ShardingStrategy                     mShardingStrategy;
    ShardSplitMethod                     mShardSplitMethod;
    ShardKeyDataType                     mShardKeyDataType;
    ShardRangeList                       mShardRangeList;
    int                                  mShardParamIdx;
    int                                  mShardSubParamIdx;
    private String                       mServerCharSet;

    ShardKeyBaseRoutingEngine(CmProtocolContextShardStmt aShardStmtCtx)
    {
        mShardStmtCtx = aShardStmtCtx;
        mShardSplitMethod = mShardStmtCtx.getShardAnalyzeResult().getShardSplitMethod();
        mShardKeyDataType = mShardStmtCtx.getShardAnalyzeResult().getShardKeyDataType();
        mShardRangeList = mShardStmtCtx.getShardAnalyzeResult().getShardRangeList();
        CmProtocolContextShardConnect sShardContextConn = aShardStmtCtx.getShardContextConnect();
        mServerCharSet = sShardContextConn.getServerCharSet();
    }

    List<Comparable<?>> getShardValues(List<Column> aParameters,
                                       List<ShardValueInfo> aShardValueInfoList,
                                       ShardSplitMethod aShardSplitMethod,
                                       ShardKeyDataType aShardKeyDataType,
                                       boolean aIsSub) throws SQLException
    {
        List<Comparable<?>> sResult = new LinkedList<Comparable<?>>();
        for (ShardValueInfo sShardValueInfo : aShardValueInfoList)
        {
            Column sParameter = getShardParameterInfo(aParameters, aIsSub, sShardValueInfo);
            if (aShardSplitMethod == ShardSplitMethod.HASH)
            {
                long sHashValue = HashGenerator.getInstance().doHash(sParameter, aShardKeyDataType,
                                                                     mServerCharSet);
                int sHashModValue = (int)(sHashValue % 1000);
                shard_log("(SHARD HASH VALUE) {0}", sHashValue);
                sResult.add(sHashModValue);
            }
            else
            {
                if (sParameter.isNull())
                {
                    sResult.add(null);
                }
                else
                {
                    switch (aShardKeyDataType)
                    {
                        case SMALLINT:
                            sResult.add(sParameter.getShort());
                            break;
                        case INTEGER:
                            sResult.add(sParameter.getInt());
                            break;
                        case BIGINT:
                            sResult.add(sParameter.getLong());
                            break;
                        case CHAR:
                        case VARCHAR:
                            String sOriginalStr = sParameter.getString();
                            String sEncodedStr = null;
                            try
                            {
                                // client 캐릭터셋으로 들어온 String을 서버 캐릭터셋으로 변환한다.
                                byte[] sOriginalArry = sOriginalStr.getBytes(mServerCharSet);
                                sEncodedStr = new String(sOriginalArry, mServerCharSet);
                            }
                            catch (UnsupportedEncodingException aException)
                            {
                                // 서버의 캐릭터셋으로 encoding이 되지 않을 경우에는 예외를 던진다.
                                Error.throwSQLException(ErrorDef.INVALID_DATA_CONVERSION);
                            }
                            sResult.add(sEncodedStr);
                    }
                }            
            }
        }

        return sResult;
    }

    private Column getShardParameterInfo(List<Column> aParameters, boolean aIsSub, 
                                         ShardValueInfo aShardValueInfo)
    {
        Column sParameter;
        if (aShardValueInfo.getType() == 0)        // prepared execute
        {
            int sBindParamId = aShardValueInfo.getBindParamId();
            if (aIsSub)
            {
                mShardSubParamIdx = sBindParamId;
            }
            else
            {
                mShardParamIdx = sBindParamId;
            }
            sParameter = aParameters.get(sBindParamId);
        }
        else     // bind가 아닐경우
        {
            sParameter = aShardValueInfo.getValue();
        }
        return sParameter;
    }

    DataNode getDefaultNode()
    {
        int sDefaultNodeId = mShardStmtCtx.getShardAnalyzeResult().getShardDefaultNodeID();
        DataNode sDefaultNode = null;
        if (sDefaultNodeId > 0)
        {
            ShardNodeConfig sShardNodeConfig = mShardStmtCtx.getShardContextConnect().getShardNodeConfig();
            for (DataNode sEach : sShardNodeConfig.getDataNodes())
            {
                if (sDefaultNodeId == sEach.getNodeId())
                {
                    sDefaultNode = sEach;
                    break;
                }
            }
        }
        return sDefaultNode;
    }
}
