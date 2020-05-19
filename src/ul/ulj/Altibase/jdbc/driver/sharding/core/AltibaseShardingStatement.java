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

import Altibase.jdbc.driver.AltibaseFailover;
import Altibase.jdbc.driver.AltibaseResultSet;
import Altibase.jdbc.driver.AltibaseStatement;
import Altibase.jdbc.driver.cm.CmProtocolContextShardConnect;
import Altibase.jdbc.driver.cm.CmProtocolContextShardStmt;
import Altibase.jdbc.driver.cm.CmShardProtocol;
import Altibase.jdbc.driver.ex.Error;
import Altibase.jdbc.driver.ex.ErrorDef;

import java.sql.*;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import static Altibase.jdbc.driver.sharding.util.ShardingTraceLogger.shard_log;

public class AltibaseShardingStatement extends JdbcMethodInvoker implements Statement
{
    CmProtocolContextShardStmt                 mShardStmtCtx;
    CmShardProtocol                            mShardProtocol;
    SQLWarning                                 mSqlwarning;
    AltibaseShardingConnection                 mMetaConn;
    String                                     mSql;
    CmProtocolContextShardConnect              mShardContextConnect;
    Map<DataNode, Statement>                   mRoutedStatementMap;
    Map<Integer, JdbcMethodInvocation>         mSetParamInvocationMap;
    InternalShardingStatement                  mInternalStmt;
    protected Statement                        mServerSideStmt;
    long                                       mShardMetaNumber;
    int                                        mResultSetType;
    int                                        mResultSetConcurrency;
    int                                        mResultSetHoldability;
    boolean                                    mIsCoordQuery;
    private static int                         DEFAULT_CURSOR_HOLDABILITY = ResultSet.CLOSE_CURSORS_AT_COMMIT;
    private Class<? extends Statement>         mTargetClass;

    AltibaseShardingStatement(AltibaseShardingConnection aConn) throws SQLException
    {
        this(aConn, ResultSet.TYPE_FORWARD_ONLY, ResultSet.CONCUR_READ_ONLY,
             DEFAULT_CURSOR_HOLDABILITY);
    }

    AltibaseShardingStatement(AltibaseShardingConnection aConn, int aResultSetType,
                              int aResultSetConcurrency, int aResultSetHoldability) throws SQLException
    {
        mMetaConn = aConn;
        mResultSetType = aResultSetType;
        mResultSetConcurrency = aResultSetConcurrency;
        mResultSetHoldability = aResultSetHoldability;
        mShardContextConnect = aConn.getShardContext();
        mShardStmtCtx = new CmProtocolContextShardStmt(mShardContextConnect);
        mShardProtocol = aConn.getShardProtocol();
        mTargetClass = Statement.class;
        mRoutedStatementMap = new ConcurrentHashMap<DataNode, Statement>();
        if (aResultSetType == ResultSet.TYPE_SCROLL_SENSITIVE ||
                aResultSetType == ResultSet.TYPE_SCROLL_INSENSITIVE)
        {
            Error.throwSQLException(ErrorDef.UNSUPPORTED_FEATURE,
                                    "scrollable resultset is not supported in sharding.");
        }
        aConn.registerStatement(this);
    }

    AltibaseShardingStatement(AltibaseShardingConnection aConn, int aResultSetType,
                              int aResultSetConcurrency) throws SQLException
    {
        this(aConn, aResultSetType, aResultSetConcurrency, DEFAULT_CURSOR_HOLDABILITY);
    }

    public ResultSet executeQuery(String aSql) throws SQLException
    {
        shardAnalyze(aSql);
        ResultSet sResult = mInternalStmt.executeQuery(aSql);
        updateShardMetaNumberIfNecessary();
        clearClosedResultSets(sResult);

        return sResult;
    }

    public int executeUpdate(String aSql) throws SQLException
    {
        shardAnalyze(aSql);
        int sResult = mInternalStmt.executeUpdate(aSql);
        updateShardMetaNumberIfNecessary();

        return sResult;
    }

    public void close() throws SQLException
    {
        mMetaConn.unregisterStatement(this);
        if (mInternalStmt != null)
        {
            mInternalStmt.close();
        }
    }

    public int getMaxFieldSize() throws SQLException
    {
        return mInternalStmt.getMaxFieldSize();
    }

    public void setMaxFieldSize(int aMax) throws SQLException
    {
        recordMethodInvocation(mTargetClass, "setMaxFieldSize", new Class[] {int.class}, new Object[] {aMax});
        mInternalStmt.setMaxFieldSize(aMax);
    }

    public int getMaxRows() throws SQLException
    {
        return mInternalStmt.getMaxRows();
    }

