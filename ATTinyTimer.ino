//How long you want the interval to be, positive integer in seconds
#define INTERVAL 3600

#define PIN_VOUT 0
#define PIN_DONE 3
#define PIN_DBG 4
#define PIN_FUNC 1


const long interval = INTERVAL * 1000000;
const long blinkInterval = 1000000 / 2;

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


