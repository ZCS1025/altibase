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

import Altibase.jdbc.driver.sharding.routing.SQLExecutionUnit;

import java.sql.Connection;
import java.sql.SQLException;
import java.util.Collection;
import java.util.List;
import java.util.Set;

public interface ExecutorEngine
{
    <T> List<T> executeStatement(List<? extends BaseStatementUnit> aStatements,
                                 ExecuteCallback<T> aExecuteCallback) throws SQLException;

    <T> List<T> generateStatement(Set<SQLExecutionUnit> aSqlExecutionUnit,
                                  GenerateCallback<T> aGenerateStmtCallback) throws SQLException;

    void doTransaction(Collection<Connection> aConnections,
                       ConnectionParallelProcessCallback aParallelProcessCallback) throws SQLException;

}
