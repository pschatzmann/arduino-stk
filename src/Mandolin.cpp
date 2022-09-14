/***************************************************/
/*! \class Mandolin
    \brief STK mandolin instrument model class.

    This class uses two "twang" models and "commuted
    synthesis" techniques to model a mandolin
    instrument.

    This is a digital waveguide model, making its
    use possibly subject to patents held by Stanford
    University, Yamaha, and others.  Commuted
    Synthesis, in particular, is covered by patents,
    granted, pending, and/or applied-for.  All are
    assigned to the Board of Trustees, Stanford
    University.  For information, contact the Office
    of Technology Licensing, Stanford University.

    Control Change Numbers: 
       - Body Size = 2
       - Pluck Position = 4
       - String Sustain = 11
       - String Detuning = 1
       - Microphone Position = 128

    by Perry R. Cook and Gary P. Scavone, 1995--2019.
*/
/***************************************************/

#include "Mandolin.h"
#include "SKINImsg.h"
#include "ArdConfig.h"

// extern const unsigned char mand1_raw[],mand2_raw[],mand3_raw[],mand4_raw[],mand5_raw[],mand6_raw[],mand7_raw[],mand8_raw[],
//                     mand9_raw[],mand10_raw[],mand11_raw[];
// extern unsigned int mand1_raw_len,mand2_raw_len,mand3_raw_len,mand4_raw_len,mand5_raw_len,mand6_raw_len,mand7_raw_len,mand8_raw_len,
//                     mand9_raw_len,mand10_raw_len, mand11_raw_len;

