#include "../Common.h"

#if USE_RTC && !defined(RTC_H)
#define RTC_H


/*******************************************************************************
 * Public
 ******************************************************************************/

uint32_t RTC_setAlarm(uint32_t ms);

#endif // USE_RTC && defined(RTC_H)
