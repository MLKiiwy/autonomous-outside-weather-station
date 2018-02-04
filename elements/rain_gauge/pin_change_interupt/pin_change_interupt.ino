/*
 * Pin Change Interrupt Example
 * Version: 1.0
 * Author: Alex from Inside Gadgets (http://www.insidegadgets.com)
 * Created: 25/02/2011
 *
 * Demonstration of the pin change interrupt
 * LED on digital pin 0
 * Interrupt on digital pin 1
 * 10k resistor on digital pin 1 to GND
 *
 */
 
#include <avr/sleep.h>

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define INTERRUPTPIN PCINT4 // == PB4 == PIN 3
#define DEBUG_LED PB1 // == PIN 6

volatile int count = 0;

void setup(){
  count = 0;
  pinMode(DEBUG_LED,OUTPUT);
  pinMode(INTERRUPTPIN,INPUT);
  
  sbi(GIMSK,PCIE); // Turn on Pin Change interrupt
  sbi(PCMSK,INTERRUPTPIN); // Which pins are affected by the interrupt
}

void loop(){

  for( int i=0; i<count; i++) {
    digitalWrite(DEBUG_LED,HIGH); 
    delay(500);
    digitalWrite(DEBUG_LED,LOW);
    delay(500);
  }

  delay(4000);

  // system_sleep();
}

// From http://interface.khm.de/index.php/lab/experiments/sleep_watchdog_battery/
void system_sleep() {
  cbi(ADCSRA,ADEN); // Switch Analog to Digital converter OFF
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Set sleep mode
  sleep_mode(); // System sleeps here
  sbi(ADCSRA,ADEN);  // Switch Analog to Digital converter ON
}

ISR(PCINT0_vect) {
  count++;

  // To test
  byte pinState;
  pinState = (PINB >> PINB4)& 1; //PINB is the register to read the state of the pins
  if (pinState >0) {
    
  }
}

