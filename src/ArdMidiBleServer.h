#include "ArdConfig.h"
#ifdef __MIDI_BLE__
#ifndef ARDMIDIBLESERVER_H
#define ARDMIDIBLESERVER_H

#include "ArdMidiCommon.h"
#include "ArdMidiBleEventHandler.h"

namespace stk {

/***************************************************/
/*! \class ArdMidiBleServer
    \brief A Bluetooth Low Energy BLE Server which
    can send or receive Bluetooth messages.
    
    by Phil Schatzmann
*/
/***************************************************/

class ArdMidiBleServer : public ArdMidiCommon {
    public:
        //! Default constructor
        ArdMidiBleServer(char* name, ArdMidiBleEventHandler* pEventHandler=NULL);
        
        //! Starts the BLE server
        void start(Voicer &voicer);
        void start();
        void  writeData(MidiMessage *pMsg, int len);


    protected:
        ArdMidiBleEventHandler *pEventHandler;
        BLEServer *pServer;
        BLECharacteristic* pCharacteristic;
        char *name;
};

class ArdMidiBleServerCallback: public BLEServerCallbacks {
    public:
        ArdMidiBleServerCallback(ConnectionStatus *pStatus);
        void start(Voicer &voicer);
        void onConnect(BLEServer* pServer);
        void onDisconnect(BLEServer* pServer);

    protected:
        ConnectionStatus *pConnectionStatus;
};

} // namespace

#endif
#endif
