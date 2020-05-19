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

import Altibase.jdbc.driver.AltibaseStatement;
import Altibase.jdbc.driver.cm.CmProtocolContextShardConnect;
import Altibase.jdbc.driver.cm.CmProtocolContextShardStmt;
import Altibase.jdbc.driver.ex.*;
import Altibase.jdbc.driver.ex.Error;
import Altibase.jdbc.driver.sharding.executor.*;
import Altibase.jdbc.driver.sharding.merger.IteratorStreamResultSetMerger;
import Altibase.jdbc.driver.sharding.routing.*;

import java.sql.*;
import java.util.*;

import static Altibase.jdbc.driver.sharding.util.ShardingTraceLogger.shard_log;

public class DataNodeShardingStatement implements InternalShardingStatement
{
    AltibaseShardingConnection         mMetaConn;
    CmProtocolContextShardStmt         mShardStmtCtx;
    SQLRouteResult                     mRouteResult;
    ResultSet                          mCurrentResultSet;
    AltibaseShardingStatement          mShardStmt;
    Map<DataNode, Statement>           mRoutedStatementMap;
    SQLWarning                         mSqlwarning;
    int                                mResultSetType;
    int                                mResultSetConcurrency;
    int                                mResultSetHoldability;
    private ShardTransactionLevel      mShardTransactionLevel;
    private int                        mFetchSize;
    private StatementExecutor          mStmtExecutor;

    DataNodeShardingStatement(AltibaseShardingConnection aMetaConn,
                              int aResultSetType, int aResultSetConcurrency,
                              int aResultSetHoldability, AltibaseShardingStatement aShardStmt)
    {
        mMetaConn = aMetaConn;
        mShardStmt = aShardStmt;
        mShardStmtCtx = aShardStmt.mShardStmtCtx;
        mResultSetType = aResultSetType;
        mResultSetConcurrency = aResultSetConcurrency;
        mResultSetHoldability = aResultSetHoldability;
        mRoutedStatementMap = aShardStmt.getRoutedStatementMap();
        mStmtExecutor = new StatementExecutor(mMetaConn.getExecutorEngine());
        mShardTransactionLevel = mShardStmtCtx.getShardContextConnect().getShardTransactionLevel();
    }

    public ResultSet executeQuery(String aSql) throws SQLException
    {
        ResultSet sResult;

        try
        {
            List<? extends BaseStatementUnit> sStatementUnits = route(aSql);
            List<ResultSet> sResultSets = mStmtExecutor.executeQuery(sStatementUnits);
            mShardStmtCtx.setUpdateRowcount(getNodeUpdateRowCount());
            sResult = new AltibaseShardingResultSet(sResultSets,
                                                    new IteratorStreamResultSetMerger(sResultSets),
                                                    mShardStmt);
            setOneNodeTransactionInfo(mRouteResult.getExecutionUnits());
            getNodeSqlWarnings();
        }
        catch (ShardJdbcException aShardJdbcEx)
        {
            clearRoutedStatementMap(aShardJdbcEx);
            throw aShardJdbcEx;
        }
        finally
        {
            mCurrentResultSet = null;
        }

        mCurrentResultSet = sResult;
        return sResult;
    }

    public int executeUpdate(String aSql) throws SQLException
    {
        int sResult;
        try
        {
            List<? extends BaseStatementUnit> sStatementUnits = route(aSql);
            sResult = mStmtExecutor.executeUpdate(sStatementUnits);
            mShardStmtCtx.setUpdateRowcount(sResult);
        }
        catch (ShardJdbcException aShardJdbcEx)
        {
            clearRoutedStatementMap(aShardJdbcEx);
            throw aShardJdbcEx;
        }
        setOneNodeTransactionInfo(mRouteResult.getExecutionUnits());
        getNodeSqlWarnings();

        return sResult;
    }

