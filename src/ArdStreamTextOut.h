#include "Stk.h"
#ifdef __STREAMS__

#ifndef ARDSERIALBINARYOUT_H
#define ARDSERIALBINARYOUT_H

#include "WvOut.h"
#include "ArdStreamOut.h"
#include "arduino.h"

namespace stk {
/***************************************************/
/*! \class stk::ArdStreamOut
    \brief Output int16 data Arduino Stream

    We are wrting the data as text

    by Phil Schatzmann
*/
/***************************************************/
class ArdStreamTextOut: public ArdStreamOut {
    public:
        ArdStreamTextOut(Print &stream, unsigned channels=1);
        
    protected:
        virtual void write(int16_t value) override;

};

}; //stk
#endif
#endif
