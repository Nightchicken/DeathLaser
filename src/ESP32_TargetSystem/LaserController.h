#ifndef LASER_CONTROLLER_H
#define LASER_CONTROLLER_H

#include <Arduino.h>
#include "CameraController.h"  // for TargetInfo

class LaserController {
public:
    LaserController();
    bool begin();
    void setTarget(const TargetInfo &target);
    void update(bool engage);
    
    // MOVED FROM PRIVATE TO PUBLIC:
    void fireLaser(bool on); 

private:
    bool initialized;
    int laserPin;
    TargetInfo currentTarget;
};

#endif // LASER_CONTROLLER_H
