import pyodbc
import datetime

start_time = datetime.datetime.now()

def save_db(df, table, con, cur):
    try:
#        debugPrint("Start save_db...")
#        engine = create_engine("mysql+mysqldb://root:"+"Qkdlxpr12#"+"@59.14.104.158/test", encoding='utf-8')
#        conn = engine.connect()
#        # Save dataframe to database
#
#        df.to_sql(name=table, con=engine, if_exists='append')
#        debugPrint(str(df.shape[0]) + " rows saved successfully!!")
#
        debugPrint("Start save_db...")

        for index, row in df.iterrows():
            print(df)
            values1 = [index]
            query1 = "INSERT INTO " + table + "(CDATE"
            query2 = " values (?"

            for col in df.columns:
                values1 = values1 + [row[col]]
                query1 = query1 + "," + col
                query2 = query2 + ",?"

            query1 = query1 + ") " +  query2 + ")"


            #print(query1)
            cur.execute(query1, values1)

        con.commit()

        debugPrint(str(df.shape[0]) + " rows saved successfully!!")
    except:
        traceback.print_exc()

    #finally:
        #conn.close()


def debugPrint(str):
    print("[" + datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S') + "] " + str)

def JOBLOG(Program_name, Job_Id, message = 1):
    try:
        global start_time
        con = pyodbc.connect('DSN=Altiodbc; SERVER=127.0.0.1; PORT_NO=20300; NLS_USE=US7ASCII; UID=dvup; PWD=databrick')
        cur = con.cursor()

        if message == 1:
            message = "successfully"
            s_flag = "S"
            end_message = "SUCCESS : end process..."
        else:
            s_flag = "E"
            end_message = "ERROR : end process..."

        values = (Program_name, Job_Id, start_time, datetime.datetime.now(), s_flag, message)
        query = ("insert into T_COM_JOBLOG" "(Program_name,Job_Id,start_time,End_time,S_flag,Message)" "values (?,?,?,?,?,?)")
        cur.execute(query,values)
        con.commit()
        debugPrint(end_message)

    except Exception as ex:
        ex_str = str(type(ex))
        print(ex_str)
        traceback.print_exc()

    finally:
        cur.close()
        con.close()

def JOBLOG_START():
    debugPrint("start process...")
    start_time = datetime.datetime.now()