    public void close() throws SQLException
    {
        List<SQLException> sExceptions = new LinkedList<SQLException>();

        for (Statement sEach : mRoutedStatementMap.values())
        {
            try
            {
                shard_log("(NODE STATEMENT CLOSE) {0} ", sEach);
                sEach.close();
            }
            catch (SQLException aEx)
            {
                sExceptions.add(aEx);
            }
        }
        mRoutedStatementMap.clear();
        throwSQLExceptionIfNecessary(sExceptions);
    }

    public int getMaxFieldSize() throws SQLException
    {
        return mRoutedStatementMap.isEmpty() ? 0 : mRoutedStatementMap.values().iterator().next().getMaxFieldSize();
    }

    public void setMaxFieldSize(int aMax) throws SQLException
    {
        for (Statement sEach : mRoutedStatementMap.values())
        {
            sEach.setMaxFieldSize(aMax);
        }
    }

    public int getMaxRows() throws SQLException
    {
        return mRoutedStatementMap.isEmpty() ? -1 : mRoutedStatementMap.values().iterator().next().getMaxRows();
    }

    public void setMaxRows(int aMax) throws SQLException
    {
        for (Statement sEach : mRoutedStatementMap.values())
        {
            sEach.setMaxRows(aMax);
        }
    }

    public void setEscapeProcessing(boolean aEnable) throws SQLException
    {
        for (Statement sEach : mRoutedStatementMap.values())
        {
            sEach.setEscapeProcessing(aEnable);
        }
    }

    public int getQueryTimeout() throws SQLException
    {
        return mRoutedStatementMap.isEmpty() ? 0 : mRoutedStatementMap.values().iterator().next().getQueryTimeout();
    }

    public void setQueryTimeout(int aSeconds) throws SQLException
    {
        for (Statement sEach : mRoutedStatementMap.values())
        {
            sEach.setQueryTimeout(aSeconds);
        }
    }

    public void cancel() throws SQLException
    {
        for (Statement sEach : mRoutedStatementMap.values())
        {
            sEach.cancel();
        }
    }

    public SQLWarning getWarnings()
    {
        return mSqlwarning;
    }

    public void clearWarnings()
    {
        mSqlwarning = null;
    }

    public void setCursorName(String aName) throws SQLException
    {
        Error.throwSQLException(ErrorDef.UNSUPPORTED_FEATURE, "cursor name and positioned update");
    }

    public boolean execute(String aSql) throws SQLException
    {
        boolean sResult;
        try
        {
            List<? extends BaseStatementUnit> sStatementUnits = route(aSql);
            sResult = mStmtExecutor.execute(sStatementUnits);
            mShardStmtCtx.setUpdateRowcount(getNodeUpdateRowCount());
            setOneNodeTransactionInfo(mRouteResult.getExecutionUnits());
            getNodeSqlWarnings();
        }
        catch (ShardJdbcException aShardJdbcEx)
        {
            clearRoutedStatementMap(aShardJdbcEx);
            throw aShardJdbcEx;
        }
        finally
        {
            mCurrentResultSet = null;
        }

        return sResult;
    }

    public ResultSet getResultSet() throws SQLException
    {
        if (mCurrentResultSet != null)
        {
            return mCurrentResultSet;
        }

        if (mRoutedStatementMap.size() == 1)
        {
            mCurrentResultSet = mRoutedStatementMap.values().iterator().next().getResultSet();
            return mCurrentResultSet;
        }

        List<ResultSet> sResultSets = new ArrayList<ResultSet>(mRoutedStatementMap.size());
        // BUG-46513 mRoutedStatementMap이 순서를 보장하지 않기 때문에 TreeMap을 이용해 sorting한다.
        TreeMap<DataNode, Statement> sSortedRouteStmtMap = new TreeMap<DataNode, Statement>(mRoutedStatementMap);

        for (Statement sEach : sSortedRouteStmtMap.values())
        {
            sResultSets.add(sEach.getResultSet());
        }

        if (mShardStmtCtx.getShardPrepareResult().isSelectStatement())
        {
            mCurrentResultSet = new AltibaseShardingResultSet(sResultSets, new IteratorStreamResultSetMerger(sResultSets), mShardStmt);
        }
        else
        {
            mCurrentResultSet = sResultSets.get(0);
        }

        return mCurrentResultSet;
    }

