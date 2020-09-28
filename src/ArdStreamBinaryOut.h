#include "Stk.h"
#ifdef __STREAMS__

#ifndef ARDSERIALBINARYOUT_H
#define ARDSERIALBINARYOUT_H

#include "WvOut.h"
#include "ArdStreamOutCommon.h"
#include "arduino.h"

namespace stk {
/***************************************************/
/*! \class ArdStreamOut
    \brief Output int32 data to Arduiono HardwareArdStream

    We are wrting the data to the indicated Ardino HardwareArdStream device.
    SoftArdStream is not supported because it is too slow. 

    by Phil Schatzmann

*/
/***************************************************/
class ArdStreamBinaryOut: public ArdStreamOutCommon {
    public:
        ArdStreamBinaryOut(Stream &stream, unsigned samples=512, unsigned channels=1);
        
    protected:
        void writeBuffer(unsigned long len);

};

}; //stk
#endif
#endif
