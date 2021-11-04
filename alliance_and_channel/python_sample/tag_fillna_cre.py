import pyodbc
import pandas as pd
import datetime
import common
import inspect

def main():
    try:
        common.JOBLOG_START()

        user = "user"

        con = pyodbc.connect('DSN=Altiodbc; SERVER=127.0.0.1; PORT_NO=20300; NLS_USE=US7ASCII; UID=dvup; PWD=databrick')

        cur = con.cursor()

        common.debugPrint("Start read...")
        df = pd.read_sql('select * from T_DAT_MERG', con, index_col='CDATE')

        common.debugPrint("Start sort...")
        df.sort_index(axis=0, inplace=True)

        common.debugPrint("Start fillna...")
        df = df.fillna(method='ffill')
        df = df.fillna(method='bfill')


        #print(df)

        query = ("""
                DROP TABLE T_DAT_FILL
                """)
        cur.execute(query)

        idx = 0
        test = "CREATE TABLE T_DAT_FILL (CDATE VARCHAR(255)"
        for col in df.columns:
            test = test + "\n," + col + " VARCHAR(255)"
            idx = idx + 1

        query = test + ")"
        cur.execute(query)

        #print(table)
        #common.save_db(df, "T_DAT_FILL", con, cur)

        common.debugPrint("Start save_db...")

        for index, row in df.iterrows():
            values1 = [index]
            query1 = "INSERT INTO T_DAT_FILL(CDATE"
            query2 = " values (?"

            for col in df.columns:
                values1 = values1 + [row[col]]
                query1 = query1 + "," + col
                query2 = query2 + ",?"

            query1 = query1 + ") " +  query2 + ")"
            #break;
            #print(values1)
            #print(query1)
            cur.execute(query1, values1)

        con.commit()

        common.debugPrint(str(df.shape[0]) + " rows saved successfully!!")

        common.JOBLOG(inspect.getfile(inspect.currentframe()), user)

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