    public int getUpdateCount()
    {
        return mShardStmtCtx.getUpdateRowcount();
    }

    public boolean getMoreResults()
    {
        return false;
    }

    public void setFetchSize(int aRows) throws SQLException
    {
        mFetchSize = aRows;
        for (Statement sEach : mRoutedStatementMap.values())
        {
            sEach.setFetchSize(aRows);
        }
    }

    public int getFetchSize()
    {
        return mFetchSize;
    }

    public void clearBatch() throws SQLException
    {
        Error.throwSQLException(ErrorDef.UNSUPPORTED_FEATURE, "clearBatch is not supported in sharding");
    }

    public int[] executeBatch() throws SQLException
    {
        Error.throwSQLException(ErrorDef.UNSUPPORTED_FEATURE, "executeBatch is not supported in sharding");
        return null;
    }

    public Connection getConnection()
    {
        return mMetaConn;
    }

    public boolean getMoreResults(int aCurrent)
    {
        return false;
    }

    public ResultSet getGeneratedKeys()
    {
        return null;
    }

    public int executeUpdate(String aSql, int aAutoGeneratedKeys) throws SQLException
    {
        Error.throwSQLException(ErrorDef.UNSUPPORTED_FEATURE,
                                "executeUpdate with generatedKeys is not supported in sharding");
        return 0;
    }

    public int executeUpdate(String aSql, int[] aColumnIndexes) throws SQLException
    {
        int sResult;

        try
        {
            List<? extends BaseStatementUnit> sStatementUnits = route(aSql);
            sResult =  mStmtExecutor.executeUpdate(aColumnIndexes, sStatementUnits);
            mShardStmtCtx.setUpdateRowcount(sResult);
            setOneNodeTransactionInfo(mRouteResult.getExecutionUnits());
            getNodeSqlWarnings();
        }
        catch (ShardJdbcException aShardJdbcEx)
        {
            clearRoutedStatementMap(aShardJdbcEx);
            throw aShardJdbcEx;
        }
        finally
        {
            mCurrentResultSet = null;
        }

        return sResult;
    }

    public int executeUpdate(String aSql, String[] aColumnNames) throws SQLException
    {
        int sResult;

        try
        {
            List<? extends BaseStatementUnit> sStatementUnits = route(aSql);
            sResult = mStmtExecutor.executeUpdate(aColumnNames, sStatementUnits);
            mShardStmtCtx.setUpdateRowcount(sResult);
            setOneNodeTransactionInfo(mRouteResult.getExecutionUnits());
            getNodeSqlWarnings();
        }
        catch (ShardJdbcException aShardJdbcEx)
        {
            clearRoutedStatementMap(aShardJdbcEx);
            throw aShardJdbcEx;
        }
        finally
        {
            mCurrentResultSet = null;
        }

        return sResult;
    }

    public boolean execute(String aSql, int aAutoGeneratedKeys) throws SQLException
    {
        Error.throwSQLException(ErrorDef.UNSUPPORTED_FEATURE,
                                "execute with generatedKeys is not supported in sharding");
        return false;
    }

    public boolean execute(String aSql, int[] aColumnIndexes) throws SQLException
    {
        boolean sResult;
        try
        {
            List<? extends BaseStatementUnit> sStatementUnits = route(aSql);
            sResult = mStmtExecutor.execute(aColumnIndexes, sStatementUnits);
            mShardStmtCtx.setUpdateRowcount(getNodeUpdateRowCount());
            setOneNodeTransactionInfo(mRouteResult.getExecutionUnits());
            getNodeSqlWarnings();
        }
        catch (ShardJdbcException aShardJdbcEx)
        {
            clearRoutedStatementMap(aShardJdbcEx);
            throw aShardJdbcEx;
        }
        finally
        {
            mCurrentResultSet = null;
        }

        return sResult;
    }

