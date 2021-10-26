#define DESCRIPTION "SIMPLE_BENCHMARK_INSERT_HDB_CLI_THREAD"

void* worker ( void* argument ) {
    
    char           connection[1024];
    SQLHENV        env;
    SQLHDBC        dbc;
    SQLHSTMT       stmt;
    int            k01;
    int            k02;
    int            k03;
    int            k04;
    int            k05;
    int            k06;
    int            k07;
    int            k08;
    int            k09;
    int            k10;
    int            k11;
    int            k12;
    int            k13;
    int            k14;
    int            k15;
    int            k16;
    int            k17;
    int            k18;
    int            k19;
    int            k20;
    int            k21;
    int            k22;
    int            k23;
    int            k24;
    int            k25;
    int            k26;
    int            k27;
    int            k28;
    int            k29;
    int            k30;
    int            k31;
    int            k32;
    int            k33;
    int            k34;
    int            k35;
    int            k36;
    int            k37;
    int            k38;
    int            k39;
    int            k40;
    int            k41;
    int            k42;
    int            k43;
    int            k44;
    int            k45;
    char           k46[31];
    char           k47[31];
    char           k48[31];
    char           k49[31];
    char           k50[31];
 
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
    
    sprintf( connection, "%s;UID=%s;PWD=%s", option, user, password );
    
    assert( SQLAllocEnv( &env ) == SQL_SUCCESS );
    
    assert( SQLAllocConnect( env, &dbc ) == SQL_SUCCESS );
    
    assert( SQLDriverConnect( dbc, NULL, connection, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT ) == SQL_SUCCESS );
    
    assert( SQLAllocStmt( dbc, &stmt ) == SQL_SUCCESS );


    assert( SQLPrepare( stmt, "INSERT INTO TEST VALUES ( ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ? )", SQL_NTS ) == SQL_SUCCESS );

    
    assert( SQLBindParameter( stmt,  1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k01, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt,  2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k02, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt,  3, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k03, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt,  4, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k04, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt,  5, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k05, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt,  6, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k06, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt,  7, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k07, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt,  8, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k08, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt,  9, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k09, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 10, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k10, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 11, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k11, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 12, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k12, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 13, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k13, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 14, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k14, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 15, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k15, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 16, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k16, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 17, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k17, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 18, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k18, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 19, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k19, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 20, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k20, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 21, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k21, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 22, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k22, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 23, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k23, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 24, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k24, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 25, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k25, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 26, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k26, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 27, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k27, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 28, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k28, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 29, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k29, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 30, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k30, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 31, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k31, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 32, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k32, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 33, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k33, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 34, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k34, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 35, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k35, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 36, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k36, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 37, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k37, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 38, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k38, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 39, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k39, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 40, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k40, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 41, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k41, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 42, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k42, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 43, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k43, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 44, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k44, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 45, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &k45, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 46, SQL_PARAM_INPUT, SQL_C_CHAR,  SQL_VARCHAR, 30, 0, &k46, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 47, SQL_PARAM_INPUT, SQL_C_CHAR,  SQL_VARCHAR, 30, 0, &k47, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 48, SQL_PARAM_INPUT, SQL_C_CHAR,  SQL_VARCHAR, 30, 0, &k48, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 49, SQL_PARAM_INPUT, SQL_C_CHAR,  SQL_VARCHAR, 30, 0, &k49, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindParameter( stmt, 50, SQL_PARAM_INPUT, SQL_C_CHAR,  SQL_VARCHAR, 30, 0, &k50, 0, NULL ) == SQL_SUCCESS );
    
    assert( pthread_mutex_unlock( &(context->connected) ) == 0 );
    
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
            
            assert( SQLExecute( stmt ) == SQL_SUCCESS );
            
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
    
    assert( SQLFreeStmt( stmt, SQL_DROP ) == SQL_SUCCESS );
    
    assert( SQLDisconnect( dbc ) == SQL_SUCCESS );
    
    assert( SQLFreeConnect( dbc ) == SQL_SUCCESS );
    
    assert( SQLFreeEnv( env ) == SQL_SUCCESS );    
    
    return argument;
    
}
