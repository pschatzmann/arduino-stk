#include "ArdConfig.h"
#ifdef __MIDI__
#ifndef ARDMIDICOMMON_H
#define ARDMIDICOMMON_H

#ifdef __MIDI_BLE__
#include "sdkconfig.h"
#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <freertos/task.h>
#include <esp_err.h>
#include <nvs_flash.h>
#include <esp_bt.h>            // Ard BLE
#include <esp_bt_device.h>     // Ard BLE
#include <esp_bt_main.h>       // Ard BLE
#include <esp_gap_ble_api.h>   // Ard BLE
#include <esp_gatts_api.h>     // Ard BLE
#include <esp_gattc_api.h>     // Ard BLE
#include <esp_gatt_common_api.h>// Ard BLE
#include <esp_err.h>           // Ard ESP-IDF
#include <map>                 // Part of C++ Standard library
#include <sstream>             // Part of C++ Standard library
#include <iomanip>             // Part of C++ Standard library
#include <math.h>
#include "BLEDevice.h"
#include "BLEClient.h"
#include "BLEUtils.h"
#include "GeneralUtils.h"
#include "esp_log.h"
#include "BLE2902.h"

#define MIDI_SERVICE_UUID        "03B80E5A-EDE8-4B33-A751-6CE34EC4C700"
#define MIDI_CHARACTERISTIC_UUID "7772E5DB-3868-4112-A1A9-F2669D106BF3"

#endif

#include "Arduino.h"
#include "Voicer.h"
#include "ArdMidiEventHandler.h"



namespace stk {

struct MidiMessage {
    uint8_t timestampHigh;
    uint8_t timestampLow;
    uint8_t status;
    uint8_t arg1;
    uint8_t arg2;
};

enum ConnectionStatus {
    Unconnected,
    Connected,
    Disconnected
};

/***************************************************/
/*! \class ArdMidiCommon
    \brief  The common methods provided by all Arduino Midi
    subclasses which can be used to generate Midi messages:

    - ArdMidiBleServer
    - ArdMidiBleClient
    - ArdMidiSerialOut

    by Phil Schatzmann
*/
/***************************************************/

class ArdMidiCommon {
    public:
        //! Default Constructor
        ArdMidiCommon();

        //! Activates a filter on receiving messages to the indicated channel
        void setFilterReceivingChannel(int channel);

        //! Sets the default channel for the sending commands. 
        void setDefaultSendingChannel(int8_t channel);

        //! Sends a noteOn MIDI command to the output
        void noteOn(uint8_t note, uint8_t velocity, int8_t channel=-1);

        //! Sends a noteOff MIDI command to the output
        void noteOff(uint8_t note, uint8_t velocity, int8_t channel=-1);

        //! Sends a pitchBend MIDI command to the output
        void pitchBend(uint16_t value, int8_t channel=-1);

        //! Sends a channelPressure MIDI command to the output
        void channelPressure(uint8_t value, int8_t channel=-1);

        //! Sends a polyPressure MIDI command to the output
        void polyPressure(uint8_t valuePar, int8_t channel=-1);

        //! Sends a programChange MIDI command to the output
        void programChange(uint8_t program, int8_t channel=-1);

        //! Sends a allNotesOff MIDI command to the output
        void allNotesOff( int8_t channel=-1);

        //! Sends a resetAllControllers  MIDI command to the output
        void resetAllControllers( int8_t channel=-1);

        //! Sends a localControl  MIDI command to the output
        void localControl( bool active, int8_t channel=-1);

        //! Sends a control change MIDI command to the output
        void controlChange(uint8_t msg, uint8_t value, int8_t channel=-1);

        //! Determines the connection status
        ConnectionStatus getConnectionStatus();

        //! Converts a MIDI note to a frequency in Hz
        static float noteToFrequency(uint8_t note);

        //! Converts a Frequency (in Hz) to a MIDI note
        static uint8_t frequencyToNote(float freq);

        //! Defines the voice which is used in inbound processing
        void setVoicer(Voicer &voicer);


    protected:
        void setConnectionStatus(ConnectionStatus status);
        void updateTimestamp(MidiMessage *pMsg);
        virtual void writeData(MidiMessage *msg, int len);
        
        ConnectionStatus connectionStatus;
        Voicer *pVoicer;
        MidiMessage outMessage;
        int receivingChannel = -1;  
        uint8_t sendingChannel = 0;  
        uint8_t timestampLow;
        uint8_t timestampHigh;
        char *name;
};


} // namespace

#endif
#endif
