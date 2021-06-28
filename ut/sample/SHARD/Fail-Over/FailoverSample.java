import Altibase.jdbc.driver.ex.ShardFailOverSuccessException;
import Altibase.jdbc.driver.ex.ShardFailoverIsNotAvailableException;

import java.sql.*;
import java.util.Properties;
import java.util.concurrent.TimeUnit;

/**
 * ShardJDBC Failover Sample Code
 */
public class FailoverSample
{
    private String                mPort;
    private String                mAlternatePort;
    private Connection            mConn;
    private Statement             mStmt;
    private PreparedStatement     mPStmt;
    private ResultSet             mRs;
    private String                mQueryForPrepared = "SELECT i2 FROM t1 WHERE t1.i1 != ? ORDER BY i2";

    public static void main(String[] aArgs)
    {
        if (aArgs.length < 2)
        {
            System.out.println("Usage : java FailoverSample {Port} {AlternatePort}");
            System.exit(-1);
        }

        new FailoverSample().doFailover(aArgs);
    }

    private void doFailover(String[] aArgs)
    {
        mPort = aArgs[0];
        mAlternatePort = aArgs[1];

        mConn = getConnection();

        directExecute();
        preparedExecute();

        commit();
        finish();
    }

    private void directExecute()
    {
        while (true)
        {
            try
            {
                if (mStmt == null)
                {
                    mStmt = mConn.createStatement();
                }
                mStmt.executeUpdate("INSERT INTO T1 VALUES ('F', 100)");
                break;
            }
            catch (SQLException aEx)
            {
                if (isFailOverErrorEvent(aEx))
                {
                    if (!failoverRollback())
                    {
                        // rollback이 실패하면 전체 커넥션을 끊고 다시 연결한 후 재시도 한다.
                        closeAndReconnect();
                        continue;
                    }
                    /* Service Time Fail-Over가 발생하면 다시 direct execute를 수행하면 된다. */
                    System.out.println("failover event occur at direct execute goto execute.");
                }
                else if (isShardNodeRetryNotAvailable(aEx))
                {
                    // failover가 실패한 경우 전체 커넥션을 끊고 다시 연결한 후 재시도 한다.
                    closeAndReconnect();
                }
                else
                {
                    System.out.println(aEx.getMessage());
                    /* 일반 에러에 대해서 사용자 에러 처리 로직을 수행한다. */
                }
            }
        }
    }

    private void preparedExecute()
    {
        prepare();
        execute();
        fetch();
    }

    private void prepare()
    {
        while (true)
        {
            try
            {
                mPStmt = mConn.prepareStatement(mQueryForPrepared);
                mPStmt.setString(1, "TEST DATA TEST DATA ");
                System.out.println("prepare success");
                break;
            }
            catch (SQLException aEx)
            {
                if (isFailOverErrorEvent(aEx))
                {
                    if (!failoverRollback())
                    {
                        closeAndReconnect();
                        continue;
                    }
                    System.out.println("failover event occur at prepare goto prepare");
                }
                else if (isShardNodeRetryNotAvailable(aEx))
                {
                    System.out.println("ALTIBASE_SHARD_NODE_FAILOVER_IS_NOT_AVAILABLE!! Retry after 1 seconds..");
                    closeAndReconnect();
                }
                else
                {
                    System.out.println("prepare fail");
                    System.out.format("Error : %s\n", mQueryForPrepared);
                    printError(aEx);
                    System.out.println("prepare()");
                    System.exit(-1);
                }
            }
        }
    }

    private void execute()
    {
        while (true)
        {
            try
            {
                mPStmt.execute();
                break;
            }
            catch (SQLException aEx)
            {
                if (isFailOverErrorEvent(aEx))
                {
                    if (!failoverRollback())
                    {
                        /* distributed transaction rollback fail, disconnect and reconnect */
                        closeAndReconnect();
                        continue;
                    }
                    System.out.println("failover event occur at execute goto execute");
                }
                else if (isShardNodeRetryNotAvailable(aEx))
                {
                    System.out.println("ALTIBASE_SHARD_NODE_FAIL_RETRY_CONNECTION_AVAILABLE!! Retry after 1 seconds..");
                    closeAndReconnect();
                }
                else
                {
                    System.out.println("execute fail");
                    System.out.println("PreparedStatement.execute()");
                    System.exit(-1);
                }
            }
        }
    }

    private void fetch()
    {
        while (true)
        {
            try
            {
                mRs = mPStmt.getResultSet();
                while (mRs.next())
                {
                    System.out.format("Fetch Value = %d \n", mRs.getInt(1));
                }
                break;
            }
            catch (SQLException aEx)
            {
                if (isFailOverErrorEvent(aEx))
                {
                    if (!failoverRollback())
                    {
                        closeAndReconnect();
                        prepare();
                        execute();
                        continue;
                    }
                    System.out.println("failover event occur at fetch goto prepare");
                    try
                    {
                        mRs.close();
                    }
                    catch (SQLException aEx2)
                    {
                        System.out.println(aEx2.getMessage());
                    }
                    // fetch 도중 failover가 발생하면 execute부터 다시 수행해야 한다.
                    execute();
                }
                else if (isShardNodeRetryNotAvailable(aEx))
                {
                    System.out.println("ALTIBASE_SHARD_NODE_FAIL_RETRY_CONNECTION_AVAILABLE!! Retry after 1 seconds..");
                    closeAndReconnect();
                    prepare();
                    execute();
                }
                else
                {
                    System.out.println("fetch fail");
                    System.out.format("Error : %s\n", mQueryForPrepared);
                    printError(aEx);
                    System.exit(-1);
                }
            }
        }

    }

