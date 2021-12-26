/**
 * @file MidiFromSerial.ino
 * @author Phil Schatzmann
 * @brief Example of handling midi input with Apple Midi - The sound generation requires arduino-stk
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
AppleMidiServer apple(&action);
const char *SSID = "your ssid";
const char *PWD = "your password";

void setupWIFI() {
  Serial.begin(119200);
  MidiLogLevel = MidiInfo;

  WiFi.begin(SSID, PWD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }

  Serial.print("Connected to IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  setupWIFI();
  apple.begin(5004);
}

void loop() {
  if (!apple.loop()){
    delay(10);
  }
  i2s.tick( action.tick() );
}