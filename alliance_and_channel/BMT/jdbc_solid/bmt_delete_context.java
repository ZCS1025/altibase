import java.util.concurrent.*;
import java.util.concurrent.locks.*;
import java.util.*;
import java.text.*;
import java.sql.*;

public class bmt_delete_context implements Runnable, Comparable {
    
    public String connection;
    public int    start;
    public int    end;
    public int    minimum;
    public int    maximum;
    public long   sum;
    public int    numberOfTransactions;
    public int    numberOfLongTransactions;
    
    public bmt_delete_context ( int thread ) {
        
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
        
        bmt_delete_context context;
        
        context = (bmt_delete_context)object;
        
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
        
        url        = "jdbc:solid://" + bmt_delete.host + ":1964/" + bmt_delete.user + "/" + bmt_delete.password  ;
        properties = new Properties();
        properties.put( "user",     bmt_delete.user     );
        properties.put( "password", bmt_delete.password );
        
        try {
            Statement statement;
            
            connection =  DriverManager.getConnection( url );
            
            connection.setAutoCommit( true );
            
            statement = connection.createStatement();
            
            preparedStatement = connection.prepareStatement( "DELETE FROM TEST WHERE K01 = ?" );
            
            bmt_delete.countDown.countDown();
            
            while ( bmt_delete.allocateUnit( this ) > 0 ) {
                
                for ( value = start; value < end; value++ ) {
                    
                    startTime = System.nanoTime();
                    
                    preparedStatement.setInt(  1, value );
                    
                    if ( preparedStatement.executeUpdate() != 1 ) {
                        System.out.println( "Failed" );
                        System.exit( -1 );
                    }
                    
                    endTime = System.nanoTime();
                    
                    elapsed = (int)( ( endTime - startTime ) / 1000 );
                    
                    if ( minimum > elapsed ) minimum = elapsed;
                    if ( maximum < elapsed ) maximum = elapsed;
                    sum += elapsed;
                    numberOfTransactions++;
                    if ( elapsed > bmt_delete.threshold ) numberOfLongTransactions++;
                    
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
