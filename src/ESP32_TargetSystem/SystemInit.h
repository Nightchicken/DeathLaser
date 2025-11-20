#ifndef SYSTEM_INIT_H
#define SYSTEM_INIT_H

#include <Arduino.h>

namespace SystemInit {

  // Initialize serial with given baud
  void beginSerial(unsigned long baud);

  // Print a banner at startup
  void printBanner();

  // Configure board-level GPIO (power pins, status LEDs, etc.)
  void initGPIO();

  // If you power the camera via a GPIO pin, set it up here.
  void initCameraPower();

} // namespace SystemInit

#endif // SYSTEM_INIT_H
