import java.util.concurrent.*;
import java.util.concurrent.locks.*;
import java.lang.*;
import java.util.*;
import java.text.*;
import java.sql.*;

public class bmt_select {
    
    static String         TEST_MODE = "Read";
    
    static int            numberOfThreads;
    static int            startValue;
    static int            numberOfTransactions;
    static int            sizeOfUnit;
    static int            threshold;
    static int            cap;
    
    static long           connectTime;
    static long           startTime;
    static long           endTime;
    
    static CountDownLatch countDown;
    static Lock           mutex;
    static int            remain;
    static int            cursor;
    static int            allocated;
    
    static String         host;
    static String         user;
    static String         password;
    static String         option;
    
    public static int allocateUnit( bmt_select_context context ) {
        
        int  size;
        long elapsed;
        
        mutex.lock();
        
        if ( cap > 0 ) {
            endTime = System.currentTimeMillis();
            elapsed = endTime - startTime;
            while ( (double)allocated / ( (double)elapsed / 1e3 ) > (double)cap ) {
                Thread.yield();
                endTime = System.currentTimeMillis();
                elapsed = endTime - startTime;
            }
        }
        
        if ( remain > sizeOfUnit ) {
            size    = sizeOfUnit;
            remain -= sizeOfUnit;
        } else {
            size    = remain;
            remain -= remain;
        }
        context.start  = cursor;
        context.end    = cursor + size;
        cursor        += size;
        allocated     += size;
        
        mutex.unlock();
        
        return size;
        
    }
    
    public static String workingDirectory() {
        
        String directory;
        String home;
        
        directory = System.getProperty( "user.dir" );
        home      = System.getenv( "APP_HOME" );
        
        if ( home != null ) {
            if ( directory.startsWith( home ) == true ) {
                directory = directory.substring( home.length() );
            }
        }
        while ( directory.startsWith( "/" ) == true || directory.startsWith( "\\" ) == true ) {
            directory = directory.substring( 1 );
        }
        
        return directory;
        
    }
    
