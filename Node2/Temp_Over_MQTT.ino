#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
//-----------------------
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

const char* ssid = "Mi A3";                           //!!!!!!!!!!!!!!!!!!!!!
const char* password = "pak12345";                //!!!!!!!!!!!!!!!!!!!!!
const char* mqtt_server = "192.168.223.110";                 //!!!!!!!!!!!!!!!!!!!!!

WiFiClient espClient;
PubSubClient client(espClient);


#define DHTPIN D2 
#define DHTTYPE    DHT11  

#define humidity_sensor_pin  A0


DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;

void reconnect() {
 // Loop until we're reconnected
 while (!client.connected()) {
 Serial.print("Attempting MQTT connection...");
 // Attempt to connect
 if (client.connect("ESP8266 Client-2")) {
  Serial.println("connected");
  // ... and subscribe to topic
  client.subscribe("ledStatus");
 } else {
  Serial.print("failed, rc=");
  Serial.print(client.state());
  Serial.println(" try again in 5 seconds");
  // Wait 5 seconds before retrying
  delay(5000);
  }
 }
}




void setup() {
  
Serial.begin(9600);
 WiFi.begin(ssid, password);
 while(WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
  }
 Serial.println("Connected to the WiFi network");
 
 client.setServer(mqtt_server, 1883);
  dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;

  pinMode(humidity_sensor_pin, INPUT);
}

void loop() {
if (!client.connected()) {
  reconnect();
 }
client.loop();
delay(10000);
Get_temp_hu();
read_Soil_moisture();
}

void Get_temp_hu(){
// Delay between measurements.
  delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    client.publish("Temperature",String(event.temperature).c_str());
    Serial.println(F("°C"));
    Serial.println("Sent");
  }
 // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    client.publish("Humidity",String(event.relative_humidity).c_str());
    Serial.println(F("%"));
    Serial.println("Sent");
  }

}
void read_Soil_moisture() {
  double value = analogRead(humidity_sensor_pin);
  value = map(value,750,180,0,100); 
  Serial.print(value);
  Serial.println(" %");
  client.publish("Garden",String(value).c_str());
  Serial.println("Sent");
}
