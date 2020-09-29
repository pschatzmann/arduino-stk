#include "Stk.h"
#ifdef __STREAMS__

#ifndef ARDSTREAMOUTCOMMON_H
#define ARDSTREAMOUTCOMMON_H

#include "ArdConfig.h"
#include "WvOut.h"
#include "ArdCommonOut.h"
#include "arduino.h"

namespace stk {
/***************************************************/
/*! \class ArdStreamOutCommon
    \brief Abstract class for Output to Arduiono HardwareArdStream

    We are wrting the data to the indicated Ardino HardwareArdStream device.

    by Phil Schatzmann

*/
/***************************************************/
class ArdStreamOutCommon: public ArdCommonOut {
    public:
        ArdStreamOutCommon(Stream &stream, unsigned samples=512, unsigned channels=1);
   
    protected:
        Stream* pStream;

};

}; //stk
#endif
#endif
