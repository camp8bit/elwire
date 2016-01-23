#include <NESpad.h>

NESpad nintendo = NESpad(10, 11, 12);

byte state = 0;

void setup() {
  // put your setup code here, to run once:

  pinMode(2, OUTPUT);  // channel A  
  pinMode(3, OUTPUT);  // channel B   
  pinMode(4, OUTPUT);  // channel C
  pinMode(5, OUTPUT);  // channel D    
  pinMode(6, OUTPUT);  // channel E
  pinMode(7, OUTPUT);  // channel F
  pinMode(8, OUTPUT);  // channel G
  pinMode(9, OUTPUT);  // channel H

}

#define NUM_STRANDS 6

byte mode = 0;
byte strand = 0;
long lastPressed = -5000;
long lastPressedAB = -5000;
byte lastState = 0;

bool onPush(byte button) {
  if (!(lastState & button) && (state & button)){
     return true;
  } else {
    return false;
  }
}

bool onRelease(byte button) {
  if ((lastState & button) && !(state & button)){
     return true;
  } else {
    return false;
  }
}

void loop() {

  if (millis() - lastPressed > 5000) {
    strand++;
  }

  lastState = state;
  state = nintendo.buttons();

  if ((state & NES_LEFT) || (state & NES_RIGHT) || (state & NES_UP) || (state & NES_DOWN)) {
    lastPressed = millis();
  }

  if ((state & NES_DOWN) || (state & NES_LEFT)) {
    strand--;
    if (strand < 0) {
      strand = NUM_STRANDS;
    }
  }
  
  if ((state & NES_UP) || (state & NES_RIGHT)) {
    strand++;
  }

  if (onPush(NES_SELECT)) {
     mode++;
     mode = mode % 5;
   }

  for (byte i = 0; i < NUM_STRANDS; i++ ) {
    byte v = LOW;

      if (strand % NUM_STRANDS == i) {
        v = HIGH;
      }
      
    if (millis() - lastPressedAB < 500) {
      v = LOW;
    }

    if ((state & NES_A) && (i % 2 == 1)){
      v = HIGH;
      lastPressedAB = millis();
    }
    
    if ((state & NES_B) && (i % 2 == 0)){
      v = HIGH;
      lastPressedAB = millis();
    }
    
    digitalWrite(i + 2, v);
  }

  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);

  delay(mode * 20 + 50);
}
