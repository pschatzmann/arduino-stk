#include "Stk.h"
#ifdef __STREAMS__

#ifndef STK_ArdStream_HEX_OUT_H
#define STK_ArdStream_HEX_OUT_H

#include "ArdStreamOutCommon.h"
#include "arduino.h"

namespace stk {
/***************************************************/
/*! \class ArdStreamHexOut
    \brief Output int32 data to Arduiono HardwareArdStream

    We are wrting the data to the indicated Ardino HardwareArdStream device.
    We convert the output to hex text 

    by Phil Schatzmann

*/
/***************************************************/
class ArdStreamHexOut: public ArdStreamOutCommon {
    public:
        ArdStreamHexOut(Stream &stream,unsigned samples=6);
        
    protected:
        void writeBuffer(unsigned long len);

};

}; //stk
#endif
#endif
