# coding=cp1252
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import datetime
import seaborn as sns
import  numpy as np
import pandas as pd
from pandas import DataFrame
import sys, getopt
import pyodbc
import datetime
import pandas as pd
import common
import inspect
### ½ÇÇà ¹æ¹ý
#ÆÄÀÏ¿¡ ´ëÇÑ À§Ä¡´Â 
#[ÆÄÀÌ¼Ç ½ÇÇà¸í·É] [ÆÄÀÌ½ã ½ÇÇàÆÄÀÏÀ§Ä¡] [SQL±¸¹®] [Ãâ·ÂÇÒ ÀÌ¹ÌÁöÆÄÀÏ¸í ¹× °æ·Î] 
# ÁÖÀ§ ¸ðµç °æ·Î´Â Àý´ë°æ·Î ÀÔ·ÂÇÔ
# Window ¿¹½Ã 
# python "E:\50_developer\33_python\CorrTest\CorrTest\correlationplot.py" "SELECT * FROM T_DAT_DCOR where CDATE  between  '2016-10-11 00:00:00' and  '2016-10-11 00:30:00'"  "E:\\50_developer\\33_python\\CorrTest\\correlationplot.png"

try:
    print(len(sys.argv));
    print(sys.argv[0])
    print(sys.argv[1])
    print(sys.argv[2])
    if len(sys.argv) != 3:          # ¿É¼Ç ¾øÀ¸¸é µµ¿ò¸» Ãâ·ÂÇÏ°í Á¾·á
      print ("[ÀÌ¹ÌÁö À§Ä¡] [SQL ±¸ºÐ]")
      exit(-1)
    print(sys.argv[0])
    print(sys.argv[1])
    print(sys.argv[2])
    cmd_sql=sys.argv[1] #"SELECT * FROM T_DAT_DCOR where CDATE  between  '2016-10-11 00:00:00' and  '2016-10-11 00:30:00'"
    filename=sys.argv[2] #'E:\\50_developer\\33_python\\CorrTest\\correlationplot.png'
    #con = pymysql.connect(host='59.14.104.158',port=3306, user='root', password='Qkdlxpr12#',db='test', charset='utf8')
    con = pyodbc.connect('DSN=ODBC_TEST1; SERVER=127.0.0.1; PORT_NO=20300; NLS_USE=UTF8; UID=dvup; PWD=databrick')

    print(" step 1")
	#con.close()
    df = pd.read_sql(cmd_sql, con ,index_col="CDATE" )
    print(" step 3")
    plt.rcParams["figure.figsize"] = (10,8)
    corr=df.corr();
    print(" step 3")
    xCol=corr.columns.values
    ax=sns.heatmap(corr,
            xticklabels=xCol,
            yticklabels=xCol)
    ax.set_xticklabels(xCol,rotation=90,fontsize=8)
    ax.set_yticklabels(xCol,rotation=0,fontsize=8)
    print(" step 4")
    plt.savefig(filename,  bbox_inches='tight')
    #plt.show()

  #  corr = data.corr(
  #  print(corr)
    #df['CDATE'] = pd.to_datetime(df['CDATE'])

#    output_file("tag_line.html")
#    show(p)

# create a new plot with a datetime axis type
    #p = figure(plot_width=800, plot_height=250, x_axis_type="datetime")

    #p.line(df['CDATE'], df['Kor_RegSplCull_0_12'], color='navy', alpha=0.5)
    #show(p)
except Exception as ex:
    ex_str = str(type(ex))
    print(ex_str)
#    common.JOBLOG(inspect.getfile(inspect.currentframe()), user, ex_str)
#    con.rollback()
#    traceback.print_exc()
finally:
   # cur.close()
    #con.close()
    plt.close()   
