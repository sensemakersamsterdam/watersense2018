#include <Sodaq_LSM303AGR.h>

#include "LSM303AGR.h"


/*******************************************************************************
 * State
 ******************************************************************************/

static Sodaq_LSM303AGR accel;


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

bool LSM303AGR_setup()
{
  bool b = accel.checkWhoAmI();

  if (b) {
    accel.disableAccelerometer();
    accel.disableMagnetometer();
  }

  LOG_SETUP_RESULT_TEXT(b);

  return b;
}


/*******************************************************************************
 * Public
 ******************************************************************************/

bool LSM303AGR_isActive() { return accel.checkWhoAmI(); }

int8_t LSM303AGR_measureTemperature()
{
  accel.enableAccelerometer(Sodaq_LSM303AGR::LowPowerMode, Sodaq_LSM303AGR::HrNormalLowPower100Hz,
                            Sodaq_LSM303AGR::XYZ, Sodaq_LSM303AGR::Scale2g, true);

  vTaskDelay(pdMS_TO_TICKS(30));

  int8_t i = accel.getTemperature();

  accel.disableAccelerometer();

  return i;
}
