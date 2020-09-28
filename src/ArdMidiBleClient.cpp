#include "ArdMidiBleClient.h"
#ifdef ARD_MIDI_CLIENT_H

namespace stk {

const char* APP_CLIENT = "ArdMidiBleClient";
ArdMidiBleEventHandler *pEventHandler;


ArdMidiBleClient :: ArdMidiBleClient(char* name, ArdMidiBleEventHandler* pEventHandler) {
    this->name = name;
    this->connectionStatus = Unconnected;
}

void ArdMidiBleClient :: start(Voicer &voicer) {
    this->pVoicer = &voicer;
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new ArdMidiBleClientAdvertisedDeviceCallbacks(this));
    pBLEScan->setInterval(1349);
    pBLEScan->setWindow(449);
    pBLEScan->setActiveScan(true);
    pBLEScan->start(5, false);
}


void characteristic_notify_callback(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify){
    if (pEventHandler)
        pEventHandler->parse(pData,length);
}


void ArdMidiBleClient :: start(BLEAdvertisedDevice *pDevice) {
    this->pDevice = pDevice;
    BLEClient*  pClient  = BLEDevice::createClient();
    pClient->setClientCallbacks(new ArdMidiBleClientCallback(&connectionStatus));
    //pClient->connect(name);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    pClient->connect(pDevice);  

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(MIDI_SERVICE_UUID);
    if (pRemoteService == nullptr) {
        ESP_LOGE(APP_CLIENT, "Failed to find our service UUID: %s ", MIDI_SERVICE_UUID);
        pClient->disconnect();
        return;
    }

    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic = pRemoteService->getCharacteristic(MIDI_CHARACTERISTIC_UUID);
    if (pRemoteCharacteristic == nullptr) {
        ESP_LOGE(APP_CLIENT, "Failed to find our characteristic UUID: %s", MIDI_CHARACTERISTIC_UUID);
        pClient->disconnect();
        return;
    }
    ESP_LOGD(APP_CLIENT, " - Found our characteristic");

    // Read the value of the characteristic.
    if(pRemoteCharacteristic->canRead()) {
        std::string value = pRemoteCharacteristic->readValue();
        ESP_LOGE(APP_CLIENT, "The characteristic value was: %s",value.c_str());
    }

    pRemoteCharacteristic->registerForNotify(characteristic_notify_callback, true);

}

void ArdMidiBleClient :: writeData(MidiMessage *pMsg, int len) {
    if (pRemoteCharacteristic!=NULL){
        updateTimestamp(&outMessage);
        uint8_t* cp = (uint8_t*)&outMessage;
        int messageLen = sizeof(MidiMessage);
        switch (len) {
            case 1: {
                pRemoteCharacteristic->writeValue(cp,len-1, false);
                break;
            }
            case 2: {
                pRemoteCharacteristic->writeValue(cp,len, false);
                break;
            }
        }
    }
}


BLEAdvertisedDevice * ArdMidiBleClient :: getBLEAdvertisedDevice() {
    return this->pDevice;
}


// => ArdMidiBleClientCallback

ArdMidiBleClientCallback :: ArdMidiBleClientCallback(ConnectionStatus *pStatus){
    this->pConnectionStatus = pStatus;
}
void ArdMidiBleClientCallback :: onConnect(BLEClient* client) {
    *pConnectionStatus = Connected;
};

void ArdMidiBleClientCallback :: onDisconnect(BLEClient* client) {
    *pConnectionStatus = Disconnected;
}

// => ArdMidiBleClientAdvertisedDeviceCallbacks

ArdMidiBleClientAdvertisedDeviceCallbacks :: ArdMidiBleClientAdvertisedDeviceCallbacks(ArdMidiBleClient *pClient) {
    this->pClient = pClient;
}

void ArdMidiBleClientAdvertisedDeviceCallbacks :: onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(BLEUUID(MIDI_SERVICE_UUID))) {
      BLEDevice::getScan()->stop();
      BLEAdvertisedDevice* pDevice = new BLEAdvertisedDevice(advertisedDevice);
      pClient->start(pDevice);
    }
}


} // namespace

#endif
