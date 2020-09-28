/**
 *  Write the data to the indicated ArdStream interface
*/

#include "ArdStreamBinaryOut.h"
#ifdef ARDSERIALBINARYOUT_H


namespace stk {

ArdStreamBinaryOut::ArdStreamBinaryOut(Stream &stream, unsigned samples, unsigned channels)
: ArdStreamOutCommon(stream, samples, channels) {
}

void ArdStreamBinaryOut::writeBuffer(unsigned long len){
    pStream->write((const uint8_t*) buffer_ , len);
}

}; //stk

#endif
