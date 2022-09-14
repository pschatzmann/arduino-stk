#include "ArdCommonOut.h"
#ifdef ARDCOMMONOUT_H

namespace stk {

ArdCommonOut :: ArdCommonOut( unsigned int nChannels )
{
  this->nChannels = nChannels;
}

ArdCommonOut :: ~ArdCommonOut()
{
}

void ArdCommonOut :: tick( const StkFloat sample )
{
    StkFloat out = sample;
    write(clipTest(out));
}

void ArdCommonOut :: tick( const StkFrames& frames )
{
  unsigned int iFrames = 0;
  for ( unsigned int i=0; i<frames.frames(); i++ ) {
    StkFloat sample = frames[iFrames++];
    tick(sample);
  }
}

} // stk namespace

#endif
