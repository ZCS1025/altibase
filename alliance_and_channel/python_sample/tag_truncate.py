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

        common.debugPrint("Start job...")

        query = ("""
               TRUNCATE TABLE T_DAT_LOAD
               """)
        cur.execute(query)
        common.debugPrint("Truncated table T_DAT_LOAD")
        query = ("""
               TRUNCATE TABLE T_DAT_SUMMARY
               """)
        cur.execute(query)
        common.debugPrint("Truncated table T_DAT_SUMMARY")
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
