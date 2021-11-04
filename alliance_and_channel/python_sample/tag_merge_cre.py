import pyodbc
import pandas as pd
import datetime
import common
import inspect

try:
    common.JOBLOG_START()

    user = "user"

    con = pyodbc.connect('DSN=Altiodbc; SERVER=127.0.0.1; PORT_NO=20300; NLS_USE=UTF8; UID=dvup; PWD=databrick')

    cur = con.cursor()

    common.debugPrint("Start join...")


    df1 = pd.read_sql('SELECT * FROM TIMELIST LIMIT 1', con,  index_col='CDATE')
    df2 = pd.read_sql('SELECT * FROM T_DAT_TRAN LIMIT 1', con, index_col='CDATE')

    query = ("""
            DROP TABLE T_DAT_MERG 
            """)
    cur.execute(query)
    common.debugPrint("Drop Table ...")

    query = "CREATE TABLE T_DAT_MERG (CDATE VARCHAR(255)"

    for col in df1.columns:
        query = query + "\n," +  col + " VARCHAR(255)"

    for col in df2.columns:
        query = query + "\n," + col + " VARCHAR(255)"

    query = query + ")"

    cur.execute(query)
    common.debugPrint(" Creat T_DAT_MERG Table ...")

    #query = ("""
    #        CREATE TABLE TEST_TIME LIKE TEST
    #        """)
    #cur.execute(query)

    query = "INSERT INTO T_DAT_MERG \nSELECT B.CDATE"

    for col in df1.columns:
        query = query + "\n, B." +  col

    for col in df2.columns:
        query = query + "\n, A." + col

    query = query + "\nFROM TIMELIST B left join T_DAT_TRAN A on A.CDATE = B.CDATE"

    cur.execute(query)

    con.commit()

    common.JOBLOG(inspect.getfile(inspect.currentframe()), user)

except Exception as ex:
    ex_str = str(type(ex))
    print(ex_str)
    common.JOBLOG(inspect.getfile(inspect.currentframe()), user, ex_str)
    con.rollback()
    traceback.print_exc()

finally:
    cur.close()
    con.close()
