#include <Wire.h>

#define ATTINY_I2C_ADDRESS 0x04
#define DELAY_BETWEEN_REQUEST 10000

uint8_t valueReceived = 0;

void setup()
{
  Wire.begin();       // join i2c
  Serial.begin(9600); // start serial for debug
  Serial.println("Starting");
  Serial.print("We are going to read rain value every ");
  Serial.print(DELAY_BETWEEN_REQUEST);
  Serial.println("ms");
}

void loop()
{
  Wire.requestFrom(ATTINY_I2C_ADDRESS, 1);

  while (Wire.available())
  {

    valueReceived = Wire.read();
    Serial.println("Read rain gauge value : ");
    Serial.println(valueReceived);
  }

  delay(DELAY_BETWEEN_REQUEST);
}
