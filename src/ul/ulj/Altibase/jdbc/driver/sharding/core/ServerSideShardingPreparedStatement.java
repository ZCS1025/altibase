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

package Altibase.jdbc.driver.sharding.core;

import Altibase.jdbc.driver.AltibasePreparedStatement;
import Altibase.jdbc.driver.cm.CmProtocolContextShardConnect;

import java.sql.*;

import static Altibase.jdbc.driver.sharding.util.ShardingTraceLogger.shard_log;

/**
 *  java.sql.PreparedStatement 인터페이스 중 일부를 구현하며 내부적으로 서버사이드용 PreparedStatement를
 *  이용해 쿼리를 수행한다.
 */
public class ServerSideShardingPreparedStatement extends ServerSideShardingStatement implements InternalShardingPreparedStatement
{
    private PreparedStatement mServersidePstmt;

    public ServerSideShardingPreparedStatement(PreparedStatement aServersidePstmt,
                                               CmProtocolContextShardConnect aShardContextConnect)
    {
        super(aServersidePstmt, aShardContextConnect);
        mServersidePstmt = aServersidePstmt;
    }

    public ResultSet executeQuery() throws SQLException
    {
        touchMetaNode();
        shard_log("(META NODE EXECUTEQUERY) {0}",
                  ((AltibasePreparedStatement)mServersidePstmt).getStmtId());
        return mServersidePstmt.executeQuery();
    }

    public int executeUpdate() throws SQLException
    {
        touchMetaNode();
        shard_log("(META NODE EXECUTEUPDATE) {0}",
                  ((AltibasePreparedStatement)mServersidePstmt).getStmtId());

        return mServersidePstmt.executeUpdate();
    }

    public int[] executeBatch() throws SQLException
    {
        touchMetaNode();
        shard_log("(META NODE EXECUTEBATCH) {}",
                  ((AltibasePreparedStatement)mServersidePstmt).getStmtId());

        return mServersidePstmt.executeBatch();
    }

    public void clearParameters() throws SQLException
    {
        mServersidePstmt.clearParameters();
    }

    public boolean execute() throws SQLException
    {
        touchMetaNode();
        shard_log("(META NODE EXECUTE) {}",
                  ((AltibasePreparedStatement)mServersidePstmt).getStmtId());

        return mServersidePstmt.execute();
    }

    public void addBatch() throws SQLException
    {
        mServersidePstmt.addBatch();
    }

    public ResultSetMetaData getMetaData() throws SQLException
    {
        return mServersidePstmt.getMetaData();
    }

    public ParameterMetaData getParameterMetaData() throws SQLException
    {
        return mServersidePstmt.getParameterMetaData();
    }

    public void clearBatch() throws SQLException
    {
        mServersidePstmt.clearBatch();
    }
}
