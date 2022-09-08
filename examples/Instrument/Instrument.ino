#include "Arduino.h"
#include "StkAll.h"

ArdStreamTextOut output(Serial);
BeeThree instrument;
//Clarinet instrument(440);
int note = 90; // starting midi note

float noteToFrequency(uint8_t x) {
    int a = 440; //frequency of A (coomon value is 440Hz)
    return (a / 32) * pow(2, ((x - 9) / 12));
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  note += rand() % 10 - 5; // generate some random offset
  float frequency = noteToFrequency(note);

  instrument.noteOn( frequency, 0.5 );
  long endTime = millis()+1000;
  while (millis()<endTime){
      output.tick( instrument.tick() );
  }
  delay( 100 );

}
