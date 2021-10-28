import java.util.concurrent.*;
import java.util.concurrent.locks.*;
import java.util.*;
import java.text.*;
import java.sql.*;

public class bmt_select_context implements Runnable, Comparable {
    
    public String connection;
    public int    start;
    public int    end;
    public int    minimum;
    public int    maximum;
    public long   sum;
    public int    numberOfTransactions;
    public int    numberOfLongTransactions;
    
    public bmt_select_context ( int thread ) {
        
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
        
        bmt_select_context context;
        
        context = (bmt_select_context)object;
        
        if ( this.numberOfTransactions > context.numberOfTransactions ) return  1;
        if ( this.numberOfTransactions < context.numberOfTransactions ) return -1;
        
        return 0;
        
    }
    
    public void run () {
        
        Properties        properties;
        String            url;
        Connection        connection;
        PreparedStatement preparedStatement;
        ResultSet         resultSet;
        int               value;
        long              startTime;
        long              endTime;
        int               elapsed;
        
//        url        = "jdbc:oracle:thin:@" + bmt_select.host + bmt_select.option; 
        url        = "jdbc:timesten:direct:dsn=TT_1122";
//        url        = "jdbc:timesten:client:dsn=tt_1122_cs";
        properties = new Properties();
        properties.put( "user",     bmt_select.user     );
        properties.put( "password", bmt_select.password );
        
        try {
            Statement statement;
            
            connection =  DriverManager.getConnection( url, properties );
            
            connection.setAutoCommit( true );
            
            statement = connection.createStatement();
            
            preparedStatement = connection.prepareStatement( "SELECT K01, K02, K03, K04, K05, K06 FROM TEST WHERE K01 = ?" );
            
            bmt_select.countDown.countDown();
            
            while ( bmt_select.allocateUnit( this ) > 0 ) {
                
                for ( value = start; value < end; value++ ) {
                    
                    startTime = System.nanoTime();
                    
                    preparedStatement.setInt( 1, value );
                    
                    resultSet = preparedStatement.executeQuery();
                    
                    if ( resultSet.next() == false ) {
                        System.out.println( "Failed" );
                        System.exit( -1 );
                        
                    }
                    if ( resultSet.next() == true ) {
                        System.out.println( "Failed" );
                        System.exit( -1 );
                    }
                    
                    endTime = System.nanoTime();
                    
                    elapsed = (int)( ( endTime - startTime ) / 1000 );
                    
                    if ( minimum > elapsed ) minimum = elapsed;
                    if ( maximum < elapsed ) maximum = elapsed;
                    sum += elapsed;
                    numberOfTransactions++;
                    if ( elapsed > bmt_select.threshold ) numberOfLongTransactions++;
                    
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
