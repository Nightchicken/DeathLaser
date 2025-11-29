#include <Arduino.h>
#include "SystemInit.h"
#include "CameraController.h"
#include "LaserController.h"
#include "StorageManager.h"

// Global module objects
CameraController cameraController;
LaserController  laserController;
StorageManager   storageManager;

void setup() {
  // 1. Basic hardware/peripheral init (pins, serial, etc.)
  SystemInit::beginSerial(115200);
  SystemInit::printBanner();

  // 2. Initialize subsystems
  SystemInit::initGPIO();
  SystemInit::initCameraPower();    // OPTIONAL: if you have a camera power pin

  if (!storageManager.begin()) {
    Serial.println("[ERROR] Storage init failed.");
  }

  if (!cameraController.begin()) {
    Serial.println("[ERROR] Camera init failed.");
  }

  if (!laserController.begin()) {
    Serial.println("[ERROR] Laser controller init failed.");
  }

  storageManager.logEvent("System setup complete.");
}

void loop() {
  static unsigned long lastLoopMs = 0;
  unsigned long now = millis();

  // Run this logic every 50 ms (adjust as needed)
  if (now - lastLoopMs < 50) {
    return;
  }
  lastLoopMs = now;

  // 1. Poll camera for a target
  TargetInfo target;
  bool hasTarget = cameraController.captureAndDetectTarget(target);

  if (hasTarget) {
    // 2. Aim and fire lasers
    laserController.setTarget(target);
    laserController.update(true);   // true = engage
    storageManager.saveDetection(target);
    storageManager.logEvent("Target detected and lasers engaged.");
  } else {
    // 3. No target → make sure lasers are idle
    laserController.update(false);  // false = disengage
  }

  // 4. Optional periodic housekeeping
  storageManager.update();
}
