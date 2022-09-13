#include "ArdConfig.h"

#ifdef STK_ERROR_MSG_LEN
char stk_error_msg[STK_ERROR_MSG_LEN];
#endif


#ifdef ARDUINO_ARCH_RP2040
extern "C" void __sync_synchronize() {}
#endif

// not sure why this is needed
#if defined(__USE_STM32_HACK__)
extern "C" int _open() {return -1;}
extern "C" int _gettimeofday() {return -1;}
#endif