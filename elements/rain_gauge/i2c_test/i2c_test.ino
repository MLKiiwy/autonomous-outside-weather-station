#define I2C_SLAVE_ADDRESS 0x4 // Address of the slave

#include <TinyWireS.h>

uint8_t i;

void setup()
{
    i = 0;
    
    TinyWireS.begin(I2C_SLAVE_ADDRESS); // join i2c network
    TinyWireS.onRequest(requestEvent);
 
    // Turn on LED when program starts
    pinMode(1, OUTPUT);
}
 
void loop()
{
  digitalWrite(1, HIGH);
  delay(500);
  digitalWrite(1, LOW);
  delay(500);
  
    // This needs to be here
    TinyWireS_stop_check();
}
 
// Gets called when the ATtiny receives an i2c request
void requestEvent()
{
    TinyWireS.send(i);
    i++;
}
