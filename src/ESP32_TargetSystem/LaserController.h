#ifndef LASER_CONTROLLER_H
#define LASER_CONTROLLER_H

#include <Arduino.h>
#include "CameraController.h"  // for TargetInfo

class LaserController {
public:
    LaserController();

    // Initialize laser GPIO, PWM, etc.
    bool begin();

    // Set a new target for aiming
    void setTarget(const TargetInfo &target);

    // Update laser state (called from loop regularly)
    // engage = true  → aim & fire
    // engage = false → safe/off
    void update(bool engage);

private:
    bool initialized;
    TargetInfo currentTarget;

    // Pin configuration (adjust to your wiring)
    int laserPin;
    int panServoPin;
    int tiltServoPin;

    // Optional: servo objects if using servos:
    // Servo panServo;
    // Servo tiltServo;

    void aimAtTarget(const TargetInfo &target);
    void fireLaser(bool on);
};

#endif // LASER_CONTROLLER_H
