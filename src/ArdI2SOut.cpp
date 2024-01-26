#include "ArdI2SOut.h"
#if defined(ARD_USE_I2S)

#include "ArdStkLogger.h"

#define BITS_PER_BYTE 8

namespace stk {


ArdI2SOut::ArdI2SOut(unsigned nChannels, i2s_port_t i2s_number)  
: ArdCommonOut(nChannels)
{
    setI2SConfig(default_i2s_config);
    setPinConfig(default_pin_config);
    byteCount = sizeof(int16_t) * nChannels;
}


void ArdI2SOut::setI2SConfig(i2s_config_t& i2s_config) {
    this->i2s_config = i2s_config;
}

void ArdI2SOut::setPinConfig(i2s_pin_config_t& pin_config) {
    this->pin_config = pin_config;
}

void ArdI2SOut::begin(){
    // update sample rate
    i2s_config.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT;
    i2s_config.sample_rate = Stk::sampleRate() ;

    // setup drivers
    i2s_driver_install(i2s_number, &i2s_config, 0, NULL);
    i2s_set_clk(i2s_number, i2s_config.sample_rate, i2s_config.bits_per_sample, I2S_CHANNEL_MONO);
    i2s_set_pin(this->i2s_number, &pin_config);
    active = true;
}


// convert sample to int16
void ArdI2SOut::write(StkFloat value) {
    if (active){
        write(static_cast<int16_t>(value*32766));
    }
}

// output to stream in request number of channels
void ArdI2SOut::write(int16_t value) {
    int16_t buffer[nChannels];
    for (int j=0;j<nChannels;j++){
        buffer[j]=value;
    }
    size_t written;
    i2s_write(i2s_number, (const char *)buffer, byteCount, &written, portMAX_DELAY);
    if (byteCount!=written){
        STK_LOGE("i2s_write error: requested %zu - but only %zu bytew written", byteCount, written );
    }
};


}; //STK
 
 #endif
 