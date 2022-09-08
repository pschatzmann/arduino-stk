
#include "FileLoop.h"
#include "MemoryFS.h"

#ifdef __RAW_ARRAYS__

#ifndef STK_MEMORYLOOP_H
#define STK_MEMORYLOOP_H


namespace stk {

/***************************************************/
/*! \class stk::MemoryLoop
    \brief STK looping / oscillator class using a simple
   memory array as input.

    A 'virtual' file can be registered so that a fileOpen
    call with a file name is working like in the corresponding
    FileWvIn class

    This is basically a simplified version of the FileLoop


  Phil Schatzmann
*/
/***************************************************/

class MemoryLoop : public FileLoop {

 public:
  //! Default constructor.
  MemoryLoop(unsigned long chunkSize=256);

  //! Class constructor that opens a specified file.
  MemoryLoop(std::string fileName,  const unsigned char *data, size_t size, unsigned long chunkSize=1024);

  //! Class constructor that opens a specified file. The file must have been registered
  MemoryLoop(std::string fileName, unsigned long chunkSize=1024);

  //! Class destructor.
  ~MemoryLoop( void );

  //! Open the specified file and load its data.
  /*!
    This method is provided to be compatible with the FileWvIn
  */
  virtual void openFile( std::string fileName, bool raw = true, bool doNormalize = true, bool doInt2FloatScaling = true ) override;
 
  //! Open the specified file and load its data.
  /*!
    This method is provided to be compatible with the FileWvIn
  */
  virtual void openFile( const char* fileName, bool raw = true, bool doNormalize = true, bool doInt2FloatScaling = true ) ;

  //! Add a phase offset to the current read pointer.
  /*!
    This function determines a time offset based on the file
    size and the current Stk::sampleRate.  The \e angle value
    is a multiple of file size.
  */
  void addPhaseOffset( StkFloat angle ) override;
  
  //! Close the actual file
  /*!
    This method is provided to be compatible with the FileWvIn
  */
  void  closeFile(void ) override;

  //! Query whether a file is open.
  /*!
    This does not have any importance since the data is available in the memory
    anyhow.
  */
  bool  isOpen(void ) override;

  //! Return the input file sample rate in Hz (not the data read rate).
  /*!
    STK RAW files have a sample rate of 22050 Hz
  */
  virtual StkFloat getFileRate( void ) const override;

protected:
  virtual void fileRead( StkFrames& buffer, unsigned long startFrame, bool doNormalize ) override;
  virtual void open(bool doNormalize=true,bool doInt2FloatScaling=true);

  MemoryFS memoryFS;
  StkFrames firstFrame_;
  StkFloat phaseOffset_;

};

} // stk namespace

#endif
#endif