    public static void main ( String args[] ) {
        
        bmt_select_context[] contexts;
        int                  thread;
        Thread[]             threads;
        DateFormat           format;
        java.util.Date       tm;
        long                 second;
        long                 usecond;
        long                 elapsed;
        double               tps;
        int                  minimum;
        int                  maximum;
        long                 sum;
        int                  numberOfLongTransactions;           
        int                  average;
        
        try {
//            Class.forName( "oracle.jdbc.driver.OracleDriver" );
            Class.forName( "com.timesten.jdbc.TimesTenDriver" );
//            Class.forName( "com.timesten.jdbc.TimesTenClientDriver" );
        } catch ( Exception e ) {
            System.out.println( e );
            System.exit( -1 );
        }
        
        numberOfThreads      = 1;
        startValue           = 0;
        numberOfTransactions = 1000000;
        sizeOfUnit           = 1000;
        threshold            = 1000000;
        cap                  = 0;
        host                 = "localhost";
        user                 = "SYS";
        password             = "MANAGER";
        option               = ":20300/mydb";
        
        switch ( args.length ) {
         case 10:
            option               = args[9];
         case  9:
            password             = args[8];
         case  8:
            user                 = args[7];
         case  7:
            host                 = args[6];
         case  6:
            cap                  = Integer.parseInt( args[5] );
         case  5:
            threshold            = Integer.parseInt( args[4] );
         case  4:
            sizeOfUnit           = Integer.parseInt( args[3] );
         case  3:
            numberOfTransactions = Integer.parseInt( args[2] );
         case  2:
            startValue           = Integer.parseInt( args[1] );
         case  1:
            numberOfThreads      = Integer.parseInt( args[0] );
            break;
         default:
            System.out.println( "Arguments: numberOfThreads [startValue] [numberOfTransactions] [sizeOfUnit] [threshold] [cap] [host] [user] [password] [option]" );
            System.exit( 1 );
            break;
        }
        
        System.out.println( "Working Directory                 : " + workingDirectory()                           );
        System.out.println( "Test Mode                         : " + TEST_MODE                                    );
        System.out.println( "Number of Threads                 : " + String.format( "%8d", numberOfThreads      ) );
        System.out.println( "Start Value                       : " + String.format( "%8d", startValue           ) );
        System.out.println( "Number of Transactions            : " + String.format( "%8d", numberOfTransactions ) );
        System.out.println( "Size   of Unit                    : " + String.format( "%8d", sizeOfUnit           ) );
        System.out.println( "Threshold(microsecond)            : " + String.format( "%8d", threshold            ) );
        System.out.println( "Cap(TPS)                          : " + String.format( "%8d", cap                  ) );
        System.out.println( "Host                              : " + host                                         );
        System.out.println( "User                              : " + user                                         );
        System.out.println( "Password                          : " + password                                     );
        System.out.println( "Option                            : " + option                                       );
        
        contexts = new bmt_select_context[numberOfThreads];
        for ( thread = 0; thread < numberOfThreads; thread++ ) {
            contexts[thread] = new bmt_select_context( thread );
        }
        
        threads = new Thread[numberOfThreads];
        for ( thread = 0; thread < numberOfThreads; thread++ ) {
            threads[thread] = new Thread( contexts[thread] );
        }
        
        countDown = new CountDownLatch( numberOfThreads );
        mutex     = new ReentrantLock();
        
        remain    = numberOfTransactions;
        cursor    = startValue;
        allocated = 0;
        
        format = new SimpleDateFormat( "yyyy/MM/dd hh:mm:ss" );
        
        connectTime = System.currentTimeMillis();
        
        mutex.lock();
        
        for ( thread = 0; thread < numberOfThreads; thread++ ) {
            threads[thread].start();
        }
        
        try {
            countDown.await();
        } catch ( InterruptedException e ) {
            System.out.println( e );
            System.exit( -1 );
        }
        
        startTime = System.currentTimeMillis();
        elapsed = ( startTime - connectTime ) * 1000;
        second  = elapsed / 1000000;
        usecond = elapsed % 1000000;
        System.out.println( "Connection Elapsed                : " + String.format( "%d.%06d second", second, usecond ) );
        tm = new java.util.Date( startTime );
        System.out.println( "Start Time                        : " + format.format( tm ) + String.format( ".%06d", startTime % 1000 * 1000 ) );
        
        mutex.unlock();
        
        for ( thread = 0; thread < numberOfThreads; thread++ ) {
            try {
                threads[thread].join();
            } catch ( InterruptedException e ) {
                System.out.println( e );
                System.exit( 1 );
            }
        }
        
        endTime = System.currentTimeMillis();
        tm = new java.util.Date( endTime );
        System.out.println( "End Time                          : " + format.format( tm ) + String.format( ".%06d", endTime % 1000 * 1000 ) );
        
        Arrays.sort( contexts );
        
        minimum                  = Integer.MAX_VALUE;
        maximum                  = 0;
        sum                      = 0;
        numberOfLongTransactions = 0;
        for ( thread = 0; thread < numberOfThreads; thread++ ) {
            if ( minimum > contexts[thread].minimum ) minimum = contexts[thread].minimum;
            if ( maximum < contexts[thread].maximum ) maximum = contexts[thread].maximum;
            sum                      += contexts[thread].sum;
            numberOfLongTransactions += contexts[thread].numberOfLongTransactions;
        }
        average = Integer.parseInt( String.valueOf( Math.round( sum * 1.0 / numberOfTransactions ) ) );
        
        elapsed = ( endTime - startTime ) * 1000;
        second  = elapsed / 1000000;
        usecond = elapsed % 1000000;
        System.out.println( "Elapsed                           : " + String.format( "%d.%06d second", second, usecond ) );
        
        tps = (double)numberOfTransactions / ( (double)elapsed / 1e6 );
        System.out.println( "Transactions per Second           : " + String.format( "%9.2f TPS", tps ) );
        System.out.println( "Minimum                           : " + String.format( "%6d microseconds", minimum ) );
        System.out.println( "Maximum                           : " + String.format( "%6d microseconds", maximum ) );
        System.out.println( "Average                           : " + String.format( "%6d microseconds", average ) );
        System.out.println( "Number of Long Transactions       : " + String.format( "%6.2f%% ( %d / %d )",  numberOfLongTransactions * 1.0 / numberOfTransactions * 100, numberOfLongTransactions, numberOfTransactions ) );
        System.out.print( "Number of Transactions per Thread :" );
        for ( thread = 0; thread < numberOfThreads; thread++ ){
            System.out.print( String.format( " %d", contexts[thread].numberOfTransactions ) );
        }
        System.out.println( "" );
        System.out.println( "" );        
        
    }
    
}
