#include "ArdConfig.h"

#ifdef STK_ERROR_MSG_LEN
char stk_error_msg[STK_ERROR_MSG_LEN];
#endif


#ifdef ARDUINO_ARCH_RP2040
extern "C" void __sync_synchronize() {
}
#endif