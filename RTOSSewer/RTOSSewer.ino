#include <Sodaq_LSM303AGR.h>
#include "src/RTOSSewer.h"

static StaticTask_t xT1TaskBuffer;
static StaticTask_t xT2TaskBuffer;
static StaticTask_t xT3TaskBuffer;

static StackType_t  xT1Stack[configMINIMAL_STACK_SIZE];
static StackType_t  xT2Stack[configMINIMAL_STACK_SIZE];
static StackType_t  xT3Stack[configMINIMAL_STACK_SIZE];

StaticSemaphore_t xSemaphoreBuffer;

SemaphoreHandle_t xSemaphore = NULL;

void setup() {
  setupPins();

  Wire.begin();
  SerialUSB.begin(9600);
  delay(200);
  while (!SerialUSB && millis() < 10000);

  xTaskCreateStatic(thread1, NULL, configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, xT1Stack, &xT1TaskBuffer);
  xTaskCreateStatic(thread2, NULL, configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, xT2Stack, &xT2TaskBuffer);
  xTaskCreateStatic(thread3, NULL, configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, xT3Stack, &xT3TaskBuffer);
  vTaskStartScheduler();
}

void loop() {
  SerialUSB.print(millis());
  SerialUSB.println(" idle");
  delay(1000);
}

static void setupPins()
{
  pinMode(LED_RED,   OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE,  OUTPUT);

  digitalWrite(LED_RED  , HIGH);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_BLUE , HIGH);
}

static void thread1(void* pvParameters)
{
  static int i = LOW;

  while (1) {
    digitalWrite(LED_GREEN, i);
    vTaskDelay(pdMS_TO_TICKS(1000));
    i ^= 1;
  }
}

static void thread2(void* pvParameters)
{
  Sodaq_LSM303AGR accel;

  accel.rebootAccelerometer();
  vTaskDelay(pdMS_TO_TICKS(1000));

  accel.enableAccelerometer();

  pinMode(ACCEL_INT1, INPUT);
  attachInterrupt(ACCEL_INT1, handleInterruptAccel, RISING);

  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(GCM_EIC) | GCLK_CLKCTRL_GEN_GCLK1 | GCLK_CLKCTRL_CLKEN;

  accel.enableInterrupt1(accel.ZLow, -0.8, 0, accel.PositionRecognition);

  while (1) {
    SerialUSB.print(millis());
    SerialUSB.print(" x = ");
    SerialUSB.print(accel.getX());
    SerialUSB.print(" y = ");
    SerialUSB.print(accel.getY());
    SerialUSB.print(" z = ");
    SerialUSB.println(accel.getZ());

    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

static void thread3(void* pvParameters)
{
  static int last = 0;

  xSemaphore = xSemaphoreCreateBinaryStatic(&xSemaphoreBuffer);

  while (1) {
    if (xSemaphoreTake(xSemaphore, -1) != pdTRUE) { continue; }

    int i = millis();
    if (i - last < 200) { continue; }

    SerialUSB.print(i);
    SerialUSB.println(" board flipped");
    last = i;
  }
}

static void handleInterruptAccel()
{
  if (xSemaphore == NULL) { return; }

  static BaseType_t xHigherPriorityTaskWoken;

  xHigherPriorityTaskWoken = pdFALSE;

  xSemaphoreGiveFromISR(xSemaphore, &xHigherPriorityTaskWoken);
}
