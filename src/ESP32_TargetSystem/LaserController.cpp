#include "LaserController.h"
// #include <Servo.h>  // Uncomment if using servos

LaserController::LaserController()
  : initialized(false),
    laserPin(4),       // Example GPIOs, change as needed
    panServoPin(14),
    tiltServoPin(15)
{
  currentTarget = {0, 0, 0.0f};
}

bool LaserController::begin() {
  pinMode(laserPin, OUTPUT);
  digitalWrite(laserPin, LOW);

  // If using servos:
  // panServo.attach(panServoPin);
  // tiltServo.attach(tiltServoPin);

  initialized = true;
  Serial.println("[Laser] Controller initialized.");
  return true;
}

void LaserController::setTarget(const TargetInfo &target) {
  currentTarget = target;
}

void LaserController::update(bool engage) {
  if (!initialized) return;

  if (engage && currentTarget.confidence > 0.5f) {
    aimAtTarget(currentTarget);
    fireLaser(true);
  } else {
    fireLaser(false);
  }
}

void LaserController::aimAtTarget(const TargetInfo &target) {
  // TODO: Map target.x, target.y → servo angles or mirror offsets.
  // Example (placeholder): just log the target
  Serial.print("[Laser] Aiming at (");
  Serial.print(target.x);
  Serial.print(", ");
  Serial.print(target.y);
  Serial.println(")");
}

void LaserController::fireLaser(bool on) {
  digitalWrite(laserPin, on ? HIGH : LOW);
  // Optional: print for debugging
  // Serial.println(on ? "[Laser] FIRE" : "[Laser] SAFE");
}


