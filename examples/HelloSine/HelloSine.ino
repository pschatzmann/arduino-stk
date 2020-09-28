#include "SineWave.h"
#include "ArdStreamOut.h"

using namespace stk;

SineWave input;
ArdStreamOut output(Serial);

void setup() {
  Serial.begin(115200);

  Stk::setSampleRate( 44100.0 );
  input.setFrequency( 440.0 );
}

void loop() {
  output.tick( input.tick() );
}

