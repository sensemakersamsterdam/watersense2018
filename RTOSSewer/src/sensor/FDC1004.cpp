#include "FDC1004.h"

#if USE_FDC1004

#include <Protocentral_FDC1004.h>
#include "../periph/I2C.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define FDC1004_ADDRESS 0x50

#define CHANNEL         0
#define MEASURMENT      0

#define UPPER_BOUND     0x4000 // max readout capacitance
#define LOWER_BOUND     (-1 * UPPER_BOUND)


/*******************************************************************************
 * State
 ******************************************************************************/

static FDC1004 FDC;

static uint8_t capdac = 0;


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

bool FDC1004_setup()
{
  Wire.beginTransmission(FDC1004_ADDRESS);

  bool b = Wire.endTransmission() == 0;

  LOG_SHOW_SETUP_RESULT(b);

  return b;
}


/*******************************************************************************
 * Public
 ******************************************************************************/

void FDC1004_measure()
{
  #if USE_LOGGER
  LOGS("reading a measurement...");

  uint8_t result = FDC.configureMeasurementSingle(MEASURMENT, CHANNEL, capdac);
  FDC.triggerSingleMeasurement(MEASURMENT, FDC1004_100HZ);

  if (result != 0) {
    LOGS("error: configureMeasurementSingle");
    return;
  }

  RTOS_delay(pdMS_TO_TICKS(15));

  uint16_t value[2];

  result = FDC.readMeasurement(MEASURMENT, value);

  if (result != 0) {
    LOGS("error: readMeasurement");
    return;
  }

  int16_t msb = (int16_t)value[0];
  int32_t cap = ((int32_t)457) * ((int32_t)msb); // in attofarads

  cap /= 1000;                                   // in femtofarads
  cap += ((int32_t)3028) * ((int32_t)capdac);

  LOG(VS("capacitance = "), VF((float)cap / 1000), VS(" pf"));

  if (msb > UPPER_BOUND) {                       // adjust capdac accordingly
    if (capdac < FDC1004_CAPDAC_MAX) { capdac++; }
  } else if (msb < LOWER_BOUND) {
    if (capdac > 0) { capdac--; }
  }

  #endif
}

#endif // USE_FDC1004
