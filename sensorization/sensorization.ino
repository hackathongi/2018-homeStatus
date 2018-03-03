//PINS
const int readSensor = A0;
const int photoresistorSensor = D0;
const int temperatureSensor = D1;
const int doorSensor = D2;
const int ultrasoundSensorEcho = D3;
const int ultrasoundSensorTrigger = D4;
const int soundSensor = D5;

//VARIABLES
int photoresistorValue;
int temperatureValue;
int doorValue;
int distanceValue;
int soundValue;

//SETUP
void setup() {
  Serial.begin(9600);
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
  Serial.printf("Photoresistor: %d\n", photoresistorValue);
  Serial.printf("Temperature: %d\n", temperatureValue);
  Serial.printf("Door state: %d\n", doorValue);
  Serial.printf("Distance: %d\n", distanceValue);
  Serial.printf("Sound: %d\n\n", soundValue);
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
  temperatureValue = analogRead(readSensor) - 273;
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
   
   distanceValue = duration * 10 / 292/ 2;
}

void readSound() {
  soundValue = digitalRead(soundSensor);
}

