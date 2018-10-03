#include <avr/sleep.h>
#include <TinyWireS.h>

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define I2C_SLAVE_ADDRESS 0x4 // Address of the slave (= Address of your ATTiny for your main board)
#define INTERRUPTPIN PCINT4 // == PB4 == PIN 3
#define DEBUG_LED PB1       // == PIN 6
#define DELAY_BETWEEN_BLINK 300
#define DELAY_WAIT 5000

volatile int count = 0;

void setup()
{
  count = 0;
  pinMode(DEBUG_LED, OUTPUT);
  pinMode(INTERRUPTPIN, INPUT);

  sbi(GIMSK, PCIE);         // Turn on Pin Change interrupt
  sbi(PCMSK, INTERRUPTPIN); // Which pins are affected by the interrupt

  TinyWireS.begin(I2C_SLAVE_ADDRESS); // Join the i2c network
  TinyWireS.onRequest(requestEvent);
}

void loop()
{

  for (int i = 0; i < count; i++)
  {
    digitalWrite(DEBUG_LED, HIGH);
    delay(DELAY_BETWEEN_BLINK);
    digitalWrite(DEBUG_LED, LOW);
    delay(DELAY_BETWEEN_BLINK);
  }

  delay(DELAY_WAIT);
}

ISR(PCINT0_vect)
{
  byte pinState;
  pinState = (PINB >> PINB4) & 1; //PINB is the register to read the state of the pins

  // Only count on state high (avoid the two count issue)
  if (pinState > 0)
  {
    count++;
  }
}

// Gets called when the ATtiny receives an i2c request
void requestEvent()
{
  TinyWireS.send(count);
}
