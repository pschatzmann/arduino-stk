
#include "Stk.h"
#ifdef __BT_A2DP__

#ifndef ARDBTSOURCE_H
#define ARDBTSOURCE_H

#include "WvOut.h"
#include "ArdCommonOut.h"
#include "BluetoothA2DPSource.h"
#include "Voicer.h"
#include "esp_log.h"

namespace stk {

/***************************************************/
/*! \class stk::ArdBtSource
    \brief ESP32 Output to Bluetooth

    Implementation of a A2DP Bluetooth Source for the ESP32
    which uses a Voicer as data source

    by Phil Schatzmann
*/
/***************************************************/

class ArdBtSource : public Stk
{
  public:
    //! Default constructor ... 
    ArdBtSource();

    //! Destructor
    ~ArdBtSource();

    //! Starts the Bluetooth device
    void start(char* name, Voicer &voicer);

    //! Checks if this device is still connnected
    bool isConnected();

    //! Defines if the Flash NVS should be reset on start
    void setNVSInit(bool doInit);

    //! Defines if the BLE should be reset on start
    void setResetBLE(bool doInit);


  protected:
    Voicer *pVoicer;
    BluetoothA2DPSource *a2dp_source;

};

} // stk namespace

#endif
#endif
