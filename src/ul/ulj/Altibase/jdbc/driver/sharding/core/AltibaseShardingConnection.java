/*
 *  Copyright (c) 1999~2017, Altibase Corp. and/or its affiliates. All rights reserved.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License, version 3,
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

package Altibase.jdbc.driver.sharding.core;

import Altibase.jdbc.driver.AltibaseConnection;
import Altibase.jdbc.driver.AltibaseFailover;
import Altibase.jdbc.driver.cm.*;
import Altibase.jdbc.driver.ex.Error;
import Altibase.jdbc.driver.ex.ErrorDef;
import Altibase.jdbc.driver.ex.ShardJdbcException;
import Altibase.jdbc.driver.sharding.executor.ConnectionParallelProcessCallback;
import Altibase.jdbc.driver.sharding.executor.ExecutorEngine;
import Altibase.jdbc.driver.sharding.executor.ExecutorExceptionHandler;
import Altibase.jdbc.driver.sharding.executor.MultiThreadExecutorEngine;
import Altibase.jdbc.driver.util.AltiSqlProcessor;
import Altibase.jdbc.driver.util.AltibaseProperties;

import javax.sql.DataSource;
import java.sql.*;
import java.util.*;
import java.util.concurrent.ConcurrentHashMap;

import static Altibase.jdbc.driver.sharding.core.ShardTransactionLevel.*;
import static Altibase.jdbc.driver.sharding.util.ShardingTraceLogger.shard_log;
import static Altibase.jdbc.driver.util.AltibaseProperties.*;

/**
 * java.sql.Connection 을 구현하며 메타서버와 커넥션을 생성한 후 데이터노드 리스트를 전달받는다.
 */
public class AltibaseShardingConnection extends JdbcMethodInvoker implements Connection
{
    public static final byte                DEFAULT_SHARD_CLIENT_TYPE    = (byte)1;
    private AltibaseConnection              mMetaConnection;
    private CmProtocolContextShardConnect   mShardContextConnect;
    private CmShardProtocol                 mCmShardProtocol;
    private Map<DataNode, Connection>       mCachedConnections;
    private AltibaseProperties              mProps;
    private ExecutorEngine                  mExecutorEngine;
    private boolean                         mIsClosed;
    private SQLWarning                      mWarning;
    private int                             mTransactionIsolation        = TRANSACTION_READ_UNCOMMITTED;
    private int                             mDefaultResultSetType        = ResultSet.TYPE_FORWARD_ONLY;
    private int                             mDefaultResultSetConcurrency = ResultSet.CONCUR_READ_ONLY;
    private int                             mDefaultResultSetHoldability = ResultSet.CLOSE_CURSORS_AT_COMMIT;
    private boolean                         mAutoCommit                  = true;
    private ShardTransactionLevel           mShardTransactionLevel;
    private LinkedList<Statement>           mOpenStatements;
    private boolean                         mIsLazyNodeConnect;

