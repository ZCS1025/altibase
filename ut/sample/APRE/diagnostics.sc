/******************************************************************
 * SAMPLE1 : DIAGNOSTICS
 *              INSERT with Arrays
 *              GET DIAGNOSTICS
 ******************************************************************/

/* specify path of header file */
EXEC SQL OPTION (INCLUDE=./include);
/* include header file for precompile */
EXEC SQL INCLUDE hostvar.h;

void print_diagnostics()
{
    EXEC SQL BEGIN DECLARE SECTION;
    int numErrors;
    int retsqlcode;
    char retsqlstate[6];
    char retmessage[2048];
    EXEC SQL END DECLARE SECTION;

    int j;

    EXEC SQL GET DIAGNOSTICS :numErrors = NUMBER;
    printf("> The Number of Errors : %d\n", numErrors);

    for (j = 1; j <= numErrors; j++)
    {
        EXEC SQL GET DIAGNOSTICS CONDITION :j
            :retsqlcode = RETURNED_SQLCODE,
            :retsqlstate = RETURNED_SQLSTATE,
            :retmessage = MESSAGE_TEXT;

        printf("> DiagRec(%d)\n", j);
        printf(">     SQLCODE  : [%d]\n", retsqlcode);
        printf(">     SQLSTATE : [%s]\n", retsqlstate);
        printf(">     MESSAGE  : [%s]\n", retmessage);
    }
    printf("\n");
}

int main()
{
    /* declare host variables */
    EXEC SQL BEGIN DECLARE SECTION;
    char usr[10];
    char pwd[10];
    char conn_opt[1024];

    /* for insert */
    char    a_gno[5][10+1];
    char    a_gname[5][20+1];
    char    a_goods_location[5][9+1];
    int     a_stock[5];
    double  a_price[5];

    EXEC SQL END DECLARE SECTION;

    printf("<DIAGNOSTICS>\n");

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

    /*** with INSERT ***/

    /* use scalar array host variables : column wise */

    strcpy(a_gno[0], "X111100001");
    strcpy(a_gno[1], "X111100002");
    strcpy(a_gno[2], "X111100002");
    strcpy(a_gno[3], "X111100003");
    strcpy(a_gno[4], "X111100003");
    strcpy(a_gname[0], "XX-201");
    strcpy(a_gname[1], "XX-202");
    strcpy(a_gname[2], "XX-203");
    strcpy(a_gname[3], "XX-204");
    strcpy(a_gname[4], "XX-205");
    strcpy(a_goods_location[0], "AD0010");
    strcpy(a_goods_location[1], "AD0011");
    strcpy(a_goods_location[2], "AD0012");
    strcpy(a_goods_location[3], "AD0013");
    strcpy(a_goods_location[4], "AD0014");
    a_stock[0] = 1000;
    a_stock[1] = 1000;
    a_stock[2] = 1000;
    a_stock[3] = 1000;
    a_stock[4] = 1000;
    a_price[0] = 5500.21;
    a_price[1] = 5500.45;
    a_price[2] = 5500.99;
    a_price[3] = 5500.09;
    a_price[4] = 5500.34;

    EXEC SQL INSERT INTO GOODS VALUES (:a_gno, :a_gname, :a_goods_location, :a_stock, :a_price);

    printf("------------------------------------------------------------------\n");
    printf("[Scalar Array Host Variables With Insert]\n");
    printf("------------------------------------------------------------------\n");

    /* check sqlca.sqlcode */
    if (sqlca.sqlcode == SQL_SUCCESS)
    {
        /* sqlca.sqlerrd[2] holds the rows-processed(inserted) count */
        printf("%d rows inserted\n", sqlca.sqlerrd[2]);
        /* sqlca.sqlerrd[3] holds the processing(insert) count when array in-binding */
        printf("%d times insert success\n\n", sqlca.sqlerrd[3]);
    }
    else
    {
        printf("Error : [%d] %s\n\n", SQLCODE, sqlca.sqlerrm.sqlerrmc);
        print_diagnostics();
    }

    /* disconnect */
    EXEC SQL DISCONNECT;
    /* check sqlca.sqlcode */
    if (sqlca.sqlcode != SQL_SUCCESS)
    {
        printf("Error : [%d] %s\n\n", SQLCODE, sqlca.sqlerrm.sqlerrmc);
    }
}

