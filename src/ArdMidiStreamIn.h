#ifndef ARDMIDISTREAMIN_H
#define ARDMIDISTREAMIN_H

#include "ArdConfig.h"
#ifdef __MIDI__

#include "ArdMidiCommon.h"
#include "ArdMidiEventHandler.h"

namespace stk {

#define BUFFER_LEN 30

/***************************************************/
/*! \class ArdMidiSerialIn
    \brief Input of Midi Messages from the Aruduino 
    HardwareSerial port. You need to give the default
    ArdMidiEventHandler or your own subclassed
    implementation in the constructor to process
     the midi records.

    You also need to call the loop method in the loop
    of your Arduino sketch.

    by Phil Schatzmann
*/
/***************************************************/
class ArdMidiStreamIn : public ArdMidiCommon {
    public:
        ArdMidiStreamIn(Stream &stream, ArdMidiEventHandler &handler);
        void loop();
    protected:
       int getLastStatusPos(uint8_t *buffer, int endPos);
        Stream *pStream;
        ArdMidiEventHandler *pHandler;
        uint8_t buffer[BUFFER_LEN];
        int startPos;
};

} // namespace

#endif
#endif
