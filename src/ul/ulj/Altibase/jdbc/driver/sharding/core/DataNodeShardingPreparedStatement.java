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

import Altibase.jdbc.driver.datatype.Column;
import Altibase.jdbc.driver.ex.ErrorDef;
import Altibase.jdbc.driver.ex.ShardJdbcException;
import Altibase.jdbc.driver.ex.ShardFailoverIsNotAvailableException;
import Altibase.jdbc.driver.sharding.executor.*;
import Altibase.jdbc.driver.sharding.merger.IteratorStreamResultSetMerger;
import Altibase.jdbc.driver.sharding.routing.*;

import java.sql.*;
import java.util.*;

import static Altibase.jdbc.driver.sharding.util.ShardingTraceLogger.shard_log;

public class DataNodeShardingPreparedStatement extends DataNodeShardingStatement implements InternalShardingPreparedStatement
{
    RoutingEngine                                     mRoutingEngine;
    String                                            mSql;
    List<Column>                                      mParameters;
    private int                                       mBatchCount;
    private Map<DataNode, BatchPreparedStatementUnit> mBatchStmtUnitMap;
    private PreparedStatementExecutor                 mPreparedStmtExecutor;

    DataNodeShardingPreparedStatement(AltibaseShardingConnection aShardCon, String aSql, int aResultSetType,
                                      int aResultSetConcurrency, int aResultSetHoldability,
                                      AltibaseShardingPreparedStatement aShardStmt) throws SQLException
    {
        super(aShardCon, aResultSetType, aResultSetConcurrency, aResultSetHoldability, aShardStmt);
        mSql = aSql;
        mBatchStmtUnitMap = new LinkedHashMap<DataNode, BatchPreparedStatementUnit>();
        mRoutingEngine = createRoutingEngine();
        mPreparedStmtExecutor = new PreparedStatementExecutor(mMetaConn.getExecutorEngine());
        prepareNodeDirect(aShardCon, aSql);
        // BUG-46513 Meta 커넥션에 있는 SMN값을 셋팅한다.
        mShardStmt.setShardMetaNumber(mMetaConn.getShardMetaNumber());
        mParameters = aShardStmt.getParameters();
    }

    private void prepareNodeDirect(AltibaseShardingConnection aShardCon, String aSql) throws SQLException
    {
        if (aShardCon.isLazyNodeConnect()) return;

        ShardRangeList sRangeList = mShardStmtCtx.getShardAnalyzeResult().getShardRangeList();
        Map<DataNode, Connection> sNodeConnMap = aShardCon.getCachedConnections();
        shard_log("[RANGELIST_INFO] {0}", sRangeList.getRangeList());
        prepareNodeStatements(aSql, sRangeList, sNodeConnMap);
        boolean sDefaultNodeIncluded = false;
        int sDefaultNodeId = mShardStmtCtx.getShardAnalyzeResult().getShardDefaultNodeID();
        if (sDefaultNodeId >= 0)
        {
            for (ShardRange sEach : sRangeList.getRangeList())
            {
                if (sEach.getNode().getNodeId() == sDefaultNodeId)
                {
                    sDefaultNodeIncluded = true;
                    break;
                }
            }
            if (!sDefaultNodeIncluded)
            {
                DataNode sDefaultNode = mMetaConn.getShardNodeConfig().getNode(sDefaultNodeId);
                Connection sNodeConn = sNodeConnMap.get(sDefaultNode);
                PreparedStatement sPstmt = sNodeConn.prepareStatement(aSql, mResultSetType,
                                                                      mResultSetConcurrency,
                                                                      mResultSetHoldability);
                mRoutedStatementMap.put(sDefaultNode, sPstmt);
            }
        }
    }

    /**
     * shard_lazy_connect가 false일때 range의 범위만큼 루프를 돌면서 데이터노드의 Statement를 생성한다.
     * 
     * @param aSql sql문
     * @param aRangeList 샤드 범위 객체
     * @param aNodeConnMap 노드커넥션 맵
     * @throws SQLException 노드 Statement 생성시 에러가 발생한 경우
     */
    void prepareNodeStatements(String aSql, ShardRangeList aRangeList, Map<DataNode, Connection> aNodeConnMap) throws SQLException
    {
        for (ShardRange sEach : aRangeList.getRangeList())
        {
            DataNode sNode = sEach.getNode();
            Connection sNodeConn = aNodeConnMap.get(sNode);
            // BUG-46513 새로 추가된 노드는 커넥션을 새로 생성한다.
            if (sNodeConn == null)
            {
                sNodeConn = mMetaConn.getNodeConnection(sNode);
            }
            createStatementForLazyConnectOff(sNodeConn, sNode, aSql);
        }
    }

