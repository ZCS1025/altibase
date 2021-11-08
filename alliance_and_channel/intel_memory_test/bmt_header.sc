#include <assert.h>
#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define ASSERT_SQL                                                       \
    if ( sqlca.sqlcode != SQL_SUCCESS ) {                               \
        printf( "Error : [%d] %s\n", SQLCODE, sqlca.sqlerrm.sqlerrmc ); \
        abort();                                                        \
    }

typedef struct threadContext {
    char       connection[1024];
    int        minimum;
    int        maximum;
    long long  sum;
    int        numberOfTransactions;
    int        numberOfLongTransactions;
} threadContext;

int numberOfThreads;
int startValue;
int numberOfTransactions;
int sizeOfUnit;
int threshold;
int cap;

struct timeval startTime;
struct timeval endTime;

pthread_mutex_t mutex;
int             remain;
int             cursor;
int             allocated;

EXEC SQL BEGIN DECLARE SECTION;
char user[1024];
char password[1024];
char option[1024];
EXEC SQL END DECLARE SECTION;

int compareThreadContexts ( const void* element1, const void* element2 ) {
    
    const threadContext* context1;
    const threadContext* context2;
    
    context1 = (const threadContext*)element1;
    context2 = (const threadContext*)element2;
    
    if ( context1->numberOfTransactions < context2->numberOfTransactions ) return -1;
    if ( context1->numberOfTransactions > context2->numberOfTransactions ) return  1;
    
    return 0;
    
}

int allocateUnit ( int* start, int* end ) {
    
    int size;
    int elapsed;
    
    pthread_mutex_lock( &mutex );
    
    if ( cap > 0 ) {
        assert( gettimeofday( &endTime, NULL ) == 0 );
        if ( endTime.tv_usec > startTime.tv_usec ) {
            elapsed = ( endTime.tv_sec  - startTime.tv_sec ) * 1000000 + ( endTime.tv_usec - startTime.tv_usec );
        } else {
            elapsed = ( endTime.tv_sec  - 1 - startTime.tv_sec ) * 1000000 + ( endTime.tv_usec + 1000000 - startTime.tv_usec );
        }
        while ( (double)allocated / ( (double)elapsed / 1000000. ) > (double)cap ) {
            pthread_yield();
            assert( gettimeofday( &endTime, NULL ) == 0 );
            if ( endTime.tv_usec > startTime.tv_usec ) {
                elapsed = ( endTime.tv_sec  - startTime.tv_sec ) * 1000000 + ( endTime.tv_usec - startTime.tv_usec );
            } else {
                elapsed = ( endTime.tv_sec  - 1 - startTime.tv_sec ) * 1000000 + ( endTime.tv_usec + 1000000 - startTime.tv_usec );
            }
        }
    }
    
    if ( remain > sizeOfUnit ) {
        size    = sizeOfUnit;
        remain -= sizeOfUnit;
    } else {
        size    = remain;
        remain -= remain;
    }
    *start     = cursor;
    *end       = cursor + size;
    cursor    += size;
    allocated += size;
    
    pthread_mutex_unlock( &mutex );
    
    return size;
      
}

