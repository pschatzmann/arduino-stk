#include "Stk.h"
#ifdef __ESP__

#ifndef ARDI2SOUT_H
#define ARDI2SOUT_H

#include "WvOut.h"
#include "ArdCommonOut.h"
#ifdef ESP32
#include "driver/i2s.h"
#endif
#ifdef ESP8266
#include "i2s.h"
#endif

#define SAMPLE_RATE     (36000)
#define I2S_NUM         (0)
#define PI              (3.14159265)
#define I2S_BCK_IO      (GPIO_NUM_13)
#define I2S_WS_IO       (GPIO_NUM_15)
#define I2S_DO_IO       (GPIO_NUM_21)
#define I2S_DI_IO       (-1)


typedef int (*RtESPAudioCallback)( void *outputBuffer, 
                                unsigned int nFrames,
                                void *userData );

 static i2s_config_t default_i2s_config = {
    .mode = (i2s_mode_t) (I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN),
    .sample_rate = SAMPLE_RATE, // corrected by info from bluetooth
    .bits_per_sample = (i2s_bits_per_sample_t) sizeof(stk::StkFloat), 
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S_MSB,
    .intr_alloc_flags = 0, // default interrupt priority
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false
};

static i2s_pin_config_t default_pin_config = {
    .bck_io_num = I2S_BCK_IO,
    .ws_io_num = I2S_WS_IO,
    .data_out_num = I2S_DO_IO,
    .data_in_num = I2S_DI_IO                                              
};

namespace stk {

/***************************************************/
/*! \class ArdI2SOut
    \brief Output of sound to the I2S pins of the
    ESP32.

    ESP32 contains two I2S peripherals. These peripherals 
    can be configured to input and output sample data via
    the I2S driver. The I2S peripheral supports DMA meaning 
    it can stream sample data without requiring each 
    sample to be read or written by the CPU. I2S output can 
    also be routed directly to the 
    Digital/Analog Converter output channels (GPIO 25 & GPIO 26) 
    to produce analog output directly, 
    rather than via an external I2S codec.
    
    by Phil Schatzmann
*/
/***************************************************/

class ArdI2SOut: public ArdCommonOut {
    public:
        ArdI2SOut(i2s_port_t i2s_number, unsigned sample_per_cycle=512, unsigned channels=1);
        void setup();
        void setPinConfig(i2s_pin_config_t& pin_config);
        void setI2SConfig(i2s_config_t& i2s_config);

    protected:
        void writeBuffer(unsigned long len);
   
        i2s_config_t& i2s_config=default_i2s_config;
        i2s_pin_config_t& pin_config = default_pin_config;
        i2s_port_t i2s_number;

};

}; //stk
#endif
#endif
