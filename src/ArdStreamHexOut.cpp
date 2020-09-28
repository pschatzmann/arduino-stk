/**
 *  Write the data to the indicated ArdStream interface
*/

#include "ArdConfig.h"
#ifdef __ESP__

#include "ArdStreamHexOut.h"

namespace stk {

ArdStreamHexOut::ArdStreamHexOut(Stream &stream, unsigned samples)
: ArdStreamOutCommon(stream, samples, 1) {
}

void ArdStreamHexOut::writeBuffer(unsigned long len){
    for (int j=0;j<len;j++){
        pStream->print(buffer_[j] , HEX);
        pStream->print(" ");
    }
    pStream->println();
}

}; //stk

#endif
