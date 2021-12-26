/**
 * @file MidiSerial.ino
 * @author Phil Schatzmann
 * @brief Example of handling midi input over a serial stream - The sound generation requires arduino-stk
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <Midi.h>
#include <StkAll.h>
#include "ArdStkMidiAction.h" 

ArdI2SOut i2s;
Clarinet clarinet(440);
StkMidiAction action(&clarinet);
MidiStreamIn in(Serial, action);

void setup() {
  Serial.begin(115200);
}

void loop() {
  in.loop();
  i2s.tick( action.tick() );
}
