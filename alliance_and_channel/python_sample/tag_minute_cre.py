import pyodbc
import pandas as pd
import datetime
import common
import inspect

def main():
    try:
        common.JOBLOG_START()

        user = "user"

        con = pyodbc.connect('DSN=Altiodbc; SERVER=127.0.0.1; PORT_NO=20300; NLS_USE=UTF8; UID=dvup; PWD=databrick')

        cur = con.cursor()

        common.debugPrint("Start read...")
#       df = pd.read_sql('select * from T_DAT_FILL rownum < 2', con, index_col='CDATE')
        df = pd.read_sql("select * from T_DAT_DCOR ", con, index_col='CDATE')
        #df = pd.read_sql('select * from T_DAT_FILL', con)

        #print(df.columns)

        query = ("""
               DROP TABLE T_DAT_MINSUMMARY
               """)
        cur.execute(query)
        common.debugPrint("Drop table ..")

        idx = 0
        test = " CREATE TABLE T_DAT_MINSUMMARY AS SELECT TO_DATE(TO_CHAR(CDATE,'YYYY-MM-DD HH24:MI'),'YYYY-MM-DD HH24:MI')  CDATE"
        print(test)
        for col in df.columns:
            test = test + "\n," + 'avg(' + col + ') ' + col
        query = test + "\n from T_DAT_DCOR GROUP BY TO_CHAR(CDATE,'YYYY-MM-DD HH24:MI')"
        cur.execute(query)
        con.commit()
        #print(table)
#
        common.JOBLOG(inspect.getfile(inspect.currentframe()), user)
        common.debugPrint("End of Job ..")

    except Exception as ex:
        ex_str = str(type(ex))
        print(ex_str)
        common.JOBLOG(inspect.getfile(inspect.currentframe()), user, ex_str)
        con.rollback()
        #traceback.print_exc()

    finally:
        cur.close()
        con.close()

if __name__ == '__main__':
    main()
