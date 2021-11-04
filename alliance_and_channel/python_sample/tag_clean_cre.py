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
        df = pd.read_sql("select * from T_DAT_FILL where rownum < 2", con, index_col='CDATE')
        #df = pd.read_sql('select * from T_DAT_FILL', con)

        #print(df.columns)

        query = ("""
               DROP TABLE T_DAT_CLEAN 
               """)
        cur.execute(query)
        common.debugPrint("Drop table ..")

        idx = 0
        test = " CREATE TABLE T_DAT_CLEAN AS SELECT to_date(CDATE,'YYYY-MM-DD HH24:MI:SS')  CDATE"
        for col in df.columns:
            if    idx == 0:
                        test = test + "\n," + col
            elif  idx ==  1:
                        test = test + "\n," + 'to_date(' + col + ''', 'YYYY-MM-DD') ''' + col
            elif  idx ==  2:
                        test = test + "\n," + col
            else  :
                        test = test + "\n," + 'to_number(' + col + ') ' + col
            idx = idx + 1
        query = test + "\n from T_DAT_FILL"
#       print(query)
        cur.execute(query)
        con.commit()

        #print(table)
#        common.save_db(df, "T_DAT_FILL")
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
