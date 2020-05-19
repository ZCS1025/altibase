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

package Altibase.jdbc.driver.ex;

import java.sql.SQLException;

/**
 * 접속 장애 상황에서 발생하는 Shard jdbc exception <br>
 * 내부적으로 node name을 가지고 있으며 에러메세지에 node name을 추가해서 돌려준다.
 */
public class ShardJdbcException extends SQLException
{
    private String mNodeName;

    public ShardJdbcException(String aErrorMsg, int aErrorCode, String aNodeName)
    {
        super("[" + aNodeName + "] " + aErrorMsg, ErrorDef.getErrorState(aErrorCode), aErrorCode);
        mNodeName = aNodeName;
    }

    public String getNodeName()
    {
        return mNodeName;
    }
}
