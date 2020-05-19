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

package Altibase.jdbc.driver.sharding.util;

import Altibase.jdbc.driver.util.AltibaseEnvironmentVariables;
import Altibase.jdbc.driver.util.RuntimeEnvironmentVariables;

import java.io.File;
import java.util.logging.*;

public class ShardingTraceLogger
{
    private static final String LOGGER_NAME  = ShardingTraceLogger.class.getName();
    private static final String LOGFILE_NAME = "shardjdbc.trc";
    private static Logger       mLogger;

    public static final int DEFAULT_LOG_FILE_SIZE = 15242880;
    public static final int DEFALUT_LOG_FILE_ROTATE_CNT = 5;

    static
    {
        mLogger = Logger.getLogger(LOGGER_NAME);
        String sAltibaseHomePath = AltibaseEnvironmentVariables.getAltibaseHome();
        String sLogFilePath;

        if (sAltibaseHomePath != null)
        {
            if (sAltibaseHomePath.endsWith(File.separator))
            {
                sLogFilePath = sAltibaseHomePath + "trc" + File.separator + LOGFILE_NAME;
            }
            else
            {
                sLogFilePath = sAltibaseHomePath + File.separator + "trc" + File.separator + LOGFILE_NAME;
            }
        }
        else
        {
            sLogFilePath = LOGFILE_NAME;
        }

        try
        {
            Handler sHandler = new FileHandler(sLogFilePath, DEFAULT_LOG_FILE_SIZE,
                                               DEFALUT_LOG_FILE_ROTATE_CNT, true);
            sHandler.setFormatter(new ShardSingleLineFormatter());
            mLogger.addHandler(sHandler);
            String sLogLevelStr = RuntimeEnvironmentVariables.getVariable("SHARD_JDBC_TRCLOG_LEVEL",
                                                                          "OFF").toUpperCase();
            Level sCurrentLevel = Level.parse(sLogLevelStr);
            mLogger.setLevel(sCurrentLevel);
        }
        catch (Exception aException)
        {
            aException.printStackTrace();
        }

        // decide whether or not this logger prints its output to stderr.
        // default: false (not printed to stderr)
        String sLogStderr = RuntimeEnvironmentVariables.getVariable("SHARD_JDBC_TRCLOG_PRINT_STDERR",
                                                                    "FALSE").toUpperCase();
        if (sLogStderr.equals("FALSE"))
        {
            mLogger.setUseParentHandlers(false);
        }
    }

    public static void shard_log(Level aLevel, String aLog, Exception aException)
    {
        mLogger.log(aLevel, aLog, aException);
    }

    public static void shard_log(Level aLevel, Exception aException)
    {
        mLogger.log(aLevel, aException.getMessage(), aException);
    }

    public static void shard_log(Level aLevel, String aLog)
    {
        mLogger.log(aLevel, aLog);
    }

    public static void shard_log(String aLog)
    {
        mLogger.log(Level.INFO, aLog);
    }

    public static void shard_log(String aLog, Object aParam)
    {
        mLogger.log(Level.INFO, aLog, aParam);
    }

    public static void shard_log(Level aLevel, String aLog, Object aParam)
    {
        mLogger.log(aLevel, aLog, aParam);
    }

    public static void shard_log(String aLog, Exception aException)
    {
        mLogger.log(Level.INFO, aLog, aException);
    }

    public static void shard_log(String aLog, Object[] aParams)
    {
        mLogger.log(Level.INFO, aLog, aParams);
    }

    public static void shard_log(Level aLevel, String aLog, Object[] aParams)
    {
        mLogger.log(aLevel, aLog, aParams);
    }

    private ShardingTraceLogger()
    {
    }
}
