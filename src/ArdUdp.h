#include "ArdConfig.h"
#ifdef ESP32

#ifndef ARDUPD_H
#define ARDUPD_H

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>

namespace stk {

/***************************************************/
/*! \class stk::ArdUdp
    \brief Simple UDP wrapper class which sends all 
    packages to the same destination.

    This is useful when the API asks for a stream
    as parameter to output data.

    by Phil Schatzmann

*/
/***************************************************/

class ArdUdp : public WiFiUDP { // EthernetUDP {
    public:
        ArdUdp(char* targetUdpAddressStr,int targetPort);
        ArdUdp(IPAddress targetUdpAddress,int targetPort);
        size_t write(const uint8_t * buffer,size_t size );
        bool isValidHost();

    protected:
        IPAddress targetUdpAddress;
        int targetPort;
        bool isValidHostFlag;
};

}

#endif
#endif