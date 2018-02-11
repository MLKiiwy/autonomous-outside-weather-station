#include <Wire.h>

#define ATTINY_I2C_ADDRESS 0x04
#define ATTINY_WAKEUP_PIN A0
#define DELAY_BETWEEN_REQUEST 10000

uint8_t rainGaugeLastValue = 0;

void setup()
{
  pinMode(ATTINY_WAKEUP_PIN, OUTPUT);

  Wire.begin();       // join i2c
  Serial.begin(9600); // start serial for debug
  Serial.println("Starting");
  Serial.print("We are going to read rain value every ");
  Serial.print(DELAY_BETWEEN_REQUEST);
  Serial.println("ms");
}

void loop()
{
  Serial.println("Read rain gauge value : ");
  readGauge();
  Serial.println(rainGaugeLastValue);

  delay(DELAY_BETWEEN_REQUEST);
}

void readGauge()
{
  digitalWrite(ATTINY_WAKEUP_PIN, HIGH);
  Wire.requestFrom(ATTINY_I2C_ADDRESS, 1);

  while (Wire.available())
  {
    rainGaugeLastValue = Wire.read();
  }
  digitalWrite(ATTINY_WAKEUP_PIN, LOW);
}
