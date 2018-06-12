#include "../Common.h"

#if USE_LORA && !defined(LORA_H)
#define LORA_H


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void LORA_setup();


/*******************************************************************************
 * Private
 ******************************************************************************/

#if USE_LOGGER_LORA_DEVICEINFO
static void LORA_logDeviceInfo();
#endif

#endif // USE_LORA && defined(LORA_H)
