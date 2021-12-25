#pragma once
/**
    ArdConfig.h
 
    Custom definition to manage the functionality for Microcontrollers 

    by Phil Schatzmann
 */

#if defined(ESP32)    
//  #define __MIDI__            // support Midi 
    #define __I2S__
    #define __RAW_ARRAYS__      // raw files as compilable c arrays
    #define __STREAMS__         // output to Ardiono Streams
    #define __RTOS__            // supports Free RTOS
    #define __LWIP__            // use lwip socket implementation
    #define TASK_STACK_SIZE configMINIMAL_STACK_SIZE

//  Uncomment after installing https://github.com/pschatzmann/ESP32-A2DP
//  #define __BT_A2DP__         // output to A2DP sink

#elif defined(ESP8266)
//  #define __MIDI__            // support Midi 
    #define __I2S__
    #define __RAW_ARRAYS__      
    #define __STREAMS__
    #define __NO_EXCEPTIONS__   // comment out exceptions
    #define __NO_FSTREAM__      // No Messanger, MidiFileIn and Skini and FileRead

    #include "FS.h"
    #undef round

#elif defined(ARDUINO)
//  #define __MIDI__            // support Midi 
    #define __ARDUINO__  // exclude standard c++ h files
    #define __STREAMS__  // output to Ardiono Streams
    #define __RAW_ARRAYS__      // raw files as compilable c arrays
    #define __NO_EXCEPTIONS__   // comment out exceptions
    #define __NO_FSTREAM__      // No Messanger, MidiFileIn and Skini and FileRead
    #define __NO_RT__           // no STK RT functions

    // Logging support
    #define STK_ERROR_MSG_LEN 200
    extern char stk_error_msg[STK_ERROR_MSG_LEN];
    #define ESP_LOGD(APP,...) { snprintf(stk_error_msg, STK_ERROR_MSG_LEN, __VA_ARGS__); Serial.println(stk_error_msg); }

#endif

// rtos must be loaded on top of other classes - so we add it here
// rtos support is experimental and most things might not work. I recommend
// that you do not use the netowrk functionality provided by the STK framework
// but that you use the Standard Arduino functionality instead. 
#ifdef __RTOS__  
    #include "freertos/FreeRTOS.h"
#endif
