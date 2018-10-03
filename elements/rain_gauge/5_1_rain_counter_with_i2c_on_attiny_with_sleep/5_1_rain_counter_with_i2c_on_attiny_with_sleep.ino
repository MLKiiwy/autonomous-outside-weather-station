#include <avr/sleep.h>
#include <TinyWireS.h>

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define I2C_SLAVE_ADDRESS 0x4 // Address of the slave (= Address of your ATTiny for your main board)
#define INTERRUPTPIN_RAIN PCINT4 // == PB4 == PIN 3
#define INTERRUPTPIN_WAKEUP_AND_READMODE PCINT1 // == PB1 == PIN 6

volatile int rain_count = 0;

void setup()
{
  pinMode(INTERRUPTPIN_RAIN, INPUT);
  pinMode(INTERRUPTPIN_WAKEUP_AND_READMODE, INPUT);

  sbi(GIMSK, PCIE);         // Turn on Pin Change interrupt
  sbi(PCMSK, INTERRUPTPIN_RAIN); // Which pins are affected by the interrupt
  sbi(PCMSK, INTERRUPTPIN_WAKEUP_AND_READMODE); 

  TinyWireS.begin(I2C_SLAVE_ADDRESS); // Join the i2c network
  TinyWireS.onRequest(requestEvent);
}

void loop()
{
  if (HIGH == digitalRead(INTERRUPTPIN_WAKEUP_AND_READMODE)) {
  } else {
    system_sleep();
  }
}

// From http://interface.khm.de/index.php/lab/experiments/sleep_watchdog_battery/
void system_sleep() {
  cbi(ADCSRA,ADEN); // Switch Analog to Digital converter OFF
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Set sleep mode
  sleep_mode(); // System sleeps here
  sbi(ADCSRA,ADEN);  // Switch Analog to Digital converter ON
}

ISR(PCINT0_vect)
{
  byte pinState;
  pinState = (PINB >> PINB4) & 1; //PINB is the register to read the state of the pins

  // Only count on state high (avoid the two count issue)
  if (pinState > 0)
  {
    rain_count++;
  }
}

// Gets called when the ATtiny receives an i2c request
void requestEvent()
{
  TinyWireS.send(rain_count);
  rain_count = 0;
}
