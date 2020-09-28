#include "ArdConfig.h"

#ifdef __MIDI__
#ifndef ARDMIDISTREAMOUT_H
#define ARDMIDISTREAMOUT_H

#include "ArdMidiCommon.h"

namespace stk {
/***************************************************/
/*! \class ArdMidiSerialOut
    \brief Output of Midi Messages to an Arduino 
    Stream (eg output to Serial, UDP or IP).

    by Phil Schatzmann
*/
/***************************************************/
class ArdMidiStreamOut : public ArdMidiCommon {
    public:
        ArdMidiStreamOut(Stream &stream);

    protected:
        virtual void writeData(MidiMessage *pMsg, int len);

        Stream *pStream;
        uint8_t buffer[4];
};

} // namespace

#endif
#endif
