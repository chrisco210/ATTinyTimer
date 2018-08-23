

#include "LowPower.h"
//How long you want the interval to be, positive integer in seconds
#define INTERVAL 30

//Define if you want debug things


//Define your pins.  you may experience errors if you change these
#define PIN_VOUT 0
#define PIN_DONE 2

//Time elapsed since last reset
unsigned long elapsedTime = 0;
//If an interupt was received
volatile boolean intRec = false;   


typedef enum { STATE_SYTEM_ON, STATE_SYTEM_OFF } state_t;

//True for on, false for off
state_t currentState = STATE_SYTEM_ON;
state_t nextState = STATE_SYTEM_ON;

void setup() {
  //Main vout pin
  pinMode(PIN_VOUT, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(4, OUTPUT);

  
  
  pinMode(PIN_DONE, INPUT_PULLUP);
  delay(500);

  //MCUCR = (MCUCR & ~0x3);
  MCUCR = ((MCUCR | 0b00000001) & 0b11111101);
  GIMSK |= _BV(6);
  
}

void loop() {
  handleState();
  act();
  LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);
  //delay(1000);
  elapsedTime += 1;
}

void handleState() {
  nextState = currentState;
  switch(currentState) {
    case STATE_SYTEM_ON:
      if(elapsedTime > INTERVAL) {
        digitalWrite(1, HIGH);
        nextState = STATE_SYTEM_OFF;
      } else if(intRec) {
        nextState = STATE_SYTEM_OFF;
        digitalWrite(4, HIGH);
      }
      break;
    case STATE_SYTEM_OFF:
      if(elapsedTime > INTERVAL) {
        nextState = STATE_SYTEM_ON;
        elapsedTime = 0;
        digitalWrite(1, LOW);
        digitalWrite(4, LOW);
      }
      break;
  }
  intRec = false;
  currentState = nextState;
}

void act() {
  digitalWrite(3, HIGH);
  if(currentState == STATE_SYTEM_ON) {
    digitalWrite(PIN_VOUT, LOW);
  } else {
    digitalWrite(PIN_VOUT, HIGH);
  }
  delay(5);
  digitalWrite(3, LOW);
}


//Called on interupts
ISR(INT0_vect) {
  intRec = true;
  digitalWrite(4, HIGH);
}

/*
unsigned long last;
  
unsigned long ledLast;
bool currentState;
bool dbgLed = false;

void setup() {
  pinMode(PIN_FUNC, OUTPUT);
  pinMode(PIN_VOUT, OUTPUT);
  pinMode(PIN_DBG, OUTPUT);
    digitalWrite(PIN_FUNC, LOW);


  setState(true);
  ledLast = micros();
  last = micros();
}

void loop() {
  unsigned long micro = micros();
  
  if((micro - last) >= (interval)) {
    cycle();
    
  } else if(analogRead(PIN_DONE) * (5.0 / 1023.0) > 2.9) {
    setState(false);
  } 

  if(micro - ledLast > blinkInterval) {
    digitalWrite(PIN_DBG, dbgLed);
    dbgLed = !dbgLed;
    ledLast = micros();
  }

  //free(&micro);
  
}

void cycle() {
  setState(false);
  delay(5);
  setState(true);
  last = micros();
}

//True for on, false for off
void setState(bool state) {
  digitalWrite(PIN_FUNC, HIGH);
  if(state) {
    digitalWrite(PIN_VOUT, LOW);
  } else {
    digitalWrite(PIN_VOUT, HIGH);
  }

  //free(&state);
  digitalWrite(PIN_FUNC, LOW);
}
*/


