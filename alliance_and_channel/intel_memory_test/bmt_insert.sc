#define DESCRIPTION "INSERT KOS_TEST"

void* worker ( void* argument ) {
    
    EXEC SQL BEGIN DECLARE SECTION;
    char connection[1024];
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
    int  k11;
    int  k12;
    int  k13;
    int  k14;
    int  k15;
    int  k16;
    int  k17;
    int  k18;
    int  k19;
    int  k20;
    int  k21;
    int  k22;
    int  k23;
    int  k24;
    int  k25;
    int  k26;
    int  k27;
    int  k28;
    int  k29;
    int  k30;
    int  k31;
    int  k32;
    int  k33;
    int  k34;
    int  k35;
    int  k36;
    int  k37;
    int  k38;
    int  k39;
    int  k40;
    int  k41;
    int  k42;
    int  k43;
    int  k44;
    int  k45;
    char k46[31];
    char k47[31];
    char k48[31];
    char k49[31];
    char k50[31];
    EXEC SQL END DECLARE SECTION;
    
    int            start;
    int            end;
    int            value;
    struct timeval startTime;
    struct timeval endTime;
    int            elapsed;
    
    memset( k46, 0, sizeof(k46) );
    memset( k47, 0, sizeof(k47) );
    memset( k48, 0, sizeof(k48) );
    memset( k49, 0, sizeof(k49) );
    memset( k50, 0, sizeof(k50) );
    strcpy( k46, "test_insert_coscom1" );
    strcpy( k47, "abcdefghijklmnopqrs" );
    strcpy( k48, "abcdefghijklmnopqrs" );
    strcpy( k49, "0123456789012345678" );
    strcpy( k50, "test_insert_coscom2" );
    
    threadContext* context = (threadContext*)argument;
    
    strcpy( connection, context->connection );
    
    EXEC SQL AT :connection CONNECT :user IDENTIFIED BY :password USING :option;
    ASSERT_SQL;
    
    while ( allocateUnit( &start, &end ) > 0 ) {
        
        for ( value = start; value < end; value++ ) {
            
            assert( gettimeofday( &startTime, NULL ) == 0 );
            
            k01 = value;
            k02 = value;
            k03 = value;
            k04 = value;
            k05 = value;
            k06 = value;
            k07 = value;
            k08 = value;
            k09 = value;
            k10 = value;
            k11 = value;
            k12 = value;
            k13 = value;
            k14 = value;
            k15 = value;
            k16 = value;
            k17 = value;
            k18 = value;
            k19 = value;
            k20 = value;
            k21 = value;
            k22 = value;
            k23 = value;
            k24 = value;
            k25 = value;
            k26 = value;
            k27 = value;
            k28 = value;
            k29 = value;
            k30 = value;
            k31 = value;
            k32 = value;
            k33 = value;
            k34 = value;
            k35 = value;
            k36 = value;
            k37 = value;
            k38 = value;
            k39 = value;
            k40 = value;
            k41 = value;
            k42 = value;
            k43 = value;
            k44 = value;
            k45 = value;
            
            EXEC SQL AT :connection INSERT INTO KOS_TEST
                                       VALUES ( :k01, :k02, :k03, :k04, :k05, :k06, :k07, :k08, :k09, :k10, 
                                                :k11, :k12, :k13, :k14, :k15, :k16, :k17, :k18, :k19, :k20,
                                                :k21, :k22, :k23, :k24, :k25, :k26, :k27, :k28, :k29, :k30, 
                                                :k31, :k32, :k33, :k34, :k35, :k36, :k37, :k38, :k39, :k40, 
                                                :k41, :k42, :k43, :k44, :k45, :k46, :k47, :k48, :k49, :k50 );
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
