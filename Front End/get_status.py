import paho.mqtt.subscribe as subscribe
import sqlite3
from sqlite3 import Error
from datetime import datetime

def on_message_print(client, userdata, message):
    print("%s %s" % (message.topic, message.payload))
    now = datetime.now()
    dt_string = now.strftime("%d/%m/%Y %H:%M:%S") 
    conn = sqlite3.connect(r"D:\Masters\IoT\Project\Front End\Home_Automation")
    cursor = conn.cursor()
    if message.topic=="Temperature":
        cursor.execute("INSERT INTO Temp_Humidity(Date,Name,Unit, Value) VALUES(?,?,?,?)",(str(dt_string), "Temperature", "C", float(message.payload)))
    elif message.topic=="Humidity":
        cursor.execute("INSERT INTO Temp_Humidity(Date,Name,Unit, Value) VALUES(?,?,?,?)",(str(dt_string), "Humidity", "%", float(message.payload)))
    elif message.topic=="Garden":
        cursor.execute("INSERT INTO Temp_Humidity(Date,Name,Unit, Value) VALUES(?,?,?,?)",(str(dt_string), "Soil Moisture", "%", float(message.payload)))
    elif message.topic=="Garden":
        cursor.execute("INSERT INTO Temp_Humidity(Date,Name,Unit, Value) VALUES(?,?,?,?)",(str(dt_string), "Soil Moisture", "%", float(message.payload)))
    elif message.topic=="App-Ack-Bulb":
        cursor.execute("INSERT INTO Appliance_Status(Date,Appliance, Status) VALUES(?,?,?)",(str(dt_string), "Bulb", str(message.payload)))
    conn.commit()
    conn.close()
    


subscribe.callback(on_message_print, ["Temperature", "Humidity","Garden","Bulb"], hostname="192.168.223.110")