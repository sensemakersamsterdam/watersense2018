/*
RTOSSewer project: sensing water quality in a variety of ways.

Copyright (C) 2018 Sensemakers Amsterdam. All rights reserved.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "board/SODAQ_ONE_V3.h"
#include "periph/I2C.h"
#include "periph/LoRaSodaq.h"
#include "periph/OneWire.h"
#include "periph/WDT.h"
#include "sensor/BMP280.h"
#include "sensor/Cond.h"
#include "sensor/DS18B20.h"
#include "sensor/HCSR04.h"
#include "sensor/LSM303AGR.h"
#include "sensor/MB7092.h"
#include "sensor/SEN0189.h"
#include "sensor/AquaP.h"
#include "sensor/VL53L0X.h"
#include "util/Collection.h"
#include "Sewer.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MAINTHREAD_DELAY_LOOP 180000
#define MAINTHREAD_STACK_SIZE 384
#define MAINTHREAD_PRIORITY   tskIDLE_PRIORITY + 1

#define DATA_BIT_LSM303AGR    1
#define DATA_BIT_BMP280       2
#define DATA_BIT_VL53L0X      4
#define DATA_BIT_MB7092       8
#define DATA_BIT_HCSR04       16
#define DATA_BIT_DS18B20      32
#define DATA_BIT_COND         64
#define DATA_BIT_SEN0189      128
#define DATA_BIT_AQUAP        256


/*******************************************************************************
 * State
 ******************************************************************************/

struct __attribute__((__packed__)) {
  uint16_t version;
  uint16_t modules;
  uint16_t board_vo;
  int8_t   lsm303agr_te;
  int8_t   bmp280_te;
  uint32_t bmp280_pr;    // TODO: reduce to 2 bytes (minus 100.000)
  uint16_t vl53l0x_di;   // TODO: 1 byte for distance
  uint16_t mb7092_di;
  uint16_t hcsr04_di;
  int8_t   ds18b20_te;
  uint16_t cond_co;
  uint16_t sen0189_di;
  uint16_t aquap_di;
} data;


/*******************************************************************************
 * Private declarations
 ******************************************************************************/

static void Sewer_initModules();
static void Sewer_logData();
static void Sewer_measureData();
static void Sewer_sendData();
static void Sewer_threadMain(void* pvParameters);


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void Sewer_setup()
{
  pinMode(PIN_SENSORS_POWER, OUTPUT);
  digitalWrite(PIN_SENSORS_POWER, LOW);

  static StaticTask_t xT0TaskBuffer;
  static StackType_t  xT0Stack[MAINTHREAD_STACK_SIZE];

  xTaskCreateStatic(Sewer_threadMain, "M", MAINTHREAD_STACK_SIZE, NULL, MAINTHREAD_PRIORITY, xT0Stack, &xT0TaskBuffer);
  vTaskStartScheduler();
}


/*******************************************************************************
 * Private
 ******************************************************************************/

static void Sewer_initModules()
{
  #if USE_LOGGER
  Logger_setup();
  #endif

  WDT_setup();

  WDT_enable();
  digitalWrite(PIN_SENSORS_POWER, HIGH);

  Board_setup();

  #if USE_LOGGER
  I2C_logDevices();
  OneWire_logDevices();
  #endif

  digitalWrite(PIN_SENSORS_POWER, LOW);
  WDT_disable();

  vTaskDelay(pdMS_TO_TICKS(200));
}

static void Sewer_logData()
{
  LOG(VS("BOARD battery: "), VUI16(data.board_vo));

  if (data.modules & DATA_BIT_LSM303AGR) {
    LOG(VS("LSM303AGR temperature: "), VI8(data.lsm303agr_te), VS(" *C"));
  }

  if (data.modules & DATA_BIT_BMP280) {
    LOG(VS("BMP280 temperature: "), VI8  (data.bmp280_te), VS(" *C"));
    LOG(VS("BMP280 pressure: "),    VUI32(data.bmp280_pr),    VS(" Pa"));
  }

  if (data.modules & DATA_BIT_VL53L0X) {
    if (data.vl53l0x_di <= 2000) {
      LOG(VS("VL53L0X distance: "), VUI16(data.vl53l0x_di), VS(" mm"));
    } else {
      LOGS("VL53L0X distance: out of range");
    }
  }

  if (data.modules & DATA_BIT_MB7092) {
    LOG(VS("MB7092 distance: "), VUI16(data.mb7092_di));
  }

  if (data.modules & DATA_BIT_HCSR04) {
    LOG(VS("HCSR04 distance: "), VUI16(data.hcsr04_di));
  }

  if (data.modules & DATA_BIT_DS18B20) {
    LOG(VS("DS18B20 temperature: "), VI8(data.ds18b20_te));
  }

  if (data.modules & DATA_BIT_COND) {
    LOG(VS("Cond value: "), VUI16(data.cond_co));
  }

  if (data.modules & DATA_BIT_SEN0189) {
    LOG(VS("SEN0189 distance: "), VUI16(data.sen0189_di));
  }

  if (data.modules & DATA_BIT_AQUAP) {
    LOG(VS("AquaP distance: "), VUI16(data.aquap_di));
  }
}

