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
#ifdef __VFS__

namespace stk {

MemoryLoop :: MemoryLoop(unsigned long chunkSize){
  chunkSize_ = chunkSize;
  Stk::addSampleRateAlert( this );
}

MemoryLoop :: MemoryLoop(const char* fileName,  const unsigned char *data, size_t size, unsigned long chunkSize ){
  chunkSize_ = chunkSize;
  Stk::addSampleRateAlert( this );
  fs_ptr->registerFile(fileName,data, size);
  this->openFile(fileName, true, true, true);
}

MemoryLoop :: MemoryLoop(MemoryFS *data, unsigned long chunkSize) {
  chunkSize_ = chunkSize;
  Stk::addSampleRateAlert( this );
  open(data, false);
}

MemoryLoop :: ~MemoryLoop( void ){
  if (owning_fs_ptr)
    delete this->fs_ptr;
}

void MemoryLoop :: addPhaseOffset( StkFloat angle ){
  // Add a phase offset in cycles, where 1.0 = fileSize.
  phaseOffset_ = fs_ptr->getSize() * angle;
}

void MemoryLoop ::openFile( std::string fileName, bool raw, bool doNormalize, bool doInt2FloatScaling ){
  openFile(fileName.c_str(), raw, doNormalize,doInt2FloatScaling );
}

void MemoryLoop ::openFile( const char* fileName, bool raw, bool doNormalize, bool doInt2FloatScaling ){
  MemoryFS *fs = this->fs_ptr;
  bool owning = false;
  if (fs==NULL){
    fs = new MemoryFS();
    owning = true;
  }
  // Attempt to open the file ... an error might be thrown here.
  if (!fs->open(fileName)){
    finished_ = true;
    return;
  }
  open(fs, owning, doNormalize, doInt2FloatScaling);
}

void MemoryLoop ::open(MemoryFS *fs, bool owning_fs_ptr, bool doNormalize,bool doInt2FloatScaling) {
  this->owning_fs_ptr = owning_fs_ptr;
  this->fs_ptr = fs;
  finished_ = false;
  fileSize_ = fs_ptr->getSize();

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
  this->setRate( data_.dataRate() / Stk::sampleRate() );

  if ( doNormalize & !chunking_ ) this->normalize();

  this->reset();
}

void MemoryLoop :: fileRead( StkFrames& buffer, unsigned long startFrame, bool doNormalize ){
  finished_ = fs_ptr->fileRead(buffer, startFrame, doNormalize);
}

void MemoryLoop :: closeFile( void ){
  fs_ptr->close();
  finished_ = true;
}

bool MemoryLoop :: isOpen( void ) { 
  return fs_ptr->isOpen();
}


} // stk namespace

#endif