    public AltibaseShardingConnection(AltibaseProperties aProp) throws SQLException
    {
        mShardContextConnect = new CmProtocolContextShardConnect(this);
        mMetaConnection = new AltibaseConnection(aProp, null, this);
        mProps = mMetaConnection.getProp();
        mCmShardProtocol = new CmShardProtocol(mShardContextConnect);
        mOpenStatements = new LinkedList<Statement>();
        mShardContextConnect.setShardConnType(mProps.isSet(PROP_SHARD_CONNTYPE) ?
                                              CmConnType.toConnType(mProps.getShardConnType()) :
                                              CmConnType.toConnType(mProps.getConnType()));
        mCachedConnections = new ConcurrentHashMap<DataNode, Connection>();
        mIsLazyNodeConnect = mProps.isShardLazyConnectEnabled();

        try
        {
            mCmShardProtocol.getNodeList();
            // BUG-46513 getNodeList를 호출했을때 서버에러가 발생한 경우 처리해 준다.
            if (mShardContextConnect.getError() != null)
            {
                mWarning = Error.processServerError(mWarning, mShardContextConnect.getError());
            }
            mShardContextConnect.createDataSources(mProps);
            // PROJ-2690 shard_lazy_connect가 false일때는 Meta접속 후 바로 노드커넥션을 확보한다.
            if (!mIsLazyNodeConnect)
            {
                for (DataNode sEach : mShardContextConnect.getShardNodeConfig().getDataNodes())
                {
                    try
                    {
                        getNodeConnection(sEach);
                    }
                    catch (SQLException aEx)
                    {
                        // BUG-46513 lazy_connect false일 경우에는 node connect시 에러가 발생했을때 node name을 추가해 줘야 한다.
                        StringBuilder sSb = new StringBuilder();
                        sSb.append("[").append(sEach.getNodeName()).append("] ").append(aEx.getMessage());
                        String sSqlState = aEx.getSQLState();
                        int sErrorCode = aEx.getErrorCode();
                        throw new SQLException(sSb.toString(), sSqlState, sErrorCode);
                    }
                }
            }
        }
        catch (SQLException aException)
        {
            AltibaseFailover.trySTF(mMetaConnection.failoverContext(), aException);
        }
        mShardContextConnect.setAutoCommit(mAutoCommit);
        mExecutorEngine = new MultiThreadExecutorEngine(this);
        mShardTransactionLevel = mProps.getShardTransactionLevel();
        mShardContextConnect.setShardTransactionLevel(mShardTransactionLevel);
        mShardContextConnect.setServerCharSet(mMetaConnection.getServerCharacterSet());
    }

    public String nativeSQL(String aSql)
    {
        return AltiSqlProcessor.processEscape(aSql);
    }

    public void setAutoCommit(boolean aAutoCommit) throws SQLException
    {
        mAutoCommit = aAutoCommit;
        mShardContextConnect.setAutoCommit(aAutoCommit);
        recordMethodInvocation(Connection.class, "setAutoCommit", new Class[] {boolean.class},  new
                Object[] {aAutoCommit});
        for (Connection sEach : mCachedConnections.values())
        {
            sEach.setAutoCommit(aAutoCommit);
        }
        mMetaConnection.setAutoCommit(mAutoCommit);
        sendShardTransactionLevel();
    }

    private void sendShardTransactionLevel() throws SQLException
    {
        if (mShardTransactionLevel != ONE_NODE)
        {
            mShardContextConnect.clearProperties();
            mShardContextConnect.addProperty(PROP_CODE_SHARD_TRANSACTION_LEVEL,
                                             mShardTransactionLevel.getValue());
            CmProtocol.sendProperties(mShardContextConnect);
            if (mShardContextConnect.getError() != null)
            {
                mWarning = Error.processServerError(mWarning, mShardContextConnect.getError());
            }
        }
    }

    public boolean getAutoCommit()
    {
        return mAutoCommit;
    }

    public void commit() throws SQLException
    {
        doTransaction(new TransactionCallback()
        {
            public void doTransaction(Connection aConn) throws SQLException
            {
                shard_log("(NODE CONNECTION COMMIT)");
                aConn.commit();
            }
        }, true);
    }

    public void rollback() throws SQLException
    {
        doTransaction(new TransactionCallback()
        {
            public void doTransaction(Connection aConn) throws SQLException
            {
                shard_log("(NODE CONNECTION ROLLBACK)");
                aConn.rollback();
            }
        }, false);
    }

