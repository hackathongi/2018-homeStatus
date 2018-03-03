//PINS
const int readSensor = A0;
const int photoresistorSensor = D0;
const int temperatureSensor = D1;
const int doorSensor = D4;

//VARIABLES
int photoresistorValue;
int temperatureValue;
int doorValue;

//SETUP
void setup() {
  Serial.begin(9600);
  pinMode(photoresistorSensor, OUTPUT);
  pinMode(temperatureSensor, OUTPUT);
  pinMode(doorSensor, INPUT_PULLUP);
}

//MAIN
void loop() {
  //Read sensors
  readPhotoresistor();
  readTemperature();

  //Print data
  Serial.printf("Photoresistor: %d\n", photoresistorValue);
  Serial.printf("Temperature: %d\n", temperatureValue);
  Serial.printf("Door state: %d\n\n", digitalRead(doorSensor));
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
