#define I2C_SLAVE_ADDRESS 0x4 // Address of the slave

#include <avr/io.h>
#include <avr/interrupt.h>
#include <TinyWireS.h>

#define INTERRUPTPIN 4 
#define DEBUG_LED 3
#define DATADIRECTIONPIN DDB1
#define ON_LED 1

volatile uint8_t value=0;
volatile uint8_t ledOn=0;

void setup()
{
    cli();//disable interrupts during setup
    value = 0;
    ledOn = HIGH;

    pinMode(DEBUG_LED, OUTPUT);
    pinMode(ON_LED, OUTPUT);
    
    TinyWireS.begin(I2C_SLAVE_ADDRESS); // join i2c network
    TinyWireS.onRequest(requestEvent);

    //PCMSK |= (1 << INTERRUPTPIN); //sbi(PCMSK,INTERRUPTPIN) also works but I think this is more clear // tell pin change mask to listen to pin2 /pb3 //SBI
    //GIMSK |= (1 << PCIE);         // enable PCINT interrupt in the general interrupt mask //SBI

    //DDRB &= ~(1 << DATADIRECTIONPIN); //cbi(DDRB, DATADIRECTIONPIN);//  set up as input  - pin2 clear bit  - set to zero
    //PORTB |= (1<< INTERRUPTPIN); //cbi(PORTB, PORTPIN);// disable pull-up. hook up pulldown resistor. - set to zero
     //GIMSK = 0b00100000;    // turns on pin change interrupts
     //PCMSK = 0b00010000;    // turn on interrupts on pins "PB4" = PIN3

//GIMSK |= _BV(PCINT4);
GIMSK=_BV(PCIE);
  PCMSK =_BV(PCINT4);
    
    sei();                 // enables interrupts
}
 
void loop()
{ 
    digitalWrite(ON_LED, HIGH);
    digitalWrite(DEBUG_LED, ledOn);
  
    // This needs to be here
    TinyWireS_stop_check();
}
 
// Gets called when the ATtiny receives an i2c request
void requestEvent()
{
    TinyWireS.send(value);
    TinyWireS.send('a');
}

ISR(PCINT0_vect)
{
  //Since the PCINTn triggers on both rising and falling edge let's just looks for rising edge
  //i.e. pin goes to 5v
  //byte pinState;
  //pinState = (PINB >> INTERRUPTPIN)& 1; //PINB is the register to read the state of the pins
  
 // if (pinState > 0)      //look at the pin state on the pin PINB register- returns 1 if high
  //{
    value++;

if (value >= 255) {
  value = 0;
}
    
    if (ledOn == HIGH) {
      ledOn = LOW;
    } else {
      ledOn = HIGH;
    }

  //}
}