    public void setMaxRows(int aMax) throws SQLException
    {
        recordMethodInvocation(mTargetClass, "setMaxRows", new Class[] {int.class}, new Object[] {aMax});
        mInternalStmt.setMaxRows(aMax);
    }

    public void setEscapeProcessing(boolean aEnable) throws SQLException
    {
        recordMethodInvocation(mTargetClass, "setEscapeProcessing", new Class[] {boolean.class},
                               new Object[] {aEnable});
        mInternalStmt.setEscapeProcessing(aEnable);
    }

    public int getQueryTimeout() throws SQLException
    {
        return mInternalStmt.getQueryTimeout();
    }

    public void setQueryTimeout(int aSeconds) throws SQLException
    {
        recordMethodInvocation(mTargetClass, "setQueryTimeout", new Class[] {int.class},
                               new Object[] {aSeconds});
        mInternalStmt.setQueryTimeout(aSeconds);
    }

    public void cancel() throws SQLException
    {
        mInternalStmt.cancel();
    }

    public SQLWarning getWarnings() throws SQLException
    {
        if (mSqlwarning == null)
        {
            mSqlwarning = mInternalStmt.getWarnings();
        }
        else
        {
            mSqlwarning.setNextWarning(mInternalStmt.getWarnings());
        }

        return mSqlwarning;
    }

    public void clearWarnings() throws SQLException
    {
        mInternalStmt.clearWarnings();
        mSqlwarning = null;
    }

    public void setCursorName(String aName) throws SQLException
    {
        mInternalStmt.setCursorName(aName);
    }

    public boolean execute(String aSql) throws SQLException
    {
        shardAnalyze(aSql);
        boolean sResult = mInternalStmt.execute(aSql);
        updateShardMetaNumberIfNecessary();

        return sResult;
    }

    public ResultSet getResultSet() throws SQLException
    {
        return mInternalStmt.getResultSet();
    }

    public int getUpdateCount() throws SQLException
    {
        return mInternalStmt.getUpdateCount();
    }

    public boolean getMoreResults() throws SQLException
    {
        boolean sResult = mInternalStmt.getMoreResults();

        if (mMetaConn.getAutoCommit() && !sResult && mMetaConn.shouldUpdateShardMetaNumber())
        {
            mMetaConn.updateShardMetaNumber();
        }

        return sResult;
    }

    public void setFetchDirection(int aDirection) throws SQLException
    {
        AltibaseResultSet.checkFetchDirection(aDirection);
    }

    public int getFetchDirection()
    {
        return ResultSet.FETCH_FORWARD;
    }

    public void setFetchSize(int aRows) throws SQLException
    {
        recordMethodInvocation(mTargetClass, "setFetchSize", new Class[] {int.class},
                               new Object[] {aRows});
        mInternalStmt.setFetchSize(aRows);
    }

    public int getFetchSize() throws SQLException
    {
        return mInternalStmt.getFetchSize();
    }

    public int getResultSetConcurrency()
    {
        return mResultSetConcurrency;
    }

    public int getResultSetType()
    {
        return mResultSetType;
    }

    public void addBatch(String aSql) throws SQLException
    {
        Error.throwSQLException(ErrorDef.UNSUPPORTED_FEATURE, "addBatch is not supported in sharding");
    }

    public void clearBatch() throws SQLException
    {
        Error.throwSQLException(ErrorDef.UNSUPPORTED_FEATURE, "clearBatch is not supported in sharding");
    }

    public int[] executeBatch() throws SQLException
    {
        return null;
    }

    public Connection getConnection() throws SQLException
    {
        return mInternalStmt.getConnection();
    }

    public boolean getMoreResults(int aCurrent) throws SQLException
    {
        return mInternalStmt.getMoreResults(aCurrent);
    }

    public ResultSet getGeneratedKeys() throws SQLException
    {
        return mInternalStmt.getGeneratedKeys();
    }

    public int executeUpdate(String aSql, int aAutoGeneratedKeys) throws SQLException
    {
        shardAnalyze(aSql);
        int sResult = mInternalStmt.executeUpdate(aSql, aAutoGeneratedKeys);
        updateShardMetaNumberIfNecessary();

        return sResult;
    }

    public int executeUpdate(String aSql, int[] aColumnIndexes) throws SQLException
    {
        shardAnalyze(aSql);
        int sResult = mInternalStmt.executeUpdate(aSql, aColumnIndexes);
        updateShardMetaNumberIfNecessary();

        return sResult;
    }

    public int executeUpdate(String aSql, String[] aColumnNames) throws SQLException
    {
        shardAnalyze(aSql);
        int sResult = mInternalStmt.executeUpdate(aSql, aColumnNames);
        updateShardMetaNumberIfNecessary();

        return sResult;
    }

