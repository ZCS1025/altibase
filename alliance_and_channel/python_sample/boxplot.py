import matplotlib
matplotlib.use('Agg')
import numpy as np
import datetime
import seaborn as sns
import matplotlib.pyplot as plt
import pandas as pd
from pandas import DataFrame
import sys, getopt
import pyodbc
import datetime
import pandas as pd
import common
import inspect
### 실행 방법
#파일에 대한 위치는 
#[파이션 실행명령] [파이썬 실행파일위치] [SQL구문] [출력할 이미지파일명 및 경로] 
# 주위 모든 경로는 절대경로 입력함
# Window 예시 
# python E:\50_developer\33_python\CorrTest\CorrTest\boxplot.py "SELECT * FROM T_DAT_DCOR" "E:\50_developer\33_python\CorrTest\boxplot.png"
try:
    if len(sys.argv) != 3:          # 옵션 없으면 도움말 출력하고 종료
      print ("[이미지 위치] [SQL 구분]")
      exit(-1)
    print(sys.argv[0])
    print(sys.argv[1])
    print(sys.argv[2])
    cmd_sql=sys.argv[1] #"SELECT Kor_RegSplCull_0_0 as C0,Kor_RegSplCull_0_1 as C1,Kor_RegSplCull_0_2 as C2,Kor_RegSplCull_0_3 as C3 ,Kor_RegSplCull_0_4 as C4 FROM T_DAT_DCOR "
    filename=sys.argv[2] #'E:\\50_developer\\33_python\\CorrTest\\boxplot.png'
    print("step1")
    #con = pymysql.connect(host='59.14.104.158',port=3306, user='root', password='Qkdlxpr12#',db='test', charset='utf8')
    con = pyodbc.connect('DSN=Altiodbc; SERVER=127.0.0.1; PORT_NO=20300; NLS_USE=US7ASCII; UID=dvup; PWD=databrick')
    df = pd.read_sql(cmd_sql, con ,index_col="CDATE" )
    print("step2")
    plt.rcParams["figure.figsize"] = (10,5)
    g =sns.boxplot(data=df)
    print("step3")
    g.set_xticklabels(df.columns.tolist(),rotation=90,fontsize=8)
    plt.savefig(filename,  bbox_inches='tight')
    print("step4")
except Exception as ex:
    ex_str = str(type(ex))
    print(ex_str)
finally:
    #con.close()
    plt.close()   
exit(0)


