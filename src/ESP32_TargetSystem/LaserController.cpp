#include "LaserController.h"

// Constructor
LaserController::LaserController() 
  : initialized(false),
    laserPin(4)       // GPIO 4 for Laser (Check your wiring!)
{
    currentTarget = {0, 0, 0.0f};
}

bool LaserController::begin() {
    pinMode(laserPin, OUTPUT);
    digitalWrite(laserPin, LOW); // Ensure laser is OFF at startup

    initialized = true;
    Serial.println("[Laser] Controller initialized.");
    return true;
}

void LaserController::setTarget(const TargetInfo &target) {
    currentTarget = target;
}

// Update loop: Decides whether to pull the trigger
void LaserController::update(bool engage) {
    if (!initialized) return;

    if (engage && currentTarget.confidence > 0.6f) {
        // We are engaged and have a good target
        fireLaser(true);
    } else {
        // Safety: Always default to off
        fireLaser(false);
    }
}

// "aimAtTarget" removed because Patrol.cpp handles movement now.

void LaserController::fireLaser(bool on) {
    digitalWrite(laserPin, on ? HIGH : LOW);
}
