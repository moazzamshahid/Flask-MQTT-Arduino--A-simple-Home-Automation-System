from msilib.schema import Control
import os
from tempfile import TemporaryDirectory
from flask import Flask, render_template, Response,request, jsonify,make_response
import datetime
import paho.mqtt.publish as publish
import sqlite3
from sqlite3 import Date, Error
import pandas as pd
from datetime import datetime
import json
import cv2
from sqlalchemy import true

camera=cv2.VideoCapture(0)

def generate_frames():
    while True:
            
        ## read the camera frame
        success,frame=camera.read()
        if not success:
            break
        else:
            ret,buffer=cv2.imencode('.jpg',frame)
            frame=buffer.tobytes()

        yield(b'--frame\r\n'
                   b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n')


conn = sqlite3.connect("Home_Automation")
crs = conn.cursor()

Control=0
app=Flask(__name__)
    
@app.route("/", methods=['GET', 'POST'])
def index():
    if request.method == 'POST':
        if list(request.form.keys())[0] == 'Bulb-On':
            Control=request.form['Bulb-On']
            print(request.form['Bulb-On'])
            publish.single("Bulb",Control, hostname="192.168.223.110")
            print(Control)
        if list(request.form.keys())[0] == 'Bulb-Off':
            print(request.form['Bulb-Off'])
            Control=request.form['Bulb-Off']
            publish.single("Bulb",Control, hostname="192.168.223.110")
            print(Control)
    conn = sqlite3.connect("Home_Automation")
    df1=pd.read_sql('SELECT * FROM Appliance_Status Where Status == "ON" OR Status == "OFF" ',conn)
    df=pd.read_sql("SELECT * FROM Temp_Humidity where Value not like ('b%')",conn)
    Last_status = df1.loc[df1["Appliance"]=="Bulb","Status"].tail(n=1).values[0]
    temprature = df.loc[df["Name"]=="Temperature","Value"].tail(n=1).values[0]
    humidity=df.loc[df["Name"]=="Humidity","Value"].tail(n=1).values[0] 
    Soil_mois=df.loc[df["Name"]=="Soil Moisture","Value"].tail(n=1).values[0]
    df1.loc[ (df1["Appliance"] == "Bulb") & (df1["Status"] == "ON") , "Status"] = 1
    df1.loc[ (df1["Appliance"] == "Bulb") & (df1["Status"] == "OFF") , "Status"] = 0 
    water_required=""
    # df1['Date']=pd.to_datetime(df1['Date'])
    app_date=df1['Date'].tolist()
   # print(app_date)
    
    Bulb_graph=df1[df1["Appliance"] == "Bulb"]["Status"].tolist()
    temp=[]
    temp1=[]
    for i in Bulb_graph:
        temp.append(int(i))
        temp1.append(temp)
        temp=[]
    Bulb_graph=temp1

    temp=[]
    temp1=[]
    for i in app_date:
        i = datetime.strptime(i, '%d/%m/%Y %H:%M:%S')
        temp.append(str(i))
    app_date=temp

    y = {"Status":Bulb_graph, "Date":app_date } 
  
    
    #----------------------------------------------------------------------------------------

    Temp_hist=df.loc[df["Name"]=="Temperature","Value"].tolist()
    Temp_Date=df.loc[df["Name"]=="Temperature","Date"].tolist()
    temp=[]
    temp1=[]
    for i in Temp_hist:
        temp.append(float(i))
        temp1.append(temp)
        temp=[]
    Temp_hist=temp1
    Soil_hist=df.loc[df["Name"]=="Soil Moisture","Value"].tolist()
    Soil_Date=df.loc[df["Name"]=="Soil Moisture","Date"].tolist()
    temp=[]
    temp1=[]
    for i in Soil_hist:
        temp.append(float(i))
        temp1.append(temp)
        temp=[]
    Soil_hist=temp1

    if Soil_mois >80:
        water_required="No"
    else:
        water_required="Yes"

    conn.commit()
    conn.close()
    

    
    return render_template("index.html", Last_status=Last_status, temprature=temprature,
    Humidity=humidity, SM=Soil_mois, app_date=app_date,Bulb_graph=Bulb_graph,
    Temp_hist=Temp_hist,Temp_Date=Temp_Date, Soil_hist=Soil_hist,Soil_Date=Soil_Date, water_required=water_required)

@app.route('/video')
def video():
    return Response(generate_frames(),mimetype='multipart/x-mixed-replace; boundary=frame')

if __name__=='__main__':
    app.run(host="0.0.0.0")
    #local web server http://192.168.1.200:5000/
    #after Port forwarding Manipulation http://xx.xx.xx.xx:5000/