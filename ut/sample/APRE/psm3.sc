/******************************************************************
 * SAMPLE1 : SQL/PSM
 *           1. Host variables of IN type can PSM array Type 
 *           2. Host variables of OUT type can PSM array Type 
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
    int      iCol[ARRAY_SIZE];
    long     lCol[ARRAY_SIZE];
    float    fCol[ARRAY_SIZE];
    double   dCol[ARRAY_SIZE];

    EXEC SQL END DECLARE SECTION;

    int i = 0;

    printf("<SQL/PSM 3>\n");

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
        sCol[i] = i + 1;
        iCol[i] = i + 10;
        lCol[i] = i + 100;
        fCol[i] = i + 1.1;
        dCol[i] = i + 1.01;
    }

    /* execute procedure */
    EXEC SQL EXECUTE
    BEGIN
        PSM3_1(:sCol in,
               :iCol in,
               :lCol in,
               :fCol in,
               :dCol in);
    END;
    END-EXEC;

    printf("------------------------------------------------------------------\n");
    printf("[Execute Procedure]\n");
    printf("------------------------------------------------------------------\n");

    /* check sqlca.sqlcode */
    if (sqlca.sqlcode == SQL_SUCCESS)
    {
        printf("Success execute procedure PSM3_1\n\n");
    }
    else
    {
        printf("Error : [%d] %s\n\n", SQLCODE, sqlca.sqlerrm.sqlerrmc);
    }

    for (i = 0; i < ARRAY_SIZE; i++ )
    {
        sCol[i] = 0;
        iCol[i] = 0;
        lCol[i] = 0;
        fCol[i] = 0;
        dCol[i] = 0;
    }
    
    /* execute procedure */
    EXEC SQL EXECUTE
    BEGIN
        PSM3_2(:sCol out,
               :iCol out,
               :lCol out,
               :fCol out,
               :dCol out);
    END;
    END-EXEC;

    printf("------------------------------------------------------------------\n");
    printf("[Execute Procedure]\n");
    printf("------------------------------------------------------------------\n");

    /* check sqlca.sqlcode */
    if (sqlca.sqlcode == SQL_SUCCESS)
    {
        printf("Success execute procedure PSM3_2\n\n");

        printf("sCol        iCol        lCol        fCol        dCol \n");
        for(i=0; i < ARRAY_SIZE; i++)
        {
            printf("%d           %d          %d         %f    %f\n", sCol[i], iCol[i], lCol[i], fCol[i], dCol[i]);
        }
    }
    else
    {
        printf("Error : [%d] %s\n\n", SQLCODE, sqlca.sqlerrm.sqlerrmc);
    }

    /* disconnect */
    EXEC SQL DISCONNECT;
    /* check sqlca.sqlcode */
    if (sqlca.sqlcode != SQL_SUCCESS)
    {
        printf("Error : [%d] %s\n\n", SQLCODE, sqlca.sqlerrm.sqlerrmc);
    }
}
