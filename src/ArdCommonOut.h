#include "Stk.h"
#ifdef __STREAMS__

#ifndef ARDCOMMONOUT_H
#define ARDCOMMONOUT_H

#include "WvOut.h"

namespace stk {
/***************************************************/
/*! \class stk::ArdCommonOut
    \brief STK Common Output for Arduiono

    Common Implementation of Output of int16 data to different 
    devices used by Arduino

    by Phil Schatzmann

*/
/***************************************************/

class ArdCommonOut : public WvOut
{
 public:
  //! Default constructor ... 
  ArdCommonOut( unsigned int nChannels = 1 );

  //! Class destructor.
  ~ArdCommonOut();


  //! Output a single sample to all channels in a sample frame.
  /*!
    An StkError is thrown if an output error occurs.  If a socket
    connection does not exist, the function does nothing (a warning
    will be issued if _STK_DEBUG_ is defined during compilation).
  */
  virtual void tick( const StkFloat sample );

  //! Output the StkFrames data.
  /*!
    An StkError will be thrown if an output error occurs.  An
    StkError will also be thrown if _STK_DEBUG_ is defined during
    compilation and there is an incompatability between the number of
    channels in the FileWvOut object and that in the StkFrames object.
    If a socket connection does not exist, the function does nothing
    (a warning will be issued if _STK_DEBUG_ is defined during
    compilation).
  */
  virtual void tick( const StkFrames& frames );

 protected:

  virtual void write(StkFloat value)  = 0;
  virtual void write(int16_t value) = 0;

  unsigned int nChannels;
};

} // stk namespace

#endif
#endif
