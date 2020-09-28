/**
    ArdConfig.h
 
    Custom definition to manage the functionality for Microcontrollers 

    by Phil Schatzmann
 */

#if defined(ESP32)    
    #define __VFS__   // raw files as compilable c arrays
    #define __RTOS__ // supports Free RTOS
    #define __LWIP__  // use lwip socket implementation
    #define __MIDI__  // support Midi 
    #define __BLE_MIDI__ // support BLE 
    #define TASK_STACK_SIZE configMINIMAL_STACK_SIZE
    #define __STREAMS__  // output to Ardiono Streams

    #undef PI
    #undef TWO_PI

#elif defined(ESP8266)
    #define __VFS__   // raw files as compilable c arrays
    #define __STREAMS__
    #define __NO_EXCEPTIONS__
    #define __NO_RT__
    // the call of stat is giving link errors, we deactivate the stat 
    // function here with the effect that file based operations will not
    // work
    //#include "Arduino.h"
    #include "FS.h"
    #undef PI
    #undef TWO_PI
    #undef round(x)
    
#endif

// rtos must be loaded on top of other classes - so we add it here
#ifdef __RTOS__  
    #include "FreeRTOS.h"
#endif


// #ifdef PI 
//   #undef PI
// #endif
// #ifdef TWO_PI
//   #undef TWO_PI
// #endif