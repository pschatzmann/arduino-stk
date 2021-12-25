#include "Stk.h"
#ifdef __STREAMS__

#ifndef STK_ArdStream_HEX_OUT_H
#define STK_ArdStream_HEX_OUT_H

#include "ArdStreamOut.h"
#include "Print.h"

namespace stk {
/***************************************************/
/*! \class stk::ArdStreamHexOut
    \brief Output int32 data to Arduiono HardwareArdStream

    We are wrting the data to the indicated Ardino HardwareArdStream device.
    We convert the output to hex text 

    by Phil Schatzmann

*/
/***************************************************/
class ArdStreamHexOut: public ArdStreamOut {
    public:
        ArdStreamHexOut(Print &stream,unsigned channels=1);
    protected:
        virtual void write(int16_t value) override;
    
};

}; //stk
#endif
#endif
