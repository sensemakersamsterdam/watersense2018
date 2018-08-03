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

bool LoRa_setup()
{
  #if USE_BOARD_LED
  Board_setLed(0b001);
  #endif

  #if USE_LOGGER_LORA
  LOGS("starting...");
  #endif

  SERIAL_LORA.begin(LoRaBee.getDefaultBaudRate());

  #if USE_LOGGER_LORA
  LoRaBee.setDiag(SERIAL_DEBUG);
  #endif

  bool b = LoRaBee.init(SERIAL_LORA, LORA_RESET);

  if (b) {
    #if USE_LOGGER_LORA_DEVICEINFO
    LoRa_logDeviceInfo();
    #endif

    LoRa_sleep();
  }

  #if USE_LOGGER_LORA
  if (b) { LOGS("started"); } else { LOGS("failed"); }
  #endif

  return b;
}


/*******************************************************************************
 * Public
 ******************************************************************************/

bool LoRa_initOTAA()
{
  #if USE_LOGGER_LORA
  LOGS("init OTAA...");
  #endif

  uint8_t eui[8];

  if (LoRaBee.getHWEUI(eui, sizeof(eui)) != 8) {
    #if USE_LOGGER_LORA
    LOGS("LoRa is down");
    #endif
    return false;
  }

  bool b = false;

  for (uint8_t i = 0; i < JOINNETWORK_MAX_RETRIES; i++) {
    #if USE_LOGGER_LORA
    LOG(VS("initOTA (attempt "), VUI8(i), VS(")..."));
    #endif

    if (LoRaBee.initOTA(eui, APP_EUI, APP_KEY, false)) { b = true; break; }
  }

  #if USE_LOGGER_LORA
  if (b) { LOGS("accepted"); } else { LOGS("denied"); }
  #endif

  // TODO: following is for debug only(!), it will be removed later
  // TODO: instead of this => send data from sensors every 5 minutes
  // if (b) {
  //   #if USE_LOGGER_LORA
  //   LOGS("sending test...");
  //   #endif
  //
  //   static uint8_t testPayload[] = { 0x01, 0x10, 0x02, 0x22 };
  //
  //   LoRa_send(testPayload, sizeof(testPayload));
  // }

  return b;
}

// bool LoRa_send(const uint8_t *buffer, uint8_t size)
// {
//   for (uint8_t i = 0; i < SEND_MAX_RETRIES; i++) {
//     #if USE_LOGGER_LORA
//     LOG(VS("send (attempt "), VUI8(i), VS(")..."));
//     #endif
//
//     uint8_t result = LoRaBee.send(1, buffer, size);
//
//     #if USE_LOGGER_LORA
//     LoRa_logTransmissionResult(result);
//     #endif
//
//     if (result == NoError) { return true; }
//
//     if (result == Busy || result == Timeout) {
//       vTaskDelay(pdMS_TO_TICKS(10000));
//       continue;
//     }
//
//     if (result == NoResponse || result == InternalError || result == NetworkFatalError || result == NotConnected) {
//       if (!LoRa_initOTAA()) { return false; }
//       continue;
//     }
//
//     if (result == PayloadSizeError) { return false; }
//   }
//
//   return false;
// }

void LoRa_sleep()
{
  LoRaBee.sleep();

  #if USE_LOGGER_LORA
  LOGS("sleep mode on");
  #endif

  #if USE_BOARD_LED
  Board_setLed(0b000);
  #endif
}

void LoRa_wakeUp()
{
  LoRaBee.wakeUp();

  #if USE_LOGGER_LORA
  LOGS("active mode on");
  #endif

  #if USE_BOARD_LED
  Board_setLed(0b001);
  #endif
}


/*******************************************************************************
 * Private
 ******************************************************************************/

#if USE_LOGGER_LORA_DEVICEINFO
static void LoRa_logDeviceInfo()
{
  union {
    char version[32];
    uint8_t eui[8];
  } u;

  uint8_t i = LoRaBee.getVersion(u.version, sizeof(u.version));
  if (i > 1) { LOG(VS("version: "), VSZ(u.version)); } else { LOGS("version: N/A"); }

  i = LoRaBee.getHWEUI(u.eui, sizeof(u.eui));
  if (i > 0) { LOG(VS("device EUI: "), VUI8AH02(u.eui, i)); } else { LOGS("device EUI: N/A"); }
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
