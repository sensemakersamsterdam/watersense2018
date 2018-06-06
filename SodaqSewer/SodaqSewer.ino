#include "Arduino.h"


// Sodaq Vars
/*
 * Compatible with:
 * SODAQ MBILI
 * SODAQ Autonomo
 * SODAQ ONE
 * SODAQ ONE BETA
 * SODAQ EXPLORER
 */
#include "Config.h"
#include "LedColor.h"
#include "Enums.h"
#include "MyTime.h"

#include "RTCTimer.h"
#include "RTCZero.h"

#include "ReportDataRecord.h"
#include "GpsFixDataRecord.h"
#include "GpsFixLiFoRingBuffer.h"
#include "OverTheAirConfigDataRecord.h"
#include "CayenneLPP.h"

#include <Sodaq_RN2483.h>
#include "Sodaq_wdt.h"
#include "Sodaq_LIS3DE.h"
#include "ublox.h"

#define debugSerial SERIAL_PORT_MONITOR
#define DEBUG_STREAM SerialUSB

#define consolePrint(x) CONSOLE_STREAM.print(x)
#define consolePrintln(x) CONSOLE_STREAM.println(x)

#define debugPrint(x) if (params.getIsDebugOn()) { DEBUG_STREAM.print(x); }
#define debugPrintln(x) if (params.getIsDebugOn()) { DEBUG_STREAM.println(x); }


// RTC Stuff
RTCZero rtc;
RTCTimer timer;
Time time;
#define MAX_RTC_EPOCH_OFFSET 25
void setNow(uint32_t now);
static int64_t rtcEpochDelta; // set in setNow() and used in getGpsFixAndTransmit() for correcting time in loop
static bool isRtcInitialized;


// Battery Stuff
#define ADC_AREF 3.3f
//#define BATVOLT_R1 2.0f // One v1
//#define BATVOLT_R2 2.0f // One v1
#define BATVOLT_R1 4.7f // One v2
#define BATVOLT_R2 10.0f // One v2




#if defined(ARDUINO_AVR_SODAQ_MBILI)
#define loraSerial Serial1
#define LORA_STREAM Serial2
#define DEBUG_STREAM SerialUSB
#define CONSOLE_STREAM SerialUSB

#define BEE_VCC 20

#elif defined(ARDUINO_SODAQ_AUTONOMO) || defined(ARDUINO_SODAQ_ONE) || defined(ARDUINO_SODAQ_ONE_BETA)
#define loraSerial Serial1
#define LORA_STREAM Serial1
#define DEBUG_STREAM SerialUSB
#define CONSOLE_STREAM SerialUSB

#elif defined(ARDUINO_SODAQ_EXPLORER)
#define loraSerial Serial2
#define LORA_STREAM Serial2
#define DEBUG_STREAM SerialUSB
#define CONSOLE_STREAM SerialUSB

#else
// please select a sodaq board
debugSerial.println("Please select a sodaq board!!");
#endif

// Accelerometerstuff
volatile bool isOnTheMoveActivated;
Sodaq_LIS3DE accelerometer;
void setAccelerometerTempSensorActive(bool on);
static bool isOnTheMoveInitialized;
volatile uint32_t lastOnTheMoveActivationTimestamp;
void accelerometerInt1Handler();


//GPS Stuff
UBlox ublox;
static bool isGpsInitialized;
bool initGps();
void setGpsActive(bool on);
void delegateNavPvt(NavigationPositionVelocityTimeSolution* NavPvt);
#define GPS_TIME_VALIDITY 0b00000011 // date and time (but not fully resolved)
#define GPS_FIX_FLAGS 0b00000001 // just gnssFixOK

// Data stuff Careful: here the sensors are read and send away
bool getGpsFixAndTransmit();
ReportDataRecord pendingReportDataRecord;
bool isPendingReportDataRecordNew; // this is set to true only when pendingReportDataRecord is written by the delegate
static bool isLoraInitialized;
static uint8_t receiveBuffer[16];
static uint8_t receiveBufferSize;
static uint8_t sendBuffer[51];
static uint8_t sendBufferSize;
CayenneLPP cayenneRecord(51); // buffer is set to the same size as the sendBuffer[]

