//PINS
const int photoresistorSensor = 0;
const int temperatureSensor = 1;
const int humiditySensor = 2;

//VARIABLES
int photoresistorValue;
int temperatureValue;
int humidityValue;

void setup() {
  Serial.begin(9600);
  pinMode(photoresistorSensor, INPUT);
  pinMode(temperatureSensor, INPUT);
  pinMode(humiditySensor, INPUT);
}

void loop() {
  //Read sensors
  photoresistorValue = analogRead(photoresistorSensor);
  temperatureValue = analogRead(temperatureSensor);
  humidityValue = analogRead(humiditySensor);

  //Print data
  Serial.print("Photoresistor: ");
  Serial.println(photoresistorValue);
  Serial.print("Temperature: ");
  Serial.println(temperatureValue);
  Serial.print("Humidity: ");
  Serial.println(humidityValue);
  delay(100);
}
