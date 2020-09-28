#include "ArdI2SOut.h"
#ifdef ARDI2SOUT_H

#define BITS_PER_BYTE 8

namespace stk {


ArdI2SOut::ArdI2SOut(i2s_port_t i2s_number, unsigned packetFrames, unsigned nChannels)  
: ArdCommonOut(packetFrames, nChannels)
{
    setI2SConfig(default_i2s_config);
    setPinConfig(default_pin_config);
}


void ArdI2SOut::setI2SConfig(i2s_config_t& i2s_config) {
    this->i2s_config = i2s_config;
}

void ArdI2SOut::setPinConfig(i2s_pin_config_t& pin_config) {
    this->pin_config = pin_config;
}

void ArdI2SOut::setup(){
    // update sample rate
    i2s_config.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT;
    i2s_config.sample_rate = Stk::sampleRate() ;

    // setup drivers
    i2s_driver_install(i2s_number, &i2s_config, 0, NULL);
    i2s_set_clk(i2s_number, i2s_config.sample_rate, i2s_config.bits_per_sample, I2S_CHANNEL_MONO);
    i2s_set_pin(this->i2s_number, &pin_config);
}


void ArdI2SOut :: writeBuffer(unsigned long len) {
  i2s_write_bytes(i2s_number, (const char *)buffer_, len, portMAX_DELAY);
}


}; //STK
 #endif
