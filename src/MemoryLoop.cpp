/***************************************************/
/*! \class MemoryLoop
    \brief STK file looping / oscillator class.

    This class provides audio file looping functionality.  Any audio
    memory array can be looped using this
    class.

    Phil Schatzmannn
*/
/***************************************************/

#include "MemoryLoop.h"
#include "ArdStkLogger.h"

#ifdef __RAW_ARRAYS__

namespace stk {

MemoryLoop :: MemoryLoop(unsigned long chunkSize)
: FileLoop(chunkSize*10, chunkSize)  {
  Stk::addSampleRateAlert( this );
}

MemoryLoop ::  MemoryLoop(std::string fileName, unsigned long chunkSize)
: FileLoop(chunkSize*10, chunkSize)  {
  Stk::addSampleRateAlert( this );
  bool ok = memoryFS.open(fileName.c_str(), chunkSize);
  if (!ok){
    STK_LOGE("Could not find file: %s", fileName.c_str());
  }
}

MemoryLoop :: MemoryLoop(std::string fileName,  const unsigned char *data, size_t size, unsigned long chunkSize )
: FileLoop(chunkSize*10, chunkSize)  {
  Stk::addSampleRateAlert( this );
  memoryFS.registerFile(fileName.c_str(), data, size);
  this->openFile(fileName, true, true, true);
}

MemoryLoop :: ~MemoryLoop( void ){
  Stk::removeSampleRateAlert( this );
}

void MemoryLoop :: addPhaseOffset( StkFloat angle ){
  // Add a phase offset in cycles, where 1.0 = fileSize.
  phaseOffset_ = memoryFS.getSize() * angle;
}

void MemoryLoop ::openFile( std::string fileName, bool raw, bool doNormalize, bool doInt2FloatScaling ){
  openFile(fileName.c_str(), raw, doNormalize,doInt2FloatScaling );
}

void MemoryLoop ::openFile( const char* fileName, bool raw, bool doNormalize, bool doInt2FloatScaling ){
  if (!memoryFS.open(fileName)){
    finished_ = true;
    STK_LOGE("failed to open file %s", fileName);
    return;
  }
  open(doNormalize, doInt2FloatScaling);
}

void MemoryLoop ::open(bool doNormalize,bool doInt2FloatScaling) {
  finished_ = false;
  fileSize_ = memoryFS.getSize();

  // Determine whether chunking or not.
  if ( fileSize_ > chunkThreshold_ ) {
    chunking_ = true;
    chunkPointer_ = 0;
    data_.resize( chunkSize_ + 1, 1 );
  }
  else {
    chunking_ = false;
    data_.resize( fileSize_ + 1, 1 );
  }

  if ( doInt2FloatScaling )
    int2floatscaling_ = true;
  else
    int2floatscaling_ = false;

  // Load all or part of the data.
  fileRead( data_, 0, int2floatscaling_ );

  if ( chunking_ ) { // If chunking, save the first sample frame for later.
    firstFrame_.resize( 1, data_.channels() );
    for ( unsigned int i=0; i<data_.channels(); i++ )
      firstFrame_[i] = data_[i];
  }
  else {  // If not chunking, copy the first sample frame to the last.
    for ( unsigned int i=0; i<data_.channels(); i++ )
      data_( data_.frames() - 1, i ) = data_[i];
  }

  // Resize our lastOutputs container.
  lastFrame_.resize( 1, 1 );

  // Set default rate based on file sampling rate.
  this->setRate( getFileRate() / Stk::sampleRate() );

  if ( doNormalize && !chunking_ ) this->normalize();

  this->reset();
}

void MemoryLoop :: fileRead( StkFrames& buffer, unsigned long startFrame, bool doNormalize ){
  finished_ = memoryFS.fileRead(buffer, startFrame, doNormalize);
}

void MemoryLoop :: closeFile( void ){
  memoryFS.close();
  finished_ = true;
}

StkFloat MemoryLoop :: getFileRate( void ) const { 
  return 22050.0;
}


bool MemoryLoop :: isOpen( void ) { 
  return memoryFS.isOpen();
}


} // stk namespace

#endif