    void createStatementForLazyConnectOff(Connection aNodeConn, DataNode aNode, String aSql) throws SQLException
    {
        PreparedStatement sPstmt = aNodeConn.prepareStatement(aSql, mResultSetType,
                                                              mResultSetConcurrency, mResultSetHoldability);
        mRoutedStatementMap.put(aNode, sPstmt);
    }

    public ResultSet executeQuery() throws SQLException
    {
        try
        {
            ResultSet sResult;
            List<? extends BaseStatementUnit> sStatementUnits = route();
            if (!mShardStmtCtx.isAutoCommitMode())
            {
                touchNodes();
            }

            List<ResultSet> sResultSets = mPreparedStmtExecutor.executeQuery(sStatementUnits);
            sResult = new AltibaseShardingResultSet(sResultSets, new IteratorStreamResultSetMerger(sResultSets), mShardStmt);
            // BUG-46513 update된 row count를 Statement context에 저장한다.
            mShardStmtCtx.setUpdateRowcount(getNodeUpdateRowCount());
            setOneNodeTransactionInfo(mRouteResult.getExecutionUnits());
            getNodeSqlWarnings();
            mCurrentResultSet = sResult;
            return sResult;
        }
        catch (ShardJdbcException aShardJdbcEx)
        {
            clearRoutedStatementMap(aShardJdbcEx);
            throw aShardJdbcEx;
        }
    }

    public int executeUpdate() throws SQLException
    {
        try
        {
            List<? extends BaseStatementUnit> sStatementUnits = route();
            if (!mShardStmtCtx.isAutoCommitMode())
            {
                touchNodes();
            }

            int sUpdateCnt = mPreparedStmtExecutor.executeUpdate(sStatementUnits);
            // BUG-46513 update된 row count를 Statement context에 저장한다.
            mShardStmtCtx.setUpdateRowcount(sUpdateCnt);
            setOneNodeTransactionInfo(mRouteResult.getExecutionUnits());
            getNodeSqlWarnings();

            return sUpdateCnt;
        }
        catch (ShardJdbcException aShardJdbcEx)
        {
            clearRoutedStatementMap(aShardJdbcEx);
            throw aShardJdbcEx;
        }
    }

    public int[] executeBatch() throws SQLException
    {
        try
        {
            return new BatchPreparedStatementExecutor(mMetaConn.getExecutorEngine(),
                                                      mBatchStmtUnitMap, mBatchCount).executeBatch();
        }
        finally
        {
            getNodeSqlWarnings();
            clearBatch();
        }
    }


    public void clearParameters() throws SQLException
    {
        mCurrentResultSet = null;

        for (Statement sEach : mRoutedStatementMap.values())
        {
            ((PreparedStatement)sEach).clearParameters();
        }
    }

    public boolean execute() throws SQLException
    {
        try
        {
            List<? extends BaseStatementUnit> sStatementUnits = route();
            if (!mShardStmtCtx.isAutoCommitMode())
            {
                touchNodes();
            }

            boolean sResult = mPreparedStmtExecutor.execute(sStatementUnits);
            // BUG-46513 update된 row count를 Statement context에 저장한다.
            mShardStmtCtx.setUpdateRowcount(getNodeUpdateRowCount());
            setOneNodeTransactionInfo(mRouteResult.getExecutionUnits());
            getNodeSqlWarnings();

            return sResult;
        }
        catch (ShardJdbcException aShardJdbcEx)
        {
            clearRoutedStatementMap(aShardJdbcEx);
            throw aShardJdbcEx;
        }
    }

    public void addBatch() throws SQLException
    {
        try
        {
            for (BatchPreparedStatementUnit sEach : routeBatch())
            {
                sEach.getStatement().addBatch();
                sEach.mapAddBatchCount(mBatchCount);
            }
            if (!mShardStmtCtx.isAutoCommitMode())
            {
                touchNodes();
            }
            mBatchCount++;
        }
        finally
        {
            mCurrentResultSet = null;
        }
    }

    public ResultSetMetaData getMetaData() throws SQLException
    {
        route();

        if (mRoutedStatementMap.size() > 0)
        {
            return ((PreparedStatement)mRoutedStatementMap.values().iterator().next()).getMetaData();
        }

        return null;
    }

    public ParameterMetaData getParameterMetaData() throws SQLException
    {
        route();

        if (mRoutedStatementMap.size() > 0)
        {
            return ((PreparedStatement)mRoutedStatementMap.values().iterator().next()).getParameterMetaData();
        }

        return null;
    }

