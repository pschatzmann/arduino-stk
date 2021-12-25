#include "StkAll.h"

ArdI2SOut output;
Clarinet clarinet(440);
Voicer voicer;
int note = 90; // starting midi note
int group = 0;
float noteAmplitude = 128;

void setup() {
  Serial.begin(115200);
  voicer.addInstrument(&clarinet, group);

  i2s_pin_config_t my_pin_config = {
      .bck_io_num = 26,
      .ws_io_num = 25,
      .data_out_num = 22,
      .data_in_num = I2S_PIN_NO_CHANGE
  };

  output.setPinConfig(my_pin_config);
  output.begin();
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
