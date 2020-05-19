
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

package Altibase.jdbc.driver.sharding.executor;

import Altibase.jdbc.driver.ex.ShardJdbcException;
import Altibase.jdbc.driver.ex.ShardFailoverIsNotAvailableException;
import Altibase.jdbc.driver.sharding.core.AltibaseShardingConnection;
import Altibase.jdbc.driver.sharding.core.DataNode;
import Altibase.jdbc.driver.sharding.core.ShardNodeConfig;
import Altibase.jdbc.driver.sharding.routing.SQLExecutionUnit;

import java.sql.Connection;
import java.sql.SQLException;
import java.util.*;
import java.util.concurrent.*;
import java.util.logging.Level;

import static Altibase.jdbc.driver.sharding.util.ShardingTraceLogger.shard_log;

public class MultiThreadExecutorEngine implements ExecutorEngine
{
    private AltibaseShardingConnection mShardConn;
    private ShardNodeConfig mShardNodeConfig;

    public MultiThreadExecutorEngine(AltibaseShardingConnection aShardConn)
    {
        mShardConn = aShardConn;
        mShardNodeConfig = aShardConn.getShardNodeConfig();
    }

    public <T> List<T> executeStatement(List<? extends BaseStatementUnit> aStatements,
                                        ExecuteCallback<T> aExecuteCallback) throws SQLException
    {
        return execute(aStatements, aExecuteCallback);
    }

    public <T> List<T> generateStatement(Set<SQLExecutionUnit> aSqlExecutionUnit,
                                         GenerateCallback<T> aGenerateStmtCallback) throws SQLException
    {
        if (aSqlExecutionUnit.isEmpty())
        {
            return Collections.emptyList();
        }

        Iterator<SQLExecutionUnit> sItr = aSqlExecutionUnit.iterator();
        SQLExecutionUnit sFirst = sItr.next();
        List<Future<T>> sRestFutures = null;
        if (sItr.hasNext())
        {
            sRestFutures = asyncGenerate(getExecutionUnitForAsync(sItr), aGenerateStmtCallback);
        }
        T sFirstStatement = null;
        List<T> sRestStatements;
        List<SQLException> sExceptionList = new ArrayList<SQLException>();

        try
        {
            sFirstStatement = syncGenerate(sFirst, aGenerateStmtCallback);
        }
        catch (SQLException aException)
        {
            shard_log(Level.SEVERE, "(GENERATE STATEMENT EXCEPTION) ", aException);
            sExceptionList.add(aException);
        }

        sRestStatements = getRestFutures(sRestFutures, sExceptionList);
        throwSQLExceptionIfExists(sExceptionList);
        sRestStatements.add(0, sFirstStatement);

        return sRestStatements;
    }

    private <T> T syncGenerate(SQLExecutionUnit aFirst,
                               GenerateCallback<T> aGenerateStmtCallback) throws SQLException
    {
        return generateInternal(aFirst, aGenerateStmtCallback);
    }

    private <T> List<Future<T>> asyncGenerate(List<SQLExecutionUnit> aSqlExecutionUnits,
                                              final GenerateCallback<T> aGenerateStmtCallback) throws SQLException
    {
        List<Callable<T>> sCallables = new ArrayList<Callable<T>>(aSqlExecutionUnits.size());

        for (final SQLExecutionUnit sEach : aSqlExecutionUnits)
        {
            sCallables.add(new Callable<T>()
            {
                public T call() throws Exception
                {
                    return generateInternal(sEach, aGenerateStmtCallback);
                }
            });
        }

        List<Future<T>> sResult = null;

        try
        {
            ExecutorService sExecutorService = SingletonExecutorService.getExecutorService();
            sResult = sExecutorService.invokeAll(sCallables);
        }
        catch (InterruptedException aException)
        {
            ExecutorExceptionHandler.handleException(aException);
        }

        return sResult;

    }

    private <T> T generateInternal(SQLExecutionUnit aSqlExecutionUnit,
                                   GenerateCallback<T> aGenerateCallback) throws SQLException
    {
        T sResult = null;

        try
        {
            sResult = aGenerateCallback.generate(aSqlExecutionUnit);
        }
        catch (ShardJdbcException aShardJdbcEx)
        {
            throw aShardJdbcEx;
        }
        catch (SQLException aException)
        {
            ExecutorExceptionHandler.handleException(aException,
                                                     aSqlExecutionUnit.getNode().getNodeName());
        }

        return sResult;

    }

