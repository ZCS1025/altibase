/******************************************************************
 * SAMPLE : NUMERIC
 *          1. Using SQL_NUMERIC_STRUCT
 ******************************************************************/

/* specify path of header file */
EXEC SQL OPTION (INCLUDE=./include);
/* include header file for precompile */
EXEC SQL INCLUDE hostvar.h;

int main()
{
    /* declare host variables */
    EXEC SQL BEGIN DECLARE SECTION;
    char usr[10];
    char pwd[10];
    char conn_opt[1024];

    /* scalar type */
    char                 s_gno[10+1];
    char                 s_gname[20+1];
    char                 s_goods_location[9+1];
    int                  s_stock;
    SQL_NUMERIC_STRUCT  s_price;
    EXEC SQL END DECLARE SECTION;
  
    int s_price_val = 0;
  
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

    /* use scalar host variables */
    strcpy(s_gno, "F111100002");
    strcpy(s_gname, "XX-101");
    strcpy(s_goods_location, "FD0003");
    s_stock = 5000;

    /* set value 123.4 on SQL_NUMERIC_STRUCT */
    memset(&s_price, 0, sizeof(s_price));
    s_price.precision = 4;
    s_price.scale = 1;
    s_price.sign = 1;
    s_price_val = 1234;
    memcpy(&s_price.val, &s_price_val, sizeof(int));

    printf("------------------------------------------------------------------\n");
    printf("[SQL_NUMERIC_STRUCT Insert]\n");
    printf("------------------------------------------------------------------\n");

    EXEC SQL INSERT INTO GOODS VALUES (:s_gno, :s_gname, :s_goods_location, :s_stock, :s_price);

    /* check sqlca.sqlcode */
    if (sqlca.sqlcode == SQL_SUCCESS)
    {
        /* sqlca.sqlerrd[2] holds the rows-processed(inserted) count */
        printf("%d rows inserted\n\n", sqlca.sqlerrd[2]);
    }
    else
    {
        printf("Error : [%d] %s\n\n", SQLCODE, sqlca.sqlerrm.sqlerrmc);
    }

    printf("------------------------------------------------------------------\n");
    printf("[SQL_NUMERIC_STRUCT Select]\n");
    printf("------------------------------------------------------------------\n");
    
    memset(s_gname, 0, sizeof(s_gname));
    memset(s_goods_location, 0, sizeof(s_goods_location));
    s_stock = 0;
    memset(&s_price, 0, sizeof(s_price));
    
    EXEC SQL SELECT GNAME, GOODS_LOCATION, STOCK, PRICE INTO :s_gname, :s_goods_location, :s_stock, :s_price FROM GOODS WHERE GNO = :s_gno;
    /* check sqlca.sqlcode */
    if (sqlca.sqlcode == SQL_SUCCESS)
    {
        /* sqlca.sqlerrd[2] holds the rows-processed(inserted) count */
        printf("%d rows select s_gno=%s, s_gname=%s, s_goods_location=%s, s_stock=%d, s_price=%.15G \n\n", 
                sqlca.sqlerrd[2], s_gno, s_gname, s_goods_location, s_stock, APRE_NUMERIC_TO_DOUBLE(s_price));
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
