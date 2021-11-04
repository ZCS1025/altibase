import sys
import os
import re
import pyodbc
import inspect
import common
import time
from  datetime import datetime
# load File Find
user="khlee"
ts = time.time()
timestamp = datetime.fromtimestamp(ts).strftime('%Y-%m-%d %H:%M:%S')
def tagFilefind():
    if len(sys.argv) is 1:
        ex_str="Not in directory name"
        common.JOBLOG(inspect.getfile(inspect.currentframe()), user, ex_str)
        return False
    os.chdir(sys.argv[1])
    for path, dirs, files in os.walk(sys.argv[1]):
    # print(path)
        if files:
            for filename in files:
                if tagInsert(os.path.join(path,filename)) == False:
                    return False
                print(os.path.join(path,filename))
        else:
            print("File Not Found")
            return False
    return True
#
def tagInsert(fileName):

    con = pyodbc.connect('DSN=ODBC_TEST1; SERVER=127.0.0.1; PORT_NO=20300; NLS_USE=US7ASCII; UID=dvup; PWD=databrick')
#   print("CONN Error")
#   con = pyodbc.connect('DNS=Altiodbc; SERVER=127.0.0.1; PORT_NO=20300; NLS_USE=US7ASCII; UID=dvup; PWD=databrick')
    cur = con.cursor()

#   cur.execute('select count(*) from T_COM_USER')
#   data = cur.fetchall()

#   for x in data:
#     print(x[0])
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

    old_char = '[,'
    new_char = '__'
    try:
        f = open(fileName,'r')
        while True:
            line = f.readline()
#           print(" read file",line)
            if not line: break
            value = line.split(';')
            s_value = float(value[3])
            tag_name = value[2][value[2].find('!')+1:]
            d_dt = value[0]+value[1]
            s_dt = d_dt[:d_dt.find('.')]
            log_dt = datetime.strptime(d_dt,'%Y-%m-%d%p %I:%M:%S.%f')
            log_date = datetime.strptime(s_dt,'%Y-%m-%d%p %I:%M:%S')
#           print("1")
            modify_date = datetime.now()
#           print("2")
            tag = tag_name.translate({ord(x):y for (x,y) in zip(old_char,new_char)})
            tag_1 = tag.translate({ord(']'):''})
            tag_2 = tag_1.replace("Program:","P_")
            tag_3 = tag_2.replace(".","_")
#           print("data",log_dt, log_date,tag_3,s_value,modify_date)
#           values = (log_dt,log_date,tag_3,s_value,timestamp)
#           query = ("insert into T_DAT_LOAD" "(log_dt,log_date,tag_name,tag_value,modify_date)" "values (%s,%s,%s,%s,%s)")
            cur.execute(
                         """ 
                         INSERT INTO T_DAT_LOAD (log_dt,log_date,tag_name,tag_value,modify_date) values (?,?,?,?,?)
                         """, 
                         (log_dt,log_date,tag_3,s_value,modify_date)
            )
#           print("first",value[0]+"Second",value[1]+" third",s_value)
        f.close
        con.commit()
        iquery = "insert into T_DAT_SUMMARY (log_date,tag_name,tag_value) select log_date, tag_name, avg(tag_value) from T_DAT_LOAD group by log_date,tag_name"
        cur.execute(iquery)
#       cur.execute('insert into T_DAT_SUMMARY select log_date, tag_name, avg(tag_value) from T_DATA_LOAD group by 1, 2')
        con.commit()
        con.close()
        return True
    except Exception as ex:
        con.close()
        ex_str = str(type(ex))
        common.JOBLOG(inspect.getfile(inspect.currentframe()), user, ex_str)
        return False
    return True

def main():
    print("본 프로그램은 TAG DATA 를  Altibase DB에 Inster 하는  프로그램입니다")
    print("============================")
    common.JOBLOG_START()
    flag=tagFilefind()
#   flag1=logUpdate()
    common.JOBLOG(inspect.getfile(inspect.currentframe()), user)
    print("===========================")
    print("프로그램이 종료 되었습니다. =>", flag)
if __name__ == '__main__':
    main()
