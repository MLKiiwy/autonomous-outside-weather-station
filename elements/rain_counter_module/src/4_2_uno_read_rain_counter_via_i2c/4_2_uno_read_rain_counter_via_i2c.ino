#include <Wire.h>
#include <math.h>

// Uno - Basic I2C	A4 (SDA), A5 (SCL)

#define ATTINY_I2C_ADDRESS 0x04
#define DELAY_BETWEEN_REQUEST 10000

uint16_t valueReceived = 0;

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

    // Why doing /10 => because the attiny count around 10 times close of the circuit for one real close
    // Need to find a way to avoid these jumps (maybe with a capacitor?)
    valueReceived = ceil(Wire.read() / 10);
    Serial.println("Read rain gauge value in half millimeter : ");
    Serial.println(valueReceived);
  }

  delay(DELAY_BETWEEN_REQUEST);
}
