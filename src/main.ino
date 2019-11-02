#include <ESP8266WiFi.h>
#include <math.h>

// Everything required for WifiManager
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

// MQTT
#include <PubSubClient.h>

#include <Wire.h>
#include <SPI.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_TSL2561_U.h>

#include <MoistureSensor.h>

#include <MAX17043.h>

#include <DHT.h>
#include <DHT_U.h>

#define DELAY_BETWEEN_TWO_WIFI_CONNECT_IN_S 4

#define SLEEP_TIME_IN_S 900

#define BMP280_I2C_ADDRESS 0x76

//Change the "1050.35" to your city current barrometric pressure (https://www.wunderground.com)
#define CURRENT_CITY_BAROMETRIC 1011 // Amsterdam

// I2C Configuration
// SDA (=SDI) ; SCL (=SCK)
#define SDA_I2C 5 // D1
#define SDL_I2C 4 // D2

#define MOISTURE_SENSOR_SWITCH 12 // D6
#define MOISTURE_SENSOR_ANALOG_READ A0 // A0
#define NUM_READINGS 50
#define DELAY_AFTER_MOISTURE_SENSOR_POWER_UP_IN_MS 2000

#define ATTINY_I2C_ADDRESS 0x04
#define ATTINY_WAKEUP_PIN 0 // D3 - GPIO0

#define DHT22_DATA 14     // D5, Pin which is connected to the DHT sensor.
#define DHTTYPE DHT22     // DHT 22

// Define MQTT topics
const char* TOPIC_TEMPERATURE = "weather/outside/temperature";
const char* TOPIC_PRESSURE = "weather/outside/pressure";
const char* TOPIC_ALTIMETER = "weather/outside/altimeter";
const char* TOPIC_CHARGE = "weather/outside/charge";
const char* TOPIC_VOLTAGE = "weather/outside/voltage";
const char* TOPIC_HUMIDITY = "weather/outside/humidity";
const char* TOPIC_LIGHT = "weather/outside/light";
const char* TOPIC_INFRARED = "weather/outside/infrared";
const char* TOPIC_BROADBAND = "weather/outside/broadband";
const char* TOPIC_MOISTURE = "weather/outside/moisture";
const char* TOPIC_RAIN = "weather/outside/rain";
const char* TOPIC_TEMPERATURE_BOARD = "weather/outside/temperature_board";

Adafruit_BMP280 bmp; // BMP 280 (I2C)
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);
MoistureSensor ms(MOISTURE_SENSOR_ANALOG_READ, NUM_READINGS);
MAX17043 batteryMonitor;  // Fuel gauge
DHT_Unified dht(DHT22_DATA, DHTTYPE);

WiFiManager wifiManager;

// MQTT
const char* mqtt_server = "192.168.1.12";
WiFiClient espClient;
PubSubClient mqttClient(espClient);

// Barometric sensor
float pressure;           //To store the barometric pressure (Pa)
float temperatureInside;  //To store the temperature (oC)
int altimeter;            //To store the altimeter (m) (you can also use it as a float variable)

// Light sensor
uint16_t light = 0;
uint16_t broadband = 0;
uint16_t infrared = 0;

// Moisture sensor
float moistureSensorLastValue = 0;

// Battery infos
float cellVoltage;
float stateOfCharge;

// Rain gauge
uint16_t rainGaugeRawLastValue = 0;
uint16_t rainGaugeLastValue = 0;

// Temperature and humidity
float temperature;
float humidity;

int deepSleepTime;

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

  tsl.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
}

void initMoistureSensor() {
  pinMode(MOISTURE_SENSOR_SWITCH, OUTPUT);
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
  ms.reset();

  digitalWrite(MOISTURE_SENSOR_SWITCH, LOW);

  return moistureSensorLastValue;
}

void readBatteryLevelSensor() {
  cellVoltage = batteryMonitor.getVCell();
  stateOfCharge = batteryMonitor.getSoC();
}

void initBatteryLevelSensor() {
  batteryMonitor.reset();
  batteryMonitor.quickStart();
}

void initRainGauge() {
  pinMode(ATTINY_WAKEUP_PIN, OUTPUT);
}

void readGauge()
{
  digitalWrite(ATTINY_WAKEUP_PIN, HIGH);
  Wire.requestFrom(ATTINY_I2C_ADDRESS, 1);

  while (Wire.available())
  {
    rainGaugeRawLastValue = Wire.read();
  }
  digitalWrite(ATTINY_WAKEUP_PIN, LOW);
  rainGaugeLastValue = ceil(rainGaugeRawLastValue / 10);
}

