
import sqlite3
from sqlite3 import Error
import pandas as pd

conn = sqlite3.connect("Home_Automation")
crs = conn.cursor()
sql = '''SELECT  *  FROM Temp_Humidity Where Name=="Temperature" Order by TH_ID desc Limit 1'''
crs.execute(sql)
result = crs.fetchall();
print(result)

df1=pd.read_sql('SELECT * FROM Appliance_Status',conn)
df=pd.read_sql('SELECT * FROM Temp_Humidity',conn)
#crs.execute(sql)
#result = crs.fetchall();

print(df1.dtypes)
print(df.dtypes)
print(df1)
print(df)
status = df1.loc[df1["Appliance"]=="Bulb","Status"].tail(n=1).values[0]
print(status)
conn.commit()
conn.close()