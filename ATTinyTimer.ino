#include "LowPower.h"
//How long you want the interval to be, positive integer in seconds
#define INTERVAL 30

#define PIN_VOUT 0
#define PIN_DONE 2
#define PIN_CFG_EN 1
#define PIN_CFG_OUT 3
#define PIN_CFG_IND 4

//Time elapsed since last reset
unsigned long elapsedTime = 0;
//If an interupt was received
volatile boolean intRec = false;   


typedef enum { STATE_SYSTEM_ON, STATE_SYSTEM_OFF, STATE_SYSTEM_CFG } state_t;

//True for on, false for off
state_t currentState = STATE_SYSTEM_ON;
state_t nextState = STATE_SYSTEM_ON;

void setup() {
  //Main vout pin
  pinMode(PIN_VOUT, OUTPUT);
  pinMode(PIN_DONE, INPUT_PULLUP);
  digitalWrite(PIN_CFG_OUT, LOW);
  pinMode(PIN_CFG_OUT, OUTPUT);
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
    case STATE_SYSTEM_ON:
      if(digitalRead(PIN_CFG_EN) == HIGH) {
        nextState = STATE_SYSTEM_OFF;
      } else if(elapsedTime > INTERVAL) {
        nextState = STATE_SYSTEM_OFF;
      } else if(intRec) {
        nextState = STATE_SYSTEM_OFF;
        //digitalWrite(4, HIGH);
      }
      break;
    case STATE_SYSTEM_OFF:
      if(digitalRead(PIN_CFG_EN) == HIGH) {
        
        nextState = STATE_SYSTEM_CFG;
      } else if(elapsedTime > INTERVAL) {
        nextState = STATE_SYSTEM_ON;
        elapsedTime = 0;
      }
      break;
    case STATE_SYSTEM_CFG:
      if(intRec) {
        nextState = STATE_SYSTEM_ON;
      }
      break;
  }
  intRec = false;
  currentState = nextState;
}

void act() {
  if(currentState == STATE_SYSTEM_CFG) {
    digitalWrite(PIN_CFG_OUT, HIGH);
  } else if(currentState == STATE_SYSTEM_ON) {
    digitalWrite(PIN_VOUT, LOW);
  } else {
    digitalWrite(PIN_VOUT, HIGH);
  }
}


//Called on interupts
ISR(INT0_vect) {
  intRec = true;
}