    public boolean execute(String aSql, String[] aColumnNames) throws SQLException
    {
        boolean sResult;

        try
        {
            List<? extends BaseStatementUnit> sStatementUnits = route(aSql);
            sResult = mStmtExecutor.execute(aColumnNames, sStatementUnits);
            mShardStmtCtx.setUpdateRowcount(getNodeUpdateRowCount());
            setOneNodeTransactionInfo(mRouteResult.getExecutionUnits());
            getNodeSqlWarnings();
        }
        catch (ShardJdbcException aShardJdbcEx)
        {
            clearRoutedStatementMap(aShardJdbcEx);
            throw aShardJdbcEx;
        }
        finally
        {
            mCurrentResultSet = null;
        }

        return sResult;
    }

    private List<? extends BaseStatementUnit> route(String aSql) throws SQLException
    {
        mCurrentResultSet = null;
        mRouteResult = createRoutingEngine().route(aSql, null);
        if (!mShardStmtCtx.isAutoCommitMode())
        {
            touchNodes();
        }
        ExecutorEngine sExecutorEngine = mMetaConn.getExecutorEngine();
        // ExecutorEngine을 통해 병렬로 Statement 객체 생성
        List<StatementUnit> sStatementUnits = sExecutorEngine.generateStatement(
                mRouteResult.getExecutionUnits(),
                new GenerateCallback<StatementUnit>()
                {
                    public StatementUnit generate(SQLExecutionUnit aSqlExecutionUnit) throws SQLException
                    {
                        Connection sConn = mMetaConn.getNodeConnection(aSqlExecutionUnit.getNode());
                        Statement sStatement = getStatement(aSqlExecutionUnit, sConn);
                        mRoutedStatementMap.put(aSqlExecutionUnit.getNode(), sStatement);
                        return new StatementUnit(aSqlExecutionUnit, sStatement);
                    }
                });

        return sStatementUnits;
    }

    private Statement getStatement(SQLExecutionUnit aSqlExecutionUnit, Connection aConn) throws SQLException
    {
        Statement sStatement = mRoutedStatementMap.get(aSqlExecutionUnit.getNode());
        if (sStatement != null) return sStatement;
        
        try
        {
            // routed statemap에 해당 statement가 없다면 새로 생성한다.
            sStatement = aConn.createStatement(mResultSetType, mResultSetConcurrency, mResultSetHoldability);
            mShardStmt.setShardMetaNumber(mMetaConn.getShardMetaNumber());
        }
        catch (ShardFailoverIsNotAvailableException aFailoverEx)
        {
            mMetaConn.getCachedConnections().remove(aSqlExecutionUnit.getNode());
            throw aFailoverEx;
        }
        mShardStmt.replayMethodsInvocation(sStatement);
        
        return sStatement;
    }

    RoutingEngine createRoutingEngine()
    {
        int sShardValueCnt = mShardStmtCtx.getShardAnalyzeResult().getShardValueCount();
        int sShardSubValueCnt = mShardStmtCtx.getShardAnalyzeResult().getShardSubValueCount();
        ShardSplitMethod sSplitMethod = mShardStmtCtx.getShardAnalyzeResult().getShardSplitMethod();
        switch (sSplitMethod)
        {
            case CLONE:
                /* shard value가 없는 경우는 random하게 노드를 선택해야 하며 그렇지 않은
                   경우에는 모든 노드가 실행대상이 된다. */
                return (sShardValueCnt == 0) ? new RandomNodeRoutingEngine(mShardStmtCtx) :
                       new AllNodesRoutingEngine(mShardStmtCtx);
            case SOLO:
                return new AllNodesRoutingEngine(mShardStmtCtx);
            case LIST:
            case RANGE:
            case HASH:
                if (sShardValueCnt >= 0 && sShardSubValueCnt > 0)
                {
                    return new CompositeShardKeyRoutingEngine(mShardStmtCtx);
                }
                else if (sShardValueCnt > 0 && sShardSubValueCnt == 0)
                {
                    return new SimpleShardKeyRoutingEngine(mShardStmtCtx);
                }
                else  // shard value 가 없다면 모든 노드가 수행 대상이 된다.
                {
                    return new AllNodesRoutingEngine(mShardStmtCtx);
                }
        }

        return null;
    }

