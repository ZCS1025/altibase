#define DESCRIPTION "SELECT KOS_TEST"

void* worker ( void* argument ) {
    
    EXEC SQL BEGIN DECLARE SECTION;
    char connection[1024];
    int  p01;
    int  k01;
    int  k02;
    int  k03;
    int  k04;
    int  k05;
    int  k06;
    int  k07;
    int  k08;
    int  k09;
    int  k10;
    EXEC SQL END DECLARE SECTION;
    
    int            start;
    int            end;
    int            value;
    struct timeval startTime;
    struct timeval endTime;
    int            elapsed;
    
    threadContext* context = (threadContext*)argument;
    
    strcpy( connection, context->connection );
    
    EXEC SQL AT :connection CONNECT :user IDENTIFIED BY :password USING :option;
    ASSERT_SQL;
    
    while ( allocateUnit( &start, &end ) > 0 ) {
        
        for ( value = start; value < end; value++ ) {
            
            assert( gettimeofday( &startTime, NULL ) == 0 );
            
            p01 = value;
            
            EXEC SQL AT :connection SELECT  K01,  K02,  K03,  K04,  K05,  K06,  K07,  K08,  K09,  K10
                                      INTO :k01, :k02, :k03, :k04, :k05, :k06, :k07, :k08, :k09, :k10
                                      FROM KOS_TEST
                                     WHERE K01 = :p01;
            ASSERT_SQL;
            
            assert( gettimeofday( &endTime, NULL ) == 0 );
            
            if ( endTime.tv_usec > startTime.tv_usec ) {
                elapsed = ( endTime.tv_sec  - startTime.tv_sec ) * 1000000 + ( endTime.tv_usec - startTime.tv_usec );
            } else {
                elapsed = ( endTime.tv_sec  - 1 - startTime.tv_sec ) * 1000000 + ( endTime.tv_usec + 1000000 - startTime.tv_usec );
            }
            
            if ( context->minimum > elapsed ) context->minimum = elapsed;
            if ( context->maximum < elapsed ) context->maximum = elapsed;
            context->sum += elapsed;
            context->numberOfTransactions++;
            if ( elapsed >= threshold ) context->numberOfLongTransactions++;
            
        }
        
    }
    
    EXEC SQL AT :connection DISCONNECT;
    ASSERT_SQL;
    
    return argument;
    
}
