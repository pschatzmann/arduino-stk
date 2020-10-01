#include "ArdMidiBleServer.h"
#ifdef ARDMIDIBLESERVER_H

namespace stk {

const char* APP_SERVER = "ArdMidiBleServer";

ArdMidiBleServer::ArdMidiBleServer(char* name, ArdMidiBleEventHandler* pEventHandler){
     this->name = name;
     this->pEventHandler = pEventHandler;
     this->connectionStatus = Unconnected;
 }

void ArdMidiBleServer :: start() {
    // Create the BLE Device
    BLEDevice::init(this->name);

    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new ArdMidiBleServerCallback(&connectionStatus));

    //BLEDevice::setEncryptionLevel((esp_ble_sec_act_t)ESP_LE_AUTH_REQ_SC_BOND);

    // Create the BLE Service
    BLEService *pService = pServer->createService(BLEUUID(MIDI_SERVICE_UUID));

    // Create a BLE Characteristic
    pCharacteristic = pService->createCharacteristic(
                        BLEUUID(MIDI_CHARACTERISTIC_UUID),
                        BLECharacteristic::PROPERTY_READ   |
                        BLECharacteristic::PROPERTY_WRITE_NR  |
                        BLECharacteristic::PROPERTY_NOTIFY |
                        BLECharacteristic::PROPERTY_INDICATE
                    );

                    
    if (this->pVoicer != nullptr) {                
        if (this->pEventHandler == nullptr){
            this->pEventHandler =  new ArdMidiBleEventHandler(pVoicer, &(this->receivingChannel));
        }
        pCharacteristic->setCallbacks(this->pEventHandler);
    }

    //pCharacteristic->setAccessPermissions(ESP_GATT_PERM_READ_ENCRYPTED | ESP_GATT_PERM_WRITE_ENCRYPTED);
    pCharacteristic->addDescriptor(new BLE2902());
    pCharacteristic->setBroadcastProperty(true);

    // Start the service
    pService->start();

    // Start advertising
    // BLESecurity *pSecurity = new BLESecurity();
    // pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_BOND);
    // pSecurity->setCapability(ESP_IO_CAP_NONE);
    // pSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);

    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(MIDI_SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);
    pAdvertising->start();

    ESP_LOGD(APP_SERVER, "started");

}

void ArdMidiBleServer :: start(Voicer &voicer) {
	ESP_LOGD(APP_SERVER, "start");
    setVoicer(voicer);
    start();
}

void ArdMidiBleServer :: writeData(MidiMessage *pMsg, int len) {
    updateTimestamp(&outMessage);

    switch (len) {
        case 1: 
            pCharacteristic->setValue((uint8_t*)&outMessage, sizeof(MidiMessage)-1); 
            pCharacteristic->notify();  
            break;
        case 2: 
            pCharacteristic->setValue((uint8_t*)&outMessage, sizeof(MidiMessage)); 
            pCharacteristic->notify();  
            break;
    }
}


// => ArdMidiBleServerCallback

ArdMidiBleServerCallback :: ArdMidiBleServerCallback(ConnectionStatus *pStatus){
    this->pConnectionStatus = pStatus;
}
void ArdMidiBleServerCallback :: onConnect(BLEServer* pServer) {
	ESP_LOGD(APP_SERVER, "onConnect");
    *pConnectionStatus = Connected;
};

void ArdMidiBleServerCallback :: onDisconnect(BLEServer* pServer) {
	ESP_LOGD(APP_SERVER, "onDisconnect");
    *pConnectionStatus = Disconnected;
}



} // namespace

#endif
