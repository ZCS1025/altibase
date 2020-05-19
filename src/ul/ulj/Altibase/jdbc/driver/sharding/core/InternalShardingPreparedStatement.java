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

import java.sql.ParameterMetaData;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;

/**
 * ServerSide 또는 ClientSide에서 공통적으로 실행되는 java.sql.PreparedStatement 인터페이스 모음
 */
public interface InternalShardingPreparedStatement extends InternalShardingStatement
{
    ResultSet executeQuery() throws SQLException;
    int executeUpdate() throws SQLException;
    int[] executeBatch() throws SQLException;
    void clearParameters() throws SQLException;
    boolean execute() throws SQLException;
    void addBatch() throws SQLException;
    ResultSetMetaData getMetaData() throws SQLException;
    ParameterMetaData getParameterMetaData() throws SQLException;
    void clearBatch() throws SQLException;
}
