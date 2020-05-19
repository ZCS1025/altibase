/******************************************************************
 * SAMPLE1 : SQL/PSM
 *           1. Fetch column value is NULL
 ******************************************************************/

#define ARRAY_SIZE 5

int main()
{
    /* declare host variables */
    EXEC SQL BEGIN DECLARE SECTION;
    char usr[10];
    char pwd[10];
    char conn_opt[1024];

    short    sCol[ARRAY_SIZE];
    EXEC SQL END DECLARE SECTION;

    int i = 0;

    printf("<SQL/PSM 4>\n");

    /* set username */
    strcpy(usr, "SYS");
    /* set password */
    strcpy(pwd, "MANAGER");
    /* set various options */
    strcpy(conn_opt, "Server=127.0.0.1"); /* Port=20300 */

    /* connect to altibase server */
    EXEC SQL CONNECT :usr IDENTIFIED BY :pwd USING :conn_opt;
    /* check sqlca.sqlcode */
    if (sqlca.sqlcode != SQL_SUCCESS)
    {
        printf("Error : [%d] %s\n\n", SQLCODE, sqlca.sqlerrm.sqlerrmc);
        exit(1);
    }

    for (i = 0; i < ARRAY_SIZE; i++ )
    {
        sCol[i] = 0;
    }
    
    /* execute procedure */
    EXEC SQL EXECUTE
    BEGIN
        PSM4(:sCol out);
    END;
    END-EXEC;

    printf("------------------------------------------------------------------\n");
    printf("[Execute Procedure]\n");
    printf("------------------------------------------------------------------\n");

    /* check sqlca.sqlcode */
    if (sqlca.sqlcode == SQL_SUCCESS)
    {
        printf("Success execute procedure PSM3_2\n\n");

    }
    else
    {
        printf("Error : [%d] %s\n\n", SQLCODE, sqlca.sqlerrm.sqlerrmc);
    }
    
    if( SQLCODE == (int)-331898)
    {
        printf("sCol \n");
        for(i=0; i < ARRAY_SIZE; i++)
        {
            if( APRE_SHORT_IS_NULL(sCol[i]) == true )
            {
                printf("NULL\n");
            }
            else
            {
                printf("%d\n", sCol[i]);
            }
        }
    }

    /* disconnect */
    EXEC SQL DISCONNECT;
    /* check sqlca.sqlcode */
    if (sqlca.sqlcode != SQL_SUCCESS)
    {
        printf("Error : [%d] %s\n\n", SQLCODE, sqlca.sqlerrm.sqlerrmc);
    }
}
