#include <avr/io.h>
#include <avr/interrupt.h>

#define INTERRUPTPIN PCINT4 // == PB4 == PIN 3
#define DEBUG_LED PB1 // == PIN 6

volatile int ledOn = LOW;

void setup()
{
    cli();

    pinMode(DEBUG_LED, OUTPUT);
    pinMode(INTERRUPTPIN, INPUT); 

      digitalWrite(DEBUG_LED, ledOn);

    GIMSK=_BV(PCIE);
    PCMSK =_BV(INTERRUPTPIN);
    
    sei();
}
 
void loop()
{ 
    digitalWrite(DEBUG_LED, ledOn);
}

ISR(PCINT0_vect)
{    
    ledOn = ledOn == HIGH ? LOW : HIGH;
}