volatile bool minuteFlag;

// LORA stuff
#ifndef LORA_RESET
#define LORA_RESET -1
#endif

#define LORA_MAX_RETRIES 3
void updateSendBuffer();
void transmit();
bool initLoraAbp(LoraInitConsoleMessages messages);
bool initLoraOtaa(LoraInitConsoleMessages messages);
bool initLora(LoraInitConsoleMessages messages, LoraInitJoin join);

// ABP
#include "abpkeys.h"
//const uint8_t devAddr[4] = { 0x00, 0x00, 0x00, 0x00 };
//const uint8_t appSKey[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//const uint8_t nwkSKey[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

// If you want to add your own keys without replacing the default keys, please place them in otaakeys.h in the same folder as this ino file
#include "otaakeys.h"
// OTAA
//uint8_t DevEUI[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//uint8_t AppEUI[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//uint8_t AppKey[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };



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

//Generic Vars
int waitfor = 300000;       // wait in loop 60000 is one minute: 300000 is 5 minutes
bool SODAQ = true;
bool TOF = true;
bool AquaPlumb = true;
bool Turbidity = false;
void systemSleep();

//Generic Stuff
// version of "hex to bin" macro that supports both lower and upper case
#define HEX_CHAR_TO_NIBBLE(c) ((c >= 'a') ? (c - 'a' + 0x0A) : ((c >= 'A') ? (c - 'A' + 0x0A) : (c - '0')))
#define HEX_PAIR_TO_BYTE(h, l) ((HEX_CHAR_TO_NIBBLE(h) << 4) + HEX_CHAR_TO_NIBBLE(l))

bool convertAndCheckHexArray(uint8_t* result, const char* hex, size_t resultSize);


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
     debugSerial.println(F("Failed to boot VL53L0X"));
      while(1);
    }
  }
  
  
}

