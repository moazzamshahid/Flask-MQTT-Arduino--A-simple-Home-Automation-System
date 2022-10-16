#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
//#include <Adafruit_INA219.h>

const char* ssid = "PTCL216";                           //!!!!!!!!!!!!!!!!!!!!!
const char* password = "moazzam1";                //!!!!!!!!!!!!!!!!!!!!!
const char* mqtt_server = "192.168.100.224";                 //!!!!!!!!!!!!!!!!!!!!!

WiFiClient espClient;
PubSubClient client(espClient);


void callback(char* topic, byte* payload, unsigned int length) {
 Serial.print("Message arrived [");
 Serial.print(topic);
 Serial.print("] ");
 for (int i=0;i<length;i++) {
  char receivedChar = (char)payload[i];
  Serial.print(receivedChar);
  if (receivedChar == '1'){
  digitalWrite(LED_BUILTIN, HIGH);
  client.publish("LedStatus-Ack","Led Turned Off");
  }
  if (receivedChar == '0'){
   digitalWrite(LED_BUILTIN, LOW);
   client.publish("LedStatus-Ack","Led Turned On");
  }
  }
  Serial.println();
}

void reconnect() {
 // Loop until we're reconnected
 while (!client.connected()) {
 Serial.print("Attempting MQTT connection...");
 // Attempt to connect
 if (client.connect("ESP8266 Client")) {
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

void setup()
{
 Serial.begin(9600);
 WiFi.begin(ssid, password);
 while(WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
  }
 Serial.println("Connected to the WiFi network");
 client.setServer(mqtt_server, 1883);
 client.setCallback(callback);
 
 pinMode(LED_BUILTIN, OUTPUT);
 digitalWrite(LED_BUILTIN,HIGH);
 delay(5000);
 digitalWrite(LED_BUILTIN, LOW);
}
 
void loop()
{
 if (!client.connected()) {
  reconnect();
 }
 client.loop();
}
