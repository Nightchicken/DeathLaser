#include "SystemInit.h"

namespace SystemInit {

  // Example pins; adjust to your hardware
  static const int STATUS_LED_PIN   = 2;   // On-board LED on some ESP32 dev boards
  static const int CAMERA_PWR_PIN   = 12;  // If you gate camera power via MOSFET

  void beginSerial(unsigned long baud) {
    Serial.begin(baud);
    // Wait a moment for serial connection (optional)
    delay(100);
  }

  void printBanner() {
    Serial.println();
    Serial.println("========================================");
    Serial.println("   ESP32 Target System - Start");
    Serial.println("========================================");
  }

  void initGPIO() {
    pinMode(STATUS_LED_PIN, OUTPUT);
    digitalWrite(STATUS_LED_PIN, LOW);
  }

  void initCameraPower() {
    pinMode(CAMERA_PWR_PIN, OUTPUT);
    digitalWrite(CAMERA_PWR_PIN, HIGH);  // Power ON camera
  }

} // namespace SystemInit
