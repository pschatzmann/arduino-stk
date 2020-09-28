
#include <ArdUdp.h>
#ifdef ARDUPD_H

namespace stk {

const char* APP_ArdUdp = "ArdUdp";

ArdUdp :: ArdUdp(char* addessStr,int targetPort) {
    this->isValidHostFlag = WiFi.hostByName(addessStr, targetUdpAddress);
    if (!this->isValidHostFlag){
        ESP_LOGE(APP_ArdUdp, "x%x, Could not resolve host %s ", __func__, addessStr);
    }
    this->targetPort = targetPort;    
}

ArdUdp :: ArdUdp(IPAddress address,int targetPort) {
    this->targetUdpAddress = address;
    this->targetPort = targetPort;    
}

bool ArdUdp :: isValidHost() {
    return this->isValidHostFlag;
}

size_t ArdUdp :: write(const uint8_t * buffer, size_t size ) {
    size_t result = 0;
    if (this->beginPacket(targetUdpAddress, targetPort) == 1){
        result = WiFiUDP::write(buffer, size);
        if (result>0){
            bool packetOk = this->endPacket();
            ESP_LOGD(APP_ArdUdp, "x%x, Number of bytes have %s been sent out: %d ", __func__, packetOk?"":"not", result);
        }
        //this->flush();
    } else {
        ESP_LOGD(APP_ArdUdp, "x%x, beginPacked has failed ", __func__);
        this->isValidHostFlag = false;
    }
    return result;
}

} // stk

#endif