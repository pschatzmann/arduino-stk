
#include "MemoryFS.h"
#ifdef __VFS__

namespace stk {

const char *APP_VFS ="MemoryFS";

// Support for file registration
static VFS_FD *registry = NULL;
static int registry_size=0;
static int registry_last_entry=0;

MemoryFS :: MemoryFS(const unsigned char *raw, unsigned int size, int bytesPerSample, bool swapBytes) {
  if (raw!=NULL){
    fd = registerFile(NULL, raw, size);
    if (fd != NULL){
        fd->is_open = true;
        this->fileSize_ = fd->size / bytesPerSample;
        this->current_pos_ = 0;
    }
  }
}

MemoryFS :: MemoryFS(const char* fileName) {
  this->swapBytes = true;
  open(fileName);
}

bool MemoryFS :: open(const char* fileName, int bytesPerSample) {
    bool result = false;
    int idx = findByName(fileName);
    if (idx>=0){
        fd = &(registry[idx]);
        fd->is_open = true;
        this->fileSize_ = fd->size / bytesPerSample;
        this->current_pos_ = 0;
        result = true;
    }
    return result;
}

VFS_FD* MemoryFS :: getFD(){
  return fd;
}


size_t MemoryFS :: getSize(){
  return fileSize_;
}

bool MemoryFS :: isOpen() {
  return fd==NULL ? false : fd->is_open;
}

void MemoryFS :: close(){
  if (fd)
    fd->is_open = false;
}


bool MemoryFS :: fileRead( StkFrames& frames, unsigned long startFrame, bool doNormalize ){
  //ESP_LOGD(APP_VFS, "x%x, %s: reading %d from start %ld of %ld", __func__, fd->name, frames.size(), startFrame, this->getSize());

  // data is available as array of uint16_t
  bool is_finished = false;

  // calculate max size of next batch
  this->current_pos_ = startFrame;
  int result_size = MIN(frames.size() , this->getSize() - startFrame);

  if (result_size>0) {
    if (frames.size() != result_size){
      frames.resize( result_size, 1 );
    }

    // we get memory access errors when we access the data directly so we copy it
    int16_t tmp[result_size*2];
    memcpy_P(tmp, (fd->data)+(startFrame*2), result_size*2);

    for (int j=0; j<result_size; j++) {
      if (this->swapBytes)
        Stk::swap16((unsigned char *) &(tmp[j]));
      frames[j] = tmp[j] / 32768.0;
      this->current_pos_++;
    }
  } else {
      is_finished = true;
  }
  return is_finished;
}

int MemoryFS :: findByName(const char * path){
    int result = -1;
    if (registry!=NULL && path!=NULL){
      for (int fd=0; fd<registry_last_entry; fd++){
          //ESP_LOGD(APP_VFS, "x%x, matching with %s", __func__, registry[fd].name);
          if (strcmp(path, registry[fd].name)==0){
              result = fd;
              break;
          }
      }
    }
    return result;;
}      

VFS_FD * MemoryFS :: registerFile(const char* name, const unsigned char *raw, unsigned int size){
  const char *nameToLog = name==NULL?"":name;
  // extend registry if it is not big enough
  //ESP_LOGD(APP_VFS, "x%x, file %s registration requested with size: %d ", __func__, nameToLog, size );
  VFS_FD * result = NULL;
  if (registry==NULL || registry_last_entry >= registry_size){
      VFS_FD *tmp = registry;
      registry_size+=VFS_INC_SIZE;
      //ESP_LOGD(APP_VFS, "x%x, Increasing registry to size: %d ", __func__, registry_size);
      registry = new VFS_FD[registry_size];
      if (tmp!=NULL) {
          std::memcpy(registry, tmp, (registry_last_entry-1)*sizeof(VFS_FD) );
          delete[] tmp;
      }
  }
  
  // add only new files
  int idx = findByName(name);
  if (idx==-1) {
      if (name!=NULL) {
        registry[registry_last_entry].name = strdup(name);
      }
      registry[registry_last_entry].data = raw;
      registry[registry_last_entry].size = size;
      registry[registry_last_entry].is_open = false;
      result = &(registry[registry_last_entry]);
      //ESP_LOGD(APP_VFS, "x%x, file %s registed with size: %d ", __func__, nameToLog, size );
      registry_last_entry++;
  } else {
      //ESP_LOGD(APP_VFS, "x%x, file %s already exists", __func__, nameToLog );
  }
  return result;
}

} // namespace

#endif