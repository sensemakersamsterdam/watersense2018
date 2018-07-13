#include "LoRa.h"

#if USE_LORA

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

#define REPEAT_JOINNETWORK 5
#define SERIAL_DEBUG       SerialUSB
#define SERIAL_LORA        Serial1


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

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

  if (b) { LoRa_sendTest(); }
}


/*******************************************************************************
 * Public
 ******************************************************************************/

void LoRa_sendTest()
{
  static uint8_t testPayload[] = { 0x01, 0x10, 0x02, 0x22 };

  #if USE_LOGGER_LORA
  LOGS("sending test...");
  #endif

  uint8_t result = LoRaBee.send(1, testPayload, sizeof(testPayload));

  #if USE_LOGGER_LORA
  LoRa_logTransmissionResult(result);
  #endif

  // switch (result)
  // {
  //   case NoError:
  //      LOGS("Successful transmission.");
  //      break;
  //    case NoResponse:
  //      LOGS("There was no response from the device.");
  //      //TODO: setupLoRa();
  //      break;
  //    case Timeout:
  //      LOGS("Connection timed-out. Check your serial connection to the device! Sleeping for 20sec.");
  //      //TODO: delay(20000);
  //      break;
  //    case PayloadSizeError:
  //      LOGS("The size of the payload is greater than allowed. Transmission failed!");
  //      break;
  //    case InternalError:
  //      LOGS("Oh No! This shouldn't happen. Something is really wrong! Try restarting the device! The network connection will reset.");
  //      //TODO: setupLoRa();
  //      break;
  //    case Busy:
  //      LOGS("The device is busy. Sleeping for 10 extra seconds.");
  //      //TODO: delay(10000);
  //      break;
  //    case NetworkFatalError:
  //      LOGS("There is a non-recoverable error with the network connection. You should re-connect. The network connection will reset.");
  //      //TODO: setupLoRa();
  //      break;
  //    case NotConnected:
  //      LOGS("The device is not connected to the network. Please connect to the network before attempting to send data. The network connection will reset.");
  //      //TODO: setupLoRa();
  //      break;
  //    case NoAcknowledgment:
  //      LOGS("There was no acknowledgment sent back!");
  //      // When you this message you are probaly out of range of the network.
  //      break;
  //    default:
  //      break;
  // }

  #if USE_LOGGER_LORA
  LOGS("done");
  #endif
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

  for (uint8_t i = 0; i < REPEAT_JOINNETWORK; i++) {
    #if USE_LOGGER_LORA
    LOG(VS("initOTA (attempt "), VUI8(i), VS(")..."));
    #endif

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
    case NoError:
       LOGS("Successful transmission.");
       break;
     case NoResponse:
       LOGS("There was no response from the device.");
       //TODO: setupLoRa();
       break;
     case Timeout:
       LOGS("Connection timed-out. Check your serial connection to the device! Sleeping for 20sec.");
       //TODO: delay(20000);
       break;
     case PayloadSizeError:
       LOGS("The size of the payload is greater than allowed. Transmission failed!");
       break;
     case InternalError:
       LOGS("Oh No! This shouldn't happen. Something is really wrong! Try restarting the device! The network connection will reset.");
       //TODO: setupLoRa();
       break;
     case Busy:
       LOGS("The device is busy. Sleeping for 10 extra seconds.");
       //TODO: delay(10000);
       break;
     case NetworkFatalError:
       LOGS("There is a non-recoverable error with the network connection. You should re-connect. The network connection will reset.");
       //TODO: setupLoRa();
       break;
     case NotConnected:
       LOGS("The device is not connected to the network. Please connect to the network before attempting to send data. The network connection will reset.");
       //TODO: setupLoRa();
       break;
     case NoAcknowledgment:
       LOGS("There was no acknowledgment sent back!");
       // When you this message you are probaly out of range of the network.
       break;
     default:
       break;
  }
}
#endif


#endif // USE_LORA
