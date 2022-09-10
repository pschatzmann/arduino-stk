
#include "MemoryFS.h"
#include "ArdStkLogger.h"

#ifdef __RAW_ARRAYS__

namespace stk {

//! Simulated File Descriptor of registered virtual "Memory" files
struct VFS_FD {
  const char *name="";
  const unsigned char *data=nullptr;
  long size=0; // in bytes
};

const char *APP_VFS ="MemoryFS";

// Support for file registration
static const int registry_size=30;
VFS_FD **registry = nullptr;
int registry_last_entry=0;

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

// VFS_FD* MemoryFS :: getFD(){
//   return (VFS_FD*) p_fd;
// }


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

  // calculate max size of next batch
  this->current_pos_ = startFrame;
  int result_size = MIN(frames.size() , this->getSize() - startFrame);

  if (result_size>0) {
    if (frames.size() != result_size){
      frames.resize( result_size, 1 );
    }

    for (int j=0; j<result_size; j++) {
      int16_t tmp = data_[current_pos_];
      if (this->swapBytes){
        Stk::swap16((unsigned char *) &tmp);
      }
      frames[j] = static_cast<float>(tmp) / 32768.0;
      this->current_pos_++;
    }   

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
  STK_LOGW("Register file %s (%d)",nameToLog, size);

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
      assert(raw!=nullptr);
      result->data = raw;
      result->size = size;
      STK_LOGW( "file %s registed with size: %d", nameToLog, size );
      registry_last_entry++;
  } else {
      STK_LOGW( "file %s already exists",  nameToLog );
  }
  return result;
}

} // namespace

#endif