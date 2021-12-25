
// install https://github.com/pschatzmann/ESP32-A2DP
// and activate __BT_A2DP__ in ArdConfig.h
#include "StkAll.h"

Clarinet clarinet(440);
Voicer voicer;
ArdBtSource bt;
StkFloat note = 64; // 0 to 128
StkFloat amplitude = 100; // 0 to 128

void setup() {
  Serial.begin(115200);
  
  Stk::setSampleRate( 44100.0 );
  voicer.addInstrument(&clarinet);

  bt.start("RadioPlayer", voicer);
}

void loop() {
  if (bt.is_connected()) {
    Serial.print("playing ");
    Serial.println(++note);

    voicer.noteOn( note, amplitude );
    delay(900);
    voicer.noteOff( note, 20 );
    delay(200);
    if (note>=90) {
      note = 30;
    }
  }
}