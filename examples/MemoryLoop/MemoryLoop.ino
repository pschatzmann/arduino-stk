#include "ArdStreamOut.h"
#include "MemoryLoop.h"

using namespace stk;

extern const unsigned char sinewave_raw[];
extern unsigned int sinewave_raw_len;

MemoryLoop input(Stk::rawwavePath() + "sinewave.raw", sinewave_raw, sinewave_raw_len);
ArdStreamOut output(Serial);

void setup() {
  Serial.begin(115200);

  Stk::setSampleRate( 44100.0 );
  input.setFrequency( 440.0 );
}

void loop() {
  output.tick( input.tick() );
}

