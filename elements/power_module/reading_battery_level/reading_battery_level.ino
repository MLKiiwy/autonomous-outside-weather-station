#include <Wire.h>
#include <MAX17043.h>

#define DELAY_BETWEEN_READING_IN_S 5

// I2C Configuration
// SDA (=SDI) ; SCL (=SCK)
#define SDA_I2C 5 // D1
#define SDL_I2C 4 // D2

MAX17043 batteryMonitor;  // Fuel gauge

// Battery infos
float cellVoltage;
float stateOfCharge;

void readSensor() {
  cellVoltage = batteryMonitor.getVCell();
  stateOfCharge = batteryMonitor.getSoC();
}

void initSensor() {
  batteryMonitor.reset();
  batteryMonitor.quickStart();
}

void setup() {
  Wire.begin(SDA_I2C, SDL_I2C);

  Serial.begin(115200);
  Serial.println("Starting");
  Serial.print("We are going to read the battery level from fuel gauge sensor value every ");
  Serial.print(DELAY_BETWEEN_READING_IN_S);
  Serial.println("s");

  initSensor();
}

void loop() {
  readSensor();

  Serial.print("Voltage = ");
  Serial.println(cellVoltage);

  Serial.print("Charge in percent = ");
  Serial.println(stateOfCharge);

  delay(DELAY_BETWEEN_READING_IN_S * 1000);
}
