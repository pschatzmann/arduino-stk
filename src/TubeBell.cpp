/***************************************************/
/*! \class TubeBell
    \brief STK tubular bell (orchestral chime) FM
           synthesis instrument.

    This class implements two simple FM Pairs
    summed together, also referred to as algorithm
    5 of the TX81Z.

    \code
    Algorithm 5 is :  4->3--\
                             + --> Out
                      2->1--/
    \endcode

    Control Change Numbers: 
       - Modulator Index One = 2
       - Crossfade of Outputs = 4
       - LFO Speed = 11
       - LFO Depth = 1
       - ADSR 2 & 4 Target = 128

    The basic Chowning/Stanford FM patent expired
    in 1995, but there exist follow-on patents,
    mostly assigned to Yamaha.  If you are of the
    type who should worry about this (making
    money) worry away.

    by Perry R. Cook and Gary P. Scavone, 1995--2019.
*/
/***************************************************/

#include "TubeBell.h"
#include "ArdConfig.h"

// #ifdef __RAW_ARRAYS__
//   extern const unsigned char *fwavblnk_raw, *sinewave_raw;
//   extern const unsigned int fwavblnk_raw_len,sinewave_raw_len;
// #endif

namespace stk {

TubeBell :: TubeBell( void )
  : FM()
{

  #ifdef __RAW_ARRAYS__
  for ( unsigned int i=0; i<3; i++ )
    waves_[i] = new MemoryLoop( Stk::rawwavePath() + "sinewave.raw", sinewave_raw,sinewave_raw_len );
  waves_[3] = new MemoryLoop( Stk::rawwavePath() + "fwavblnk.raw", fwavblnk_raw,fwavblnk_raw_len );
  #else
  // Concatenate the STK rawwave path to the rawwave files
  for ( unsigned int i=0; i<3; i++ )
    waves_[i] = new FileLoop( Stk::rawwavePath() + "sinewave.raw", true );
  waves_[3] = new FileLoop( Stk::rawwavePath() + "fwavblnk.raw", true );
  #endif

  this->setRatio(0, 1.0   * 0.995);
  this->setRatio(1, 1.414 * 0.995);
  this->setRatio(2, 1.0   * 1.005);
  this->setRatio(3, 1.414 * 1.000);

  gains_[0] = fmGains_[94];
  gains_[1] = fmGains_[76];
  gains_[2] = fmGains_[99];
  gains_[3] = fmGains_[71];

  adsr_[0]->setAllTimes( 0.005, 4.0, 0.0, 0.04);
  adsr_[1]->setAllTimes( 0.005, 4.0, 0.0, 0.04);
  adsr_[2]->setAllTimes( 0.001, 2.0, 0.0, 0.04);
  adsr_[3]->setAllTimes( 0.004, 4.0, 0.0, 0.04);

  twozero_.setGain( 0.5 );
  vibrato_.setFrequency( 2.0 );
}  

TubeBell :: ~TubeBell( void )
{
}

void TubeBell :: noteOn( StkFloat frequency, StkFloat amplitude )
{
  gains_[0] = amplitude * fmGains_[94];
  gains_[1] = amplitude * fmGains_[76];
  gains_[2] = amplitude * fmGains_[99];
  gains_[3] = amplitude * fmGains_[71];
  this->setFrequency( frequency );
  this->keyOn();
}

} // stk namespace
