#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

const char* ssid = "Mi A3";                           //!!!!!!!!!!!!!!!!!!!!!
const char* password = "pak12345";                //!!!!!!!!!!!!!!!!!!!!!
const char* mqtt_server = "192.168.223.110";
const int relay = D2;

WiFiClient espClient;
PubSubClient client(espClient);
void callback(char* topic, byte* payload, unsigned int length) {
  String receivedChar="";
   if (strcmp(topic,"Bulb")==0){
      for (int i=0;i<length;i++) {
        receivedChar = receivedChar + (char)payload[i]; 
      }
      Serial.println(receivedChar);
       if (receivedChar == "1"){
        digitalWrite(relay, HIGH);
        client.publish("App-Ack-Bulb","ON");
       }
       else{
        digitalWrite(relay, LOW);
        client.publish("App-Ack-Bulb","OFF");
       }
       
    
  }
 
}

void reconnect() {
 // Loop until we're reconnected
 while (!client.connected()) {
 Serial.print("Attempting MQTT connection...");
 // Attempt to connect
 if (client.connect("ESP8266 Client")) {
  Serial.println("connected");
  // ... and subscribe to topic
  client.subscribe("Bulb");
 } else {
  Serial.print("failed, rc=");
  Serial.print(client.state());
  Serial.println(" try again in 5 seconds");
  // Wait 5 seconds before retrying
  delay(5000);
  }
 }
}

void Wifi() {
 WiFi.begin(ssid, password);
 while(WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
  }
 client.setServer(mqtt_server, 1883);
 client.setCallback(callback);

}


void setup() {
  Serial.begin(115200);
  Wifi();
  pinMode(relay, OUTPUT);
  digitalWrite(relay,LOW);
}

void loop() {
 if (!client.connected()) {
  reconnect();
 }
 client.loop();

}
