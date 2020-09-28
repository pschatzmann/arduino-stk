#include "Stk.h"
#ifdef __STREAMS__

#ifndef ARDCOMMONOUT_H
#define ARDCOMMONOUT_H

#include "WvOut.h"

namespace stk {
/***************************************************/
/*! \class ArdCommonOut
    \brief STK Common Output for Arduiono

    Common Implementation of Output of int32 data to different 
    devices used by Arduino

    by Phil Schatzmann

*/
/***************************************************/

class ArdCommonOut : public WvOut
{
 public:
  //! Default constructor ... 
  ArdCommonOut( unsigned long packetFrames = 1024, unsigned int nChannels = 1 );

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

  virtual void incrementFrame( void );

  // Write a buffer of length frames via the socket connection.
  virtual void writeData( unsigned long frames );

   // Writes the buffer to the device
  virtual void writeBuffer(unsigned long len) = 0;

  char *buffer_ = NULL; 
  unsigned long bufferFrames_;
  unsigned long bufferBytes_;
  unsigned long bufferIndex_;
  unsigned long dataBytes_;
  unsigned long iData_;
  unsigned int nChannels;
};

} // stk namespace

#endif
#endif
