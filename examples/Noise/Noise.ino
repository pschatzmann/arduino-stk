#include "Noise.h"
using namespace stk;

void setup() {
  Serial.begin(115200);

  StkFloat output;
  Noise noise;
  for ( unsigned int i=0; i<20; i++ ) {
    output = noise.tick();
    Serial.println(output);
  }
}

void loop() {
}
