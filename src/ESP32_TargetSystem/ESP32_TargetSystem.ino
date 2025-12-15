#include <Arduino.h>
#include "SystemInit.h"
#include "CameraController.h"
#include "LaserController.h"
#include "StorageManager.h"
#include "Patrol.h"

// ================================================================
// CONFIGURATION
// ================================================================
const int PAN_SERVO_PIN  = 25;
const int TILT_SERVO_PIN = 26;
const int SERIAL_BAUD    = 115200;

// ================================================================
// GLOBAL OBJECTS
// ================================================================
Patrol            patrol;            // Handles servo movement and pausing
LaserController   laserController;   // Handles laser firing logic
CameraController* cameraController = nullptr; // Singleton pointer
StorageManager* storageManager   = nullptr; // Singleton pointer

// ================================================================
// SETUP
// ================================================================
void setup() {
  // 1. Basic System Init
  SystemInit::beginSerial(SERIAL_BAUD);
  SystemInit::printBanner();

  // 2. Initialize Storage Singleton
  storageManager = StorageManager::getInstance();
  if (!storageManager->begin()) {
    Serial.println("[Error] SD Card failed or missing.");
  } else {
    storageManager->logEvent("System boot started.");
  }

  // 3. Initialize Patrol System
  // This automatically attaches servos and moves them to start position
  patrol.begin(PAN_SERVO_PIN, TILT_SERVO_PIN);
  Serial.println("[System] Patrol initialized.");

  // 4. Initialize Power & GPIO
  SystemInit::initGPIO();
  SystemInit::initCameraPower();

  // 5. Initialize Camera Singleton
  cameraController = CameraController::getInstance();
  if (!cameraController) {
    Serial.println("[CRITICAL] CameraController returned null!");
  }

  // 6. Initialize Laser Controller
  if (!laserController.begin()) {
    Serial.println("[Error] Laser controller init failed.");
  }

  if (storageManager) storageManager->logEvent("System setup complete.");
}

// ================================================================
// LOOP
// ================================================================
void loop() {
  // 1. UPDATE PATROL
  // This manages the servo movement AND the 5-second internal timer.
  patrol.update();

  // 2. CHECK LOCK STATE
  // If the patrol is "locked" (paused because we saw a target), 
  // we skip detection and just keep the signal active.
  if (patrol.isLocked()) {
    // Keep the laser ON to signal "I saw something"
    laserController.fireLaser(true); 
    
    // Exit loop early. We don't want to detect or save new photos 
    // while we are waiting for the 5-second timer to finish.
    return; 
  }

  // --- If we get here, we are actively searching ---

  // 3. LOOP RATE LIMITER
  // Camera reads are slow (~60ms), so we limit the loop speed to match.
  static unsigned long lastLoopMs = 0;
  unsigned long now = millis();
  if (now - lastLoopMs < 60) {
    return; 
  }
  lastLoopMs = now;

  // Safety Checks
  if (!cameraController || !storageManager) return;

  // 4. DETECT TARGET
  TargetInfo target = cameraController->captureAndDetectTarget(0.0f);
  
  // 5. DECISION LOGIC
  bool hasTarget = (target.confidence > 0.6f);

  if (hasTarget) {
    // --- TARGET ACQUIRED ---
    
    // A. Freeze the patrol (starts the 5-second timer inside Patrol class)
    patrol.onTargetFound();

    // B. Signal Immediately (Laser ON)
    laserController.fireLaser(true); 

    // C. Save Data & Thermal Photo (Happens once per detection event)
    storageManager->saveDetection(target.x, target.y, target.confidence);
    storageManager->saveThermalFrame(cameraController->getConstFrame(), 768);
    
    // Log it
    String logMsg = "Target Locked. Conf: " + String(target.confidence);
    storageManager->logEvent(logMsg);
    Serial.println(logMsg);

  } else {
    // --- NO TARGET ---
    // Ensure lasers are OFF while searching
    laserController.fireLaser(false);
  }
}