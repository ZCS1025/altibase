#define DESCRIPTION "SIMPLE_BENCHMARK_SELECT_HDB_CLI_THREAD"

void* worker ( void* argument ) {
    
    char           connection[1024];
    SQLHENV        env;
    SQLHDBC        dbc;
    SQLHSTMT       stmt;
    int            p01;
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
    
    int            start;
    int            end;
    int            value;
    struct timeval startTime;
    struct timeval endTime;
    int            elapsed;
    
    threadContext* context = (threadContext*)argument;
    
    sprintf( connection, "%s;UID=%s;PWD=%s", option, user, password );
    
    assert( SQLAllocEnv( &env ) == SQL_SUCCESS );
    
    assert( SQLAllocConnect( env, &dbc ) == SQL_SUCCESS );
    
    assert( SQLDriverConnect( dbc, NULL, connection, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT ) == SQL_SUCCESS );
    
    assert( SQLAllocStmt( dbc, &stmt ) == SQL_SUCCESS );
    
    assert( SQLPrepare( stmt, "SELECT K01, K02, K03, K04, K05, K06, K07, K08, K09, K10 FROM TEST WHERE K01 = ?", SQL_NTS ) == SQL_SUCCESS );
    
    assert( SQLBindParameter( stmt,  1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER,  0, 0, &p01, 0, NULL ) == SQL_SUCCESS );
    
    assert( SQLBindCol( stmt,  1, SQL_C_SLONG, &k01, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindCol( stmt,  2, SQL_C_SLONG, &k02, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindCol( stmt,  3, SQL_C_SLONG, &k03, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindCol( stmt,  4, SQL_C_SLONG, &k04, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindCol( stmt,  5, SQL_C_SLONG, &k05, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindCol( stmt,  6, SQL_C_SLONG, &k06, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindCol( stmt,  7, SQL_C_SLONG, &k07, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindCol( stmt,  8, SQL_C_SLONG, &k08, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindCol( stmt,  9, SQL_C_SLONG, &k09, 0, NULL ) == SQL_SUCCESS );
    assert( SQLBindCol( stmt, 10, SQL_C_SLONG, &k10, 0, NULL ) == SQL_SUCCESS );
    
    assert( pthread_mutex_unlock( &(context->connected) ) == 0 );
    
    while ( allocateUnit( &start, &end ) > 0 ) {
        
        for ( value = start; value < end; value++ ) {
            
            assert( gettimeofday( &startTime, NULL ) == 0 );
            
            p01 = value;
            
            assert( SQLExecute( stmt ) == SQL_SUCCESS );
            
            while( SQLFetch( stmt ) == SQL_SUCCESS );
            
            assert( SQLFreeStmt( stmt, SQL_CLOSE ) == SQL_SUCCESS );
            
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