void readDHT22() {
  sensors_event_t event;

  dht.temperature().getEvent(&event);
  if (!isnan(event.temperature)) {
    temperature = event.temperature;
  }
  dht.humidity().getEvent(&event);
  if (!isnan(event.relative_humidity)) {
    humidity = event.relative_humidity;
  }
}

void initTempAndHumiditySensor() {
  dht.begin();
}

void initWifi() {
  // If can't find previous network, start in AP and wait config
  if (!wifiManager.autoConnect("AP-Weather-Station")) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(DELAY_BETWEEN_TWO_WIFI_CONNECT_IN_S * 1000);
  }
}

void initMqtt() {
  mqttClient.setServer(mqtt_server, 1883);

  if (!mqttClient.connected()) {
    reconnectMqttClient();
  }
}

void reconnectMqttClient() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void debugSensorsValuesOnSerial() {
  Serial.print("Pressure = ");
  Serial.println(pressure);

  Serial.print("Temperature = ");
  Serial.println(temperatureInside);

  Serial.print("Altitude = ");
  Serial.println(altimeter);

  Serial.print("Light level (LUX) = ");
  Serial.println(light);

  Serial.print("Infrared = ");
  Serial.println(infrared);

  Serial.print("Broadband = ");
  Serial.println(broadband);

  Serial.print("Moisture = ");
  Serial.println(moistureSensorLastValue);

  Serial.print("Voltage = ");
  Serial.println(cellVoltage);

  Serial.print("Charge in percent = ");
  Serial.println(stateOfCharge);

  Serial.println("Read rain gauge value in half millimeter = ");
  Serial.println(rainGaugeLastValue);

  Serial.print("Temperature = ");
  Serial.println(temperature);

  Serial.print("Humidity = ");
  Serial.println(humidity);

  Serial.println("------------------------");
}

void readSensors() {
  readBMP280();
  readLightSensor();
  readMoistureSensor();
  readBatteryLevelSensor();
  readGauge();
  readDHT22();
}

void publishMqttMessages() {
  mqttClient.publish(TOPIC_PRESSURE, (char*) String(pressure).c_str());
  mqttClient.publish(TOPIC_TEMPERATURE_BOARD, (char*) String(temperatureInside).c_str());
  mqttClient.publish(TOPIC_ALTIMETER, (char*) String(altimeter).c_str());
  mqttClient.publish(TOPIC_LIGHT, (char*) String(light).c_str());
  mqttClient.publish(TOPIC_INFRARED, (char*) String(infrared).c_str());
  mqttClient.publish(TOPIC_BROADBAND, (char*) String(broadband).c_str());
  mqttClient.publish(TOPIC_MOISTURE, (char*) String(moistureSensorLastValue).c_str());
  mqttClient.publish(TOPIC_VOLTAGE, (char*) String(cellVoltage).c_str());
  mqttClient.publish(TOPIC_CHARGE, (char*) String(stateOfCharge).c_str());
  mqttClient.publish(TOPIC_RAIN, (char*) String(rainGaugeLastValue).c_str());
  mqttClient.publish(TOPIC_TEMPERATURE, (char*) String(temperature).c_str());
  mqttClient.publish(TOPIC_HUMIDITY, (char*) String(humidity).c_str());
}

void gotoDeepSleep() {

  // Optimization for avoid no battery
  deepSleepTime = SLEEP_TIME_IN_S;
  if (stateOfCharge < 60 && stateOfCharge >= 40) {
    deepSleepTime = SLEEP_TIME_IN_S * 2;
  } else if (stateOfCharge < 40 && stateOfCharge >= 30 ) {
    deepSleepTime = SLEEP_TIME_IN_S * 3;
  } else if (stateOfCharge < 30 && stateOfCharge >= 20) {
    deepSleepTime = SLEEP_TIME_IN_S * 5;
  } else if (stateOfCharge < 20) {
    deepSleepTime = SLEEP_TIME_IN_S * 10;
  }

  ESP.deepSleep(deepSleepTime * 1000000);
}

void setup() {
  initMoistureSensor();
  initRainGauge();

  Wire.begin(SDA_I2C, SDL_I2C);

  Serial.begin(115200);

  initBarrometricSensor();
  initLightSensor();
  initBatteryLevelSensor();
  initTempAndHumiditySensor();

  initWifi();

  initMqtt();

  mainLoop();

  gotoDeepSleep();
}

void mainLoop()
{
  mqttClient.loop();
  readSensors();
  debugSensorsValuesOnSerial();
  publishMqttMessages();
}

void loop() {
}