    /**
     * 트랜잭션 모드일때 노드의 touch 플래그를 셋팅한다. 또한 이때 onenode transaction인 경우에는 <br>
     * 트잰잭션이 유효한지 체크한다.
     * @throws SQLException onenode일때 정상적인 트랜잭션 상태가 아닌 경우
     */
    void touchNodes() throws SQLException
    {
        if (mShardTransactionLevel == ShardTransactionLevel.ONE_NODE)
        {
            /* one node tx에서 1개이상 노드가 선택된 경우 에러 */
            if (mRouteResult.getExecutionUnits().size() > 1)
            {
                Error.throwSQLException(ErrorDef.SHARD_MULTINODE_TRANSACTION_REQUIRED);
            }
            // one node tx에서 이전 노드와 다르면 에러
            DataNode sShardOnTransactionNode = mShardStmtCtx.getShardContextConnect().getShardOnTransactionNode();
            DataNode sCurrentNode = mRouteResult.getExecutionUnits().iterator().next().getNode();
            if (!isValidTransaction(sShardOnTransactionNode, sCurrentNode))
            {
                Error.throwSQLException(ErrorDef.SHARD_INVALID_NODE_TOUCH);
            }
        }

        // touch nodes
        for (SQLExecutionUnit sEach : mRouteResult.getExecutionUnits())
        {
            sEach.getNode().setTouched(true);
        }
    }

    private boolean isValidTransaction(DataNode sOldNode, DataNode sCurrentNode)
    {
        return !mShardStmtCtx.getShardContextConnect().isNodeTransactionStarted() ||
               sOldNode.equals(sCurrentNode);
    }

    void setOneNodeTransactionInfo(Set<SQLExecutionUnit> aSqlExecutionUnits)
    {
        if (aSqlExecutionUnits.size() == 0) return;

        if (mShardTransactionLevel == ShardTransactionLevel.ONE_NODE)
        {
            SQLExecutionUnit sSqlExecutionUnit = aSqlExecutionUnits.iterator().next();
            CmProtocolContextShardConnect sShardContextConnect = mShardStmtCtx.getShardContextConnect();
            sShardContextConnect.setShardOnTransactionNode(sSqlExecutionUnit.getNode());
            sShardContextConnect.setNodeTransactionStarted(true);
        }
    }

    void setShardStmtCtx(CmProtocolContextShardStmt aShardStmtCtx)
    {
        mShardStmtCtx = aShardStmtCtx;
    }

    /**
     * ShardRetryAvailableException이나 STF success 상황인 경우에는 statement가 유효하지 않기 때문에 <br>
     * 해당 statement를 routedStatementMap에서 clear한다.
     * @param aShardJdbcEx ShardJdbcException 객체
     */
    void clearRoutedStatementMap(ShardJdbcException aShardJdbcEx)
    {
        ShardNodeConfig sNodeConfig = mMetaConn.getShardNodeConfig();
        String sNodeName = aShardJdbcEx.getNodeName();
        mRoutedStatementMap.remove(sNodeConfig.getNodeByName(sNodeName));
    }