    private void printError(SQLException aEx)
    {
        do
        {
            System.out.format("     SQLSTATE     : %s\n", aEx.getSQLState());
            System.out.format("     Message text : %s\n", aEx.getMessage());
            System.out.format("     Native error : %s\n", aEx.getErrorCode());
            aEx = aEx.getNextException();
        }
        while (aEx != null);
    }

    private void closeAndReconnect()
    {
        /* CTF가 불가능한 상황에서 서버가 뜰 때까지 재시작할 수 있다. */
        try
        {
            mConn.close();
        }
        catch (SQLException aEx2)
        {
            System.out.println(aEx2.getMessage());
            System.exit(-1);
        }
        mConn = getConnection();
        mStmt = null;
        System.out.println("ALTIBASE_SHARD_NODE_FAILOVER_IS_NOT_AVAILABLE!! Retry after 1 seconds.");
        sleep(1);
    }

    /**
     * 샤드 failover 발생 후 분산 트랜잭션 rollback을 위한 함수
     * 분산 트랜잭션 rollback 시에도 failover가 발생할 수 있으므로 Rollback을 두 번 시도한다.
     */
    private boolean failoverRollback()
    {
        boolean sResult;
        try
        {
            mConn.rollback();
            sResult = true;
        }
        catch (SQLException aEx)
        {
            if (isFailOverErrorEvent(aEx))
            {
                try
                {
                    mConn.rollback();
                    sResult = true;
                }
                catch (SQLException aEx2)
                {
                    sResult = false;
                }
            }
            else
            {
                sResult = false;
            }
        }

        return sResult;
    }

    /**
     * Fail-Over Success가 된 노드가 있는지 여부를 판단한다.
     * @param aEx 예외가 올라온 Exception 객체
     * @return STF success 예외인지 여부
     */
    private boolean isFailOverErrorEvent(SQLException aEx)
    {
        do
        {
            if (aEx instanceof ShardFailOverSuccessException)
            {
                System.out.println(aEx.getMessage());
                return true;
            }
            aEx = aEx.getNextException();
        }
        while (aEx != null);

        return false;
    }

    private boolean isShardNodeRetryNotAvailable(SQLException aEx)
    {
        do
        {
            if (aEx instanceof ShardFailoverIsNotAvailableException)
            {
                System.out.println(aEx.getMessage());
                return true;
            }
            aEx = aEx.getNextException();
        }
        while (aEx != null);

        return false;
    }

    private Connection getConnection()
    {
        Properties sProps = new Properties();
        Connection sCon;

        String sURL = "jdbc:sharding:Altibase://localhost:" + mPort + "/mydb";
        String sUser = "sys";
        String sPassword = "manager";

        sProps.put("user", sUser);
        sProps.put("password", sPassword);
        sProps.put("connectionretrycount", "1");
        sProps.put("connectionretrydelay", "0");
        sProps.put("app_info", "failoversample");
        sProps.put("alternateservers", "(127.0.0.1:" + mAlternatePort + ")");
        sProps.put("sessionfailover", "on");

        try
        {
            Class.forName("Altibase.jdbc.driver.AltibaseDriver");
        }
        catch (ClassNotFoundException aEx)
        {
            aEx.printStackTrace();
            System.exit(-1);
        }

        // CTF가 불가능한 상황에서 접속이 실패하면 서버가 뜰 때까지 재시도 한다.
        while (true)
        {
            try
            {
                sCon = DriverManager.getConnection(sURL, sProps);
                sCon.setAutoCommit(false);
                break;
            }
            catch (SQLException aEx)
            {
                sleep(1);
            }
        }

        return sCon;
    }

    private void sleep(long aSec)
    {
        try
        {
            TimeUnit.SECONDS.sleep(aSec);
        }
        catch (InterruptedException aEx)
        {
            aEx.printStackTrace();
        }
    }

    private void commit()
    {
        try
        {
            mConn.commit();
        }
        catch (SQLException aEx)
        {
            System.out.println("Connection.commit()");
            System.exit(-1);
        }
    }

    private void finish()
    {
        try
        {
            mPStmt.close();
        }
        catch (SQLException aEx)
        {
            System.out.println("PreparedStatement.close()");
            System.exit(-1);
        }

        try
        {
            mStmt.close();
        }
        catch (SQLException aEx)
        {
            System.out.println("Statement.close()");
            System.exit(-1);
        }

        try
        {
            mConn.close();
        }
        catch (SQLException aEx)
        {
            System.out.println("Connection.close()");
            System.exit(-1);
        }
    }
}
