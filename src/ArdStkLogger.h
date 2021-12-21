#pragma once

#define STK_LOGD(fmt,...) stk_log(StkDebug, fmt, ##__VA_ARGS__)
#define STK_LOGI(fmt,...) stk_log(StkInfo, fmt, ##__VA_ARGS__)
#define STK_LOGW(fmt,...) stk_log(StkWarning, fmt, ##__VA_ARGS__)
#define STK_LOGE(fmt,...) stk_log(StkError, fmt, ##__VA_ARGS__)

namespace stk {

enum StkLogLevel_t {StkDebug, StkInfo, StkWarning, StkError};

extern StkLogLevel_t StkLogLevel;

void stk_log(StkLogLevel_t level, const char* fmr,...);

}

