#include "Voicer.h"
#include "Clarinet.h"
#include "ArdMidiStreamIn.h"

using namespace stk;

Voicer voicer;
Clarinet clarinet(440);
ArdMidiEventHandler handler(&voicer);
ArdMidiStreamIn in(Serial, handler);

void setup() {
  Serial.begin(115200);
  voicer.addInstrument(&clarinet);
}

void loop() {
  in.loop();
}
