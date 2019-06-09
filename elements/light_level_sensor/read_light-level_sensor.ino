#include <Wire.h>
#include <SPI.h> //Why? Because library supports SPI and I2C connection

#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>

// I2C Configuration
// SDA (=SDI) ; SCL (=SCK)
#define SDA_I2C 5 // D1
#define SDL_I2C 4 // D2

#define DELAY_BETWEEN_READING_IN_S 5

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

uint16_t light = 0;
uint16_t broadband = 0;
uint16_t infrared = 0;

void readLightSensor() {
  sensors_event_t event;

  // Lux sensor
  tsl.getEvent(&event);
  if (event.light) {
    light = event.light;
  } else {
    light = 17000;
  }

  tsl.getLuminosity (&broadband, &infrared);
}

void initLightSensor() {
  if (!tsl.begin())
  {
    /* There was a problem detecting the TSL2561 ... check your connections */
    Serial.print("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
  }

  /* You can also manually set the gain or enable auto-gain support */
  // tsl.setGain(TSL2561_GAIN_1X);      /* No gain ... use in bright light to avoid sensor saturation */
  // tsl.setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
  tsl.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */

  /* Changing the integration time gives you better sensor resolution (402ms = 16-bit data) */
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */
}

void getLuxSensorInfoForDebug(void)
{
  sensor_t sensor;
  tsl.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" lux");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" lux");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" lux");
  Serial.println("------------------------------------");
  Serial.println("");
}

void setup() {
  Wire.begin(SDA_I2C, SDL_I2C);

  Serial.begin(115200);
  Serial.println("Starting");
  Serial.print("We are going to read light level from TSL2561 sensor value every ");
  Serial.print(DELAY_BETWEEN_READING_IN_S);
  Serial.println("s");

  initLightSensor();
}

void loop() {
  readLightSensor();

  Serial.print("Light level (LUX) = ");
  Serial.println(light);

  Serial.print("Infrared = ");
  Serial.println(infrared);

  Serial.print("Broadband = ");
  Serial.println(broadband);

  getLuxSensorInfoForDebug();

  delay(DELAY_BETWEEN_READING_IN_S * 1000);
}
