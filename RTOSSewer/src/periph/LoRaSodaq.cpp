#include "LoRaSodaq.h"

#if USE_LORA_SODAQ

#include <Sodaq_RN2483.h>


/*******************************************************************************
 * Configuration
 ******************************************************************************/

#include "../LoRaConfig.h"

/*
static const uint8_t APP_EUI[]  = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static const uint8_t APP_KEY[]  = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
*/


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define JOINNETWORK_MAX_RETRIES 3
#define SEND_MAX_RETRIES        3

#define SERIAL_DEBUG SerialUSB
#define SERIAL_LORA  Serial1


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

// TODO - LoRaBee.sleep()
// TODO - LoRaBee.awake()

void LoRa_setup()
{
  #if USE_LOGGER_LORA
  LOGS("starting...");
  #endif

  bool b = LoRa_initOTA();

  #if USE_LOGGER_LORA
  if (b) {
    LOGS("started");
  } else {
    LOGS("failed");
  }
  #endif

  // TODO: following is for debug only(!), it will be removed later
  // TODO: instead of this => send data from sensors every 5 minutes
  if (b) {
    #if USE_LOGGER_LORA
    LOGS("sending test...");
    #endif

    static uint8_t testPayload[] = { 0x01, 0x10, 0x02, 0x22 };

    LoRa_send(testPayload, sizeof(testPayload));
  }
}


/*******************************************************************************
 * Public
 ******************************************************************************/

bool LoRa_send(const uint8_t *buffer, uint8_t size)
{
  for (uint8_t i = 0; i < SEND_MAX_RETRIES; i++) {
    #if USE_LOGGER_LORA
    LOG(VS("send (attempt "), VUI8(i), VS(")..."));
    #endif

    uint8_t result = LoRaBee.send(1, buffer, size);

    #if USE_LOGGER_LORA
    LoRa_logTransmissionResult(result);
    #endif

    if (result == NoError) { return true; }

    if (result == Busy || result == Timeout) {
      vTaskDelay(pdMS_TO_TICKS(10000));
      continue;
    }

    if (result == NoResponse || result == InternalError || result == NetworkFatalError || result == NotConnected) {
      if (!LoRa_initOTA()) { return false; }
      continue;
    }

    if (result == PayloadSizeError) { return false; }
  }

  return false;
}


/*******************************************************************************
 * Private
 ******************************************************************************/

static bool LoRa_initOTA()
{
  SERIAL_LORA.begin(LoRaBee.getDefaultBaudRate());

  #if USE_LOGGER_LORA
  LoRaBee.setDiag(SERIAL_DEBUG);
  #endif

  if (!LoRaBee.init(SERIAL_LORA, LORA_RESET)) { return false; }

  #if USE_LOGGER_LORA
  LOGS("init: OK");
  #endif

  uint8_t eui[8];

  if (LoRaBee.getHWEUI(eui, sizeof(eui)) != 8) { return false; }

  #if USE_LOGGER_LORA
  LOGS("getHWEUI: OK");
  #endif

  #if USE_LOGGER_LORA_DEVICEINFO
  LoRa_logDeviceInfo();
  #endif

  for (uint8_t i = 0; i < JOINNETWORK_MAX_RETRIES; i++) {
    #if USE_LOGGER_LORA
    LOG(VS("initOTA (attempt "), VUI8(i), VS(")..."));
    #endif

    //LoRaBee.setFsbChannels(1);
    //LoRaBee.setSpreadingFactor(7);

    if (LoRaBee.initOTA(eui, APP_EUI, APP_KEY, false)) { return true; }
  }

  return false;
}

#if USE_LOGGER_LORA_DEVICEINFO
static void LoRa_logDeviceInfo()
{
  union {
    char version[32];
    uint8_t eui[8];
  } u;

  uint8_t i = LoRaBee.getVersion(u.version, sizeof(u.version));
  if (i > 1) {
    LOG(VS("version: "), VSZ(u.version));
  } else {
    LOGS("version: N/A");
  }

  i = LoRaBee.getHWEUI(u.eui, sizeof(u.eui));
  if (i > 0) {
    LOG(VS("device EUI: "), VUI8AH02(u.eui, i));
  } else {
    LOGS("device EUI: N/A");
  }
}
#endif

#if USE_LOGGER_LORA
static void LoRa_logTransmissionResult(uint8_t result)
{
  switch (result)
  {
    case NoError:           LOGS("NoError");           break;
    case NoResponse:        LOGS("NoResponse");        break;
    case Timeout:           LOGS("Timeout");           break;
    case PayloadSizeError:  LOGS("PayloadSizeError");  break;
    case InternalError:     LOGS("InternalError");     break;
    case Busy:              LOGS("Busy");              break;
    case NetworkFatalError: LOGS("NetworkFatalError"); break;
    case NotConnected:      LOGS("NotConnected");      break;
    case NoAcknowledgment:  LOGS("NoAcknowledgment");  break;
    default:                LOGS("Unknown");           break; // should be impossible
  }
}
#endif

#endif // USE_LORA
