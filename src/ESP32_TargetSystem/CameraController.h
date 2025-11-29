#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H
#include <Arduino.h>


struct TargetInfo {
    int x;
    int y;
    float confidence;   // 0.0 - 1.0
};

class CameraController {
public:
    ~CameraController();
    CameraController(const CameraController& obj) = delete;
    CameraController* getInstance();
    TargetInfo captureAndDetectTarget(float temp);
    void testOutput();

private:
    float frame[24*32*4];
    CameraController();
    float* getFrame();
    bool isConfigured;
    static CameraController* instancePtr;
    bool configureCamera();
};
#endif 
