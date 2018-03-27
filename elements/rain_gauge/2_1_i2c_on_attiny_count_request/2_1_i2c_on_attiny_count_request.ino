#include <TinyWireS.h>

#define I2C_SLAVE_ADDRESS 0x4 // Address of the slave (= Address of your ATTiny for your main board)
#define DEBUG_LED PB1         // = PIN 6
#define DEBUG_LED_BLINK_DELAY 500

uint8_t count = 0;

void setup()
{
  count = 0;

  TinyWireS.begin(I2C_SLAVE_ADDRESS); // Join the i2c network
  TinyWireS.onRequest(requestEvent);

  // Turn on LED when program starts
  pinMode(DEBUG_LED, OUTPUT);
}

void loop()
{
  digitalWrite(DEBUG_LED, HIGH);
  delay(DEBUG_LED_BLINK_DELAY);
  digitalWrite(DEBUG_LED, LOW);
  delay(DEBUG_LED_BLINK_DELAY);

  // This needs to be here
  TinyWireS_stop_check();
}

// Gets called when the ATtiny receives an i2c request
void requestEvent()
{
  TinyWireS.send(count);
  count++;
}
