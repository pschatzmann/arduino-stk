#include "Stk.h"
#ifdef __STREAMS__

#ifndef ARDSTREAMOUT_H
#define ARDSTREAMOUT_H

#include "WvOut.h"
#include "ArdStreamOutCommon.h"

namespace stk {
/***************************************************/
/*! \class ArdStreamOut
    \brief Output (of as is data) to Arduiono HardwareArdStream

    We are wrting the data to the indicated Ardino HardwareArdStream device.
    SoftArdStream is not supported because it is too slow. 

    by Phil Schatzmann

*/
/***************************************************/
class ArdStreamOut: public ArdStreamOutCommon {
    public:
        ArdStreamOut(Stream &stream, unsigned samples=512, unsigned channels=1);
       
    protected:
        void writeData( unsigned long frames );
        void writeBuffer(unsigned long len);

};

}; //stk
#endif
#endif