void setupLoRa(){
  // ABP
  setupLoRaABP();
  // OTAA
  //setupLoRaOTAA();
  LoRaBee.setSpreadingFactor(9);
}



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
    packet = String("AquaPlumb height " + String(height) + " ");
    debugSerial.println(packet);
  }
  if ( TOF == true ) {
  // VL53L0X
    VL53L0X_RangingMeasurementData_t measure;
    debugSerial.print("Reading a measurement... ");
    lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

    if (measure.RangeStatus != 4) {  // phase failures have incorrect data
      debugSerial.print("Distance (mm): "); debugSerial.println(measure.RangeMilliMeter);
      packet = String(packet + "VL53L0X Distance: " + String(measure.RangeMilliMeter)+ " " );
      debugSerial.println(packet);

    } else {
      debugSerial.println(" out of range ");
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
    debugSerial.println(packet);
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

// SODAQ functions for sleeping the system between measurements and
// initializing sensors, reading sensors. 

/**
 * Initializes the CPU sleep mode.
 */
void initSleep()
{
    // Set the sleep mode
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
}

/**
 * Returns the battery voltage minus 3 volts.
 */
uint8_t getBatteryVoltage()
{
    uint16_t voltage = (uint16_t)((ADC_AREF / 1.023) * (BATVOLT_R1 + BATVOLT_R2) / BATVOLT_R2 * (float)analogRead(BAT_VOLT));
    voltage = (voltage - 3000) / 10;

    return (voltage > 255 ? 255 : (uint8_t)voltage);
}

/**
 * Returns the board temperature.
*/
int8_t getBoardTemperature()
{
    setAccelerometerTempSensorActive(true);

    int8_t temp = params.getTemperatureSensorOffset() + accelerometer.getTemperatureDelta();

    setAccelerometerTempSensorActive(false);

    return temp;
}

/**
* Initializes the on-the-move functionality (interrupt on acceleration).
*/
void initOnTheMove()
{
    pinMode(ACCEL_INT1, INPUT);
    attachInterrupt(ACCEL_INT1, accelerometerInt1Handler, CHANGE);

    // Configure EIC to use GCLK1 which uses XOSC32K, XOSC32K is already running in standby
    // This has to be done after the first call to attachInterrupt()
    GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(GCM_EIC) |
        GCLK_CLKCTRL_GEN_GCLK1 |
        GCLK_CLKCTRL_CLKEN;

    accelerometer.enable(true, 
        Sodaq_LIS3DE::NormalLowPower10Hz, 
        Sodaq_LIS3DE::XYZ, 
        Sodaq_LIS3DE::Scale8g, 
        true);
    sodaq_wdt_safe_delay(100);

    accelerometer.enableInterrupt1(
        Sodaq_LIS3DE::XHigh | Sodaq_LIS3DE::XLow | Sodaq_LIS3DE::YHigh | Sodaq_LIS3DE::YLow | Sodaq_LIS3DE::ZHigh | Sodaq_LIS3DE::ZLow,
        params.getAccelerationPercentage() * 8.0 / 100.0,
        params.getAccelerationDuration(),
        Sodaq_LIS3DE::MovementRecognition);
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
 * Initializes the accelerometer or puts it in power-down mode
 * for the purpose of reading its temperature delta.
*/
void setAccelerometerTempSensorActive(bool on)
{
    // if on-the-move is initialized then the accelerometer is enabled anyway
    if (isOnTheMoveInitialized) {
        return;
    }

    if (on) {
        accelerometer.enable(false, Sodaq_LIS3DE::NormalLowPower100Hz, Sodaq_LIS3DE::XYZ, Sodaq_LIS3DE::Scale2g, true);
        sodaq_wdt_safe_delay(30); // should be enough for initilization and 2 measurement periods
    }
    else {
        accelerometer.disable();
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

/**
 * Initializes the RTC.
 */
void initRtc()
{
    rtc.begin();

    // Schedule the wakeup interrupt for every minute
    // Alarm is triggered 1 cycle after match
    rtc.setAlarmSeconds(59);
    rtc.enableAlarm(RTCZero::MATCH_SS); // alarm every minute

    // Attach handler
    rtc.attachInterrupt(rtcAlarmHandler);

    // This sets it to 2000-01-01
    rtc.setEpoch(0);
}

/**
 * Runs every minute by the rtc alarm.
*/
void rtcAlarmHandler()
{
    minuteFlag = true;
}

/**
 * Runs every time acceleration is over the limits
 * set by the user (if enabled).
*/
void accelerometerInt1Handler()
{
    if (digitalRead(ACCEL_INT1)) {
        if (params.getIsLedEnabled()) {
            setLedColor(YELLOW);
        }

        // debugPrintln("On-the-move is triggered");

        isOnTheMoveActivated = true;
        lastOnTheMoveActivationTimestamp = getNow();
    }
}

/**
 * Initializes the RTC Timer and schedules the default events.
 */
void initRtcTimer()
{
    timer.setNowCallback(getNow); // set how to get the current time
    timer.allowMultipleEvents();

    resetRtcTimerEvents();
}

/**
 * Clears the RTC Timer events and schedules the default events.
 */
void resetRtcTimerEvents()
{
    timer.clearAllEvents();

    // Schedule the default fix event (if applicable)
    if (params.getDefaultFixInterval() > 0) {
        timer.every(params.getDefaultFixInterval() * 60, runDefaultFixEvent);
    }

    // check if the alternative fix event should be scheduled at all
    if (params.getAlternativeFixInterval() > 0) {
        // Schedule the alternative fix event
        timer.every(params.getAlternativeFixInterval() * 60, runAlternativeFixEvent);
    }

    if (isOnTheMoveInitialized) {
        timer.every(params.getOnTheMoveFixInterval() * 60, runOnTheMoveFixEvent);
    }

    // if lora is not enabled, schedule an event that takes care of extending the sleep time of the module
    if (!isLoraInitialized) {
        timer.every(24 * 60 * 60, runLoraModuleSleepExtendEvent); // once a day
    }
}

/**
 * Returns true if the alternative fix event should run at the current time.
*/
bool isAlternativeFixEventApplicable()
{
    // - RTC should be initialized (synced time)
    // - alternative fix interval should be set
    // - the span between FROM and TO should be at least as much as the alternative fix interval
    // - current time should be within the FROM and TO times set
    return (isRtcInitialized
        && (params.getAlternativeFixInterval() > 0)
        && (params.getAlternativeFixTo() - params.getAlternativeFixFrom() >= params.getAlternativeFixInterval() * 60)
        && (isCurrentTimeOfDayWithin(params.getAlternativeFixFrom(), params.getAlternativeFixTo())));
}

/**
 * Returns true if the current rtc time is within the given times of day (in seconds).
*/
bool isCurrentTimeOfDayWithin(uint32_t daySecondsFrom, uint32_t daySecondsTo)
{
    uint32_t daySecondsCurrent = rtc.getHours() * 60 * 60 + rtc.getMinutes() * 60;

    return (daySecondsCurrent >= daySecondsFrom && daySecondsCurrent < daySecondsTo);
}

/**
 * Runs the default fix event sequence (only if applicable).
 */
void runDefaultFixEvent(uint32_t now)
{
    if (!isAlternativeFixEventApplicable()) {
        debugPrintln("Default fix event started.");
        getGpsFixAndTransmit();
    }
}

/**
 * Runs the alternative fix event sequence (only if it set and is within the set time).
 */
void runAlternativeFixEvent(uint32_t now)
{
    if (isAlternativeFixEventApplicable()) {
        debugPrintln("Alternative fix event started.");
        getGpsFixAndTransmit();
    }
}

/**
* Runs the "moving" fix event sequence (only if enabled, device is on the move and timeout hasn't elapsed).
*/
void runOnTheMoveFixEvent(uint32_t now)
{
    if (isOnTheMoveActivated) {
        if (now - lastOnTheMoveActivationTimestamp < params.getOnTheMoveTimeout() * 60) {
            debugPrintln("On-the-move fix event started.");
            getGpsFixAndTransmit();
        }
        else {
            // timeout elapsed -disable it completely until there is another on-the-move interrupt triggered
            debugPrintln("On-the-move has timed-out (no movement) -disabling.");
            isOnTheMoveActivated = false;
        }
    }
}

/**
 * Wakes up the lora module to put it back to sleep, i.e. extends the sleep period
*/
void runLoraModuleSleepExtendEvent(uint32_t now)
{
    debugPrintln("Extending LoRa module sleep period.");

    setLoraActive(true);
    sodaq_wdt_safe_delay(80);
    setLoraActive(false);
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
 * Tries to get a GPS fix and sends the data through LoRa if applicable.
 * Times-out after params.getGpsFixTimeout seconds.
 * Please see the documentation for more details on how this process works.
 */
bool getGpsFixAndTransmit()
{
    debugPrintln("Starting getGpsFixAndTransmit()...");

    if (!isGpsInitialized) {
        debugPrintln("GPS is not initialized, exiting...");

        return false;
    }

    bool isSuccessful = false;
    setGpsActive(true);

    pendingReportDataRecord.setSatelliteCount(0); // reset satellites to use them as a quality metric in the loop
    uint32_t startTime = getNow();
    while ((getNow() - startTime <= params.getGpsFixTimeout())
        && (pendingReportDataRecord.getSatelliteCount() < params.getGpsMinSatelliteCount()))
    {
        sodaq_wdt_reset();
        uint16_t bytes = ublox.available();

        if (bytes) {
            rtcEpochDelta = 0;
            isPendingReportDataRecordNew = false;
            ublox.GetPeriodic(bytes); // calls the delegate method for passing results

            startTime += rtcEpochDelta; // just in case the clock was changed (by the delegate in ublox.GetPeriodic)

            // isPendingReportDataRecordNew guarantees at least a 3d fix or GNSS + dead reckoning combined
            // and is good enough to keep, but the while loop should keep trying until timeout or sat count larger than set
            if (isPendingReportDataRecordNew) {
                isSuccessful = true;
            }
        }
    }

    setGpsActive(false); // turn off gps as soon as it is not needed

    // populate all fields of the report record
    pendingReportDataRecord.setTimestamp(getNow());
    pendingReportDataRecord.setBatteryVoltage(getBatteryVoltage());
    pendingReportDataRecord.setBoardTemperature(getBoardTemperature());
//    pendingReportDataRecord.setOneWireTemperature(getExternalTemperature()*10);

    GpsFixDataRecord record;
    record.init();
    if (isSuccessful) {
        pendingReportDataRecord.setTimeToFix(pendingReportDataRecord.getTimestamp() - startTime);

        // add the new gpsFixDataRecord to the ringBuffer
        record.setLat(pendingReportDataRecord.getLat());
        record.setLong(pendingReportDataRecord.getLong());
        record.setTimestamp(pendingReportDataRecord.getTimestamp());

        gpsFixLiFoRingBuffer_push(&record);
    }
    else {
        pendingReportDataRecord.setTimeToFix(0xFF);

        // no need to check the buffer or the record for validity, default for Lat/Long is 0 anyway
        gpsFixLiFoRingBuffer_peek(0, &record);
        pendingReportDataRecord.setLat(record.getLat());
        pendingReportDataRecord.setLong(record.getLong());
    }

    if (params.getIsDebugOn()) {
        pendingReportDataRecord.printHeaderLn(&DEBUG_STREAM);
        pendingReportDataRecord.printRecordLn(&DEBUG_STREAM);
        debugPrintln();
    }

    updateSendBuffer();
    transmit();

    return isSuccessful;
}


/**
 * Updates the "sendBuffer" using the current "pendingReportDataRecord" and its "sendBufferSize".
 */
void updateSendBuffer()
{
    if (params.getIsCayennePayloadEnabled()) {
        // Reset the record
        cayenneRecord.reset();


        // Add GPS record on data channel 1
        float latitude = (float)pendingReportDataRecord.getLat() / 10000000.0f;
        float longitude = (float)pendingReportDataRecord.getLong() / 10000000.0f;
        float altitude = (float)pendingReportDataRecord.getAltitude();
        cayenneRecord.addGPS(1, latitude, longitude, altitude);
        
        // Add battery voltage on data channel 2
        float voltage = (float)pendingReportDataRecord.getBatteryVoltage() * 10 + 3000;
        cayenneRecord.addAnalogInput(2, voltage / 1000);
       
        // Add board temperature on data channel 3 // not to be used with SodaqOne board version v1
        float temp = (float)pendingReportDataRecord.getBoardTemperature();
        cayenneRecord.addTemperature(3, temp);

        // Add external temperature on data channel 4
        float externalTemp = (float)pendingReportDataRecord.getOneWireTemperature()/10;
        cayenneRecord.addTemperature(4, externalTemp);


        // Copy out the formatted record
        sendBufferSize = cayenneRecord.copy(sendBuffer);
    }
    else {
        // copy the pendingReportDataRecord into the sendBuffer
        memcpy(sendBuffer, pendingReportDataRecord.getBuffer(), pendingReportDataRecord.getSize());
        sendBufferSize = pendingReportDataRecord.getSize();

        // copy the previous coordinates if applicable (-1 because one coordinate is already in the report record)
        GpsFixDataRecord record;
        for (uint8_t i = 0; i < params.getCoordinateUploadCount() - 1; i++) {
            record.init();

            // (skip first record because it is in the report record already)
            if (!gpsFixLiFoRingBuffer_peek(1 + i, &record)) {
                break;
            }

            if (!record.isValid()) {
                break;
            }

            record.updatePreviousFixValue(pendingReportDataRecord.getTimestamp());
            memcpy(&sendBuffer[sendBufferSize - 1], record.getBuffer(), record.getSize());
            sendBufferSize += record.getSize();
        }
    }
}

/**
 * Simple wrapper method for sending with/without ack.
*/
uint8_t inline loRaBeeSend(bool ack, uint8_t port, const uint8_t* payload, uint8_t size)
{
    if (ack) {
        return LoRaBee.sendReqAck(port, payload, size, LORA_MAX_RETRIES);
    }
    else {
        return LoRaBee.send(port, payload, size);
    }
}

/**
 * Retries the initialization of Lora (suppressing the messages to the console).
 * When successful return true and sets the global variable isLoraInitialized.
*/
bool retryInitLora()
{
    if (initLora(LORA_INIT_SKIP_CONSOLE_MESSAGES, LORA_INIT_JOIN)) {
        isLoraInitialized = true;

        return true;
    }

    return false;
}

/**
 * Sends the current sendBuffer through lora (if enabled).
 * Repeats the transmitions according to params.getRepeatCount().
*/
void transmit()
{
    if (!isLoraInitialized) {
        // check for a retry
        if (!params.getShouldRetryConnectionOnSend() || !retryInitLora()) {
            return;
        }
    }

    setLoraActive(true);

    for (uint8_t i = 0; i < 1 + params.getRepeatCount(); i++) {
        if (loRaBeeSend(params.getIsAckOn(), params.getLoraPort(), sendBuffer, sendBufferSize) != 0) {
            debugPrintln("There was an error while transmitting through LoRaWAN.");
        }
        else {
            debugPrintln("Data transmitted successfully.");

            uint16_t size = LoRaBee.receive(receiveBuffer, sizeof(receiveBuffer));
            receiveBufferSize = size;

            if (size > 0) {
                debugPrintln("Received OTA Configuration.");
                updateConfigOverTheAir();
            }
        }
    }

    setLoraActive(false);
}

bool initLoraOtaa(LoraInitConsoleMessages messages)
{
    uint8_t devEui[8];
    uint8_t appEui[8];
    uint8_t appKey[16];

    bool allParametersValid = convertAndCheckHexArray((uint8_t*)devEui, params.getDevAddrOrEUI(), sizeof(devEui))
        && convertAndCheckHexArray((uint8_t*)appEui, params.getAppSKeyOrEUI(), sizeof(appEui))
        && convertAndCheckHexArray((uint8_t*)appKey, params.getNwSKeyOrAppKey(), sizeof(appKey));

    // check the parameters first
    if (!allParametersValid) {
        if (messages == LORA_INIT_SHOW_CONSOLE_MESSAGES) {
            consolePrintln("The parameters for LoRa are not valid. LoRa cannot be enabled.");
        }

        return false;
    }

    if (LoRaBee.initOTA(LORA_STREAM, devEui, appEui, appKey, params.getIsAdrOn(), LORA_RESET)) {
        if (messages == LORA_INIT_SHOW_CONSOLE_MESSAGES) {
            consolePrintln("LoRa OTAA init success!");
        }

        return true;
    }
    else {
        if (messages == LORA_INIT_SHOW_CONSOLE_MESSAGES) {
            consolePrintln("LoRa OTAA init failed!");
        }

        return false;
    }
}

bool initLoraAbp(LoraInitConsoleMessages messages)
{
    uint8_t devAddr[4];
    uint8_t appSKey[16];
    uint8_t nwkSKey[16];

    bool allParametersValid = convertAndCheckHexArray((uint8_t*)devAddr, params.getDevAddrOrEUI(), sizeof(devAddr))
        && convertAndCheckHexArray((uint8_t*)appSKey, params.getAppSKeyOrEUI(), sizeof(appSKey))
        && convertAndCheckHexArray((uint8_t*)nwkSKey, params.getNwSKeyOrAppKey(), sizeof(nwkSKey));

    // check the parameters first
    if (!allParametersValid) {
        if (messages == LORA_INIT_SHOW_CONSOLE_MESSAGES) {
            consolePrintln("The parameters for LoRa are not valid. LoRa cannot be enabled.");
        }

        return false;
    }

    if (LoRaBee.initABP(LORA_STREAM, devAddr, appSKey, nwkSKey, params.getIsAdrOn(), LORA_RESET)) {
        if (messages == LORA_INIT_SHOW_CONSOLE_MESSAGES) {
            consolePrintln("LoRa ABP init success!");
        }

        return true;
    }
    else {
        if (messages == LORA_INIT_SHOW_CONSOLE_MESSAGES) {
            consolePrintln("LoRa ABP init failed!");
        }

        return false;
    }
}

/**
 * Initializes the lora module according to the given operation (join or skip).
 * Returns true if the operation was successful.
*/
bool initLora(LoraInitConsoleMessages messages, LoraInitJoin join)
{
    debugPrintln("Initializing LoRa...");

    if (messages == LORA_INIT_SHOW_CONSOLE_MESSAGES) {
        consolePrintln("Initializing LoRa...");
    }

    LORA_STREAM.begin(LoRaBee.getDefaultBaudRate());
    if (params.getIsDebugOn()) {
        LoRaBee.setDiag(DEBUG_STREAM);
    }

    bool result;
    if (join == LORA_INIT_SKIP_JOIN) {
        // just reset and check connectivity
        result = LoRaBee.init(LORA_STREAM, LORA_RESET);
    }
    else if (join == LORA_INIT_JOIN) {
        if (params.getIsOtaaEnabled()) {
            result = initLoraOtaa(messages);
        }
        else {
            result = initLoraAbp(messages);
        }

        if (result) {
            if (!params.getIsAdrOn()) {
                LoRaBee.setSpreadingFactor(params.getSpreadingFactor());
            }

            LoRaBee.setPowerIndex(params.getPowerIndex());
        }
    }
    else {
        result = false;
    }

    setLoraActive(false); // make sure it is off
    return result;
}


/**
 * Uses the "receiveBuffer" (received from LoRaWAN) to update the configuration.
*/
void updateConfigOverTheAir()
{
    OverTheAirConfigDataRecord record;
    record.init();
    record.copyFrom(receiveBuffer, receiveBufferSize);

    if (record.isValid()) {
        params._defaultFixInterval = record.getDefaultFixInterval();
        params._alternativeFixInterval = record.getAlternativeFixInterval();

        // time of day seconds assumed
        params._alternativeFixFromHours = record.getAlternativeFixFrom() / 3600;
        params._alternativeFixFromMinutes = (record.getAlternativeFixFrom() - params._alternativeFixFromHours * 3600) / 60;

        params._alternativeFixToHours = record.getAlternativeFixTo() / 3600;
        params._alternativeFixToMinutes = (record.getAlternativeFixTo() - params._alternativeFixToHours * 3600) / 60;

        params._gpsFixTimeout = record.getGpsFixTimeout();

        params.commit(true);
        debugPrintln("OTAA Config commited!");

        // apply the rtc timer changes
        resetRtcTimerEvents();
    }
    else {
        debugPrintln("OTAA Config record is not valid!");
    }
}

/**
 * Converts the given hex array and returns true if it is valid hex and non-zero.
 * "hex" is assumed to be 2*resultSize bytes.
 */
bool convertAndCheckHexArray(uint8_t* result, const char* hex, size_t resultSize)
{
    bool foundNonZero = false;

    uint16_t inputIndex = 0;
    uint16_t outputIndex = 0;

    // stop at the first string termination char, or if output buffer is over
    while (outputIndex < resultSize && hex[inputIndex] != 0 && hex[inputIndex + 1] != 0) {
        if (!isxdigit(hex[inputIndex]) || !isxdigit(hex[inputIndex + 1])) {
            return false;
        }

        result[outputIndex] = HEX_PAIR_TO_BYTE(hex[inputIndex], hex[inputIndex + 1]);

        if (result[outputIndex] > 0) {
            foundNonZero = true;
        }

        inputIndex += 2;
        outputIndex++;
    }

    return foundNonZero;
}



