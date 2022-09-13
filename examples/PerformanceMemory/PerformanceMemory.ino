/**
 * @file PerformanceMemory.ino
 * @author Phil Schatzmann
 * @brief We mesure the MemoryLoop performance with different memory types
 * @version 0.1
 * @date 2022-09-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "StkAll.h"
#include <stdio.h>
#ifdef ESP32
#include "esp_heap_caps.h"
#endif

void measure(const char*title, MemoryLoop *src) {
    char msg[80];
    uint64_t start = millis();
    // generate samples for 10 seconds at 44100 
    int sec = 10;
    for (int j=0;j<44100*sec;j++){
        float f = src->tick();
    }
    snprintf(msg, 80, "%s - Time for %d sec music: %f sec",title, sec, static_cast<float>(millis()-start)/1000.0);
    Serial.println(msg);
}


void setup() {    
    Serial.begin(115200);
    // wait for serial to be available
    while(!Serial); 

    StkLogLevel = StkWarning;
    Stk::setSampleRate(44100);

    MemoryFS::reset();
    MemoryFS::useRAM(false); // default value
    MemoryLoop *src = new MemoryLoop( Stk::rawwavePath() + "sinewave.raw", sinewave_raw,sinewave_raw_len );
    measure("progmem", src);

    delete src;
    MemoryFS::reset();
    MemoryFS::useRAM(true);
    src = new MemoryLoop( Stk::rawwavePath() + "sinewave.raw", sinewave_raw,sinewave_raw_len );
    measure("ram", src);

#ifdef ESP32
    delete src;
    MemoryFS::reset();
    heap_caps_malloc_extmem_enable(sinewave_raw_len-10); // allocate in psram
    MemoryFS::useRAM(true);
    src = new MemoryLoop( Stk::rawwavePath() + "sinewave.raw", sinewave_raw,sinewave_raw_len );
    measure("psram", src);
#endif

}

void loop() {

}