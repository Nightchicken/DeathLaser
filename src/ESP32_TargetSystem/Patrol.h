#ifndef PATROL_H
#define PATROL_H

#include <Arduino.h>
#include <ESP32Servo.h>

class Patrol {
public:
    Patrol();
    void begin(int panPin, int tiltPin);
    void update();
    void onTargetFound();
    
    // NEW: Returns true if we are currently in the 5-second pause
    bool isLocked(); 

private:
    Servo panServo;
    Servo tiltServo;

    // Settings
    const int MIN_ANGLE = 30;
    const int MAX_ANGLE = 150;
    const int STEP_SIZE = 2;
    const int MOVE_INTERVAL = 40; 
    const int PAUSE_DURATION = 5000; // 5 Seconds

    // State
    int panPos, tiltPos;
    int panDir, tiltDir;
    unsigned long lastMoveTime;
    unsigned long targetLockTime;
    bool _isPaused; // Internal flag
};
#endif