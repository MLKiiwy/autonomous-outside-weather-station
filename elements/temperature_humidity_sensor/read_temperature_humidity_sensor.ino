#include <DHT.h> 
#include <DHT_U.h>

#define DELAY_BETWEEN_READING_IN_S 5

#define DHT22_DATA 14     // D5, Pin which is connected to the DHT sensor.
#define DHTTYPE DHT22     // DHT 22

float temperature;
float humidity;

DHT_Unified dht(DHT22_DATA, DHTTYPE);

void readDHT22() {
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
}

void initTempAndHumiditySensor() {
  dht.begin();
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

void setup() {
  Serial.begin(115200);
  Serial.println("Starting");
  Serial.print("We are going to read pressure,temperature,altitude from BMP280 sensor value every ");
  Serial.print(DELAY_BETWEEN_READING_IN_S);
  Serial.println("s");

  initTempAndHumiditySensor();
}

void loop() {
  readDHT22();

  Serial.print("Temperature = ");
  Serial.println(temperature);

  Serial.print("Humidity = ");
  Serial.println(humidity);

  getDHTInfoForDebug();

  delay(DELAY_BETWEEN_READING_IN_S * 1000);
}
