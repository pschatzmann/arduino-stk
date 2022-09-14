#pragma once
/**
    ArdConfig.h
 
    Custom definition to manage the functionality for Microcontrollers 

    by Phil Schatzmann
 */

//#define STK_USE_FILES

#ifndef STK_USE_FILES
    #define __RAW_ARRAYS__      // raw files as compilable c arrays
#endif

#if defined(ESP32)    
//  #define __MIDI__            // support Midi 
    #define __I2S__
    #define __NO_EXCEPTIONS__
    #define __STREAMS__         // output to Ardiono Streams
    #define __RTOS__            // supports Free RTOS
    #define __LWIP__            // use lwip socket implementation
    #define __NO_RT__           // no STK RT functions
    #define TASK_STACK_SIZE configMINIMAL_STACK_SIZE
    #define RAWWAVE_PATH "/sdcard/rawwaves/"

//  Uncomment after installing https://github.com/pschatzmann/ESP32-A2DP
//  #define __BT_A2DP__         // output to A2DP sink

#elif defined(ESP8266)
//  #define __MIDI__            // support Midi 
//  #define __I2S__
    #define __STREAMS__
    #define __NO_EXCEPTIONS__   // comment out exceptions
    #define __NO_FSTREAM__      // No Messanger, MidiFileIn and Skini and FileRead
    #define __NO_RT__           // no STK RT functions
    #define RAWWAVE_PATH "/sdcard/rawwaves/"

    #include "FS.h"
    #undef round

#elif defined(ARDUINO_ARCH_STM32)
    #define __STREAMS__
    #define __ARDUINO__  // exclude standard c++ h files
    #define __NO_EXCEPTIONS__   // comment out exceptions
    #define __NO_FSTREAM__      // No Messanger, MidiFileIn and Skini and FileRead
    #define __NO_RT__           // no STK RT functions
    #define __USE_STM32_HACK__
    #define RAWWAVE_PATH "/sdcard/rawwaves/"
    #undef HAVE_GETTIMEOFDAY

#elif defined(ARDUINO)
//  #define __MIDI__            // support Midi 
    #define __ARDUINO__  // exclude standard c++ h files
    #define __STREAMS__  // output to Ardiono Streams
    #define __NO_EXCEPTIONS__   // comment out exceptions
    #define __NO_FSTREAM__      // No Messanger, MidiFileIn and Skini and FileRead
    #define __NO_RT__           // no STK RT functions
    #define RAWWAVE_PATH "/rawwaves/"

#elif defined(IS_DESKTOP)
//  #define __MIDI__            // support Midi 
    #define __ARDUINO__  // exclude standard c++ h files
    #define __STREAMS__  // output to Ardiono Streams
//  #define _STK_DEBUG_
    #define __NO_EXCEPTIONS__   // comment out exceptions
//    #define __NO_FSTREAM__      // No Messanger, MidiFileIn and Skini and FileRead
//    #define __NO_RT__           // no STK RT functions

#endif

// rtos must be loaded on top of other classes - so we add it here
// rtos support is experimental and most things might not work. I recommend
// that you do not use the netowrk functionality provided by the STK framework
// but that you use the Standard Arduino functionality instead. 
#ifdef __RTOS__  
    #include "freertos/FreeRTOS.h"
#endif

#ifdef __RAW_ARRAYS__
#include "raw/all.h"
#endif
