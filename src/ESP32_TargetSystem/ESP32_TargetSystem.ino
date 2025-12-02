#include <Arduino.h>
#include <ESP32Servo.h>
#include "SystemInit.h"
#include "CameraController.h"
#include "LaserController.h"
#include "StorageManager.h"


//GPIO 22 is our output
// GPIO 19 is our input

Servo panServo;
Servo tiltServo;

// pick actual pins you wired to the servo signal lines
const int PAN_SERVO_PIN  = 23;
const int TILT_SERVO_PIN = 19;

 #define SERVO_TEST_MODE 1

void patrolServos(Servo &pan, Servo &tilt) {
  static int panPos  = 90;
  static int tiltPos = 90;
  static int panDir  = 1;
  static int tiltDir = -1;

  const int MIN_ANGLE = 30;
  const int MAX_ANGLE = 150;
  const int STEP = 2;

  panPos  += STEP * panDir;
  tiltPos += STEP * tiltDir;

  if (panPos >= MAX_ANGLE || panPos <= MIN_ANGLE) panDir *= -1;
  if (tiltPos >= MAX_ANGLE || tiltPos <= MIN_ANGLE) tiltDir *= -1;

  pan.write(panPos);
  tilt.write(tiltPos);

  delay(40);
}


// Global module objects
CameraController* cameraController = nullptr;  // singleton pointer
LaserController   laserController;
StorageManager    storageManager;

void setup() {

  // 1. Basic hardware/peripheral init (pins, serial, etc.)
  SystemInit::beginSerial(115200);
  SystemInit::printBanner();

  // Attach servos for test
  panServo.attach(PAN_SERVO_PIN);
  tiltServo.attach(TILT_SERVO_PIN);


  // 2. Initialize subsystems
  SystemInit::initGPIO();
  SystemInit::initCameraPower();    // OPTIONAL: if you have a camera power pin

  if (!storageManager.begin()) {
    Serial.println("[ERROR] Storage init failed.");
  }

  // --- IMPORTANT: use getInstance() as defined in your header/cpp ---
  // getInstance() is a NON-static member, and the ctor is private.
  // The only way to call it without changing the header is via a pointer.
  //
  // This compiles and works with your implementation because getInstance()
  // does not use `this` and only touches the static `instancePtr`.
  // (Formally UB in C++, but it matches your class as-is.)
  cameraController = cameraController->getInstance();

  if (cameraController == nullptr) {
    Serial.println("[ERROR] CameraController::getInstance() returned null.");
  }

  if (!laserController.begin()) {
    Serial.println("[ERROR] Laser controller init failed.");
  }

  storageManager.logEvent("System setup complete.");
}

void loop() {

  #if SERVO_TEST_MODE
  // Run ONLY servo patrol while testing; rest of logic is skipped.
  patrolServos(panServo, tiltServo);
  return;
  #endif
  static unsigned long lastLoopMs = 0;
  unsigned long now = millis();

  // Run this logic every 50 ms (adjust as needed)
  if (now - lastLoopMs < 50) {
    return;
  }
  lastLoopMs = now;

  if (!cameraController) {
    // If the singleton wasn't created, there's nothing useful to do
    return;
  }

  // ------------------------------------------------------------------
  // 1. Poll camera for a target
  // ------------------------------------------------------------------
  // Your current API is:
  //   TargetInfo captureAndDetectTarget(float temp);
  //
  // So we MUST pass a float and get a TargetInfo back.
  // We'll use a placeholder temperature value for now.
  float desiredTemp = 0.0f;   // TODO: decide what this should mean in your logic

  TargetInfo target = cameraController->captureAndDetectTarget(desiredTemp);

  // Decide whether a "real" target exists based on confidence
  bool hasTarget = (target.confidence > 0.0f);

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

