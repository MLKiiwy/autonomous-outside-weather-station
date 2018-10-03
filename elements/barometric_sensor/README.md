# Atmospheric Pressure (+Temperature and altitude sensor)

Use a BMP280 sensor.

See a good guide from adafruit here : https://learn.adafruit.com/adafruit-bmp280-barometric-pressure-plus-temperature-sensor-breakout/overview

## Install library 

- Search in arduino IDE the lib "Adafruit Unified Sensor" And "Adafruit BMP280 Library"

## Warning : if you use a clone of the "adafruit" BMP280 sensor

If like me, you use a clone (chinese) version of the BMP280 sensor, you may have a different I2C address for the component.
Normally the adafruit BMP280 use the address "0x77" as you can see here in the lib : https://github.com/adafruit/Adafruit_BMP280_Library/blob/master/Adafruit_BMP280.h#L37
In my case the address is : 0x76

To find your address, use an I2C scanner on the ESP8266 : [tools/i2c_scanner]
After this operation, I simply change the address use by the library by doing this : 

```c
#define BMP280_I2C_ADDRESS 0x76

// ....

void initBarrometricSensor() {
  if (!bmp.begin(BMP280_I2C_ADDRESS)) {  
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
  }
}

```


