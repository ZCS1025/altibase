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
import Altibase.jdbc.driver.sharding.core.NodeConnectionReport;
import Altibase.jdbc.driver.util.AltiSqlProcessor;

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
            mShardOperation.readProtocolResult(mShardContextConnect);
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
            mShardOperation.readProtocolResult(mShardContextConnect);
        }
    }

    /**
     *
     * @param aShardContextStmt shard statement context 객체
     * @param aSql sql string
     * @param aStmtID Statement ID
     */
    public void shardAnalyze(CmProtocolContextShardStmt aShardContextStmt,
                             String aSql, int aStmtID) throws SQLException
    {
        aShardContextStmt.clearError();
        // BUG-49197
        // BUGBUG : prepare or execute 에도 processEscape 하므로 중복수행됨.
        //          그러나 analyze에서도 processEscape 처리가 필요하여 일단 여기에서도 수행.
        //          sharding이 아닌 경우에는 prepare or execute에서 processEscape 해야하므로 prepare/execute에서 processEscape을 제거할 수 없는 상황.
        aSql = AltiSqlProcessor.processEscape(aSql);
        synchronized (mShardContextConnect.channel())
        {
            mShardOperation.writeShardAnalyze(aSql, aStmtID);
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
            mShardOperation.readProtocolResult(mShardContextConnect);
        }
    }

    /* PROJ-2733 */
    public void shardTransaction(CmProtocolContext aContext, boolean aIsCommit) throws SQLException
    {
        aContext.clearError();
        synchronized (aContext.channel())
        {
            mShardOperation.writeShardTransaction(aContext.channel(), aIsCommit);
            aContext.channel().sendAndReceive();
            mShardOperation.readProtocolResult(aContext);
        }
    }

    /**
     * 서버로 Failover Connection Report를 전송한다.
     * @param aReport 커넥션 레포트 객체
     * @throws SQLException 프로토콜 송/수신 도중 예외가 발생한 경우
     */
    public void shardNodeReport(NodeConnectionReport aReport) throws SQLException 
    {
        mShardContextConnect.clearError();
        synchronized (mShardContextConnect.channel()) 
        {
            mShardOperation.writeShardNodeReport(aReport);
            mShardContextConnect.channel().sendAndReceive();
            mShardOperation.readProtocolResult(mShardContextConnect);
        }
    }

    public void shardStmtPartialRollback(CmProtocolContextConnect aContext) throws SQLException
    {
        aContext.clearError();
        synchronized (aContext.channel())
        {
            mShardOperation.writeShardStmtPartialRollback(aContext.channel());
            aContext.channel().sendAndReceive();
            mShardOperation.readProtocolResult(aContext);
        }
    }

    public void setChannel(CmChannel aChannel)
    {
        mShardOperation.setChannel(aChannel);
    }
}
