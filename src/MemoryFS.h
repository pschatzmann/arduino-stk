#include "Stk.h"
#ifdef __RAW_ARRAYS__
#ifndef MEMORYFS_H
#define MEMORYFS_H

#include "Stk.h"
#if defined(ESP32)
#  include "esp_system.h"
#  include "esp32-hal-log.h"
#   include "pgmspace.h"
#elif defined(IS_DESKTOP)
#  ifndef memcpy_P
#   define memcpy_P memcpy
#  endif
#elif defined(ESP8266)
#  include "pgmspace.h"
#elif defined(ARDUINO) && __has_include(<pgmspace.h>) 
#  include <pgmspace.h>
#endif

#ifndef USE_RAM_DEFAUT
#  define USE_RAM_DEFAUT false
#endif

#define VFS_INC_SIZE 10
#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

namespace stk {

//! Simulated File Descriptor of registered virtual "Memory" files
struct VFS_FD;
extern VFS_FD **registry;
extern int registry_last_entry;
extern bool registry_use_ram;

/***************************************************/
/*! \class stk::MemoryFS
    \brief STK in memory File
    An instance is representing an individual memory array which
    can be registered with a file name.

    This functionality is used by the MemoryLoop and MemoryWvIn
    as compatibility layer for the related file based operations.
    So that we can use the instruments which are based on recorded
    samples.

  Phil Schatzmann
*/
/***************************************************/
class MemoryFS {
    public:
        MemoryFS(const unsigned char *raw=NULL, unsigned int size=0, int bytesPerSample=2, bool swapBytes=true);
        MemoryFS (const char* fileName);

        //! makes the file available so that the open (by name) operation is working
        static VFS_FD * registerFile(const char* fileName, const unsigned char *raw, unsigned int size);
        //! opens the indicated file
        bool open(const char* fileName, int bytesPerSample=2);
        //! Returns a descriptor with additional information
        //VFS_FD* getFD();
        //! Determines the size (in samples)
        size_t getSize(); 
        //! sets to open flag to false
        void close();
        //! checks if the file has been opened
        bool isOpen();
        //! Reads the data into the buffer at the indicated start frame
        bool fileRead( StkFrames& buffer, unsigned long startFrame, bool doNormalize) ;
        /// Clears all stored tables
        static void reset();
        /// Defines if the audio data is copied from progmem to RAM
        static void useRAM(bool ram) {
            registry_use_ram = ram;
        }
        size_t current_pos_; // current read position in the memory in samples


    protected:
        long  fileSize_ = 0;;
        int16_t *data_ = nullptr;
        bool swapBytes = true;
        bool is_open = false;

        static VFS_FD* findByName(const char * path);
};

}

#endif
#endif