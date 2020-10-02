#include "ArdConfig.h"
#ifdef __MIDI_BLE__
#ifndef ARDMIDIBLEEVENTHANDLER_H
#define ARDMIDIBLEEVENTHANDLER_H

#include "Arduino.h"
#include "Voicer.h"
#include "ArdMidiEventHandler.h"
#include "BLECharacteristic.h"
#include "esp_log.h"

namespace stk {

/***************************************************/
/*! \class ArdMidiBleEventHandler
    \brief  A simple Midi Parser for BLE Midi messages
    that calls the corresponding events. 
  
    In this implementation the handler just passes the noteOn 
    and noteOff to the Voicer.
  
    http://www.hangar42.nl/wp-content/uploads/2017/10/BLE-MIDI-spec.pdf

    by Phil Schatzmann
*/
/***************************************************/
class ArdMidiBleEventHandler 
: public BLECharacteristicCallbacks , public  ArdMidiEventHandler {
    public:
        ArdMidiBleEventHandler(Voicer *voicer, int *p_channel = NULL );
         ~ArdMidiBleEventHandler();
        void onRead(BLECharacteristic* pCharacteristic);
	    void onWrite(BLECharacteristic* pCharacteristic);

};


} // namespace

#endif
#endif
