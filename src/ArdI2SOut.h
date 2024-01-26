#pragma once
#include "ArdConfig.h"


#include "Stk.h"
#include "WvOut.h"
#include "ArdCommonOut.h"
#ifdef ESP32
#  include "esp_idf_version.h"
#  if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0 , 0)
#    include "driver/i2s.h"
#    define ARD_USE_I2S
#  endif
#endif
#ifdef ESP8266
#  include "I2S.h"
#  define ARD_USE_I2S
#endif

#define SAMPLE_RATE     ((int)stk::SRATE)

#if defined(ESP32)  && defined(ARDUINO_ESP32C3_DEV)
#  define ESP32C3
#  define ESP32X
#endif
#if defined(ESP32)  && defined(ARDUINO_ESP32S2_DEV)
#  define ESP32S2
#  define ESP32X
#endif
#if defined(ESP32)  && defined(ARDUINO_ESP32S3_DEV)
#  define ESP32S3
#  define ESP32X
#endif

#ifdef ARD_USE_I2S

// Legacy support: We allow only the Standard ESP32 
#ifndef ESP32X
#  define I2S_BCK_IO      (GPIO_NUM_13)
#  define I2S_WS_IO       (GPIO_NUM_15)
#  define I2S_DO_IO       (GPIO_NUM_21)
#  define I2S_DI_IO       (-1)
#  ifndef I2S_NUM
#    define I2S_NUM         (0)
#  endif

#if ESP_IDF_VERSION_MAJOR < 4 && !defined(I2S_COMM_FORMAT_STAND_I2S)
# define I2S_COMM_FORMAT_STAND_I2S (I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB)
#endif


typedef int (*RtESPAudioCallback)( void *outputBuffer, 
                                unsigned int nFrames,
                                void *userData );

 static i2s_config_t default_i2s_config = {
    .mode = (i2s_mode_t) (I2S_MODE_MASTER | I2S_MODE_TX),
    .sample_rate = SAMPLE_RATE, // corrected by info from bluetooth
    .bits_per_sample = (i2s_bits_per_sample_t) 16, 
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = (i2s_comm_format_t)I2S_COMM_FORMAT_STAND_I2S,
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
/*! \class stk::ArdI2SOut
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
        ArdI2SOut(unsigned channels=1, i2s_port_t i2s_number=(i2s_port_t)0);
        void begin();
        void setPinConfig(i2s_pin_config_t& pin_config);
        void setI2SConfig(i2s_config_t& i2s_config);

    protected:   
        int byteCount;
        i2s_config_t& i2s_config=default_i2s_config;
        i2s_pin_config_t& pin_config = default_pin_config;
        i2s_port_t i2s_number;
        bool active = false;

        virtual void write(StkFloat value) override;
        virtual void write(int16_t value) override;

};

}; //stk

#endif
#endif

