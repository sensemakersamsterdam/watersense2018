#include "RTC.h"

#if USE_RTC

#include <RTCZero.h>


/*******************************************************************************
 * State
 ******************************************************************************/

static RTCZero rtc;


/*******************************************************************************
 * Public
 ******************************************************************************/

uint32_t RTC_setAlarm(uint32_t ms)
{
  ms /= 1000;

  rtc.begin(false);
  rtc.setTime(0, 0, 0);
  rtc.setDate(1, 1, 18);
  rtc.setAlarmTime(ms / 60 / 60, ms / 60 % 60, ms % 60);
  rtc.enableAlarm(rtc.MATCH_HHMMSS);

  return ms * 1000;
}

#endif // USE_RTC
