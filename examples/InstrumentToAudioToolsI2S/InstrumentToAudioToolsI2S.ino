// Output using https://github.com/pschatzmann/arduino-audio-tools
#include "AudioTools.h"
#include "StkAll.h"

I2SStream i2s;
ArdStreamOut output(&i2s);
Clarinet clarinet(440);
Voicer voicer;
int note = 90; // starting midi note
int group = 0;
float noteAmplitude = 128;

void setup() {
  Serial.begin(115200);
  voicer.addInstrument(&clarinet, group);

  auto cfg = i2s.defaultConfig(TX_MODE);
  cfg.bits_per_sample = 16;
  cfg.channels = 1;
  cfg.sample_rate = Stk::sampleRate();
  cfg.pin_bck = 26,
  cfg.pin_ws = 25,
  cfg.pin_data = 22,
  i2s.begin(cfg);
}


void loop() {
  note += rand() % 10 - 5; // generate some random offset

  // play note
  voicer.noteOn( note, noteAmplitude, group);
  long endTime = millis()+1000;
  while (millis()<endTime){
      output.tick( voicer.tick() );
  }

  // stop playing
  voicer.noteOff( note,noteAmplitude, group);
  endTime = millis()+100;
  while (millis()<endTime){
      output.tick( voicer.tick() );
  }

}
