#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include <Arduino.h>

// If using ESP32-CAM:
// #include "esp_camera.h"

// Shared target struct
struct TargetInfo {
    int x;
    int y;
    float confidence;   // 0.0 - 1.0
};

class CameraController {
public:
    CameraController();

    // Initialize camera hardware (pins, resolution, etc.)
    bool begin();

    // Capture frame and detect a target in one shot.
    // Returns true if a valid target was detected.
    bool captureAndDetectTarget(TargetInfo &outTarget);

    // Optional: separate steps if someone wants to expand later.
    // bool captureFrame();
    // bool detectTarget(TargetInfo &outTarget);

private:
    bool initialized;

    // Example of internal helper methods
    bool configureCamera();
    bool mockDetect(TargetInfo &outTarget);  // placeholder detection
};

#endif // CAMERA_CONTROLLER_H
