#include <Wire.h>
#include <SPI.h> //Why? Because library supports SPI and I2C connection


// I2C Configuration
// SDA (=SDI) ; SCL (=SCK)
#define SDA_I2C 5 // D1
#define SDL_I2C 4 // D2

#define ATTINY_I2C_ADDRESS 0x04
#define ATTINY_WAKEUP_PIN 0 // D3 - GPIO0
#define DELAY_BETWEEN_REQUEST 10000

uint8_t rainGaugeLastValue = 0;

void setup()
{
  pinMode(ATTINY_WAKEUP_PIN, OUTPUT);
  
  Serial.begin(9600); // start serial for debug
  Wire.begin(SDA_I2C, SDL_I2C);
    
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