    private void doTransaction(final TransactionCallback aTransactionCallback,
                               boolean aIsCommit) throws SQLException
    {
        ShardNodeConfig sShardNodeConfig = mShardContextConnect.getShardNodeConfig();
        if (mShardTransactionLevel == ONE_NODE)
        {
            shard_log("(ONE NODE TRANSACTION)");
            DataNode sNode;
            if (mShardContextConnect.isNodeTransactionStarted())
            {
                sNode = mShardContextConnect.getShardOnTransactionNode();
                shard_log("(ON TRANSACTION NODE) {0}", sNode.getNodeName());
                aTransactionCallback.doTransaction(getNodeConnection(sNode));
            }
            mShardContextConnect.setShardOnTransactionNode(null);
            mShardContextConnect.setNodeTransactionStarted(false);
        }
        else if (mShardTransactionLevel == MULTI_NODE)
        {
            shard_log("(MULTI NODE TRANSACTION)");
            doMultiNodeTransaction(aTransactionCallback);
        }
        else
        {
            shard_log("(GLOBAL TRANSACTION)");
            List<Connection> sTouchedConnections = getTouchedNodeConnections();
            /* 2노드 이상이고 Commit인 경우만 global transaction을 사용한다. */
            if (sTouchedConnections.size() >= 2 && aIsCommit)
            {
                sendShardTransactionCommitRequest(sShardNodeConfig.getTouchedNodeList());
            }
            else
            {
                doMultiNodeTransaction(aTransactionCallback);
            }
        }

        for (DataNode sEach : sShardNodeConfig.getDataNodes())
        {
            sEach.setTouched(false);
        }

        if (mShardContextConnect.isTouched())  // meta node touch
        {
            mShardContextConnect.setTouched(false);
        }

        getNodeSqlWarnings(aIsCommit);

        // BUG-46513 smn값이 작다면 갱신해야 한다.
        if (shouldUpdateShardMetaNumber())
        {
            updateShardMetaNumber();
        }
    }

    /**
     * 데이터노드로부터 SQLWarning이 생성되었다면 조합하여 mSqlWarning에 저장한다.<br>
     * 이때 warning 메세지에 노드네임을 포함시킨다.
     * @throws SQLException 노드로부터 SQLWarning을 받는 도중 에러가 발생한 경우.
     */
    private void getNodeSqlWarnings(boolean aIsCommit) throws SQLException
    {
        // node name 순으로 정렬하여 SQLWarning을 저장하기 위해 TreeMap 사용
        Map<DataNode, Connection> sSortedMap = new TreeMap<DataNode, Connection>(mCachedConnections);
        for (Connection sEach : sSortedMap.values())
        {
            SQLWarning sSqlWarning = sEach.getWarnings();
            if (sSqlWarning != null)
            {
                if (sSqlWarning.getErrorCode() == ErrorDef.SHARD_META_NUMBER_INVALID)
                {
                    StringBuilder sSb = new StringBuilder();
                    sSb.append("[").append(((AltibaseConnection)sEach).getNodeName()).append("] The ")
                       .append((aIsCommit) ? "Commit" : "Rollback");
                    mWarning = Error.createWarning(mWarning,
                                                   ErrorDef.SHARD_OPERATION_FAILED,
                                                   sSb.toString(), sSqlWarning.getMessage());
                }
                else
                {
                    getSQLWarningFromDataNode(sEach);
                }
            }
        }
    }

    private void doMultiNodeTransaction(final TransactionCallback aTransactionCallback) throws SQLException
    {
        mExecutorEngine.doTransaction(getTouchedNodeConnections(),
                new ConnectionParallelProcessCallback()
                    {
                        public Void processInParallel(Connection aConn) throws SQLException
                        {
                            try
                            {
                                aTransactionCallback.doTransaction(aConn);
                            }
                            catch (ShardJdbcException aShardJdbcEx)
                            {
                                throw aShardJdbcEx;
                            }
                            catch (SQLException aException)
                            {
                                String sNodeName = ((AltibaseConnection)aConn).getNodeName();
                                ExecutorExceptionHandler.handleException(aException, sNodeName);
                            }
                            return null;
                        }
                    });
    }

    private void sendShardTransactionCommitRequest(List<DataNode> aTouchedNodeList) throws SQLException
    {
        if (mMetaConnection.isClosed())
        {
            Error.throwSQLException(ErrorDef.CLOSED_CONNECTION);
        }

        if (!mAutoCommit)
        {
            try
            {
                shard_log("(SEND SHARD TRANSACTION COMMIT REQUEST) {0}", aTouchedNodeList);
                mCmShardProtocol.sendShardTransactionCommitRequest(aTouchedNodeList);
            }
            catch (SQLException aEx)
            {
                AltibaseFailover.trySTF(mMetaConnection.failoverContext(), aEx);
            }
            if (mShardContextConnect.getError() != null)
            {
                mWarning = Error.processServerError(mWarning, mShardContextConnect.getError());
            }
        }
    }

