import pyodbc
import datetime
import pandas as pd
import common
import inspect

try:

    common.JOBLOG_START()

    user = "user"

    con = pyodbc.connect('DSN=Altiodbc; SERVER=127.0.0.1; PORT_NO=20300; NLS_USE=UTF8; UID=dvup; PWD=databrick')

    cur = con.cursor()

    common.debugPrint("Start read...")
    df = pd.read_sql("select to_char(log_date,'YYYY-MM-DD HH24:MI:SS') LOG_DATE,  tag_name, tag_value from T_DAT_SUMMARY", con)

    # df = pd.DataFrame(result)
    # df.columns=['tag_name','tag_value']
#   print(df)
    common.debugPrint("Start pivot...")
    table = df.pivot(index='LOG_DATE',columns='TAG_NAME')
    common.debugPrint("step 1...")
    table.columns = table.columns.droplevel(level=0)
    common.debugPrint("step 2...")
    table.index.names = ['CDATE']

    #print(table)
    # table.to_csv('aaa.csv',index=False)

    query = ("""
            DROP TABLE T_DAT_TRAN
            """)
    cur.execute(query)

    idx = 0
    test1 = "insert into T_DAT_TRAN (\nCDATE"
    test = "CREATE TABLE T_DAT_TRAN (\nCDATE   VARCHAR(255)"

    for col in table.columns:
        test = test + "\n," + col + " VARCHAR(255)"
        test1 = test1 + "\n," + col
        idx = idx + 1

    query = (test + ")")
    cur.execute(query)

    #print(table.columns)
    #common.save_db(table, "T_DAT_TRAN", con, cur)

    common.debugPrint("Start save_db...")

    for index, row in table.iterrows():
        values1 = [index]
        query1 = "INSERT INTO T_DAT_TRAN(CDATE"
        query2 = " values (?"

        for col in table.columns:
            values1 = values1 + [row[col]]
            query1 = query1 + "," + col
            query2 = query2 + ",?"

        query1 = query1 + ") " +  query2 + ")"

#       print(values1)
#       print(query1)
        cur.execute(query1, values1)

    con.commit()

    common.debugPrint(str(table.shape[0]) + " rows saved successfully!!")

    common.JOBLOG(inspect.getfile(inspect.currentframe()), user)

except Exception as ex:
    ex_str = str(type(ex))
    print(ex_str)
    #common.JOBLOG(inspect.getfile(inspect.currentframe()), user, ex_str)
    con.rollback()
    #traceback.print_exc()

finally:
    cur.close()
    con.close()
