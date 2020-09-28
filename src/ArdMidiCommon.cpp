#include "ArdMidiCommon.h"
#ifdef ARDMIDICOMMON_H

#define MIDI_SERVICE_UUID        "03b80e5a-ede8-4b33-a751-6ce34ec4c700"
#define MIDI_CHARACTERISTIC_UUID "7772e5db-3868-4112-a1a9-f2669d106bf3"

namespace stk {

ArdMidiCommon::ArdMidiCommon(){
     this->connectionStatus = Unconnected;
 }

void ArdMidiCommon :: setVoicer(Voicer &voicer) {
    this->pVoicer = &voicer;
}

void ArdMidiCommon :: setDefaultSendingChannel(int8_t channel) {
    this->sendingChannel = channel;
}

void ArdMidiCommon :: setFilterReceivingChannel(int8_t channel){
    this->receivingChannel = channel;
}

void ArdMidiCommon ::  updateTimestamp(MidiMessage *pMsg) {
    timestampLow++;
    if (timestampLow > 0b01111111 ){
        timestampLow = 0;
        timestampHigh++;
        if (timestampHigh > 0b00111111){
            timestampHigh = 0;
        }
    }
    pMsg->timestampHigh = 0b10000000 | timestampHigh;
    pMsg->timestampHigh = 0b10000000 | timestampLow;
}


void ArdMidiCommon :: noteOn(uint8_t note, uint8_t velocity, int8_t channelPar) {
    uint8_t channel = channelPar != -1 ? channelPar : sendingChannel;
    this->outMessage.status = 0b1001 << 4 | channel;
    this->outMessage.arg1 = note;
    this->outMessage.arg2 = velocity;
    writeData(&outMessage, 2);
}

void ArdMidiCommon :: noteOff(uint8_t note, uint8_t velocity, int8_t channelPar) {
    uint8_t channel = channelPar != -1 ? channelPar : sendingChannel;
    this->outMessage.status = 0b1000 << 4 | channel;
    this->outMessage.arg1 = note;
    this->outMessage.arg2 = velocity;
    writeData(&outMessage, 2);
}

void ArdMidiCommon :: pitchBend(uint16_t value, int8_t channelPar) {
    uint8_t channel = channelPar != -1 ? channelPar : sendingChannel;
    this->outMessage.status = 0b1110 << 4 | channel;
    this->outMessage.arg1 = value >> 8;
    this->outMessage.arg2 = value | 0xFF;
    writeData(&outMessage, 2);
}

void ArdMidiCommon :: channelPressure(uint8_t valuePar, int8_t channel){
    uint8_t value = valuePar & 0b1111111;
    this->outMessage.status = 0b1101 << 4 | channel;
    this->outMessage.arg1 = value ;
    writeData(&outMessage, 1);
}

void ArdMidiCommon :: polyPressure(uint8_t valuePar, int8_t channel){
    uint8_t value = valuePar & 0b1111111;
    this->outMessage.status = 0b1010 << 4 | channel;
    this->outMessage.arg1 = value ;
    writeData(&outMessage, 1);
}

void ArdMidiCommon :: programChange(uint8_t program, int8_t channel){
    uint8_t value = program & 0b1111111;
    this->outMessage.status = 0b1100 << 4 | channel;
    this->outMessage.arg1 = value ;
    writeData(&outMessage, 1);
}

void ArdMidiCommon :: allNotesOff( int8_t channel){
    controlChange(120,0, channel);
}

void ArdMidiCommon :: resetAllControllers( int8_t channel){
    controlChange(121,0, channel);
}

void ArdMidiCommon :: localControl( bool active, int8_t channel){
    controlChange(122,active ? 127 : 0, channel);
}

void ArdMidiCommon :: controlChange(uint8_t msg, uint8_t value, int8_t channel){
    this->outMessage.status = 0b1011 << 4 | channel;
    this->outMessage.arg1 = msg;
    this->outMessage.arg2 = value;    
    writeData(&outMessage, 2);
}

float ArdMidiCommon :: noteToFrequency(uint8_t x) {
    int a = 440; //frequency of A (coomon value is 440Hz)
    return (a / 32) * pow(2, ((x - 9) / 12));
}

uint8_t ArdMidiCommon :: frequencyToNote(float freq) {
    return log(freq/440.0)/log(2) * 12 + 69;
}


void ArdMidiCommon :: writeData(MidiMessage *msg, int len){
}



} // namespace

#endif
