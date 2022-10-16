#define humidity_sensor_pin  A0
#define humidity_sensor_vcc  D0

void setup() {
  // Init the humidity sensor board
  pinMode(humidity_sensor_vcc, OUTPUT);
  digitalWrite(humidity_sensor_vcc, HIGH);

  // Setup Serial
  while (!Serial);
  delay(1000);
  Serial.begin(9600);
}

int read_humidity_sensor() {
  digitalWrite(humidity_sensor_vcc, LOW);
  delay(500);
  double value = analogRead(humidity_sensor_pin);
  value = map(value,750,180,0,100); 
  digitalWrite(humidity_sensor_vcc, HIGH);
  return (value);
}

void loop() {
  Serial.print("Humidity Level (0-1023): ");
  Serial.println(read_humidity_sensor()); 
  delay(10000);
}
