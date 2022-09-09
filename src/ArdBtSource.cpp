#include "ArdBtSource.h"
#ifdef ARDBTSOURCE_H

namespace stk {

char * APP_ArdBtSource = "ArdBtSource";

// Used to make the voicer available to the callback
Voicer *pArdBtSourceVoicer;

//! Default Constructor
ArdBtSource :: ArdBtSource( ) {
    a2dp_source = new BluetoothA2DPSource();
}

//! Destructor
ArdBtSource :: ~ArdBtSource()
{
  if (a2dp_source!=NULL)
    delete a2dp_source;
}

// logic copied from WvOut;  
StkFloat&  clipTest( StkFloat& sample )
{
    if ( sample > 1.0 ) {
        sample = 1.0;
    }
    else if ( sample < -1.0 ) {
        sample = -1.0;
    }
    return sample;
}

bool hasSoundData;
long logTime;

//! Callback: we just get the ticks from the pVoicer converted to Int16
int32_t get_stk_data(uint8_t *data, int32_t len) {

    if (len < 0 || data == NULL) {
        return 0;
    }
  
    int dataBytes = sizeof(SINT16);
    int samples = len / dataBytes / 2;
    SINT16 *ptr = (SINT16 *) data;

    StkFloat sample;
    for ( unsigned long k=0; k<samples; k++ ) {
          sample = pArdBtSourceVoicer->tick() ;
          clipTest( sample );
          // Channel 1
          *ptr = (SINT16) (sample * 32767.0);
          #ifdef __LITTLE_ENDIAN__
            swap16 ((unsigned char *)ptr);
          #endif

          ptr++;
          // Channel 2
          *ptr = *(ptr-1);
          if (*ptr!=0){
            hasSoundData = true;
          }
          ptr++;
    }

    if (logTime < millis()){
      logTime = millis()+5000;
      hasSoundData = false;
    }

    return len;
}

void ArdBtSource :: start(char* name, Voicer &voicer) {
  this->pVoicer = &voicer;
  pArdBtSourceVoicer = &voicer;

  a2dp_source->start_raw(name, get_stk_data);
}

bool ArdBtSource :: isConnected(){
  return a2dp_source->is_connected();
}

void ArdBtSource :: setNVSInit(bool doInit){
  a2dp_source->set_nvs_init(doInit);
}

void ArdBtSource ::  setResetBLE(bool doInit){
  a2dp_source->set_reset_ble(doInit);
}

} // stk namespace

#endif
