

#include "MemoryWvIn.h"
#ifdef __RAW_ARRAYS__

namespace stk {


MemoryWvIn :: MemoryWvIn(unsigned long chunkSize)
:FileWvIn(chunkSize*10,chunkSize){
  Stk::addSampleRateAlert( this );
}

MemoryWvIn :: MemoryWvIn(std::string name, const unsigned char* raw, size_t size,unsigned long chunkSize)
:FileWvIn(chunkSize*10,chunkSize){
  Stk::addSampleRateAlert( this );
  fs.registerFile(name.c_str(),raw, size);
  openFile(name);
}

MemoryWvIn :: MemoryWvIn(std::string name,unsigned long chunkSize ) 
:FileWvIn(chunkSize*10,chunkSize){
  Stk::addSampleRateAlert( this );
  openFile(name);
}

MemoryWvIn ::~MemoryWvIn( void ){
  closeFile();
  Stk::removeSampleRateAlert( this );
}

void MemoryWvIn ::openFile( std::string fileName, bool raw, bool doNormalize, bool doInt2FloatScaling ){
  openFile(fileName.c_str(), raw, doNormalize,doInt2FloatScaling );
}

void MemoryWvIn ::openFile( const char* fileName, bool raw, bool doNormalize, bool doInt2FloatScaling ){
  if (!fs.open(fileName)){
    finished_ = true;
    return;
  }
  finished_ = false;
  fileSize_ = fs.getSize();

  // Determine whether chunking or not.
  if ( fileSize_ > chunkSize_ ) {
    chunking_ = true;
    chunkPointer_ = 0;
    data_.resize( chunkSize_, 1 );
  }
  else {
    chunking_ = false;
    data_.resize( (size_t) fileSize_, 1 );
  }

  data_.setDataRate(this->getFileRate());
  fileRead(data_,0, doNormalize);

  // Resize our lastFrame container.
  lastFrame_.resize( 1, 1 );
  this->setRate( data_.dataRate() / Stk::sampleRate() );
  if ( doNormalize & !chunking_) {
    this->normalize();
  }
  this->reset();
  
}

void MemoryWvIn :: fileRead( StkFrames& buffer, unsigned long startFrame, bool doNormalize ){
  finished_ = fs.fileRead(buffer, startFrame, doNormalize);
}

void MemoryWvIn :: closeFile( void ){
  fs.close();
  finished_ = true;
}

StkFloat MemoryWvIn :: getFileRate( void )  { 
  return 22050.0;
}

bool MemoryWvIn :: isOpen( void ) { 
  return fs.isOpen();
}


} // stk namespace

#endif