namespace stk {

Mandolin :: Mandolin( StkFloat lowestFrequency )
{
  if ( lowestFrequency <= 0.0f ) {
    oStream_ << "Mandolin::Mandolin: argument is less than or equal to zero!";
    handleError( StkError::FUNCTION_ARGUMENT );
  }

  #ifdef __RAW_ARRAYS__
  MemoryFS::registerFile((Stk::rawwavePath() +"mand1.raw").c_str(), mand1_raw, mand1_raw_len);
  MemoryFS::registerFile((Stk::rawwavePath() +"mand2.raw").c_str(), mand2_raw, mand2_raw_len);
  MemoryFS::registerFile((Stk::rawwavePath() +"mand3.raw").c_str(), mand3_raw, mand3_raw_len);
  MemoryFS::registerFile((Stk::rawwavePath() +"mand4.raw").c_str(), mand4_raw, mand4_raw_len);
  MemoryFS::registerFile((Stk::rawwavePath() +"mand5.raw").c_str(), mand5_raw, mand5_raw_len);
  MemoryFS::registerFile((Stk::rawwavePath() +"mand6.raw").c_str(), mand6_raw, mand6_raw_len);
  MemoryFS::registerFile((Stk::rawwavePath() +"mand7.raw").c_str(), mand7_raw, mand7_raw_len);
  MemoryFS::registerFile((Stk::rawwavePath() +"mand8.raw").c_str(), mand8_raw, mand8_raw_len);
  MemoryFS::registerFile((Stk::rawwavePath() +"mand9.raw").c_str(), mand9_raw, mand9_raw_len);
  MemoryFS::registerFile((Stk::rawwavePath() +"mand10.raw").c_str(), mand10_raw, mand10_raw_len);
  MemoryFS::registerFile((Stk::rawwavePath() +"mand11.raw").c_str(), mand11_raw, mand11_raw_len);
  MemoryFS::registerFile((Stk::rawwavePath() +"mand12.raw").c_str(), mand12_raw, mand12_raw_len);
  #endif

  // Concatenate the STK rawwave path to the rawwave files
  soundfile_[0].openFile( (Stk::rawwavePath() + "mand1.raw").c_str(), true );
  soundfile_[1].openFile( (Stk::rawwavePath() + "mand2.raw").c_str(), true );
  soundfile_[2].openFile( (Stk::rawwavePath() + "mand3.raw").c_str(), true );
  soundfile_[3].openFile( (Stk::rawwavePath() + "mand4.raw").c_str(), true );
  soundfile_[4].openFile( (Stk::rawwavePath() + "mand5.raw").c_str(), true );
  soundfile_[5].openFile( (Stk::rawwavePath() + "mand6.raw").c_str(), true );
  soundfile_[6].openFile( (Stk::rawwavePath() + "mand7.raw").c_str(), true );
  soundfile_[7].openFile( (Stk::rawwavePath() + "mand8.raw").c_str(), true );
  soundfile_[8].openFile( (Stk::rawwavePath() + "mand9.raw").c_str(), true );
  soundfile_[9].openFile( (Stk::rawwavePath() + "mand10.raw").c_str(), true );
  soundfile_[10].openFile( (Stk::rawwavePath() + "mand11.raw").c_str(), true );
  soundfile_[11].openFile( (Stk::rawwavePath() + "mand12.raw").c_str(), true );

  mic_ = 0;
  detuning_ = 0.995;
  pluckAmplitude_ = 0.5;

  strings_[0].setLowestFrequency( lowestFrequency );
  strings_[1].setLowestFrequency( lowestFrequency );
  this->setFrequency( 220.0 );
  this->setPluckPosition( 0.4 );
}

Mandolin :: ~Mandolin( void )
{
}

void Mandolin :: clear( void )
{
  strings_[0].clear();
  strings_[1].clear();
}

void Mandolin :: setPluckPosition( StkFloat position )
{
  if ( position < 0.0f || position > 1.0f ) {
    oStream_ << "Mandolin::setPluckPosition: position parameter out of range!";
    handleError( StkError::WARNING ); return;
  }

  strings_[0].setPluckPosition( position );
  strings_[1].setPluckPosition( position );
}

void Mandolin :: setDetune( StkFloat detune )
{
  if ( detune <= 0.0f ) {
    oStream_ << "Mandolin::setDeturn: parameter is less than or equal to zero!";
    handleError( StkError::WARNING ); return;
  }

  detuning_ = detune;
  strings_[1].setFrequency( frequency_ * detuning_ );
}

void Mandolin :: setBodySize( StkFloat size )
{
  // Scale the commuted body response by its sample rate (22050).
  StkFloat rate = size * 22050.0f / Stk::sampleRate();
  for ( int i=0; i<12; i++ )
    soundfile_[i].setRate( rate );
}

void Mandolin :: setFrequency( StkFloat frequency )
{
#if defined(_STK_DEBUG_)
  if ( frequency <= 0.0 ) {
    oStream_ << "Mandolin::setFrequency: argument is less than or equal to zero!";
    handleError( StkError::WARNING ); return;
  }
#endif

  frequency_ = frequency;
  strings_[0].setFrequency( frequency_ );
  strings_[1].setFrequency( frequency_ * detuning_ );
}

void Mandolin :: pluck( StkFloat amplitude )
{
  if ( amplitude < 0.0f || amplitude > 1.0f ) {
    oStream_ << "Mandolin::pluck: amplitude parameter out of range!";
    handleError( StkError::WARNING ); return;
  }

  soundfile_[mic_].reset();
  pluckAmplitude_ = amplitude;

  //strings_[0].setLoopGain( 0.97 + pluckAmplitude_ * 0.03 );
  //strings_[1].setLoopGain( 0.97 + pluckAmplitude_ * 0.03 );
}

void Mandolin :: pluck( StkFloat amplitude, StkFloat position )
{
  this->setPluckPosition( position );
  this->pluck( amplitude );
}

void Mandolin :: noteOn( StkFloat frequency, StkFloat amplitude )
{
  this->setFrequency( frequency );
  this->pluck( amplitude );
}

void Mandolin :: noteOff( StkFloat amplitude )
{
  if ( amplitude < 0.0f || amplitude > 1.0f ) {
    oStream_ << "Mandolin::noteOff: amplitude is out of range!";
    handleError( StkError::WARNING ); return;
  }

  //strings_[0].setLoopGain( 0.97 + (1 - amplitude) * 0.03 );
  //strings_[1].setLoopGain( 0.97 + (1 - amplitude) * 0.03 );
}

void Mandolin :: controlChange( int number, StkFloat value )
{
#if defined(_STK_DEBUG_)
  if ( Stk::inRange( value, 0.0, 128.0 ) == false ) {
    oStream_ << "Mandolin::controlChange: value (" << value << ") is out of range!";
    handleError( StkError::WARNING ); return;
  }
#endif

  StkFloat normalizedValue = value * ONE_OVER_128;
  if ( number == __SK_BodySize_ ) // 2
    this->setBodySize( normalizedValue * 2.0f );
  else if ( number == __SK_PickPosition_ ) // 4
    this->setPluckPosition( normalizedValue );
  else if ( number == __SK_StringDamping_ ) { // 11
    strings_[0].setLoopGain( 0.97f + (normalizedValue * 0.03f) );
    strings_[1].setLoopGain( 0.97f + (normalizedValue * 0.03f) );
  }
  else if ( number == __SK_StringDetune_ ) // 1
    this->setDetune( 1.0f - (normalizedValue * 0.1f) );
  else if ( number == __SK_AfterTouch_Cont_ ) // 128
    mic_ = (int) (normalizedValue * 11.0f);
#if defined(_STK_DEBUG_)
  else {
    oStream_ << "Mandolin::controlChange: undefined control number (" << number << ")!";
    handleError( StkError::WARNING );
  }
#endif
}

} // stk namespace
