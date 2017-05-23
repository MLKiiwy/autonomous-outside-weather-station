#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library (you most likely already have this in your sketch)

#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <ESP8266mDNS.h>          // ??? => Todo use DNS for having a name

#include <MAX17043.h>             // Fuel gauge

#include <Wire.h>
#include <SPI.h> //Why? Because library supports SPI and I2C connection

#include <Adafruit_Sensor.h>

#include <Adafruit_BMP280.h>    // BMP280 Sensor

#include <DHT.h>  // DHT11
#include <DHT_U.h>

#include <Adafruit_TSL2561_U.h>

#include <PubSubClient.h>
const char* mqtt_server = "192.168.178.185";
WiFiClient espClient;
PubSubClient mqttClient(espClient);

Adafruit_BMP280 bmp; // BMP 280 (I2C)
MAX17043 batteryMonitor;  // Fuel gauge
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);

#define DHT11_DATA 14 // D5, Pin which is connected to the DHT sensor.
#define DHTTYPE DHT11     // DHT 11 

//Change the "1050.35" to your city current barrometric pressure (https://www.wunderground.com)
#define CURRENT_CITY_BAROMETRIC 1011 // Amsterdam

// I2C Configuration
// SDA (=SDI) ; SCL (=SCK)
#define SDA_I2C 5 // D1
#define SDL_I2C 4 // D2

// Time to sleep (in seconds):
const int sleepTimeS = 30;

const char* TOPIC_TEMPERATURE = "weather/outside/temperature";
const char* TOPIC_PRESSURE = "weather/outside/pressure";
const char* TOPIC_ALTIMETER = "weather/outside/altimeter";
const char* TOPIC_CHARGE = "weather/outside/charge";
const char* TOPIC_VOLTAGE = "weather/outside/voltage";
const char* TOPIC_HUMIDITY = "weather/outside/humidity";
const char* TOPIC_LIGHT = "weather/outside/light";

//Variables

// Barometric sensor
float pressure;           //To store the barometric pressure (Pa)
float temperatureInside;  //To store the temperature (oC)
int altimeter;            //To store the altimeter (m) (you can also use it as a float variable)

// Fuel gauge
float cellVoltage;
float stateOfCharge;

// DHT11
float temperature;
float humidity;

// Light
int light;

// DHT ( Temp + Humidity sensor)
DHT_Unified dht(DHT11_DATA, DHTTYPE);

void readSensors() {
  // From BMP280 Barometric sensor
  pressure = bmp.readPressure();
  temperatureInside = bmp.readTemperature();
  altimeter = bmp.readAltitude(CURRENT_CITY_BAROMETRIC);

  // From Fuel gauge
  cellVoltage = batteryMonitor.getVCell();
  stateOfCharge = batteryMonitor.getSoC();

  sensors_event_t event;

  // DHT 11
  dht.temperature().getEvent(&event);
  if (!isnan(event.temperature)) {
    temperature = event.temperature;
  }
  dht.humidity().getEvent(&event);
  if (!isnan(event.relative_humidity)) {
    humidity = event.relative_humidity;
  }

  // Lux sensor
  tsl.getEvent(&event);
  if (event.light) {
    light = event.light;
  } else {
    light = 0;
  }
}

void initBarrometricSensor() {
  bmp.begin();    //Begin the sensor
}

void initFuelGaugeSensor() {
  batteryMonitor.reset();
  batteryMonitor.quickStart();
}

void initTempAndHumiditySensor() {
  dht.begin();
}

void initLuxSensor() {
  if (!tsl.begin())
  {
    /* There was a problem detecting the TSL2561 ... check your connections */
    Serial.print("Ooops, no TSL2561 detected ... Check your wiring or I2C ADDR!");
  }

  configureLuxSensor();
}

void initMqtt() {
  mqttClient.setServer(mqtt_server, 1883);
}

void configureLuxSensor(void)
{
  /* You can also manually set the gain or enable auto-gain support */
  // tsl.setGain(TSL2561_GAIN_1X);      /* No gain ... use in bright light to avoid sensor saturation */
  // tsl.setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
  tsl.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */

  /* Changing the integration time gives you better sensor resolution (402ms = 16-bit data) */
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */
}

void getDHTInfoForDebug() {
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Temperature");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" *C");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" *C");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" *C");
  Serial.println("------------------------------------");
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("Humidity");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println("%");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println("%");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println("%");
  Serial.println("------------------------------------");
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
  delay(500);
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

void setup() {
  Serial.begin(115200);

  Wire.begin(SDA_I2C, SDL_I2C);

  initBarrometricSensor();
  initFuelGaugeSensor();
  initTempAndHumiditySensor();
  initLuxSensor();

  WiFiManager wifiManager;

  // If can't find previous network, start in AP and wait config
  if (!wifiManager.autoConnect("AP-ESP8266")) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }

  // mqtt
  initMqtt();

  if (!mqttClient.connected()) {
    reconnectMqttClient();
  }

  mqttClient.loop();
  readSensors();

  Serial.print("Publish messages");
    
  mqttClient.publish(TOPIC_PRESSURE, (char*) String(pressure).c_str());
  mqttClient.publish(TOPIC_ALTIMETER, (char*) String(altimeter).c_str());
  mqttClient.publish(TOPIC_VOLTAGE, (char*) String(cellVoltage).c_str());
  mqttClient.publish(TOPIC_CHARGE, (char*) String(stateOfCharge).c_str());
  mqttClient.publish(TOPIC_TEMPERATURE, (char*) String(temperature).c_str());
  mqttClient.publish(TOPIC_HUMIDITY, (char*) String(humidity).c_str());
  mqttClient.publish(TOPIC_LIGHT, (char*) String(light).c_str());

  ESP.deepSleep(sleepTimeS * 1000000);
}

void loop() {
}
