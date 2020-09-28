
#include "ArdMidiStreamIn.h"
#ifdef ARDMIDISTREAMIN_H

namespace stk {

ArdMidiStreamIn :: ArdMidiStreamIn(Stream &stream, ArdMidiEventHandler &handler) {
    pStream = &stream;
    pHandler = &handler;
    startPos = 0;
}

void ArdMidiStreamIn :: loop() {
    int lenRead = pStream->readBytes(buffer+startPos, BUFFER_LEN-startPos);
    if (lenRead>0){
        int endPos = startPos+lenRead;
        int lastStatusPos = getLastStatusPos(buffer, endPos);
        if (lastStatusPos>=0) {
            pHandler->parse(buffer, lastStatusPos);
            int lenUnprocessed = endPos-lastStatusPos;
            // move unprocessed bytes to head
            memmove(buffer, buffer+lastStatusPos, lenUnprocessed);
            startPos = lenUnprocessed;
        }
    }
}

int ArdMidiStreamIn :: getLastStatusPos(uint8_t* buffer, int pos){
    while (pos>=0){
        if (buffer[pos]>>7 == 1){
            return pos; 
        }
        pos--;
    }
    return -1;
} 


} // namespace

#endif
