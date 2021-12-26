#pragma once
#include "ArdConfig.h"

#include "ArdStkLogger.h"
#include "MidiAction.h"
#include "Voicer.h"

namespace stk {


/***************************************************/
/*! \class stk::MidiVoicer
    \brief MidiAction which forwards the midi commands 
    to a stk Voicer
    by Phil Schatzmann
*/
/***************************************************/

class StkMidiAction : public midi::MidiAction {
    public:
        StkMidiAction() = default;

        StkMidiAction(Instrmnt *instrument, int group=0) {
            addInstrument(instrument, group);
        };

        Voicer &voicer() {
            return voicer_obj;
        }

        virtual void addInstrument( Instrmnt *instrument, int group=0 ){
            STK_LOGI(__PRETTY_FUNCTION__);
            voicer_obj.addInstrument(instrument, group);
        }

        virtual void removeInstrument( Instrmnt *instrument ){
            STK_LOGI(__PRETTY_FUNCTION__);
            voicer_obj.removeInstrument(instrument);
        }

        virtual void onNoteOn(uint8_t channel, uint8_t note, uint8_t velocity=87) {
            STK_LOGI(__PRETTY_FUNCTION__);
            voicer_obj.noteOn(note, velocity, channel);
        }

        virtual void onNoteOff(uint8_t channel, uint8_t note, uint8_t velocity=87) {
            STK_LOGI(__PRETTY_FUNCTION__);
            voicer_obj.noteOff(note, velocity, channel);
        }

        virtual void onControlChange(uint8_t channel, uint8_t controller, uint8_t value) {
            STK_LOGI(__PRETTY_FUNCTION__);
            voicer_obj.controlChange(controller,(StkFloat) value, channel);
        }

        virtual void onPitchBend(uint8_t channel, uint8_t value) {
            STK_LOGI(__PRETTY_FUNCTION__);
            voicer_obj.pitchBend(value, channel);
        }

        StkFloat tick( unsigned int channel = 0 ){
            return voicer_obj.tick(channel);
        }   

        StkFrames& tick( StkFrames& frames, unsigned int channel = 0 ){
            return voicer_obj.tick(frames, channel);            
        }
    protected:
        Voicer voicer_obj;
};

}

