import pyodbc
import pandas as pd
import datetime
import common
import inspect

try:
    common.JOBLOG_START()

    user = "user"

    interval = 1

    con = pyodbc.connect('DSN=Altiodbc; SERVER=127.0.0.1; PORT_NO=20300; NLS_USE=UTF8; UID=dvup; PWD=databrick')

    cur = con.cursor()

    hour_flag = datetime.datetime.strptime("20200801000000", '%Y%m%d%H%M%S')

    hour_8_1 = datetime.datetime(hour_flag.year,hour_flag.month,hour_flag.day,0,0,0)
    hour_8_2 = hour_8_1 + datetime.timedelta(hours=8)
    hour_8_3 = hour_8_1 + datetime.timedelta(hours=16)
    Timediff =  (hour_8_1 - hour_flag).total_seconds()

    query = ("""
            DROP TABLE TIMELIST
            """)
    cur.execute(query)

    query = ("""
            CREATE TABLE TIMELIST (CDATE VARCHAR(20)
                                  ,S_WEEK VARCHAR(20)
                                  ,S_DAY VARCHAR(20)
                                  ,S_EIGHT_HOUR VARCHAR(20))
            """)
    cur.execute(query)

    sstime = stime = datetime.datetime.strptime("20200801000000", '%Y%m%d%H%M%S')
    etime = datetime.datetime.strptime("20200831235959", '%Y%m%d%H%M%S')

    ttime = ((etime - stime).total_seconds()) // interval


    rowcnt = 1

    while stime < etime:
        nowTuple = stime.timetuple()
        if rowcnt % 40000 == 1:
            common.debugPrint(str(int(round((stime - sstime).total_seconds() / ttime,2) * 100)) + "%   :   " + str(int((stime - sstime).total_seconds())) + " / " + str(int(ttime)))
            #common.debugPrint(str(round(int((stime - sstime).total_seconds()) / int(ttime),2) * 100) + "%")

        Week = stime.strftime("%Y-") + str((stime + datetime.timedelta(seconds=Timediff)).isocalendar()[1])
        day = (stime + datetime.timedelta(seconds=Timediff)).strftime("%Y%m%d")
        #hour = (stime + datetime.timedelta(seconds=0)).strftime("%Y%m%d%H")

        if hour_8_2.hour > (stime + datetime.timedelta(seconds=Timediff)).hour:
            Eight_hour = day + '-1'
        elif hour_8_2.hour <= (stime + datetime.timedelta(seconds=Timediff)).hour and hour_8_3.hour > (stime + datetime.timedelta(seconds=Timediff)).hour:
            Eight_hour = day + '-2'
        else:
            Eight_hour = day + '-3'

        rowcnt = rowcnt + 1
        query = ("insert into TIMELIST values ('" + str(stime) + "','" +Week + "','" + day + "','" + Eight_hour + "')")
        cur.execute(query)

        stime = stime + datetime.timedelta(seconds=interval)

    con.commit()

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
