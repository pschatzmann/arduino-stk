#include <Voicer.h>
#include <Clarinet.h>
#include <ArdMidiBleEventHandler.h>
#include <ArdMidiBleServer.h>

using namespace stk;

Voicer voicer;
Clarinet clarinet(440);
ArdMidiBleEventHandler handler(&voicer);
ArdMidiBleServer ble("MidiServer", &handler);

void setup() {
  voicer.addInstrument(&clarinet, 0);
  ble.start(voicer);
}

StkFloat note = 64; // 0 to 128
StkFloat amplitude = 100; // 0 to 128

void loop() {
  Serial.print("playing ");
  Serial.println(++note);

  ble.noteOn( note, amplitude );
  delay(900);
  ble.noteOff( note, 20 );
  delay(200);
  if (note>=90) {
    note = 30;
  }
}
