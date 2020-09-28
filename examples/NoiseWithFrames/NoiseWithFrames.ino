#include "Noise.h"
using namespace stk;

Noise noise;
StkFrames output(20, 1);   // initialize StkFrames to 20 frames and 1 channel (default: interleaved)

void setup() {
  Serial.begin(115200);
}

void loop() {
  noise.tick( output );
  for ( unsigned int i=0; i<output.size(); i++ ) {
    Serial.println(output[i]);
  }
}
