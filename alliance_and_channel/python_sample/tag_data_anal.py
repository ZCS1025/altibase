import pyodbc
import pandas as pd
import datetime
import common
from scipy import stats


con = pyodbc.connect('DSN=Altiodbc; SERVER=127.0.0.1; PORT_NO=20300; NLS_USE=UTF8; UID=dvup; PWD=databrick')

cur = con.cursor()


df = pd.read_sql('SELECT TAG_NAME, TAG_VALUE FROM T_DAT_SUMMARY', con)
#k = df.dropna(axis=0)

table = df.groupby(['TAG_NAME']).agg(['min', 'max', 'mean', 'median', 'std', 'var', 
'skew', 'quantile', 'count', 'sum'])
table.columns = table.columns.droplevel(level=0)    #dataframe 첫컬럼 제거
#t = t.rename(columns={'count': 'N'})    #column name 변경
#f = df.groupby(['tag_name']).describe(include = 'all');  #통계요약

gd = stats.gmean(df.iloc[:,1:2],axis=1)
#print(table)

##################################
query = ("""
        DROP TABLE T_DAT_DESC 
        """)
cur.execute(query)

sql = "CREATE TABLE T_DAT_DESC (tag_name varchar(100), \
                              min NUMERIC(16,3), \
                              max NUMERIC(16,3), \
                              mean NUMERIC(16,3), \
                              median NUMERIC(16,3), \
                              std NUMERIC(16,3), \
                              var NUMERIC(20,7), \
                              skew NUMERIC(20,7), \
                              quantile NUMERIC(20,7), \
                              count NUMERIC(16,3), \
                              sum NUMERIC(20,3))"
cur.execute(sql)

#commndex, row in table.iterrows():
for index,  row in table.iterrows():
        print(index)
        values1 = [index]
        query1 = "INSERT INTO T_DAT_DESC (tag_name"
        query2 = " values (?"
        for col in table.columns:
            values1 = values1 + [row[col]]
            query1 = query1 + "," + col
            query2 = query2 + ",?"
        query1 = query1 + ") " +  query2 + ")"
#       print(query1) 
#       print(values1)
        cur.execute(query1, values1) 

con.commit()
cur.close()
# n.save_db(t, "T_DAT_TDESC")
###################################

#t.to_sql(name='MYTABLE', con=con, flavor='mysql', if_exists='replace')

#t = df['tag_value'].min(axis=0)

#print(gg)


#gm = stats.gmean(df.iloc[:,1:3],axis=0)
#gt = stats.gmean(df.iloc[:,1:3],axis=1)
