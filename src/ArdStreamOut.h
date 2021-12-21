#include "Stk.h"
#ifdef __STREAMS__

#ifndef ARDSTREAMOUTCOMMON_H
#define ARDSTREAMOUTCOMMON_H

#include "ArdConfig.h"
#include "WvOut.h"
#include "ArdCommonOut.h"
#include "Print.h"

namespace stk {
/***************************************************/
/*! \class stk::ArdStreamOut
    \brief Abstract class for Output to Arduiono HardwareArdStream

    We are wrting the data to the indicated Ardino HardwareArdStream device. 
    If the channel is >1 we repeat the output  multiple times until  all
    channels have been filled (with the same output value)

    by Phil Schatzmann

*/
/***************************************************/
class ArdStreamOut: public ArdCommonOut {
    public:
        ArdStreamOut(Print &stream, unsigned channels=1);
        ArdStreamOut(Print *stream, unsigned channels=1);
   
    protected:
        Print* pStream;
        int byteCount;

        virtual void write(StkFloat value) override;
        virtual void write(int16_t value) override;

};

}; //stk
#endif
#endif
