
#include "Arduino.h"

//Generic Vars
int waitfor = 5000;       // wait in loop
bool SODAQ = true;
bool TOF = true;
bool AquaPlumb = true;
bool Turbidity = false;
void systemSleep();
volatile bool minuteFlag;

// Sodaq Vars
/*
 * Compatible with:
 * SODAQ MBILI
 * SODAQ Autonomo
 * SODAQ ONE
 * SODAQ ONE BETA
 * SODAQ EXPLORER
 */
#include "RTCTimer.h"
#include "RTCZero.h"
#include "MyTime.h"
#include "ReportDataRecord.h"
#include <Sodaq_RN2483.h>
#include "Sodaq_wdt.h"
#include "ublox.h"
#include "Sodaq_LIS3DE.h"
#include "Config.h"

#define debugSerial SERIAL_PORT_MONITOR
#define DEBUG_STREAM SerialUSB

#define consolePrint(x) CONSOLE_STREAM.print(x)
#define consolePrintln(x) CONSOLE_STREAM.println(x)

#define debugPrint(x) if (params.getIsDebugOn()) { DEBUG_STREAM.print(x); }
#define debugPrintln(x) if (params.getIsDebugOn()) { DEBUG_STREAM.println(x); }

void delegateNavPvt(NavigationPositionVelocityTimeSolution* NavPvt);
#define GPS_TIME_VALIDITY 0b00000011 // date and time (but not fully resolved)
#define GPS_FIX_FLAGS 0b00000001 // just gnssFixOK

#define MAX_RTC_EPOCH_OFFSET 25
void setNow(uint32_t now);
static int64_t rtcEpochDelta; // set in setNow() and used in getGpsFixAndTransmit() for correcting time in loop

#if defined(ARDUINO_AVR_SODAQ_MBILI)
#define loraSerial Serial1
#define LORA_STREAM Serial2

#define BEE_VCC 20

#elif defined(ARDUINO_SODAQ_AUTONOMO) || defined(ARDUINO_SODAQ_ONE) || defined(ARDUINO_SODAQ_ONE_BETA)
#define loraSerial Serial1
#define LORA_STREAM Serial1

#elif defined(ARDUINO_SODAQ_EXPLORER)
#define loraSerial Serial2
#define LORA_STREAM Serial2

#else
// please select a sodaq board
debugSerial.println("Please select a sodaq board!!");
#endif

