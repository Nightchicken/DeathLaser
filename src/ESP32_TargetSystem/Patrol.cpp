#include "Patrol.h"

Patrol::Patrol() {
    panPos = 90; tiltPos = 90;
    panDir = 1;  tiltDir = -1;
    lastMoveTime = 0;
    targetLockTime = 0;
    _isPaused = false;
}

void Patrol::begin(int panPin, int tiltPin) {
    panServo.attach(panPin);
    tiltServo.attach(tiltPin);
    panServo.write(panPos);
    tiltServo.write(tiltPos);
}

bool Patrol::isLocked() {
    return _isPaused;
}

void Patrol::update() {
    unsigned long now = millis();

    // 1. If Locked (Paused), check if 5 seconds have passed
    if (_isPaused) {
        if (now - targetLockTime >= PAUSE_DURATION) {
            _isPaused = false; // Time's up! Resume patrol.
            Serial.println("[Patrol] Resume patrolling.");
        }
        return; // Don't move servos while paused
    }

    // 2. Normal Patrol Movement
    if (now - lastMoveTime >= MOVE_INTERVAL) {
        lastMoveTime = now;

        panPos += (STEP_SIZE * panDir);
        
        // Reverse at edges
        if (panPos >= MAX_ANGLE || panPos <= MIN_ANGLE) {
            panDir *= -1;
            // Optional: Nudge tilt slightly at edges for better coverage
            tiltPos += 5 * tiltDir;
            if (tiltPos >= 110 || tiltPos <= 70) tiltDir *= -1;
        }

        panServo.write(panPos);
        tiltServo.write(tiltPos);
    }
}

void Patrol::onTargetFound() {
    if (!_isPaused) {
        _isPaused = true;
        targetLockTime = millis();
        Serial.println("[Patrol] TARGET LOCKED. Pausing 5 seconds.");
    }
}