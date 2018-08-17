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
 * Private declarations
 ******************************************************************************/

static void LoRa_logDeviceInfo();
static void LoRa_logTransmissionResult(uint8_t result);


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

bool LoRa_setup()
{
  LOGS("starting...");

  SERIAL_LORA.begin(LoRaBee.getDefaultBaudRate());

  #if USE_LOGGER
  LoRaBee.setDiag(SERIAL_DEBUG);
  #endif

  bool b = LoRaBee.init(SERIAL_LORA, LORA_RESET);

  if (b) {
    #if USE_LOGGER
    LoRa_logDeviceInfo();
    #endif

    LoRa_sleep();
  }

  LOG_SETUP_RESULT_TEXT(b);

  return b;
}


/*******************************************************************************
 * Public
 ******************************************************************************/

bool LoRa_initOTAA()
{
  LOGS("init OTAA...");

  uint8_t eui[8];

  if (LoRaBee.getHWEUI(eui, sizeof(eui)) != 8) {
    LOGS("LoRa is down");
    return false;
  }

  bool b = false;

  for (uint8_t i = 0; i < JOINNETWORK_MAX_RETRIES; i++) {
    LOG(VS("attempt "), VUI8(i), VS("..."));

    if (LoRaBee.initOTA(eui, APP_EUI, APP_KEY, false)) { b = true; break; }
  }

  LOG(b ? VS("accepted") : VS("denied"));

  // TODO: following is for debug only(!), it will be removed later
  // TODO: instead of this => send data from sensors every 5 minutes
  // if (b) {
  //   LOGS("sending test...");
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
//     LOG(VS("attempt "), VUI8(i), VS("..."));
//
//     uint8_t result = LoRaBee.send(1, buffer, size);
//
//     #if USE_LOGGER
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

  LOGS("sleep mode on");
}

void LoRa_wakeUp()
{
  LoRaBee.wakeUp();

  LOGS("active mode on");
}


/*******************************************************************************
 * Private
 ******************************************************************************/

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

#endif // USE_LORA
