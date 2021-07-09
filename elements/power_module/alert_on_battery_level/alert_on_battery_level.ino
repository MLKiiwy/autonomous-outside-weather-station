#include <Wire.h>
#include <MAX17043.h>   // Fuel gauge

#define DELAY_BETWEEN_READING_IN_S 5

// I2C Configuration
// SDA (=SDI) ; SCL (=SCK)
#define SDA_I2C 5 // D1
#define SDL_I2C 4 // D2

MAX17043 fuelGauge(20, 4);

void initFuelGaugeSensor() {
  fuelGauge.begin();
}

void setup() {
  Wire.begin(SDA_I2C, SDL_I2C);

  Serial.begin(115200);
  Serial.println("Starting");
  Serial.print("We are going to read the battery level from fuel gauge sensor value every ");
  Serial.print(DELAY_BETWEEN_READING_IN_S);
  Serial.println("s");

  initFuelGaugeSensor();
}

void output() {
  Serial.print("Version: ");
  Serial.println(fuelGauge.getVersion());
  Serial.print("Alert Threshold: ");
  Serial.println(fuelGauge.getAlertThreshold());
  Serial.print("Alert Threshold Register Version: ");
  Serial.println(fuelGauge.getAlertThresholdRegister());
  Serial.print("Battery Voltage: ");
  Serial.println(fuelGauge.getBatteryVoltage());
  Serial.print("Battery Percentage: ");
  Serial.println(fuelGauge.getBatteryPercentage());
  Serial.print("Is Alerting? ");
  Serial.println(fuelGauge.isAlerting());
  Serial.print("Is Sleeping? ");
  Serial.println(fuelGauge.isSleeping());
  Serial.print("Is Sleeping Register Version? ");
  Serial.println(fuelGauge.isSleepingRegister());
  Serial.println("");
}

void loop() {
  output();
  delay(DELAY_BETWEEN_READING_IN_S * 1000);
}