    private List<SQLExecutionUnit> getExecutionUnitForAsync(Iterator<SQLExecutionUnit> aItr)
    {
        List<SQLExecutionUnit> sResult = new ArrayList<SQLExecutionUnit>();
        while (aItr.hasNext())
        {
            sResult.add(aItr.next());
        }
        return sResult;
    }

    private <T> List<T> execute(List<? extends BaseStatementUnit> aStatements,
                                ExecuteCallback<T> aExecuteCallback) throws SQLException
    {
        if (aStatements.isEmpty())
        {
            return Collections.emptyList();
        }
        Iterator<? extends BaseStatementUnit> sIterator = aStatements.iterator();
        BaseStatementUnit sFirstInput = sIterator.next();
        List<Future<T>> sFutures = null;
        if (sIterator.hasNext())
        {
            sFutures = asyncExecute(newArrayList(sIterator), aExecuteCallback);
        }
        T sFirstOutput = null;
        List<T> sRestOutputs;
        List<SQLException> sExceptionList = new ArrayList<SQLException>();

        try
        {
            sFirstOutput = syncExecute(sFirstInput, aExecuteCallback);
        }
        catch (SQLException aException)
        {
            shard_log(Level.SEVERE, "(NODE EXECUTION EXCEPTION) ", aException);
            sExceptionList.add(aException);
        }
        sRestOutputs = getRestFutures(sFutures, sExceptionList);
        throwSQLExceptionIfExists(sExceptionList);
        sRestOutputs.add(0, sFirstOutput);

        return sRestOutputs;
    }

    private void throwSQLExceptionIfExists(List<SQLException> aExceptionList) throws SQLException
    {
        if (aExceptionList.size() > 0)
        {
            SQLException sException = null;
            for (SQLException sEach : aExceptionList)
            {
                if (sException == null)
                {
                    sException = sEach;
                }
                else
                {
                    sException.setNextException(sEach);
                }
            }
            shard_log(Level.SEVERE, "(NODE EXECUTION EXCEPTION) ", sException);
            throw sException;
        }
    }

    private <T> List<T> getRestFutures(List<Future<T>> aRestFutures,
                                       List<SQLException> aExceptionList)
    {
        List<T> sResult = new ArrayList<T>();
        if (aRestFutures == null)
        {
            return sResult;
        }
        for (Future<T> sFuture : aRestFutures)
        {
            try
            {
                sResult.add(sFuture.get());
            }
            catch (ExecutionException aEx)
            {
                Throwable sThrowable = aEx.getCause();
                if (sThrowable instanceof SQLException)
                {
                    aExceptionList.add((SQLException)sThrowable);
                }
                else
                {
                    throw new RuntimeException(sThrowable);
                }
            }
            catch (InterruptedException aInterruptEx)
            {
                shard_log(Level.SEVERE, aInterruptEx);
                throw new RuntimeException(aInterruptEx);
            }
        }

        return sResult;
    }

    private List<BaseStatementUnit> newArrayList(Iterator<? extends BaseStatementUnit> aIterator)
    {
        List<BaseStatementUnit> sResult = new ArrayList<BaseStatementUnit>();
        while (aIterator.hasNext())
        {
            sResult.add(aIterator.next());
        }

        return sResult;
    }

    private <T> T syncExecute(BaseStatementUnit aBaseStatementUnit,
                              ExecuteCallback<T> aExecuteCallback) throws SQLException
    {
        return executeInternal(aBaseStatementUnit, aExecuteCallback);
    }

    private <T> List<Future<T>> asyncExecute(List<BaseStatementUnit> aBaseStatementUnits,
                                             final ExecuteCallback<T> aExecuteCallback) throws SQLException
    {
        if (aBaseStatementUnits.isEmpty())
        {
            return Collections.emptyList();
        }
        List<Callable<T>> sCallableList = new ArrayList<Callable<T>>(aBaseStatementUnits.size());

        for (final BaseStatementUnit sEach : aBaseStatementUnits)
        {
            sCallableList.add(new Callable<T>()
            {
                public T call() throws Exception
                {
                    return executeInternal(sEach, aExecuteCallback);
                }
            });
        }

        List<Future<T>> sResult = null;

        try
        {
            ExecutorService sExecutorService = SingletonExecutorService.getExecutorService();
            sResult = sExecutorService.invokeAll(sCallableList);
        }
        catch (Exception aException)
        {
            ExecutorExceptionHandler.handleException(aException);
        }

        return sResult;
    }

