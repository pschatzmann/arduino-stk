/**
 *  Write the data to the indicated ArdStream interface
*/

#include "ArdStreamOut.h"
#ifdef ARDSTREAMOUTCOMMON_H

namespace stk {

ArdStreamOut::ArdStreamOut(Print &stream,  unsigned channels)
: ArdCommonOut(channels) {
    this->pStream = &stream;
    this->byteCount = sizeof(int16_t)*nChannels;
}

ArdStreamOut::ArdStreamOut(Print *stream,  unsigned channels)
: ArdCommonOut(channels) {
    this->pStream = stream;
    this->byteCount = sizeof(int16_t)*nChannels;
}

// convert sample to int16
void ArdStreamOut::write(StkFloat value) {
    write(static_cast<int16_t>(value*32766));
}

// output to stream in request number of channels
void ArdStreamOut::write(int16_t value) {
    int16_t buffer[nChannels];
    for (int j=0;j<nChannels;j++){
        buffer[j]=value;
    }
    this->pStream->write((const char *)buffer, byteCount);
};


}; //stk

#endif

