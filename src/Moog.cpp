/***************************************************/
/*! \class Moog
    \brief STK moog-like swept filter sampling synthesis class.

    This instrument uses one attack wave, one
    looped wave, and an ADSR envelope (inherited
    from the Sampler class) and adds two sweepable
    formant (FormSwep) filters.

    Control Change Numbers: 
       - Filter Q = 2
       - Filter Sweep Rate = 4
       - Vibrato Frequency = 11
       - Vibrato Gain = 1
       - Gain = 128

    by Perry R. Cook and Gary P. Scavone, 1995--2019.
*/
/***************************************************/

#include "Moog.h"
#include "SKINImsg.h"
#include "ArdConfig.h"

// #ifdef __RAW_ARRAYS__
//   extern const unsigned char *mandpluk_raw, *impuls20_raw, *sinewave_raw;
//   extern const unsigned int mandpluk_raw_len, impuls20_raw_len,sinewave_raw_len;
// #endif

namespace stk {


Moog :: Moog( void )
{

  #ifdef __RAW_ARRAYS__
  // Concatenate the STK rawwave path to the rawwave file
  attacks_.push_back( new MemoryWvIn( (Stk::rawwavePath() + "mandpluk.raw"), mandpluk_raw,mandpluk_raw_len ) );
  loops_.push_back ( new MemoryLoop( (Stk::rawwavePath() + "impuls20.raw"), impuls20_raw,impuls20_raw_len ) );
  loops_.push_back ( new MemoryLoop( (Stk::rawwavePath() + "sinewave.raw"), sinewave_raw,sinewave_raw_len ) ); // vibrato
  #else
  // Concatenate the STK rawwave path to the rawwave file
  attacks_.push_back( new FileWvIn( (Stk::rawwavePath() + "mandpluk.raw"), true ) );
  loops_.push_back ( new FileLoop( (Stk::rawwavePath() + "impuls20.raw"), true ) );
  loops_.push_back ( new FileLoop( (Stk::rawwavePath() + "sinewave.raw"), true ) ); // vibrato
  #endif
  
 
  loops_[1]->setFrequency( 6.122 );

  filters_[0].setTargets( 0.0, 0.7 );
  filters_[1].setTargets( 0.0, 0.7 );

  adsr_.setAllTimes( 0.001, 1.5, 0.6, 0.250 );
  filterQ_ = 0.85;
  filterRate_ = 0.0001;
  modDepth_ = 0.0;
}  

Moog :: ~Moog( void )
{
}

void Moog :: setFrequency( StkFloat frequency )
{
#if defined(_STK_DEBUG_)
  if ( frequency <= 0.0 ) {
    oStream_ << "Moog::setFrequency: parameter is less than or equal to zero!";
    handleError( StkError::WARNING ); return;
  }
#endif

  baseFrequency_ = frequency;
  StkFloat rate = attacks_[0]->getSize() * 0.01f * baseFrequency_ / Stk::sampleRate();
  attacks_[0]->setRate( rate );
  loops_[0]->setFrequency( baseFrequency_ );
}

void Moog :: noteOn( StkFloat frequency, StkFloat amplitude )
{
  StkFloat temp;
    
  this->setFrequency( frequency );
  this->keyOn();
  attackGain_ = amplitude * 0.5f;
  loopGain_ = amplitude;

  temp = filterQ_ + 0.05f;
  filters_[0].setStates( 2000.0, temp );
  filters_[1].setStates( 2000.0, temp );

  temp = filterQ_ + 0.099f;
  filters_[0].setTargets( frequency, temp );
  filters_[1].setTargets( frequency, temp );

  filters_[0].setSweepRate( filterRate_ * 22050.0f / Stk::sampleRate() );
  filters_[1].setSweepRate( filterRate_ * 22050.0f / Stk::sampleRate() );
}

void Moog :: controlChange( int number, StkFloat value )
{
#if defined(_STK_DEBUG_)
  if ( Stk::inRange( value, 0.0, 128.0 ) == false ) {
    oStream_ << "Moog::controlChange: value (" << value << ") is out of range!";
    handleError( StkError::WARNING ); return;
  }
#endif

  StkFloat normalizedValue = value * ONE_OVER_128;
  if (number == __SK_FilterQ_) // 2
    filterQ_ = 0.80f + ( 0.1f * normalizedValue );
  else if (number == __SK_FilterSweepRate_) // 4
    filterRate_ = normalizedValue * 0.0002f;
  else if (number == __SK_ModFrequency_) // 11
    this->setModulationSpeed( normalizedValue * 12.0f );
  else if (number == __SK_ModWheel_)  // 1
    this->setModulationDepth( normalizedValue );
  else if (number == __SK_AfterTouch_Cont_) // 128
    adsr_.setTarget( normalizedValue );
#if defined(_STK_DEBUG_)
  else {
    oStream_ << "Moog::controlChange: undefined control number (" << number << ")!";
    handleError( StkError::WARNING );
  }
#endif
}

} // stk namespace
