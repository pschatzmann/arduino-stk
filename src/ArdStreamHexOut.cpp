/**
 *  Write the data to the indicated ArdStream interface
*/

#include "ArdConfig.h"
#ifdef __ESP__

#include "ArdStreamHexOut.h"

namespace stk {

ArdStreamHexOut::ArdStreamHexOut(Print &stream, unsigned channels)
: ArdStreamOutCommon(stream, channels) {
}

void ArdStreamOutCommon::write(int16_t value) {
    for (int j=0;j<nChannels;j++){
        pStream->print(buffer_[j] , HEX);
    }
}

}; //stk

#endif
