/**
 * Write the data to the indicated ArdStream interface in a way that is suited for the ArdStream Plotter
 * 
*/

#include "ArdStreamOut.h"
#ifdef ARDSTREAMOUT_H

namespace stk {

ArdStreamOut::ArdStreamOut(Stream &stream, unsigned samples, unsigned channels)
: ArdStreamOutCommon(stream, samples, channels) {
}

void ArdStreamOut :: writeData( unsigned long frames )
{
  unsigned long sample = 0;;  // 
  for (unsigned long f=0l; f<frames;f++){
      for (int c=0; c<nChannels;c++) {
        this->clipTest( data_[sample] );
        pStream->print(data_[sample]);
        sample++;
      }
      pStream->println();
  }
}

// not used
void ArdStreamOut::writeBuffer(unsigned long len){
}


}; //stk

#endif