    private List<Connection> getTouchedNodeConnections()
    {
        List<Connection> sResult = new ArrayList<Connection>();
        for (Map.Entry<DataNode, Connection> sEntry : mCachedConnections.entrySet())
        {
            DataNode sNode = sEntry.getKey();
            if (sNode.isTouched() && sNode.hasNoErrorOnExecute())
            {
                sResult.add(sEntry.getValue());
                shard_log("(GET TOUCHED NODE CONNECTIONS) {0}", sNode);
            }
        }
        if (mShardContextConnect.isTouched()) // PROJ-2690 meta connection 이 touch 된 경우
        {
            shard_log("(GET TOUCHED META CONNECTIONS)");
            sResult.add(mMetaConnection);
        }

        return sResult;
    }

    public void rollback(Savepoint aSavepoint) throws SQLException
    {
        Error.throwSQLException(ErrorDef.UNSUPPORTED_FEATURE,
                                "rollback(savepoint) is not supported in sharding");
    }

    public void close() throws SQLException
    {
        if (mIsClosed) return;
        closeAllOpenStatements();

        List<SQLException> sExceptions = new LinkedList<SQLException>();
        for (Map.Entry<DataNode, Connection> sEntry : mCachedConnections.entrySet())
        {
            try
            {
                Connection sCon = sEntry.getValue();
                shard_log("(NODE CONNECTION CLOSE) {0}", sEntry);
                sCon.close();
            }
            catch (SQLException sEx)
            {
                sExceptions.add(sEx);
            }
            mCachedConnections.remove(sEntry.getKey());
        }
        throwSQLExceptionIfNecessary(sExceptions);
        shard_log("(META CONNECTION CLOSE) {0}", mMetaConnection);
        mMetaConnection.close();
        mIsClosed = true;
    }

    private void closeAllOpenStatements() throws SQLException
    {
        List<SQLException> sExceptions = new LinkedList<SQLException>();

        synchronized (mOpenStatements)
        {
            while (!mOpenStatements.isEmpty())
            {
                Statement sStmt = mOpenStatements.getFirst();
                try
                {
                    sStmt.close();
                }
                catch (SQLException aException)
                {
                    // BUG-46513 close 도중 error 가 발생해도 일단 다 닫고 나중에 체인으로 묶는다.
                    sExceptions.add(aException);
                }
            }
        }
        throwSQLExceptionIfNecessary(sExceptions);
    }

    void registerStatement(AltibaseShardingStatement aStatement)
    {
        synchronized (mOpenStatements)
        {
            mOpenStatements.add(aStatement);
        }
    }

    void unregisterStatement(AltibaseShardingStatement aStatement)
    {
        synchronized (mOpenStatements)
        {
            mOpenStatements.remove(aStatement);
        }
    }

    public boolean isClosed()
    {
        return mIsClosed;
    }

    public DatabaseMetaData getMetaData() throws SQLException
    {
        // BUG-46513 굳이 node connection에서 meta data값을 가져오지 않고 바로 meta connection에서 가져온다.
        return mMetaConnection.getMetaData();
    }

    public void setReadOnly(boolean aReadOnly)
    {
        mWarning = (aReadOnly) ? Error.createWarning(mWarning,
                                                     ErrorDef.READONLY_CONNECTION_NOT_SUPPORTED) : mWarning;
    }

    public boolean isReadOnly()
    {
        return false;
    }

    public void setCatalog(String catalog)
    {
        // 아무런 동작을 하지 않는다. dbname을 바꿀 수 없다.
        mWarning = Error.createWarning(mWarning, ErrorDef.CANNOT_RENAME_DB_NAME);
    }

    public String getCatalog()
    {
        return mProps.getDatabase();
    }

    public void setTransactionIsolation(int aLevel) throws SQLException
    {
        mTransactionIsolation = aLevel;
        recordMethodInvocation(Connection.class, "setTransactionIsolation", new Class[] {int.class}, new Object[] {aLevel});
        for (Connection sEach : mCachedConnections.values())
        {
            sEach.setTransactionIsolation(aLevel);
        }
    }

    public int getTransactionIsolation()
    {
        return mTransactionIsolation;
    }

    public SQLWarning getWarnings()
    {
        return mWarning;
    }

