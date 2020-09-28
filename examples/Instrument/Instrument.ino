#include "ArdStreamOut.h"
#include "ArdMidiCommon.h"
#include "Clarinet.h"

using namespace stk;

ArdStreamOut output(Serial);
Clarinet instrument(440);
int note = 90; // starting midi note

void setup() {
  Serial.begin(115200);
}

void loop() {
  note += rand() % 10 - 5; // generate some random offset
  float frequency = ArdMidiCommon::noteToFrequency(note);

  instrument.noteOn( frequency, 0.5 );
  long endTime = millis()+1000;
  while (millis()<endTime){
      output.tick( instrument.tick() );
  }
  delay( 100 );

}
