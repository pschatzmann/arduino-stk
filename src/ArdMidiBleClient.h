#include "ArdConfig.h"
#ifdef __MIDI_BLE__
#ifndef ARD_MIDI_CLIENT_H
#define ARD_MIDI_CLIENT_H

#include "ArdMidiCommon.h"
#include "ArdMidiBleEventHandler.h"


namespace stk {

/***************************************************/
/*! \class ArdMidiBleClient
    \brief A Bluetooth Low Energy BLE Client which
    can send or receive Bluetooth messages. It
    needs to connect to a running BLE Server.
    
    by Phil Schatzmann
*/
/***************************************************/

class ArdMidiBleClient : public ArdMidiCommon {
    public:
        //! Default constructor
        ArdMidiBleClient(char* serverName, ArdMidiBleEventHandler* pEventHandler = NULL);

        //! starts the discover and connects if the serverName was found
        void start(Voicer &voicer);

        //! connects to the indicated device
        void start(BLEAdvertisedDevice *pDevice);
        
        void  writeData(MidiMessage *pMsg, int len);

        //! determe in the BLEAdvertisedDevice 
        BLEAdvertisedDevice *getBLEAdvertisedDevice();
        
    protected:
        char *name;
        BLERemoteCharacteristic* pRemoteCharacteristic;
        BLEAdvertisedDevice* pDevice;

};

class ArdMidiBleClientCallback: public BLEClientCallbacks {
    public:
        ArdMidiBleClientCallback(ConnectionStatus *pStatus);
        void onConnect(BLEClient* pServer);
        void onDisconnect(BLEClient* pServer);

    protected:
        ConnectionStatus *pConnectionStatus;

};

class ArdMidiBleClientAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    public:
        ArdMidiBleClientAdvertisedDeviceCallbacks(ArdMidiBleClient *pClient);
        void onResult(BLEAdvertisedDevice advertisedDevice);
    private:
        ArdMidiBleClient *pClient;
}; 


} // namespace

#endif
#endif
