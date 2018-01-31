
#include <Wire.h>

 #define I2C_SLAVE_ADDR 0x04

void setup() {
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  Serial.println("starting");    
}

uint8_t received;

void loop() {
  Wire.requestFrom(I2C_SLAVE_ADDR, 1);   

  while (Wire.available()) {
    received = Wire.read();
    Serial.println(received);   
  }
  
  delay(1000);
}
