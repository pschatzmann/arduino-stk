#include "ArdCommonOut.h"
#ifdef ARDCOMMONOUT_H

namespace stk {

ArdCommonOut :: ArdCommonOut( unsigned long packetFrames, unsigned int nChannels )
{
  this->bufferFrames_ = packetFrames;
  this->nChannels = nChannels;
  this->bufferBytes_ = 0;

  // Allocate new memory buffer if necessary.
  data_.resize( bufferFrames_, nChannels );
  buffer_ = (char *) new char[ packetFrames * nChannels ];
  frameCounter_ = 0;
  bufferIndex_ = 0;
  iData_ = 0;

}

ArdCommonOut :: ~ArdCommonOut()
{
  if ( buffer_ ) delete [] buffer_;
}


void ArdCommonOut :: writeData( unsigned long frames )
{
  unsigned long samples = frames * nChannels;
  dataBytes_ = sizeof(SINT16);
  SINT16 *ptr = (SINT16 *) buffer_;
  for ( unsigned long k=0; k<samples; k++ ) {
        this->clipTest( data_[k] );
        *ptr = (SINT16) (data_[k] * 32767.0);
    #ifdef __LITTLE_ENDIAN__
        swap16 ((unsigned char *)ptr);
    #endif
        ptr++;
  }
  
  long bytes = dataBytes_ * samples;
  writeBuffer(bytes);
}


void ArdCommonOut :: incrementFrame( void )
{
  frameCounter_++;
  bufferIndex_++;

  if ( bufferIndex_ == bufferFrames_ ) {
    writeData( bufferFrames_ );
    bufferIndex_ = 0;
    iData_ = 0;
  }
}

void ArdCommonOut :: tick( const StkFloat sample )
{
  StkFloat input = sample;
  clipTest( input );
  for ( unsigned int j=0; j<nChannels; j++ )
    data_[iData_++] = input;

  this->incrementFrame();
}

void ArdCommonOut :: tick( const StkFrames& frames )
{
  unsigned int j;
  unsigned int iFrames = 0;
  for ( unsigned int i=0; i<frames.frames(); i++ ) {

    for ( j=0; j<nChannels; j++ ) {
      data_[iData_] = frames[iFrames++];
      clipTest( data_[iData_++] );
    }
    this->incrementFrame();
  }
}

} // stk namespace

#endif