static void Sewer_measureData()
{
  LOGS("reading a measurement...");

  SYSCTRL->BOD33.bit.ENABLE = 0; // disabled brown-out detect
  digitalWrite(PIN_SENSORS_POWER, HIGH);
  vTaskDelay(pdMS_TO_TICKS(3));
  SYSCTRL->BOD33.bit.ENABLE = 1; // enabled brown-out detect

  data.version  = PROTOCOL_VERSION;
  data.modules  = 0;
  data.board_vo = Board_measureVoltage();

  I2C_enable();

  if (LSM303AGR_isActive()) {
    data.modules     |= DATA_BIT_LSM303AGR;
    data.lsm303agr_te = LSM303AGR_measureTemperature();
  } else {
    data.modules     &= ~DATA_BIT_LSM303AGR;
    data.lsm303agr_te = 0;
  }

  if (BMP280_setup()) {
    data.modules  |= DATA_BIT_BMP280;
    data.bmp280_te = floor(BMP280_measureTemperature() + 0.5F);
    data.bmp280_pr = floor(BMP280_measurePressure() + 0.5F);
  } else {
    data.modules  &= ~DATA_BIT_BMP280;
    data.bmp280_te = 0;
    data.bmp280_pr = 0;
  }

  if (VL53L0X_setup()) {
    data.modules   |= DATA_BIT_VL53L0X;
    data.vl53l0x_di = VL53L0X_measureDistance();
  } else {
    data.modules   &= ~DATA_BIT_VL53L0X;
    data.vl53l0x_di = 0;
  }

  I2C_disable();

  MB7092_setup();
  data.mb7092_di = MB7092_measureDistance();
  if (data.mb7092_di > 10) {
    data.modules |= DATA_BIT_MB7092;
  } else {
    data.modules &= ~DATA_BIT_MB7092;
  }

  HCSR04_setup();
  data.hcsr04_di = HCSR04_measureDistance();
  if (data.hcsr04_di > 0) {
    data.modules |= DATA_BIT_HCSR04;
  } else {
    data.modules &= ~DATA_BIT_HCSR04;
  }

  if (DS18B20_setup()) {
    data.modules   |= DATA_BIT_DS18B20;
    data.ds18b20_te = floor(DS18B20_measureTemperature() + 0.5F);
  } else {
    data.modules   &= ~DATA_BIT_DS18B20;
    data.ds18b20_te = 0;
  }

  Cond_setup();
  data.cond_co = Cond_measure();
  if (data.cond_co <= 1000) {
    data.modules |= DATA_BIT_COND;
  } else {
    data.modules &= ~DATA_BIT_COND;
  }

  data.sen0189_di = SEN0189_measure();
  if (data.sen0189_di <= 1000) {
    data.modules |= DATA_BIT_SEN0189;
  } else {
    data.modules &= ~DATA_BIT_SEN0189;
  }

  data.aquap_di = AQUAP_measure();
  if (data.aquap_di <= 1000) {
    data.modules |= DATA_BIT_AQUAP;
  } else {
    data.modules &= ~DATA_BIT_AQUAP;
  }

  digitalWrite(PIN_SENSORS_POWER, LOW);

  #if USE_LOGGER
  Sewer_logData();
  #endif
}

static void Sewer_sendData()
{
  static bool b = false;

  LOG(VS("data, "), VUI8(sizeof(data)), VS(" bytes: "), VUI8AH02((const uint8_t*)&data, sizeof(data)));

  LoRa_wakeUp();

  if (!b) {
    LoRa_setSpreadingFactor(LORA_SF);
    b = LoRa_initOTAA();
  }

  if (b && LoRa_send((const uint8_t*)&data, sizeof(data)) == 2) { b = false; }

  LoRa_sleep();
}

static void Sewer_threadMain(void* pvParameters)
{
  Sewer_initModules();

  TickType_t ts = xTaskGetTickCount();

  for (;;) {
    WDT_enable();

    Sewer_measureData();

    #if USE_LORA
    Sewer_sendData();
    #endif

    WDT_disable();

    LOGS("need delay " STR(MAINTHREAD_DELAY_LOOP) " ms");
    vTaskDelayUntil(&ts, pdMS_TO_TICKS(MAINTHREAD_DELAY_LOOP));
  }
}
