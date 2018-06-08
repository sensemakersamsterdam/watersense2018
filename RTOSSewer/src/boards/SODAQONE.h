#ifndef SODAQONE_BOARD_H
#define SODAQONE_BOARD_H

class SODAQONE {
public:
  static void setup();
  static void toggleLedBlue();
  static void toggleLedGreen();
  static void toggleLedRed();
  static void turnOffLedBlue();
  static void turnOffLedGreen();
  static void turnOffLedRed();
  static void turnOnLedBlue();
  static void turnOnLedGreen();
  static void turnOnLedRed();
private:
  static uint8_t ledState;
  static void setLed(uint8_t pin, uint8_t state);
};

#endif
