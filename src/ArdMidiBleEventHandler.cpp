#include "ArdMidiBleEventHandler.h"
#ifdef ARDMIDIBLEEVENTHANDLER_H

namespace stk {

ArdMidiBleEventHandler::ArdMidiBleEventHandler(Voicer *p_voicer, uint8_t *p_channel)
: ArdMidiEventHandler(p_voicer, p_channel) {
};

ArdMidiBleEventHandler::~ArdMidiBleEventHandler(){
}

/**
 * @brief Callback function to support a read request.
 * @param [in] pCharacteristic The characteristic that is the source of the event.
 */
void ArdMidiBleEventHandler::onRead(BLECharacteristic* pCharacteristic) {
	//ESP_LOGD(APP, "onRead");
} // onRead


/**
 * @brief Callback function to support a write request.
 * @param [in] pCharacteristic The characteristic that is the source of the event.
 */
void ArdMidiBleEventHandler::onWrite(BLECharacteristic* pCharacteristic) {
  const char* str = pCharacteristic->getValue().c_str();
  int len = strlen(str);
  parse((uint8_t*)str, len);
} 

} // namespace

#endif