    public void clearWarnings() throws SQLException
    {
        mWarning = null;
        for (Connection sEach : mCachedConnections.values())
        {
            sEach.clearWarnings();
        }
    }

    public Statement createStatement() throws SQLException
    {
        return new AltibaseShardingStatement(this);
    }

    public Statement createStatement(int aResultSetType, int aResultSetConcurrency) throws SQLException
    {
        return new AltibaseShardingStatement(this, aResultSetType, aResultSetConcurrency);
    }

    public Statement createStatement(int aResultSetType, int aResultSetConcurrency,
                                     int aResultSetHoldability) throws SQLException
    {
        return new AltibaseShardingStatement(this, aResultSetType, aResultSetConcurrency,
                                             aResultSetHoldability);
    }

    public PreparedStatement prepareStatement(String aSql) throws SQLException
    {
        return new AltibaseShardingPreparedStatement(this, aSql, mDefaultResultSetType,
                                                     mDefaultResultSetConcurrency,
                                                     mDefaultResultSetHoldability);
    }

    public PreparedStatement prepareStatement(String aSql, int aResultSetType,
                                              int aResultSetConcurrency) throws SQLException
    {
        return new AltibaseShardingPreparedStatement(this, aSql, aResultSetType, aResultSetConcurrency,
                                                     mDefaultResultSetHoldability);
    }

    public PreparedStatement prepareStatement(String aSql, int aResultSetType, int aResultSetConcurrency,
                                              int aResultSetHoldability) throws SQLException
    {
        return new AltibaseShardingPreparedStatement(this, aSql, aResultSetType, aResultSetConcurrency,
                                                     aResultSetHoldability);
    }

    public PreparedStatement prepareStatement(String aSql, int aAutoGeneratedKeys) throws SQLException
    {
        Error.throwSQLException(ErrorDef.UNSUPPORTED_FEATURE, "autoGeneratedKeys is not supported in sharding.");
        return null;
    }

    public PreparedStatement prepareStatement(String aSql, int[] aColumnIndexes) throws SQLException
    {
        Error.throwSQLException(ErrorDef.UNSUPPORTED_FEATURE, "autoGeneratedKeys is not supported in sharding.");
        return null;
    }

    public PreparedStatement prepareStatement(String aSql, String[] aColumnNames) throws SQLException
    {
        Error.throwSQLException(ErrorDef.UNSUPPORTED_FEATURE, "autoGeneratedKeys is not supported in sharding.");
        return null;
    }


    public CallableStatement prepareCall(String aSql) throws SQLException
    {
        return new AltibaseShardingCallableStatement(this, aSql, mDefaultResultSetType,
                                                     mDefaultResultSetConcurrency,
                                                     mDefaultResultSetHoldability);
    }

    public CallableStatement prepareCall(String aSql, int aResultSetType, int aResultSetConcurrency) throws SQLException
    {
        return new AltibaseShardingCallableStatement(this, aSql, aResultSetType,
                                                     aResultSetConcurrency,
                                                     mDefaultResultSetHoldability);
    }

    public CallableStatement prepareCall(String aSql, int aResultSetType, int aResultSetConcurrency,
                                         int aResultSetHoldability) throws SQLException
    {
        return new AltibaseShardingCallableStatement(this, aSql, aResultSetType,
                                                     aResultSetConcurrency,
                                                     aResultSetHoldability);
    }

    public Map<String, Class<?>> getTypeMap() throws SQLException
    {
        Error.throwSQLException(ErrorDef.UNSUPPORTED_FEATURE, "getTypeMap is not supported");
        return null;
    }

    public void setTypeMap(Map aMap) throws SQLException
    {
        Error.throwSQLException(ErrorDef.UNSUPPORTED_FEATURE, "User defined type");
    }

    public void setHoldability(int aHoldability)
    {
    }

    public int getHoldability()
    {
        return mDefaultResultSetHoldability;
    }

    public Savepoint setSavepoint() throws SQLException
    {
        Error.throwSQLException(ErrorDef.UNSUPPORTED_FEATURE, "setSavepoint is not supported in sharding");
        return null;
    }

    public Savepoint setSavepoint(String aName) throws SQLException
    {
        Error.throwSQLException(ErrorDef.UNSUPPORTED_FEATURE, "setSavepoint is not supported in sharding");
        return null;
    }

