#include "LoRa.h"

#if USE_LORA

#include <Sodaq_RN2483.h>


/*******************************************************************************
 * Configuration
 ******************************************************************************/

#include "../LoRaConfig.h"
/*
static const uint8_t APP_EUI[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static const uint8_t APP_KEY[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static const uint8_t DEV_EUI[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
*/


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define SERIAL_LORA Serial1


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void LoRa_setup()
{
  #if USE_LOGGER_LORA
  LOGS("starting...");
  #endif

  SERIAL_LORA.begin(LoRaBee.getDefaultBaudRate());

  bool b = LoRaBee.initOTA(SERIAL_LORA, DEV_EUI, APP_EUI, APP_KEY, false);

  #if USE_LOGGER_LORA
  if (b) {
    LOGS("started");
  } else {
    LOGS("failed");
  }
  #endif

  #if USE_LOGGER_LORA_DEVICEINFO
  LoRa_logDeviceInfo();
  #endif

  LoRa_sendTest();
}


/*******************************************************************************
 * Public
 ******************************************************************************/

void LoRa_sendTest()
{
  static uint8_t testPayload[] = { 0x01, 0x10, 0x02, 0x20 };

  LOGS("sending test...");

  switch (LoRaBee.send(1, testPayload, sizeof(testPayload)))
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

  LOGS("done");
}


/*******************************************************************************
 * Private
 ******************************************************************************/

#if USE_LOGGER_LORA_DEVICEINFO
static void LoRa_logDeviceInfo()
{
  char    version[32];
  uint8_t eui[8];

  LOGS("receiving...");

  BaseType_t i = LoRaBee.getVersion(version, sizeof(version));
  if (i > 1) {
    LOG(VS("version: "), VSZ(version));
  } else {
    LOGS("version: N/A");
  }

  i = LoRaBee.getHWEUI(eui, sizeof(eui));
  if (i > 0) {
    LOG(VS("device EUI: "), VUI8AH02(eui, i));
  } else {
    LOGS("device EUI: N/A");
  }

  LOGS("done");
}
#endif

#endif // USE_LORA
