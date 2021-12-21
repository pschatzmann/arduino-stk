#include "StkAll.h"

ArdI2SOut output;
Clarinet instrument(440);
int note = 90; // starting midi note

void setup() {
  i2s_pin_config_t my_pin_config = {
      .bck_io_num = 26,
      .ws_io_num = 25,
      .data_out_num = 22,
      .data_in_num = I2S_PIN_NO_CHANGE
  };

  output.setPins(my_pin_config);
  output.begin();
  Serial.begin(115200);
}

float noteToFrequency(uint8_t x) {
    int a = 440; //frequency of A (coomon value is 440Hz)
    return (a / 32) * pow(2, ((x - 9) / 12));
}

void loop() {
  note += rand() % 10 - 5; // generate some random offset
  float frequency = noteToFrequency(note);

  instrument.noteOn( frequency, 0.5 );
  long endTime = millis()+1000;
  while (millis()<endTime){
      output.tick( instrument.tick() );
  }
  delay( 100 );

}