    public void releaseSavepoint(Savepoint aSavepoint) throws SQLException
    {
        Error.throwSQLException(ErrorDef.UNSUPPORTED_FEATURE, "releaseSavepoint is not supported in sharding");
    }

    public void setChannel(CmChannel aChannel)
    {
        mShardContextConnect.setChannel(aChannel);
    }

    CmProtocolContextShardConnect getShardContext()
    {
        return mShardContextConnect;
    }

    public CmShardProtocol getShardProtocol()
    {
        return mCmShardProtocol;
    }

    /**
     * 데이터노드의 Connection을 가져온다. 만약 해당하는 노드의 Connection이 캐시에 존재하면 바로 리턴하고 <br>
     * 아닌경우 새로 생성한다.
     * @param aNode 데이터노드 객체
     * @return 데이터노드 커넥션
     * @throws SQLException 커넥션 생성 도중 에러가 발생한 경우
     */
    Connection getNodeConnection(DataNode aNode) throws SQLException
    {
        if (mCachedConnections.containsKey(aNode))
        {
            shard_log("(CACHED NODE CONNECTION EXISTS) {0}", aNode);
            return mCachedConnections.get(aNode);
        }
        Connection sResult = makeNodeConnection(aNode);
        // BUG-46513 노드커넥션을 생성한 후 메타커넥션객체를 주입한다.
        ((AltibaseConnection)sResult).setMetaConnection(this);
        mCachedConnections.put(aNode, sResult);
        replayMethodsInvocation(sResult);
        return sResult;
    }

    private Connection makeNodeConnection(DataNode aNode) throws SQLException
    {
        Map<DataNode, DataSource> sDataSourceMap = mShardContextConnect.getDataSourceMap();
        Connection sResult;
        try
        {
            sResult = sDataSourceMap.get(aNode).getConnection();
            if (sResult.getWarnings() != null)
            {
                getSQLWarningFromDataNode(sResult);
            }
        }
        catch (SQLException aException)
        {
            // BUG-45967 노드커넥션 도중 SMN Invalid 에러가 발생한 경우에는 SQLException을 던진다.
            if (aException.getErrorCode() == ErrorDef.SHARD_META_NUMBER_INVALID)
            {
                Error.throwSQLException(ErrorDef.SHARD_OPERATION_FAILED,
                                        "The Node Connection", aException.getMessage());
            }
            throw aException;
        }

        shard_log("(DATASOURCE GETCONNECTION) {0}", aNode);
        // DBCP와 같이 connection pool로 부터 가져온 connection이라면 shardpin을 재설정해준다.
        if (!(sResult instanceof AltibaseConnection))
        {
            setShardPinToDataNode(sResult, mShardContextConnect.getShardPin());
        }
        return sResult;
    }

    private void getSQLWarningFromDataNode(Connection aNodeConn) throws SQLException
    {
        SQLWarning sWarning = aNodeConn.getWarnings();
        String sNodeName = ((AltibaseConnection)aNodeConn).getNodeName();
        StringBuilder sSb = new StringBuilder();
        sSb.append("[").append(sNodeName).append("] ").append(sWarning.getMessage());
        SQLWarning sNewWarning = new SQLWarning(sSb.toString(), sWarning.getSQLState(),
                                                sWarning.getErrorCode());
        if (mWarning == null)
        {
            mWarning = sNewWarning;
        }
        else
        {
            mWarning.setNextWarning(sNewWarning);
        }
    }

    private void setShardPinToDataNode(Connection aConnection, long aShardPin) throws SQLException
    {
        /* 관련된 인터페이스가 없기 때문에 setCatalog를 이용한다.
           ex : conn.setCatalog("shardpin:213213123123123");
         */
        aConnection.setCatalog("shardpin:" + aShardPin);
    }

    ExecutorEngine getExecutorEngine()
    {
        return mExecutorEngine;
    }

    AltibaseConnection getMetaConnection()
    {
        return mMetaConnection;
    }

    public Map<DataNode, Connection> getCachedConnections()
    {
        return mCachedConnections;
    }

