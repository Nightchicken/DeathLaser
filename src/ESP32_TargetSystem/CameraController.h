#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H
#include <Arduino.h>

// MLX90640 Constants (32x24 pixels)
#define MLX_PIXEL_ROWS    24
#define MLX_PIXEL_COLS    32
#define MLX_FRAME_SIZE    (MLX_PIXEL_ROWS * MLX_PIXEL_COLS) // 768

struct TargetInfo {
    int x;
    int y;
    float confidence;   // 0.0 - 1.0
};

class CameraController {
public:
    // Singleton Management
    ~CameraController();
    CameraController(const CameraController& obj) = delete; // Disable copy constructor
    
    // >>>>>> FIXED: Added 'static' keyword here <<<<<<
    static CameraController* getInstance();

    // Core Logic
    TargetInfo captureAndDetectTarget(float temp);
    void testOutput();
    
    // Frame Access for StorageManager
    const float* getConstFrame() const; 

private:
    // Private Constructor for Singleton pattern
    CameraController();

    // Internal Frame Buffer (768 floats for temperature)
    float frame[MLX_FRAME_SIZE]; 
    
    // Internal State
    bool isConfigured;
    static CameraController* instancePtr;
    
    // Internal Methods
    bool configureCamera();
    float* getFrame();
};
#endif
