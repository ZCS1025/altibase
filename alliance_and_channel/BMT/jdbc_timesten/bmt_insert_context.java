import java.util.concurrent.*;
import java.util.concurrent.locks.*;
import java.util.*;
import java.text.*;
import java.sql.*;

public class bmt_insert_context implements Runnable, Comparable {
    
    public String connection;
    public int    start;
    public int    end;
    public int    minimum;
    public int    maximum;
    public long   sum;
    public int    numberOfTransactions;
    public int    numberOfLongTransactions;
    
    public bmt_insert_context ( int thread ) {
        
        connection               = String.valueOf( thread );
        start                    = 0;
        end                      = 0;
        minimum                  = Integer.MAX_VALUE;
        maximum                  = 0;
        sum                      = 0;
        numberOfTransactions     = 0;
        numberOfLongTransactions = 0;
        
    }
    
    public int compareTo( Object object ) {
        
        bmt_insert_context context;
        
        context = (bmt_insert_context)object;
        
        if ( this.numberOfTransactions > context.numberOfTransactions ) return  1;
        if ( this.numberOfTransactions < context.numberOfTransactions ) return -1;
        
        return 0;
        
    }
    
    public void run () {
        
        Properties        properties;
        String            url;
        Connection        connection;
        PreparedStatement preparedStatement;
        int               value;
        long              startTime;
        long              endTime;
        int               elapsed;
        
//        url        = "jdbc:oracle:thin:@" + bmt_insert.host + bmt_insert.option;
        url        = "jdbc:timesten:direct:dsn=TT_1122";
//        url        = "jdbc:timesten:client:dsn=tt_1122_cs";
        properties = new Properties();
        properties.put( "user",     bmt_insert.user     );
        properties.put( "password", bmt_insert.password );
        
        try {
            Statement statement;
            
            connection =  DriverManager.getConnection( url, properties );
            
//            connection.setAutoCommit( true );
            connection.setAutoCommit( false );
            
            statement = connection.createStatement();
            
            preparedStatement = connection.prepareStatement( "INSERT INTO TEST VALUES ( ?, ?, ?, ?, ?, ? )" );
            
            preparedStatement.setString( 2, "test_insert_coscom1" );
            
            bmt_insert.countDown.countDown();
            
            while ( bmt_insert.allocateUnit( this ) > 0 ) {
                
                for ( value = start; value < end; value++ ) {
                    
                    startTime = System.nanoTime();
                    
                    preparedStatement.setInt(  1, value );
                    preparedStatement.setInt(  3, value );
                    preparedStatement.setInt(  4, value );
                    preparedStatement.setDate(  5, new java.sql.Date(System.currentTimeMillis()) );
                    preparedStatement.setInt(  6, value );
                    
                    if ( preparedStatement.executeUpdate() != 1 ) {
                        System.out.println( "Failed" );
                        System.exit( -1 );
                    }
                    
                    connection.commit();
                    endTime = System.nanoTime();
                    
                    elapsed = (int)( ( endTime - startTime ) / 1000 );
                    
                    if ( minimum > elapsed ) minimum = elapsed;
                    if ( maximum < elapsed ) maximum = elapsed;
                    sum += elapsed;
                    numberOfTransactions++;
                    if ( elapsed > bmt_insert.threshold ) numberOfLongTransactions++;
                    
                }
                
            }
            
            preparedStatement.close();
            
            connection.close();
            
        } catch ( Exception e ) {
            System.out.println( e );
            System.exit( -1 );
        }
        
    }
    
}