    public void clearBatch() throws SQLException
    {
        mCurrentResultSet = null;
        clearParameters();
        mBatchStmtUnitMap.clear();
        mBatchCount = 0;
    }

    /**
     * 샤드키값을 기준으로 해당하는 노드들의 PreparedStatement객체를 리턴한다.
     * @return PreparedStatement 객체
     */
    protected List<? extends BaseStatementUnit> route() throws SQLException
    {
        mCurrentResultSet = null;
        mRouteResult = mRoutingEngine.route(mSql, mParameters);
        ExecutorEngine sExecutorEngine = mMetaConn.getExecutorEngine();

        List<PreparedStatementUnit> sResult;

        // ExecutorEngine을 통해 병렬로 PreparedStatement 객체를 생성한다.
        sResult = sExecutorEngine.generateStatement(mRouteResult.getExecutionUnits(),
                                                    new GenerateCallback<PreparedStatementUnit>()
        {
            public PreparedStatementUnit generate(SQLExecutionUnit aSqlExecutionUnit) throws SQLException
            {
                Connection sNodeCon = mMetaConn.getNodeConnection(aSqlExecutionUnit.getNode());
                PreparedStatement sStmt = getPreparedStatement(aSqlExecutionUnit, sNodeCon);
                mShardStmt.replayMethodsInvocation(sStmt);
                mShardStmt.replaySetParameter(sStmt);
                mRoutedStatementMap.put(aSqlExecutionUnit.getNode(), sStmt);
                return new PreparedStatementUnit(aSqlExecutionUnit, sStmt, mParameters);
            }
        });

        return sResult;
    }

    private List<BatchPreparedStatementUnit> routeBatch() throws SQLException
    {
        List<BatchPreparedStatementUnit> sResult = new ArrayList<BatchPreparedStatementUnit>();
        mRouteResult = mRoutingEngine.route(mSql, mParameters);
        for (SQLExecutionUnit sEach : mRouteResult.getExecutionUnits())
        {
            BatchPreparedStatementUnit sBatchStatementUnit = getPreparedBatchStatement(sEach);
            mShardStmt.replaySetParameter(sBatchStatementUnit.getStatement());
            sResult.add(sBatchStatementUnit);
        }

        return sResult;
    }

    private BatchPreparedStatementUnit getPreparedBatchStatement(SQLExecutionUnit aSqlExecutionUnit) throws SQLException
    {
        BatchPreparedStatementUnit sBatchStmtUnit = mBatchStmtUnitMap.get(aSqlExecutionUnit.getNode());
        if (sBatchStmtUnit == null)
        {
            Connection sNodeCon = mMetaConn.getNodeConnection(aSqlExecutionUnit.getNode());
            PreparedStatement sStmt = getPreparedStatement(aSqlExecutionUnit, sNodeCon);
            sBatchStmtUnit = new BatchPreparedStatementUnit(aSqlExecutionUnit, sStmt, mParameters);
            mBatchStmtUnitMap.put(aSqlExecutionUnit.getNode(), sBatchStmtUnit);
        }

        return sBatchStmtUnit;
    }

    private PreparedStatement getPreparedStatement(SQLExecutionUnit aSqlExecutionUnit, Connection aConn) throws SQLException
    {
        PreparedStatement sStmt = (PreparedStatement)mRoutedStatementMap.get(aSqlExecutionUnit.getNode());
        if (sStmt != null) return sStmt;

        try
        {
            sStmt = aConn.prepareStatement(aSqlExecutionUnit.getSql(), mResultSetType, mResultSetConcurrency, mResultSetHoldability);
            shard_log("(NODE PREPARE) {0}", aSqlExecutionUnit);
            // BUG-46513 Meta 커넥션에 있는 SMN값을 셋팅한다.
            mShardStmt.setShardMetaNumber(mMetaConn.getShardMetaNumber());
        }
        catch (ShardFailoverIsNotAvailableException aShardFailoverEx)
        {
            mMetaConn.getCachedConnections().remove(aSqlExecutionUnit.getNode());
            throw aShardFailoverEx;
        }
        catch (SQLException aEx)
        {
            if (ErrorDef.getErrorState(ErrorDef.FAILOVER_SUCCESS).equals(aEx.getSQLState()))
            {
                mMetaConn.getCachedConnections().remove(aSqlExecutionUnit.getNode());
            }
            throw aEx;
        }

        return sStmt;
    }

}
