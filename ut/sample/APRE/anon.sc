/******************************************************************
 * SAMPLE1 : SQL/ANONYMOUS BLOCK
 *           1. anonymous block EXECUTE
 ******************************************************************/

int main()
{
    /* declare host variables */
    EXEC SQL BEGIN DECLARE SECTION;
    char usr[64];
    char pwd[64];
    char conn_opt[1024];

    long long s_ono;
    EXEC SQL END DECLARE SECTION;

    printf("<SQL/ANONYMOUS BLOCK>\n");

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

    s_ono = 999999;
    /* execute anonymous block */
    EXEC SQL EXECUTE
        <<LABEL1>>
        DECLARE
            p_order_date date;
            p_eno        integer;
            p_cno        bigint;
            p_gno        char(10);
            p_qty        integer;
        BEGIN
            SELECT ORDER_DATE, ENO, CNO, GNO, QTY
            INTO p_order_date, p_eno, p_cno, p_gno, p_qty
            FROM ORDERS
            WHERE ONO = :s_ono;
        EXCEPTION
            WHEN NO_DATA_FOUND THEN
                p_order_date := SYSDATE;
                p_eno        := 13;
                p_cno        := BIGINT'7610011000001';
                p_gno        := 'E111100013';
                p_qty        := 4580;
                INSERT INTO ORDERS (ONO, ORDER_DATE, ENO, CNO, GNO, QTY)
                       VALUES (:s_ono, p_order_date, p_eno, p_cno, p_gno, p_qty);
            WHEN OTHERS THEN
                UPDATE ORDERS
                SET ORDER_DATE = p_order_date,
                    ENO        = p_eno,
                    CNO        = p_cno,
                    GNO        = p_gno,
                    QTY        = p_qty
                WHERE ONO = :s_ono;
        END;
    END-EXEC;

    printf("------------------------------------------------------------------\n");
    printf("[Execute Anonymous block]\n");
    printf("------------------------------------------------------------------\n");

    /* check sqlca.sqlcode */
    if (sqlca.sqlcode == SQL_SUCCESS)
    {
        printf("Success execute anonymous block \n\n");
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
