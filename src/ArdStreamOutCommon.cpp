/**
 *  Write the data to the indicated ArdStream interface
*/

#include "ArdStreamOutCommon.h"
#ifdef ARDSTREAMOUTCOMMON_H

namespace stk {

ArdStreamOutCommon::ArdStreamOutCommon(Stream &stream, unsigned samples, unsigned channels)
: ArdCommonOut(samples, channels) {
    this->pStream = &stream;
}

}; //stk

#endif

