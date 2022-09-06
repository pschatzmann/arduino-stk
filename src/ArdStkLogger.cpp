#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "ArdStkLogger.h"


namespace stk {

// global actual loggin level for application
StkLogLevel_t StkLogLevel;

// Error level as string
const char* midi_log_msg[] = {"Debug", "Info", "Warning", "Error"};

/// Print log message
void stk_log(StkLogLevel_t level, const char* fmt ...) {
  if (StkLogLevel <= level) { // AUDIOKIT_LOG_LEVEL = Debug
    char log_buffer[200];
    strcpy(log_buffer,midi_log_msg[level] );
    strcat(log_buffer,":     ");
    va_list arg;
    va_start(arg, fmt);
    vsprintf(log_buffer+9, fmt, arg);
    va_end(arg);
    strcat(log_buffer, "\n");
    printf("%s",log_buffer);
  }
}

}