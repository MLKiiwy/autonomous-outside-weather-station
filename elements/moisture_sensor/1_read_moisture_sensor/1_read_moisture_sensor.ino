#include <MoistureSensor.h>

#define MOISTURE_SENSOR_SWITCH 2 // D4
#define MOISTURE_SENSOR_ANALOG_READ A0 // A0
#define NUM_READINGS 50

#define DELAY_IN_S 5
#define DELAY_AFTER_MOISTURE_SENSOR_POWER_UP_IN_MS 2000

MoistureSensor ms(MOISTURE_SENSOR_ANALOG_READ, NUM_READINGS);

float moistureSensorLastValue = 0;

void setup()
{
  pinMode(MOISTURE_SENSOR_SWITCH, OUTPUT);

  Serial.begin(9600); // start serial for debug
  Serial.println("Starting");
  Serial.print("We are going to read moisture sensor value every ");
  Serial.print(DELAY_IN_S);
  Serial.println("s");
}

float readMoistureSensor() {
  digitalWrite(MOISTURE_SENSOR_SWITCH, HIGH);

  // let the sensor warm up, the initial readings are not very accurate
  delay(DELAY_AFTER_MOISTURE_SENSOR_POWER_UP_IN_MS);
  
  for (int i = 0; i < NUM_READINGS; i++) {
    ms.reset();
    ms.read();
    delay(1);
  }
 
  moistureSensorLastValue = ms.getSmoothedReading();
   
  digitalWrite(MOISTURE_SENSOR_SWITCH, LOW);

  return moistureSensorLastValue;
}

void loop() {
  Serial.print("Moisture = ");
  Serial.println(readMoistureSensor());
  
  delay(DELAY_IN_S * 1000);
}

