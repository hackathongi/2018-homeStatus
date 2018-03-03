//LIBRARIES
#include <ESP8266WiFi.h>
#include <string.h>

//CONSTANTS
const char* ssid = "hackathongi";
const char* password = "tarla2018";
const char* host = "84.89.60.4";
const int httpPort = 80;
const char* UNIQUE_ID = "sensors";
const int loopDelay = 5000;

//PINS
const int readSensor = A0;
const int photoresistorSensor = D0;
const int temperatureSensor = D1;
const int doorSensor = D2;
const int ultrasoundSensorEcho = D3;
const int ultrasoundSensorTrigger = D4;
const int soundSensor = D5;

//VARIABLES
float photoresistorValue;
float temperatureValue;
int doorValue;
int distanceValue;
int soundValue;

//SETUP
void setup() {
  Serial.begin(9600);
  
  /* We start by connecting to a WiFi network
   *
   * Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
   * would try to act as both a client and an access-point and could cause
   * network-issues with your other WiFi-devices on your WiFi-network. */
  Serial.printf("Trying to connect %s\n", ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connection successfull!!!!!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("\n");
  } else {
    Serial.println("Connection successfull!!!!!");
  }

  //Define pin modes
  pinMode(photoresistorSensor, OUTPUT);
  pinMode(temperatureSensor, OUTPUT);
  pinMode(doorSensor, INPUT_PULLUP);
  pinMode(ultrasoundSensorTrigger, OUTPUT);
  pinMode(ultrasoundSensorEcho, INPUT);
  pinMode(soundSensor, INPUT);
}

//MAIN
void loop() {
  //Read sensors
  readPhotoresistor();
  readTemperature();
  readDoorState();
  readDistance();
  readSound();

  //Print data
  Serial.printf("Photoresistor: %f\n", photoresistorValue);
  Serial.printf("Temperature: %f\n", temperatureValue);
  Serial.printf("Door state: %d\n", doorValue);
  Serial.printf("Distance: %d\n", distanceValue);
  Serial.printf("Sound: %d\n\n", soundValue);

  //Send to server
  sendDataToServer(photoresistorValue, temperatureValue, doorValue, distanceValue, soundValue);
  delay(loopDelay);
}

//FUNCTIONS
void readPhotoresistor() {
  digitalWrite(photoresistorSensor, HIGH);
  delay(1000);
  photoresistorValue = analogRead(readSensor);
  delay(1000);
  digitalWrite(photoresistorSensor, LOW);
}

void readTemperature() {
  digitalWrite(temperatureSensor, HIGH);
  delay(1000);
  temperatureValue = (analogRead(readSensor) - 273);
  delay(1000);
  digitalWrite(temperatureSensor, LOW);
}

void readDoorState () {
  doorValue = digitalRead(doorSensor);
}

void readDistance () {
   long duration;
   
   digitalWrite(ultrasoundSensorTrigger, LOW);
   delayMicroseconds(4);
   digitalWrite(ultrasoundSensorTrigger, HIGH);
   delayMicroseconds(10);
   digitalWrite(ultrasoundSensorTrigger, LOW);
   duration = pulseIn(ultrasoundSensorEcho, HIGH);
   
   distanceValue = duration * 0.034 / 2;
}

void readSound() {
  soundValue = digitalRead(soundSensor);
}

void sendDataToServer(float photoresistor, float temperature, int doorState, int distance, int sound) {
  Serial.println("Sending data to server....");

  //Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, httpPort)) {
    Serial.println("Connection failed!!");
    return;
  }

  char PostData[255];
  sprintf(PostData, "{\"fotoresistencia\":{\"value\":%f,\"type\":\"Float\"}, \"temperatura\":{\"value\":%f,\"type\":\"Float\"},\"estatPorta\":{\"value\":%d,\"type\":\"Integer\"},\"distancia\":{\"value\":%d,\"type\":\"Integer\"},\"so\":{\"value\":%d,\"type\":\"Integer\"}}", photoresistor, temperature, doorState, distance, sound);
  String Data = String(PostData);

  Serial.println(Data);
  
  client.println("POST /v2/entities/" + String(UNIQUE_ID) + "/attrs HTTP/1.1");  
  client.println("Content-Type: application/json");
  client.println("Content-Length: " + String(Data.length()));
  client.println();
  client.println(Data);

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 10000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  //Read all the lines of the reply from server and print them to Serial
  String response = "";
  while (client.available()){
    String line = client.readStringUntil('\r');
    response += line;
  }
  if (response.indexOf("HTTP/1.1 204 No Content") != -1) {
    Serial.println("OK");
  } else {
    Serial.print("KO: ");
    Serial.println(response);
  }
  
  Serial.println("Closing connection!");
}