    private <T> T executeInternal(BaseStatementUnit aBaseStatementUnit,
                                  ExecuteCallback<T> aExecuteCallback) throws SQLException
    {
        T sResult = null;

        try
        {
            sResult = aExecuteCallback.execute(aBaseStatementUnit);
        }
        catch (ShardFailoverIsNotAvailableException aFailoverException)
        {
            /*
             * ShardFailoverIsNotAvailableException 예외가 올라온 경우에는 해당 커넥션을 cached map에서 제거해 준다.
             */
            String sNodeName = aFailoverException.getNodeName();
            DataNode sDataNode = mShardNodeConfig.getNodeByName(sNodeName);
            mShardConn.getCachedConnections().remove(sDataNode);
            shard_log(Level.SEVERE, "(SHARD FAILOVER IS NOT AVAILABLE EXCEPTION) ", aFailoverException);
            mShardNodeConfig.getNodeByName(sNodeName).setClosedOnExecute(true);
            throw aFailoverException;
        }
        catch (ShardJdbcException aShardJdbcEx)
        {
            setClosedOnExecuteOn(aBaseStatementUnit);
            throw aShardJdbcEx;
        }
        catch (SQLException aException)
        {
            DataNode sNode = setClosedOnExecuteOn(aBaseStatementUnit);
            ExecutorExceptionHandler.handleException(aException, sNode.getNodeName());
        }

        return sResult;
    }

    private DataNode setClosedOnExecuteOn(BaseStatementUnit aBaseStatementUnit)
    {
        DataNode sNode = aBaseStatementUnit.getSqlExecutionUnit().getNode();
        sNode.setClosedOnExecute(true);
        return sNode;
    }

    public void doTransaction(Collection<Connection> aConnections,
                              final ConnectionParallelProcessCallback aParallelProcessCallback) throws SQLException
    {
        if (aConnections.size() == 0) return;

        Iterator<Connection> sItr = aConnections.iterator();
        Connection sFirstConn = sItr.next();

        List<Connection> sRestConn = new ArrayList<Connection>();
        while (sItr.hasNext())
        {
            sRestConn.add(sItr.next());
        }

        List<Future<Void>> sFutureList = asyncDoTransaction(aParallelProcessCallback, sRestConn);

        aParallelProcessCallback.processInParallel(sFirstConn);
        if (sFutureList != null)
        {
            for (Future<Void> sEach : sFutureList)
            {
                try
                {
                    sEach.get();
                }
                catch (ExecutionException aEx)
                {
                    Throwable sThrowable = aEx.getCause();
                    if (sThrowable instanceof SQLException)
                    {
                        ExecutorExceptionHandler.handleException((SQLException)sThrowable);
                    }
                    else
                    {
                        throw new RuntimeException(sThrowable);
                    }
                }
                catch (InterruptedException aInterruptEx)
                {
                    shard_log(Level.SEVERE, aInterruptEx);
                    throw new RuntimeException(aInterruptEx);
                }
            }
        }
    }

    private List<Future<Void>> asyncDoTransaction(ConnectionParallelProcessCallback aParallelProcessCallback,
                                    List<Connection> aRestConn) throws SQLException
    {
        if (aRestConn.size() == 0) return null;
        List<Callable<Void>> sCallables = makeTransactionCallables(aParallelProcessCallback, aRestConn);
        List<Future<Void>> sFutureList = null;
        try
        {
            ExecutorService sExecutorService = SingletonExecutorService.getExecutorService();
            sFutureList = sExecutorService.invokeAll(sCallables);
        }
        catch (InterruptedException aException)
        {
            shard_log(Level.SEVERE, aException.getMessage(), aException);
            ExecutorExceptionHandler.handleException(aException);
        }

        return sFutureList;
    }

    private List<Callable<Void>> makeTransactionCallables(final ConnectionParallelProcessCallback aParallelProcessCallback,
                                                          List<Connection> aRestConn)
    {
        List<Callable<Void>> sCallables = new ArrayList<Callable<Void>>();
        for (final Connection sEach : aRestConn)
        {
            sCallables.add(new Callable<Void>()
            {
                public Void call() throws Exception
                {
                    return aParallelProcessCallback.processInParallel(sEach);
                }
            });
        }
        return sCallables;
    }
}