    public AltibaseProperties getProps()
    {
        return mProps;
    }

    public ShardNodeConfig getShardNodeConfig()
    {
        return mShardContextConnect.getShardNodeConfig();
    }

    /**
     * DataNode의 SMN이 클라이언트의 SMN보다 클 경우에는 DataNode의 SMN을 복사한다.
     * @param aSMN error message에서 파싱한 SMN값
     */
    public void setShardMetaNumberOfDataNode(long aSMN)
    {
        mShardContextConnect.setSMNOfDataNode(aSMN);
    }

    public void setNeedToDisconnect(boolean aNeedToDisconnect)
    {
        mShardContextConnect.setNeedToDisconnect(aNeedToDisconnect);
    }

    public boolean isLazyNodeConnect()
    {
        return mIsLazyNodeConnect;
    }

    /**
     * Meta가 가지고 있는 ShardMetaNumer와 DataNode의 SMN값을 비교하여 ShardMetaNumber가 DataNode SMN보다
     * 작을 경우 true를 리턴한다.
     * @return DataNode SMN이 더 최신인지 여부
     */
    public boolean shouldUpdateShardMetaNumber()
    {
        long sSMN = mShardContextConnect.getShardMetaNumber();
        long sSMNOfDataNode = mShardContextConnect.getSMNOfDataNode();
        boolean sResult = false;

        if ( ( sSMN != 0 ) &&
             ( sSMN < sSMNOfDataNode ) &&
             ( !mShardContextConnect.needToDisconnect() ) &&
             ( hasNodata()) )
        {
            sResult = true;
        }

        return sResult;
    }

    private boolean hasNodata()
    {
        synchronized (mOpenStatements)
        {
            for (Statement sEach : mOpenStatements)
            {
                AltibaseShardingStatement sStmt = (AltibaseShardingStatement)sEach;
                if (sStmt.isPrepared() && !sStmt.hasNoData())
                {
                    return false;
                }
            }
        }

        return true;
    }

    /**
     * 메타서버에 getNodeList프로토콜을 다시 요청해 SMN값을 갱신한다.
     */
    public void updateShardMetaNumber() throws SQLException
    {
        long sOldShardPin = mShardContextConnect.getShardPin();
        long sOldSMN = mShardContextConnect.getShardMetaNumber();
        mCmShardProtocol.getNodeList();
        if (mShardContextConnect.getError() != null)
        {
            mWarning = Error.processServerError(mWarning, mShardContextConnect.getError());
        }

        if (sOldShardPin != mShardContextConnect.getShardPin())
        {
            Error.throwSQLException(ErrorDef.SHARD_PIN_CHANGED);
        }

        mShardContextConnect.createDataSources(mProps);

        long sNewSMN = mShardContextConnect.getShardMetaNumber();
        if (sOldSMN < sNewSMN)
        {
            for (Connection sEach : mCachedConnections.values())
            {
                AltibaseConnection sNodeConn = (AltibaseConnection)sEach;
                sNodeConn.sendSMNProperty(sNewSMN);
            }
        }
    }

    public long getShardMetaNumber()
    {
        return mShardContextConnect.getShardMetaNumber();
    }

    public void removeNode(DataNode aNode)
    {
        try
        {
            Connection sRemovedConn =  mCachedConnections.remove(aNode);
            if (sRemovedConn != null)
            {
                sRemovedConn.close();
            }
        }
        catch (SQLException aEx)
        {
            shard_log("(CLOSE NODE CONNECTION) Error occurred : {0} {1} ",
                      new Object[] { aNode, aEx.getMessage() });
        }
        /* BUG-46513 node connection을 close하더라도 해당하는 connection의 하위 statement가 statement route map
           에 존재할 수 있기 때문에 clear해준다. */
        synchronized (mOpenStatements)
        {
            for (Statement sEach : mOpenStatements)
            {
                ((AltibaseShardingStatement)sEach).removeFromStmtRouteMap(aNode);
            }
        }
    }

    private interface TransactionCallback
    {
        void doTransaction(Connection acon) throws SQLException;
    }

    public CmProtocolContextShardConnect getShardContextConnect()
    {
        return mShardContextConnect;
    }
}