// ABP
const uint8_t devAddr[4] = { 0x00, 0x00, 0x00, 0x00 };
const uint8_t appSKey[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const uint8_t nwkSKey[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

// If you want to add your own keys without replacing the default keys, please place them in otaakeys.h in the same folder as this ino file
#include "otaakeys.h"
// OTAA
//uint8_t DevEUI[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//uint8_t AppEUI[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//uint8_t AppKey[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

RTCZero rtc;
RTCTimer timer;
UBlox ublox;
Time time;
Sodaq_LIS3DE accelerometer;

ReportDataRecord pendingReportDataRecord;
bool isPendingReportDataRecordNew; // this is set to true only when pendingReportDataRecord is written by the delegate
static bool isRtcInitialized;


static bool isGpsInitialized;


// AquaPlumb Variables
float maxvoltage = 3.12;
float maxheight = 22.0;

// TOF Sensor VL53L0X Settings and Variables
#include "Adafruit_VL53L0X.h"
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

//Turpidity Sensor
int sensorPin = A0;      // select the input pin for the potentiometer
int sensorValue = 0;     // variable to store the value coming from the sensor
const int nsamples = 25; // number of samples
int isample = 0;         // sample counter
float vsample[nsamples];
float alert =0.2;        // alert level for standard deviation



void setupLoRaABP(){  
  if (LoRaBee.initABP(loraSerial, devAddr, appSKey, nwkSKey, false))
  {
    debugSerial.println("Communication to LoRaBEE successful.");
  }
  else
  {
    debugSerial.println("Communication to LoRaBEE failed!");
  }
}

void setupLoRaOTAA(){
  if (LoRaBee.initOTA(loraSerial, DevEUI, AppEUI, AppKey, false))
  {
    debugSerial.println("Communication to LoRaBEE successful.");
  }
  else
  {
    debugSerial.println("OTAA Setup failed!");
  }
}

void setup() {
  if ( SODAQ == true ) {
  //Power up the LoRaBEE
    #if defined(ARDUINO_AVR_SODAQ_MBILI) || defined(ARDUINO_SODAQ_AUTONOMO)
    pinMode(BEE_VCC, OUTPUT);
    digitalWrite(BEE_VCC, HIGH);
    #endif
    delay(3000);

    while ((!SerialUSB) && (millis() < 10000)){
      // Wait 10 seconds for the Serial Monitor
    }

    //Set baud rate
    debugSerial.begin(57600);
    loraSerial.begin(LoRaBee.getDefaultBaudRate());

  // Debug output from LoRaBee
  // LoRaBee.setDiag(debugSerial); // optional

  //connect to the LoRa Network
    setupLoRa();
  }
  // Setup VL53L0X
  if ( TOF == true ){
    if (!lox.begin()) {
     Serial.println(F("Failed to boot VL53L0X"));
      while(1);
    }
  }
  
  
}

void setupLoRa(){
  // ABP
//  setupLoRaABP();
  // OTAA
  setupLoRaOTAA();
  LoRaBee.setSpreadingFactor(9);
}

void sendPacketViaSODAQ(String packet){
  switch (LoRaBee.send(1, (uint8_t*)packet.c_str(), packet.length()))
    {
    case NoError:
      debugSerial.println("Successful transmission.");
      break;
    case NoResponse:
      debugSerial.println("There was no response from the device.");
      setupLoRa();
      break;
    case Timeout:
      debugSerial.println("Connection timed-out. Check your serial connection to the device! Sleeping for 20sec.");
      delay(20000);
      break;
    case PayloadSizeError:
      debugSerial.println("The size of the payload is greater than allowed. Transmission failed!");
      break;
    case InternalError:
      debugSerial.println("Oh No! This shouldn't happen. Something is really wrong! Try restarting the device!\r\nThe network connection will reset.");
      setupLoRa();
      break;
    case Busy:
      debugSerial.println("The device is busy. Sleeping for 10 extra seconds.");
      delay(10000);
      break;
    case NetworkFatalError:
      debugSerial.println("There is a non-recoverable error with the network connection. You should re-connect.\r\nThe network connection will reset.");
      setupLoRa();
      break;
    case NotConnected:
      debugSerial.println("The device is not connected to the network. Please connect to the network before attempting to send data.\r\nThe network connection will reset.");
      setupLoRa();
      break;
    case NoAcknowledgment:
      debugSerial.println("There was no acknowledgment sent back!");
      // When you this message you are probaly out of range of the network.
      break;
    default:
      break;
    }
}

void loop() {
  // put your main code here, to run repeatedly:
  String packet = "";
  sodaq_wdt_flag = false;

  // AquaPlumb
  if ( AquaPlumb == true ) {
  // read the input on analog pin 7:
    int sensorValue = analogRead(A7);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
    float voltage = sensorValue * (5.0 / 1023.0);
    float height = (voltage / maxvoltage) * maxheight;
    packet = String("AquaPlumb height" + String(height) );
  }
  if ( TOF == true ) {
  // VL53L0X
    VL53L0X_RangingMeasurementData_t measure;
    Serial.print("Reading a measurement... ");
    lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

    if (measure.RangeStatus != 4) {  // phase failures have incorrect data
      Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
      String packet = String(packet+"VL53L0X Distance: " + measure.RangeMilliMeter);
    } else {
      Serial.println(" out of range ");
      packet = String(packet + "VL53L0X Distance: out of range");
    }
  }
  // Turbidity
  if ( Turbidity == true  ) {
    isample++;
    if (isample==nsamples) {isample=0;}
    sensorValue = analogRead(sensorPin);
  
    vsample[isample] = sensorValue * (5.0 / 1023.0);

    float mn = average(vsample, nsamples);
    float st = stdev(vsample, nsamples);

    if (st > alert) {
      packet = String(packet + "Turpidity not OK:" + String(st) );
    } else {
      packet = String(packet + "Turpidity OK:" + String(st) );
    }
  
  }
  if ( SODAQ == true ) {
    sendPacketViaSODAQ(packet);
  } else {
    Serial.println(packet);
  }
  delay(waitfor);
}


/*********************************
 * Gemiddelde van een array floats
 */
float average(float * v, int n) {
  float sum = 0.0;
  for (int is=0;is<n;is++) {sum+=v[is];}
  sum = sum / n;
  return sum;
}

/******************************************
 * Standaard afwijking van een array floats
 */
float stdev(float * v, int n) {
  float mean  = average(v, n);
  float stdev = 0.0;
  for (int is=0;is<n;is++) {stdev+=(v[is]-mean)*(v[is]-mean);}
  stdev = stdev / n;
  stdev = sqrt(stdev);
  return stdev;
}

/**
 * Powers down all devices and puts the system to deep sleep.
 */
void systemSleep()
{
    LORA_STREAM.flush();

    setGpsActive(false); // explicitly disable after resetting the pins

    // go to sleep, unless USB is used for debugging
    if (!params.getIsDebugOn() || ((long)&DEBUG_STREAM != (long)&SerialUSB)) {
        noInterrupts();
        if (!(sodaq_wdt_flag || minuteFlag)) {
            interrupts();

            __WFI(); // SAMD sleep
        }
        interrupts();
    }
}

/**
 * Turns the GPS on or off.
 */
void setGpsActive(bool on)
{
    sodaq_wdt_reset();

    if (on) {
        ublox.enable();
        ublox.flush();

        sodaq_wdt_safe_delay(80);

        PortConfigurationDDC pcd;

        uint8_t maxRetries = 6;
        int8_t retriesLeft;

        retriesLeft = maxRetries;
        while (!ublox.getPortConfigurationDDC(&pcd) && (retriesLeft-- > 0)) {
            debugPrintln("Retrying ublox.getPortConfigurationDDC(&pcd)...");
            sodaq_wdt_safe_delay(15);
        }
        if (retriesLeft == -1) {
            debugPrintln("ublox.getPortConfigurationDDC(&pcd) failed!");

            return;
        }

        pcd.outProtoMask = 1; // Disable NMEA
        retriesLeft = maxRetries;
        while (!ublox.setPortConfigurationDDC(&pcd) && (retriesLeft-- > 0)) {
            debugPrintln("Retrying ublox.setPortConfigurationDDC(&pcd)...");
            sodaq_wdt_safe_delay(15);
        }
        if (retriesLeft == -1) {
            debugPrintln("ublox.setPortConfigurationDDC(&pcd) failed!");

            return;
        }

        ublox.CfgMsg(UBX_NAV_PVT, 1); // Navigation Position Velocity TimeSolution
        ublox.funcNavPvt = delegateNavPvt;
    }
    else {
        ublox.disable();
    }
}

/**
 * Turns the LoRa module on or off (wake up or sleep)
 */
void setLoraActive(bool on)
{
    sodaq_wdt_reset();

    if (on) {
        LoRaBee.wakeUp();
        
    }
    else {
        LoRaBee.sleep();
    }

    sodaq_wdt_safe_delay(10);
}


/**
 *  Checks validity of data, adds valid points to the points list, syncs the RTC
 */
void delegateNavPvt(NavigationPositionVelocityTimeSolution* NavPvt)
{
    sodaq_wdt_reset();

    if (!isGpsInitialized) {
        debugPrintln("delegateNavPvt exiting because GPS is not initialized.");

        return;
    }

    // note: db_printf gets enabled/disabled according to the "DEBUG" define (ublox.cpp)
    ublox.db_printf("%4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d.%d valid=%2.2x lat=%d lon=%d sats=%d fixType=%2.2x\r\n",
        NavPvt->year, NavPvt->month, NavPvt->day,
        NavPvt->hour, NavPvt->minute, NavPvt->seconds, NavPvt->nano, NavPvt->valid,
        NavPvt->lat, NavPvt->lon, NavPvt->numSV, NavPvt->fixType);

    // sync the RTC time
    if ((NavPvt->valid & GPS_TIME_VALIDITY) == GPS_TIME_VALIDITY) {
        uint32_t epoch = time.mktime(NavPvt->year, NavPvt->month, NavPvt->day, NavPvt->hour, NavPvt->minute, NavPvt->seconds);

        // check if there is an actual offset before setting the RTC
        if (abs((int64_t)getNow() - (int64_t)epoch) > MAX_RTC_EPOCH_OFFSET) {
            setNow(epoch);
        }
    }

    // check that the fix is OK and that it is a 3d fix or GNSS + dead reckoning combined
    if (((NavPvt->flags & GPS_FIX_FLAGS) == GPS_FIX_FLAGS) && ((NavPvt->fixType == 3) || (NavPvt->fixType == 4))) {
        pendingReportDataRecord.setCourse((constrain(NavPvt->heading, 0, INT32_MAX) * 255) / (360 * 100000)); // scale and range to 0..255
        pendingReportDataRecord.setAltitude((int16_t)constrain(NavPvt->hMSL / 1000, INT16_MIN, INT16_MAX)); // mm to m
        pendingReportDataRecord.setLat(NavPvt->lat);
        pendingReportDataRecord.setLong(NavPvt->lon);
        pendingReportDataRecord.setSatelliteCount(NavPvt->numSV);
        pendingReportDataRecord.setSpeed((NavPvt->gSpeed * 36) / 10000); // mm/s converted to km/h

        isPendingReportDataRecordNew = true;
    }
}

/**
 * Returns the current datetime (seconds since unix epoch).
 */
uint32_t getNow()
{
    return rtc.getEpoch();
}


/**
 * Sets the RTC epoch and "rtcEpochDelta".
 */
void setNow(uint32_t newEpoch)
{
    uint32_t currentEpoch = getNow();

    debugPrint("Setting RTC from ");
    debugPrint(currentEpoch);
    debugPrint(" to ");
    debugPrintln(newEpoch);

    rtcEpochDelta = newEpoch - currentEpoch;
    rtc.setEpoch(newEpoch);

    timer.adjust(currentEpoch, newEpoch);

    isRtcInitialized = true;
}


