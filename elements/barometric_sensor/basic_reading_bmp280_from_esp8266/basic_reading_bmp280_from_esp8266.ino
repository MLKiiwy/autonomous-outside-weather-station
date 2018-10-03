#include <Wire.h>
#include <SPI.h> //Why? Because library supports SPI and I2C connection

#include <Adafruit_Sensor.h>    // Adafruit Unified Library
#include <Adafruit_BMP280.h>    // BMP280 Sensor

#define DELAY_BETWEEN_READING_IN_S 5

#define BMP280_I2C_ADDRESS 0x76

//Change the "1050.35" to your city current barrometric pressure (https://www.wunderground.com)
#define CURRENT_CITY_BAROMETRIC 1011 // Amsterdam

// I2C Configuration
// SDA (=SDI) ; SCL (=SCK)
#define SDA_I2C 5 // D1
#define SDL_I2C 4 // D2

Adafruit_BMP280 bmp; // BMP 280 (I2C)

// Barometric sensor
float pressure;           //To store the barometric pressure (Pa)
float temperatureInside;  //To store the temperature (oC)
int altimeter;            //To store the altimeter (m) (you can also use it as a float variable)

void readBMP280() {
  // From BMP280 Barometric sensor
  pressure = bmp.readPressure();
  temperatureInside = bmp.readTemperature();
  altimeter = bmp.readAltitude(CURRENT_CITY_BAROMETRIC);
}

void initBarrometricSensor() {
  if (!bmp.begin(BMP280_I2C_ADDRESS)) {  
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
  }
}

void setup() {
  
  Wire.begin(SDA_I2C, SDL_I2C);
  
  Serial.begin(115200);
  Serial.println("Starting");
  Serial.print("We are going to read pressure,temperature,altitude from BMP280 sensor value every ");
  Serial.print(DELAY_BETWEEN_READING_IN_S);
  Serial.println("s");


  initBarrometricSensor();
}

void loop() {
  readBMP280();
  
  Serial.print("Pressure = ");
  Serial.println(pressure);

  Serial.print("Temperature = ");
  Serial.println(temperatureInside);

  Serial.print("Altitude = ");
  Serial.println(altimeter);
  
  delay(DELAY_BETWEEN_READING_IN_S * 1000);
}
