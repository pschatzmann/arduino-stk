/**
 * @file AppleMidiAudioKit.ino
 * @author Phil Schatzmann
 * @brief Example of handling midi input with Apple Midi and audio output to a AI Thinker AudioKit 
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <Midi.h>
#include <StkAll.h>
#include "ArdStkMidiAction.h" 
#include "AudioTools.h"
#include "AudioLibs/AudioKit.h"

AudioKitStream kit;
int channels = 2
ArdStreamOut out(kit, channels);
Clarinet clarinet(440);
StkMidiAction action(&clarinet);
AppleMidiServer apple(&action);

const char *SSID = "your ssid";
const char *PWD = "your password";

// setup wifi for apple midi udp
void setupWIFI() {
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
  MidiLogLevel = MidiInfo;
  StkLogLevel = StkInfo;

  auto cfg = kit.defaultConfig(TX_MODE);
  cfg.channels = channels;
  cfg.bits_per_sample = 16;
  cfg.sample_rate = Stk::sampleRate();
  kit.begin(cfg);

  setupWIFI();
  apple.begin(5004);
}

void loop() {
  apple.loop();  
  for (int j=0;j<256;j++){
    out.tick( action.tick() );
  }
  delay(1); // feed watchdog
}