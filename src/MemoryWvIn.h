#ifndef STK_MEMORYWVIN_H
#define STK_MEMORYWVIN_H

#include "FileWvIn.h"
#include "MemoryFS.h"
#ifdef __RAW_ARRAYS__

//#include <math.h>

namespace stk {

/***************************************************/
/*! \class stk::MemoryWvIn
    \brief STK audio input class from Memory

    Class which simulates the FileWvIn class so that it can
    be used by microcontrollrs. We only support data
    recorded as uint16  in 1 channel at 22050 Hz!

    A 'virtual' file can be registered so that a fileOpen
    call with a file name is working like in the corresponding
    FileWvIn class

    This is basically a simplified c of the FileWvIn

    by Phil Schatzmann
*/
/***************************************************/

class MemoryWvIn : public FileWvIn
{
public:
  //! Default constructor.
  MemoryWvIn(unsigned long chunkSize=512);

  //! Overloaded constructor for file input.
  /*!
    The file is also registed
  */
  MemoryWvIn(std::string fileName,  const unsigned char *data, size_t size, unsigned long chunkSize=256);

  //! Overloaded constructor for file input.
  /*!
    The file must have been registed before
  */
  MemoryWvIn(std::string fileName, unsigned long chunkSize=256);


  //! Class destructor.
  ~MemoryWvIn( void );

  //! Open the specified file and load its data.
  /*!
    This method is provided to be compatible with the FileWvIn
  */
  virtual void openFile( std::string fileName, bool raw = true, bool doNormalize = true, bool doInt2FloatScaling = true );
 
  //! Open the specified file and load its data.
  /*!
    This method is provided to be compatible with the FileWvIn
  */
  virtual void openFile( const char* fileName, bool raw = true, bool doNormalize = true, bool doInt2FloatScaling = true );

  //! Close a file if one is open.
  virtual void closeFile( void );

  //! Return the input file sample rate in Hz (not the data read rate).
  /*!
    STK RAW files have a sample rate of 22050 Hz
  */
  virtual StkFloat getFileRate( void );

  //! Query whether a file is open.
  /*!
    This does not have any importance since the data is available in the memory
    anyhow.
  */
  bool isOpen( void );

  //! Set the data interpolation rate based on a looping frequency.
  /*!
    This function determines the interpolation rate based on the file
    size and the current Stk::sampleRate.  The \e frequency value
    corresponds to file cycles per second.  The frequency can be
    negative, in which case the loop is read in reverse order.
  */
  void setFrequency( StkFloat frequency ) { this->setRate( fileSize_ * frequency / Stk::sampleRate() ); };


protected:
  virtual void fileRead( StkFrames& buffer, unsigned long startFrame, bool doNormalize );

  MemoryFS fs;
};

} // stk namespace

#endif
#endif
