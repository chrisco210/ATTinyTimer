//How long you want the interval to be, positive integer in seconds
#define INTERVAL 60

#define PIN_VOUT 0
#define PIN_DONE 1


const unsigned long long interval = INTERVAL;
unsigned long long last;
bool currentState;

void setup() {
  pinMode(PIN_VOUT, OUTPUT);
  pinMode(PIN_DONE, INPUT);

  
  setState(true);
  last = millis();
}

void loop() {
  if((millis() - last) > (interval * 1000)) {
    cycle();
  } else if(digitalRead(PIN_DONE) == HIGH) {
    setState(false);
  } else {
  }
  
}

void cycle() {
  setState(false);
  delay(500);
  setState(true);
  last = millis();
}

//True for on, false for off
void setState(bool state) {
  currentState = state;
  if(state) {
    digitalWrite(PIN_VOUT, false);
  } else {
    digitalWrite(PIN_VOUT, true);
  }
}