    /**
     * RoutedStatementMap을 순환하면서 SqlWarning이 있으면 조합한다.
     * @throws SQLException 노드 Statement에서 SqlWarning을 가져오는 도중 에러가 발생한 경우
     */
    void getNodeSqlWarnings() throws SQLException
    {
        boolean sWarningExists = false;
        for (Statement sEach : mRoutedStatementMap.values())
        {
            if (sEach.getWarnings() != null)
            {
                sWarningExists = true;
                break;
            }
        }
        if (!sWarningExists) return;

        // 순서대로 SQLWarning을 저장하기 위해 TreeMap을 사용한다.
        Map<DataNode, Statement> sSortedMap = new TreeMap<DataNode, Statement>(mRoutedStatementMap);
        for (Map.Entry<DataNode, Statement> sEntry : sSortedMap.entrySet())
        {
            Statement sStmt = sEntry.getValue();
            SQLWarning sSqlWarning = sStmt.getWarnings();

            if (sSqlWarning != null)
            {
                if (sSqlWarning.getErrorCode() == ErrorDef.SHARD_META_NUMBER_INVALID)
                {
                    StringBuilder sSb = new StringBuilder();
                    sSb.append("[").append(sEntry.getKey().getNodeName()).append("] The ").append("Execute");
                    mSqlwarning = Error.createWarning(mSqlwarning,
                                                      ErrorDef.SHARD_OPERATION_FAILED,
                                                      sSb.toString(), sSqlWarning.getMessage());
                }
                else
                {
                    getSQLWarningFromDataNode(sEntry.getKey().getNodeName(), sSqlWarning);
                }
            }
        }
    }

    private void getSQLWarningFromDataNode(String aNodeName, SQLWarning aWarning)
    {
        StringBuilder sSb = new StringBuilder();
        sSb.append("[").append(aNodeName).append("] ").append(aWarning.getMessage());
        SQLWarning sNewWarning = new SQLWarning(sSb.toString(), aWarning.getSQLState(),
                                                aWarning.getErrorCode());
        if (mSqlwarning == null)
        {
            mSqlwarning = sNewWarning;
        }
        else
        {
            mSqlwarning.setNextWarning(sNewWarning);
        }

    }

    public boolean isPrepared()
    {
        for (Statement sEach : mRoutedStatementMap.values())
        {
            if (((AltibaseStatement)sEach).isPrepared())
            {
                return true;
            }
        }

        return false;
    }

    public boolean hasNoData()
    {
        for (Statement sEach : mRoutedStatementMap.values())
        {
            AltibaseStatement sStmt = (AltibaseStatement)sEach;
            if (sStmt.isPrepared() && !sStmt.cursorhasNoData())
            {
                return false;
            }
        }

        return true;
    }

    public void removeFromStmtRouteMap(DataNode aNode)
    {
        // BUG-46513 connection이 정리될때 Statement Free를 수행하므로 여기서는 메핑정보만 정리한다.
        mRoutedStatementMap.remove(aNode);
    }

    protected int getNodeUpdateRowCount()
    {
        long sResult = 0;
        boolean sHasResult = false;
        for (Statement sEach : mRoutedStatementMap.values())
        {
            int sUpdatedCount = 0;
            try
            {
                sUpdatedCount = sEach.getUpdateCount();
            }
            catch (SQLException aEx)
            {
                /* BUG-46513 lazyConnect 가 false일때는 statement가 아직 실행전일수 있기때문에 해당 exception이
                   발생하면 로그를 남기고 넘어간다. */
                if (aEx.getErrorCode() == ErrorDef.STATEMENT_NOT_YET_EXECUTED)
                {
                    shard_log("Statement not yet executed : {0}", sEach);
                    continue;
                }
            }
            if (sUpdatedCount > -1)
            {
                sHasResult = true;
                sResult += sUpdatedCount;
            }
        }
        if (sResult > Integer.MAX_VALUE)
        {
            sResult = Integer.MAX_VALUE;
        }

        return sHasResult ? Long.valueOf(sResult).intValue() : -1;
    }

    void throwSQLExceptionIfNecessary(List<SQLException> aExceptions) throws SQLException
    {
        if (aExceptions.isEmpty())
        {
            return;
        }

        SQLException sException = new SQLException();
        for (SQLException sEach : aExceptions)
        {
            sException.setNextException(sEach);
        }

        throw sException;
    }
}
