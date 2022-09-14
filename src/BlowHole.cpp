/***************************************************/
/*! \class BlowHole
    \brief STK clarinet physical model with one
           register hole and one tonehole.

    This class is based on the clarinet model,
    with the addition of a two-port register hole
    and a three-port dynamic tonehole
    implementation, as discussed by Scavone and
    Cook (1998).

    In this implementation, the distances between
    the reed/register hole and tonehole/bell are
    fixed.  As a result, both the tonehole and
    register hole will have variable influence on
    the playing frequency, which is dependent on
    the length of the air column.  In addition,
    the highest playing freqeuency is limited by
    these fixed lengths.

    This is a digital waveguide model, making its
    use possibly subject to patents held by Stanford
    University, Yamaha, and others.

    Control Change Numbers: 
       - Reed Stiffness = 2
       - Noise Gain = 4
       - Tonehole State = 11
       - Register State = 1
       - Breath Pressure = 128

    by Perry R. Cook and Gary P. Scavone, 1995--2019.
*/
/***************************************************/

#include "BlowHole.h"
#include "SKINImsg.h"
#include <cmath>

namespace stk {

BlowHole :: BlowHole( StkFloat lowestFrequency )
{
  if ( lowestFrequency <= 0.0f ) {
    oStream_ << "BlowHole::BlowHole: argument is less than or equal to zero!";
    handleError( StkError::FUNCTION_ARGUMENT );
  }

  unsigned long nDelays = (unsigned long) ( 0.5f * Stk::sampleRate() / lowestFrequency );

  // delays[0] is the delay line between the reed and the register vent.
  delays_[0].setDelay( 5.0f * Stk::sampleRate() / 22050.0f );
  // delays[1] is the delay line between the register vent and the tonehole.
  delays_[1].setMaximumDelay( nDelays + 1 );
  // delays[2] is the delay line between the tonehole and the end of the bore.
  delays_[2].setDelay( 4.0f * Stk::sampleRate() / 22050.0f );

  reedTable_.setOffset( 0.7 );
  reedTable_.setSlope( -0.3 );

  // Calculate the initial tonehole three-port scattering coefficient
  StkFloat rb = 0.0075;    // main bore radius
  StkFloat rth = 0.003;    // tonehole radius
  scatter_ = -std::pow(rth,2) / ( std::pow(rth,2) + 2*std::pow(rb,2) );

  // Calculate tonehole coefficients and set for initially open.
  StkFloat te = 1.4f * rth;    // effective length of the open hole
  thCoeff_ = (te*2*Stk::sampleRate() - 347.23f) / (te*2*Stk::sampleRate() + 347.23f   );
  tonehole_.setA1( -thCoeff_ );
  tonehole_.setB0( thCoeff_ );
  tonehole_.setB1( -1.0 );

  // Calculate register hole filter coefficients
  StkFloat r_rh = 0.0015f;    // register vent radius
  te = 1.4f * r_rh;         // effective length of the open hole
  StkFloat xi = 0.0;         // series resistance term
  StkFloat zeta = 347.23f + 2.0f*STK_PI*powf(rb,2)*xi/1.1769f;
  StkFloat psi = 2.0f*STK_PI*powf(rb,2)*te / (STK_PI*powf(r_rh,2));
  StkFloat rhCoeff = (zeta - 2.0f * Stk::sampleRate() * psi) / (zeta + 2.0f * Stk::sampleRate() * psi);
  rhGain_ = -347.23f / (zeta + 2.0f * Stk::sampleRate() * psi);
  vent_.setA1( rhCoeff );
  vent_.setB0( 1.0 );
  vent_.setB1( 1.0 );
  // Start with register vent closed
  vent_.setGain( 0.0 );

  vibrato_.setFrequency((StkFloat) 5.735);
  outputGain_ = 1.0;
  noiseGain_ = 0.2;
  vibratoGain_ = 0.01;

  this->setFrequency( 220.0 );
  this->clear();
}

BlowHole :: ~BlowHole( void )
{
}

void BlowHole :: clear( void )
{
  delays_[0].clear();
  delays_[1].clear();
  delays_[2].clear();
  filter_.tick( 0.0 );
  tonehole_.tick( 0.0 );
  vent_.tick( 0.0 );
}

void BlowHole :: setFrequency( StkFloat frequency )
{
#if defined(_STK_DEBUG_)
  if ( frequency <= 0.0 ) {
    oStream_ << "BlowHole::setFrequency: argument is less than or equal to zero!";
    handleError( StkError::WARNING ); return;
  }
#endif

  // Account for approximate filter delays and one sample "lastOut" delay.
  StkFloat delay = ( Stk::sampleRate() / frequency ) * 0.5f - 3.5f;
  delay -= delays_[0].getDelay() + delays_[2].getDelay();

  delays_[1].setDelay( delay );
}

void BlowHole :: setVent( StkFloat newValue )
{
  // This method allows setting of the register vent "open-ness" at
  // any point between "Open" (newValue = 1) and "Closed"
  // (newValue = 0).

  StkFloat gain;

  if ( newValue <= 0.0f )
    gain = 0.0f;
  else if ( newValue >= 1.0f )
    gain = rhGain_;
  else
    gain = newValue * rhGain_;

  vent_.setGain( gain );
}

void BlowHole :: setTonehole( StkFloat newValue )
{
  // This method allows setting of the tonehole "open-ness" at
  // any point between "Open" (newValue = 1) and "Closed"
  // (newValue = 0).
  StkFloat new_coeff;

  if ( newValue <= 0.0f )
    new_coeff = 0.9995;
  else if ( newValue >= 1.0f )
    new_coeff = thCoeff_;
  else
    new_coeff = ( newValue * (thCoeff_ - 0.9995f) ) + 0.9995f;

  tonehole_.setA1( -new_coeff );
  tonehole_.setB0( new_coeff );
}

void BlowHole :: startBlowing( StkFloat amplitude, StkFloat rate )
{
  if ( amplitude <= 0.0f || rate <= 0.0f ) {
    oStream_ << "BlowHole::startBlowing: one or more arguments is less than or equal to zero!";
    handleError( StkError::WARNING ); return;
  }

  envelope_.setRate( rate );
  envelope_.setTarget( amplitude );
}

void BlowHole :: stopBlowing( StkFloat rate )
{
  if ( rate <= 0.0f ) {
    oStream_ << "BlowHole::stopBlowing: argument is less than or equal to zero!";
    handleError( StkError::WARNING ); return;
  }

  envelope_.setRate( rate );
  envelope_.setTarget( 0.0 ); 
}

void BlowHole :: noteOn( StkFloat frequency, StkFloat amplitude )
{
  this->setFrequency( frequency );
  this->startBlowing( 0.55f + (amplitude * 0.30f), amplitude * 0.005f );
  outputGain_ = amplitude + 0.001f;
}

void BlowHole :: noteOff( StkFloat amplitude )
{
  this->stopBlowing( amplitude * 0.01f );
}

void BlowHole :: controlChange( int number, StkFloat value )
{
#if defined(_STK_DEBUG_)
  if ( Stk::inRange( value, 0.0, 128.0 ) == false ) {
    oStream_ << "BlowHole::controlChange: value (" << value << ") is out of range!";
    handleError( StkError::WARNING ); return;
  }
#endif

  StkFloat normalizedValue = value * ONE_OVER_128;
  if (number == __SK_ReedStiffness_) // 2
    reedTable_.setSlope( -0.44f + (0.26f * normalizedValue) );
  else if (number == __SK_NoiseLevel_) // 4
    noiseGain_ = ( normalizedValue * 0.4f);
  else if (number == __SK_ModFrequency_) // 11
    this->setTonehole( normalizedValue );
  else if (number == __SK_ModWheel_) // 1
    this->setVent( normalizedValue );
  else if (number == __SK_AfterTouch_Cont_) // 128
    envelope_.setValue( normalizedValue );
#if defined(_STK_DEBUG_)
  else {
    oStream_ << "BlowHole::controlChange: undefined control number (" << number << ")!";
    handleError( StkError::WARNING );
  }
#endif
}

} // stk namespace
