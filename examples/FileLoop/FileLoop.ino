#include "FileLoop.h"
#include "ArdStreamOut.h"

using namespace stk;

FileLoop input;
ArdStreamOut output(Serial);

void setup() {
  Serial.begin(115200);

  input.openFile( "rawwaves/sinewave.raw", true );

  Stk::setSampleRate( 44100.0 );
  input.setFrequency( 440.0 );
}

void loop() {
  output.tick( input.tick() );
}
