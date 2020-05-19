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

import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.List;

import static Altibase.jdbc.driver.sharding.util.ShardingTraceLogger.shard_log;

public class StatementExecutor
{
    private ExecutorEngine      mExecutorEngine;

    public StatementExecutor(ExecutorEngine aExecutorEngine)
    {
        this.mExecutorEngine = aExecutorEngine;
    }

    public int executeUpdate(List<? extends BaseStatementUnit> aStatementUnits) throws SQLException
    {
        return executeUpdate(new Updater() {
            public int executeUpdate(final Statement aStatement, final String aSql) throws SQLException
            {
                return aStatement.executeUpdate(aSql);
            }
        }, aStatementUnits);
    }

    public int executeUpdate(final int[] aColumnIndexes,
                             List<? extends BaseStatementUnit> aStatementUnits) throws SQLException
    {
        return executeUpdate(new Updater()
        {
            public int executeUpdate(Statement aStatement, String aSql) throws SQLException
            {
                return aStatement.executeUpdate(aSql, aColumnIndexes);
            }
        }, aStatementUnits);
    }

    public int executeUpdate(final String[] aColumnNames,
                             List<? extends BaseStatementUnit> aStatementUnits) throws SQLException
    {
        return executeUpdate(new Updater()
        {
            public int executeUpdate(Statement aStatement, String aSql) throws SQLException
            {
                return aStatement.executeUpdate(aSql, aColumnNames);
            }
        }, aStatementUnits);
    }

    private int executeUpdate(final Updater aUpdater, List<? extends BaseStatementUnit> aStatementUnits) throws SQLException
    {
        List<Integer> sResults = mExecutorEngine.executeStatement(aStatementUnits, new ExecuteCallback<Integer>()
        {
            public Integer execute(BaseStatementUnit aBaseStatement) throws SQLException
            {
                shard_log("(NODE DIRECT EXECUTEUPDATE) {0}", aBaseStatement);
                return aUpdater.executeUpdate(aBaseStatement.getStatement(), aBaseStatement
                        .getSqlExecutionUnit().getSql());
            }
        });
        return accumulate(sResults);
    }

    private int accumulate(List<Integer> aResults)
    {
        int sResult = 0;

        for (Integer sEach : aResults)
        {
            sResult += (sEach == null) ? 0 : sEach;
        }

        return sResult;
    }

    public List<ResultSet> executeQuery(List<? extends BaseStatementUnit> aStatementUnits) throws SQLException
    {
        return mExecutorEngine.executeStatement(aStatementUnits, new ExecuteCallback<ResultSet>()
        {
            public ResultSet execute(BaseStatementUnit aBaseStatement) throws SQLException
            {
                shard_log("(NODE DIRECT EXECUTEQUERY) {0}", aBaseStatement);
                return aBaseStatement.getStatement().executeQuery(aBaseStatement.getSqlExecutionUnit().getSql());
            }
        });
    }

    public boolean execute(List<? extends BaseStatementUnit> aStatementUnits) throws SQLException
    {
        return execute(new Executor()
        {
            public boolean execute(Statement aStatement, String aSql) throws SQLException
            {
                return aStatement.execute(aSql);
            }
        }, aStatementUnits);
    }

    public boolean execute(final int[] aColumnIndexes,
                           List<? extends BaseStatementUnit> aStatementUnits) throws SQLException
    {
        return execute(new Executor()
        {
            public boolean execute(Statement aStatement, String aSql) throws SQLException
            {
                return aStatement.execute(aSql, aColumnIndexes);
            }
        }, aStatementUnits);
    }

    public boolean execute(final String[] aColumnNames,
                           List<? extends BaseStatementUnit> aStatementUnits) throws SQLException
    {
        return execute(new Executor()
        {
            public boolean execute(Statement aStatement, String aSql) throws SQLException
            {
                return aStatement.execute(aSql, aColumnNames);
            }
        }, aStatementUnits);
    }

    private boolean execute(final Executor aExecutor, List<? extends BaseStatementUnit> aStatementUnits) throws SQLException
    {
        List<Boolean> sResult = mExecutorEngine.executeStatement(aStatementUnits, new ExecuteCallback<Boolean>()
        {
            public Boolean execute(BaseStatementUnit aBaseStatementUnit) throws SQLException
            {
                shard_log("(NODE DIRECT EXECUTE) {0}", aBaseStatementUnit);
                return aExecutor.execute(aBaseStatementUnit.getStatement(),
                                         aBaseStatementUnit.getSqlExecutionUnit().getSql());
            }
        });

        if (sResult == null || sResult.isEmpty() || sResult.get(0) == null)
        {
            return false;
        }

        return sResult.get(0);
    }

    private interface Updater
    {
        int executeUpdate(Statement aStatement, String aSql) throws SQLException;
    }

    private interface Executor
    {
        boolean execute(Statement aStatement, String aSql) throws SQLException;
    }
}
