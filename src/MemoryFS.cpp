
#include "MemoryFS.h"
#include "ArdStkLogger.h"

#ifdef __RAW_ARRAYS__

namespace stk {

//! Simulated File Descriptor of registered virtual "Memory" files
struct VFS_FD {
  VFS_FD() = default;

  ~VFS_FD(){
    if (is_ram) delete[] data;
  }
  const char *name="";
  const unsigned char *data=nullptr;
  long size=0; // in bytes
  bool is_ram=false;
};

const char *APP_VFS ="MemoryFS";

// Support for file registration
static const int registry_size = 30;
VFS_FD **registry = nullptr;
int registry_last_entry = 0;
bool registry_use_ram = USE_RAM_DEFAUT;

MemoryFS :: MemoryFS(const unsigned char *raw, unsigned int size, int bytesPerSample, bool swapBytes) {
  if (raw!=nullptr){
    VFS_FD* p_fd = registerFile(nullptr, raw, size);
    if (p_fd != nullptr){
        this->is_open = true;
        this->swapBytes = swapBytes;
        this->data_ = (int16_t*)p_fd->data;
        this->fileSize_ = p_fd->size / bytesPerSample;
        this->current_pos_ = 0;
    } else {
      STK_LOGE("file not registered");
    }
  }
}

MemoryFS :: MemoryFS(const char* fileName) {
  this->swapBytes = true;
  open(fileName);
}


bool MemoryFS :: open(const char* fileName, int bytesPerSample) {
    bool result = false;
    VFS_FD* fd  = findByName(fileName);
    //STK_LOGW("open file %s -> %: %s  - %ld", fileName, fd!=nullptr?"ok":"failed", fd);

    if (fd!=nullptr){
        this->is_open = true;
        this->fileSize_ = fd->size / bytesPerSample;
        this->data_ = (int16_t*) fd->data;
        this->current_pos_ = 0l;
        result = true;
    }
    return result;
}


size_t MemoryFS :: getSize(){
  return fileSize_;
}

bool MemoryFS :: isOpen() {
  return data_!=nullptr;
}

void MemoryFS :: close(){
  this->is_open = false;
}


bool MemoryFS :: fileRead( StkFrames& frames, unsigned long startFrame, bool doNormalize ){
  // data is available as array of uint16_t
  bool is_finished = false;

  if (data_==nullptr){
    STK_LOGE("data is null");
    return false;
  }

  // calculate max size of next batch
  this->current_pos_ = startFrame;
  int result_size = MIN(frames.size() , this->getSize() - startFrame);

  if (result_size>0) {
    if (frames.size() != result_size){
      frames.resize( result_size, 1 );
    }

#ifdef ESP8266
    // we need to access the progmem on 4byte boundries
    for (int j=0; j<result_size; j+=2) {
      int16_t tmp[2];
      memcmp_P(tmp,(PGM_P) data_+j, 4);

      if (this->swapBytes){
        Stk::swap16((unsigned char *) &tmp[0]);
        Stk::swap16((unsigned char *) &tmp[1]);
      }
      frames[j] = static_cast<float>(tmp[0]) / 32768.0f;
      frames[j+1] = static_cast<float>(tmp[1]) / 32768.0f;
    }   
#else
    for (int j=0; j<result_size; j++) {
      int16_t tmp = data_[current_pos_];
      if (this->swapBytes){
        Stk::swap16((unsigned char *) &tmp);
      }
      frames[j] = static_cast<float>(tmp) / 32768.0f;
      this->current_pos_++;
    }   
#endif

  } else {
      is_finished = true;
  }
  return is_finished;
}

VFS_FD* MemoryFS :: findByName(const char * path){

    // allocate registry on heap
    if (registry==nullptr){
      registry = new VFS_FD*[registry_size];
      for (int j=0;j<registry_size;j++){
        registry[j]=nullptr;
      }
    }

    // find by name
    VFS_FD* result = nullptr;
    if (path!=nullptr){
      for (int idx=0; idx<=registry_last_entry-1; idx++){
          VFS_FD* ptr = registry[idx];
          //STK_LOGW( "matching with %s", ptr->name);
          const char  *nameReg = ptr->name;
          if (nameReg!= nullptr && strcmp(path,nameReg) == 0){
              result = ptr;
              break;
          }
      }
    }
    return result;
}      


VFS_FD * MemoryFS :: registerFile(const char* name, const unsigned char *raw, unsigned int size){
  const char *nameToLog = name==nullptr?"":name;
  STK_LOGI("Register file %s (%d)",nameToLog, size);

  // add only new files
  VFS_FD * result = findByName(name);
  if (result==nullptr) {
      if (registry_last_entry>=registry_size-1){
        STK_LOGE("Too many files: limit is %d",registry_size);
        return nullptr;
      }

      result = new VFS_FD;
      registry[registry_last_entry] = result;
      if (name!=nullptr) {
        result->name = strdup(name);
      }
      // check if we need to make a copy to speed things up
      if (registry_use_ram){
         STK_LOGW( "Using RAM for file %s registed with size: %d", nameToLog, size );
         void* p_data = new uint8_t[size];
         memcpy_P(p_data, raw, size);
         result->data = (const unsigned char*) p_data;
      } else {
        result->data = raw;
      }
      result->size = size;
      result->is_ram = registry_use_ram;
      STK_LOGI( "file %s registed with size: %d", nameToLog, size );
      registry_last_entry++;
  } else {
      STK_LOGI( "file %s already exists",  nameToLog );
  }
  return result;
}

void MemoryFS :: reset() {
      for (int idx=0; idx<=registry_last_entry-1; idx++){
          VFS_FD* ptr = registry[idx];
          if (ptr!=nullptr) delete ptr;
          registry[idx] = nullptr;
      }
      registry_last_entry = 0;

}

} // namespace

#endif