    public boolean execute(String aSql, int aAutoGeneratedKeys) throws SQLException
    {
        shardAnalyze(aSql);
        boolean sResult = mInternalStmt.execute(aSql, aAutoGeneratedKeys);
        updateShardMetaNumberIfNecessary();

        return sResult;
    }

    /**
     * 오토커밋 ON 상태이고 DataNode의 SMN 값이 더 높다면 SMN값을 업데이트 한다.
     * @throws SQLException shard meta number 업데이트 중 에러가 발생했을 때
     */
    protected void updateShardMetaNumberIfNecessary() throws SQLException
    {
        if (mMetaConn.getAutoCommit() && mMetaConn.shouldUpdateShardMetaNumber())
        {
            mMetaConn.updateShardMetaNumber();
        }
    }

    protected void clearClosedResultSets(ResultSet aResultSet)
    {
        if (aResultSet instanceof AltibaseShardingResultSet)
        {
            AltibaseShardingResultSet sShardingResultSet = (AltibaseShardingResultSet)aResultSet;
            sShardingResultSet.clearClosedResultSets();
        }
    }

    public boolean execute(String aSql, int[] aColumnIndexes) throws SQLException
    {
        shardAnalyze(aSql);
        boolean sResult = mInternalStmt.execute(aSql, aColumnIndexes);
        updateShardMetaNumberIfNecessary();

        return sResult;
    }

    public boolean execute(String aSql, String[] aColumnNames) throws SQLException
    {
        shardAnalyze(aSql);
        boolean sResult = mInternalStmt.execute(aSql, aColumnNames);
        updateShardMetaNumberIfNecessary();

        return sResult;
    }

    public int getResultSetHoldability()
    {
        return mResultSetHoldability;
    }

    private void shardAnalyze(String aSql) throws SQLException
    {
        mShardStmtCtx = new CmProtocolContextShardStmt(mMetaConn.getShardContext());
        try
        {
            mShardProtocol.shardAnalyze(mShardStmtCtx, aSql);
        }
        catch (SQLException aEx)
        {
            AltibaseFailover.trySTF(mMetaConn.getMetaConnection().failoverContext(), aEx);
        }

        if (mShardStmtCtx.getShardAnalyzeResult().isCoordQuery())
        {
            // BUG-46513 ONENODE이고 analyze 결과가 성공이 아닐경우에는 예외를 던진다.
            if (mShardStmtCtx.getShardTransactionLevel() == ShardTransactionLevel.ONE_NODE)
            {
                mSqlwarning = Error.processServerError(mSqlwarning, mShardStmtCtx.getError());
            }
            Connection sConn = mMetaConn.getMetaConnection();
            mServerSideStmt = sConn.createStatement(mResultSetType, mResultSetConcurrency,
                                                    mResultSetHoldability);
            mIsCoordQuery = true;
            // BUG-46513 서버사이드일때는 meta connection을 AltibaseStatement에 주입한다.
            ((AltibaseStatement)mServerSideStmt).setMetaConnection(mMetaConn);
            mInternalStmt = new ServerSideShardingStatement(mServerSideStmt, mShardContextConnect);
        }
        else
        {
            mInternalStmt = new DataNodeShardingStatement(mMetaConn,
                                                          mResultSetType, mResultSetConcurrency,
                                                          mResultSetHoldability,
                                                          this);
            ((DataNodeShardingStatement)mInternalStmt).setShardStmtCtx(mShardStmtCtx);
        }
    }

    public boolean isPrepared()
    {
        return mInternalStmt.isPrepared();
    }

    public boolean hasNoData()
    {
        return mInternalStmt.hasNoData();
    }

    public AltibaseShardingConnection getMetaConn()
    {
        return mMetaConn;
    }

    public void setSql(String aSql)
    {
        mSql = aSql;
    }

    public void setShardMetaNumber(long aShardMetaNumber)
    {
        mShardMetaNumber = aShardMetaNumber;
    }

    public void removeFromStmtRouteMap(DataNode aNode)
    {
        if (!mIsCoordQuery)
        {
            ((DataNodeShardingStatement)mInternalStmt).removeFromStmtRouteMap(aNode);
        }
    }

    public Map<DataNode, Statement> getRoutedStatementMap()
    {
        return mRoutedStatementMap;
    }

    void replaySetParameter(PreparedStatement aPreparedStatement) throws SQLException
    {
        for (JdbcMethodInvocation sEach : mSetParamInvocationMap.values())
        {
            sEach.invoke(aPreparedStatement);
            shard_log("(REPLAY SET PARAMETER) {0}", sEach);
        }
    }
}
