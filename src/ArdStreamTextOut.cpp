/**
 *  Write the data to the indicated ArdStream interface
*/

#include "ArdStreamTextOut.h"
#ifdef ARDSERIALBINARYOUT_H


namespace stk {

ArdStreamTextOut::ArdStreamTextOut(Print &stream,  unsigned channels)
: ArdStreamOut(stream, channels) {
}

void ArdStreamTextOut::write(int16_t value) {
    for (int j=0;j<nChannels;j++){
        pStream->print(value);
        pStream->print(" ");
    }
    pStream->println();
}


}; //stk

#endif
