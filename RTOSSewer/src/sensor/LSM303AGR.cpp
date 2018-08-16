#include "BMP280.h"

#if USE_LSM303AGR

#include <Sodaq_LSM303AGR.h>


/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define TEMPERATURE_SENSOR_OFFSET 25


/*******************************************************************************
 * State
 ******************************************************************************/

bool LSM303AGR_active = false;

#if USE_BOARD_TEMPERATURE
int8_t LSM303AGR_temperature = 0;
#endif

static Sodaq_LSM303AGR accel;


/*******************************************************************************
 * Lifecycle
 ******************************************************************************/

void LSM303AGR_setup()
{
  LSM303AGR_active = accel.checkWhoAmI();

  if (LSM303AGR_active) {
    accel.disableAccelerometer();
    accel.disableMagnetometer();
  }

  LOG_SETUP_RESULT_TEXT(LSM303AGR_active);
}


/*******************************************************************************
 * Public
 ******************************************************************************/

void LSM303AGR_measure()
{
  if (!LSM303AGR_active) { return; }

  #if USE_BOARD_TEMPERATURE
  accel.enableAccelerometer(Sodaq_LSM303AGR::LowPowerMode, Sodaq_LSM303AGR::HrNormalLowPower100Hz, Sodaq_LSM303AGR::XYZ,
                            Sodaq_LSM303AGR::Scale2g, true);
  vTaskDelay(pdMS_TO_TICKS(30));
  LSM303AGR_temperature = TEMPERATURE_SENSOR_OFFSET + accel.getTemperatureDelta();
  accel.disableAccelerometer();
  #endif
}


#endif