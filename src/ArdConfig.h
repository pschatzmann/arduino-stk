/**
    ArdConfig.h
 
    Custom definition to manage the functionality for Microcontrollers 

    by Phil Schatzmann
 */

#if defined(ESP32)    
    #define __RAW_ARRAYS__      // raw files as compilable c arrays
    #define __STREAMS__         // output to Ardiono Streams
    #define __BT_A2DP__         // output to A2DP sink
    #define __MIDI__            // support Midi 
    #define __MIDI_BLE__        // support BLE 
    #define __RTOS__            // supports Free RTOS
    #define __LWIP__            // use lwip socket implementation
    #define TASK_STACK_SIZE configMINIMAL_STACK_SIZE

    #undef PI
    #undef TWO_PI

#elif defined(ESP8266)
    #define __RAW_ARRAYS__      
    #define __STREAMS__
    #define __MIDI__  
    #define __NO_EXCEPTIONS__   // comment out exceptions
    #define __NO_FSTREAM__      // No Messanger, MidiFileIn and Skini and FileRead

    #include "FS.h"
    #undef PI
    #undef TWO_PI
    #undef round(x)

#elif defined(ARDUINO)
    #define __ARDUINO__  // exclude standard c++ h files
    #define __STREAMS__  // output to Ardiono Streams
    #define __RAW_ARRAYS__      // raw files as compilable c arrays
    #define __MIDI__            // support Midi 
    #define __NO_EXCEPTIONS__   // comment out exceptions
    #define __NO_FSTREAM__      // No Messanger, MidiFileIn and Skini and FileRead
    #define __NO_RT__           // no STK RT functions

    #include <ArduinoSTL.h>

    #undef PI
    #undef TWO_PI

#endif

// rtos must be loaded on top of other classes - so we add it here
// rtos support is experimental and most things might not work. I recommend
// that you do not use the netowrk functionality provided by the STK framework
// but that you use the Standard Arduino functionality instead. 
#ifdef __RTOS__  
//    #include "FreeRTOS.h"
    #include "freertos/FreeRTOS.h"
#endif
