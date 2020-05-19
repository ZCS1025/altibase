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

import Altibase.jdbc.driver.sharding.core.DataNode;

import java.sql.SQLException;
import java.util.List;

public class CmShardProtocol
{
    private CmProtocolContextShardConnect mShardContextConnect;
    private CmShardOperation mShardOperation;
    
    public CmShardProtocol(CmProtocolContextShardConnect aShardContext)
    {
        mShardContextConnect = aShardContext;
        mShardOperation = new CmShardOperation(mShardContextConnect.channel());
    }

    /**
     * 샤딩을 구성하고 있는 노드리스트를 프로토콜을 통해 받아온다.
     */
    public void getNodeList() throws SQLException
    {
        mShardContextConnect.clearError();
        synchronized (mShardContextConnect.channel())
        {
            mShardOperation.writeGetNodeList();
            mShardContextConnect.channel().sendAndReceive();
            mShardOperation.readGetNodeListResult(mShardContextConnect);
        }
    }

    /**
     * 샤딩을 구성하고 있는 노드리스트를 프로토콜을 통해 갱신한다.
     */
    public void updateNodeList() throws SQLException
    {
        mShardContextConnect.clearError();
        synchronized (mShardContextConnect.channel())
        {
            mShardOperation.writeUpdateNodeList();
            mShardContextConnect.channel().sendAndReceive();
            mShardOperation.readUpdateNodeListResult(mShardContextConnect);
        }
    }

    /**
     *
     * @param aShardContextStmt shard statement context 객체
     * @param aSql sql string
     */
    public void shardAnalyze(CmProtocolContextShardStmt aShardContextStmt,
                             String aSql) throws SQLException
    {
        aShardContextStmt.clearError();
        synchronized (mShardContextConnect.channel())
        {
            mShardOperation.writeShardAnalyze(aSql);
            mShardContextConnect.channel().sendAndReceive();
            mShardOperation.readShardAnalyze(aShardContextStmt);
        }
    }

    public void sendShardTransactionCommitRequest(List<DataNode> aTouchedNodeList) throws SQLException
    {
        mShardContextConnect.clearError();
        synchronized (mShardContextConnect.channel())
        {
            mShardOperation.writeShardTransactionCommitRequest(aTouchedNodeList);
            mShardContextConnect.channel().sendAndReceive();
            mShardOperation.readShardTransactionCommitRequest(mShardContextConnect);
        }
    }
}
