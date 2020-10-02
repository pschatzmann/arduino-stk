#include "ArdConfig.h"
#ifdef __MIDI__

#ifndef ARDMIDIEVENTHANDLER_H
#define ARDMIDIEVENTHANDLER_H

#include "Voicer.h"

namespace stk {

/***************************************************/
/*! \class ArdMidiSerialIn
    \brief  A simple Midi Parser which calls the corresponding events. 
    It supports Midi and BLE Midi messages. The main entry point
    is the parse command which calls the related methods.
  
    In this implementation the handler just passes the noteOn 
    and noteOff to the Voicer.
  
    If you indicate the channel in the constructor, it is used as filter
    to process only the messages for the indicated channel.

    http://www.hangar42.nl/wp-content/uploads/2017/10/BLE-MIDI-spec.pdf


    by Phil Schatzmann
*/
/***************************************************/
class ArdMidiEventHandler  {
    public:
        ArdMidiEventHandler(Voicer *voicer, int *p_channel = NULL );
         ~ArdMidiEventHandler();
        void addInstrument(Instrmnt &instrument, int channel);
        void parse(uint8_t*  msg, uint8_t len);
        virtual void onCommand(uint8_t channel, uint8_t status, uint8_t p1,uint8_t p2 );
        virtual void onNoteOn(uint8_t note, uint8_t velocity,uint8_t channel);
        virtual void onNoteOff(uint8_t note, uint8_t velocity,uint8_t channel);
        virtual void onPitchBend( uint8_t value, uint8_t channel);
        virtual void onControlChange( uint8_t controller, uint8_t controllerValue, uint8_t channel);

    protected:
        Voicer *p_voicer; 
        int *p_channel;
};


} // namespace

#endif
#endif
