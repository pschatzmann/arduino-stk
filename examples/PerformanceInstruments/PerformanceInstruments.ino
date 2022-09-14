/**
 * @file PerformanceMemory.ino
 * @author Phil Schatzmann
 * @brief We mesure the performance of different instruments
 * @version 0.1
 * @date 2022-09-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "StkAll.h"
#include <stdio.h>
#include <functional>

float note = 220;
float amplitude = 0.8;
float sampleRate = 44100;
int sec = 10; 

struct InstrumentInfo {
    const char* name;
    std::function<Instrmnt * ()> instrument;
};

InstrumentInfo instrumentArray[] = {
    {"BeeThree", []() { return new BeeThree(); }},
    {"BlowHole", []() { return new BlowHole(200); }},
    {"Bowed", []() { return new Bowed(); }},
    {"Clarinet", []() { return new Clarinet(); }},
    {"Drummer", []() { return new Drummer(); }},  // comment out for STM32 or ESP8266
    {"Flute", []() { return new Flute(200); }},
    {"Rhodey", []() { return new Rhodey(); }},
    {"TubeBell", []() { return new TubeBell(); }},
    {"Mandolin", []() { return new Mandolin(200); }},
    {"ModalBar", []() { return new ModalBar(); }},
    {"Moog", []() { return new Moog(); }},
    {"Plucked", []() { return new Plucked(); }},
    {"Saxofony", []() { return new Saxofony(200); }},
    {"Shakers", []() { return new Shakers(); }},
    {"Sitar", []() { return new Sitar(); }},
    {"StifKarp", []() { return new StifKarp(); }},
    {"TubeBell", []() { return new TubeBell(); }},
    {"Wurley", []() { return new Wurley(); }},
    {"BlowBotl", []() { return new BlowBotl(); }},
    {"Brass", []() { return new Brass(); }},
    {"FMVoices", []() { return new FMVoices(); }},
    {"PercFlut", []() { return new PercFlut(); }},
    {"HevyMetl", []() { return new HevyMetl(); }},
    {"Recorder", []() { return new Recorder(); }},
    {"Resonate", []() { return new Resonate(); }},
    {"Simple", []() { return new Simple(); }},
    {"Whistle", []() { return new Whistle(); }},
};

char msg[80];

void measure(const char*title, Instrmnt *src) {
    uint64_t start = millis();
    // generate sound
    src->noteOn(note, amplitude);
    // pull samples for 10 seconds at 44100 
    for (int j=0;j<sampleRate*sec;j++){
        float f = src->tick();
        // for ESP8266 to feed the watchdog
        if (j%5000==0) yield(); 
    }
    snprintf(msg, 80, "%s: %f sec",title, static_cast<float>(millis()-start)/1000.0);
    Serial.println(msg);

    // release instrument
    delete src;
}


void setup() {    
    Serial.begin(115200);
    // wait for serial to be available
    while(!Serial); 

    StkLogLevel = StkWarning;
    Stk::setSampleRate(sampleRate);


    snprintf(msg, 80, "Time for %d sec music at %f samples per second", sec, sampleRate);
    Serial.println(msg);
    for (auto i : instrumentArray){
        measure(i.name, i.instrument());
    }
}

void loop() {

}