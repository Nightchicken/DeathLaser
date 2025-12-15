#include "CameraController.h" 
#include <Wire.h> 
#include <memory>
#include <fstream>
#include <Arduino.h> 

// >>>>>> SWITCHED TO ADAFRUIT LIBRARY <<<<<<
#include <Adafruit_MLX90640.h>

// Define the sensor object
static Adafruit_MLX90640 mlx; 

// =================================================================
// 1. STATIC MEMBER INITIALIZATION
// =================================================================
CameraController* CameraController::instancePtr = nullptr; 

// =================================================================
// 2. CONSTRUCTOR AND DESTRUCTOR
// =================================================================
CameraController::CameraController(){
    isConfigured = configureCamera();
    if (!isConfigured) {
        Serial.println("[Camera] Failed to configure camera.");
    } else {
        Serial.println("[Camera] Initialized.");
    }
}

CameraController::~CameraController(){
}

// =================================================================
// 3. SINGLETON GETTER
// =================================================================
CameraController* CameraController::getInstance(){
    if (instancePtr == nullptr){
        instancePtr = new CameraController(); 
    }
    return instancePtr;
}

// =================================================================
// 4. CORE CAMERA METHODS (Updated for Adafruit)
// =================================================================

bool CameraController::configureCamera(){
    // 1. Initialize I2C 
    Wire.begin(); 
    Wire.setClock(1000000); // 1 MHz clock for high FPS
    
    Serial.print("[Camera] Initializing MLX90640 (Adafruit Lib)...");
    
    // 2. Initialize Sensor (Adafruit uses begin() with address and Wire pointer)
    if (!mlx.begin(MLX90640_I2CADDR_DEFAULT, &Wire)) {
        Serial.println("FAILED. Check wiring!");
        isConfigured = false;
        return false;
    }
    
    // 3. Set Refresh Rate (Adafruit uses specific enums)
    mlx.setMode(MLX90640_CHESS); // 'Chess' mode is generally better for accuracy
    mlx.setResolution(MLX90640_ADC_18BIT);
    mlx.setRefreshRate(MLX90640_16_HZ);
    
    Serial.println("SUCCESS.");
    isConfigured = true;
    return true; 
}

float* CameraController::getFrame() {
    if (!isConfigured) {
        Serial.println("[Camera] ERROR: Sensor not configured. Returning default frame.");
        for (int i = 0; i < MLX_FRAME_SIZE; ++i) frame[i] = 20.0f; 
        return frame;
    }

    // Adafruit's getFrame takes the float array directly
    // It returns 0 on success
    if (mlx.getFrame(frame) != 0) {
        Serial.println("[Camera] ERROR: Read failed.");
        return frame; 
    }
    
    // Optional: Print center temp for debug
    // Serial.printf("[Camera] Frame captured. Center T: %.1f C\n", frame[12*32 + 16]);

    return frame;
}

const float* CameraController::getConstFrame() const {
    return frame;
}

void CameraController::testOutput(){
    Serial.println("[Camera] testOutput() stub running.");
}

// =================================================================
// 5. TARGET DETECTION (Unchanged)
// =================================================================
TargetInfo CameraController::captureAndDetectTarget(float desiredTemp){
    // 1. Capture a new frame
    float* currentFrame = getFrame();
    
    // 2. Normalization Setup
    float T_min = 999.0f; 
    float T_max = -999.0f; 
    
    // Find Min/Max
    for (int i = 0; i < MLX_FRAME_SIZE; ++i) {
        if (currentFrame[i] < T_min) T_min = currentFrame[i];
        if (currentFrame[i] > T_max) T_max = currentFrame[i];
    }
    
    float T_range = T_max - T_min;
    if (T_range < 0.1f) T_range = 1.0f; // Prevent div by zero

    // 3. Find Max Contrast
    float normalized_detection_threshold = 0.6f; 
    float max_normalized_value = 0.0f;
    int hotIndex = -1;

    for (int i = 0; i < MLX_FRAME_SIZE; ++i) {
        float T_normalized = (currentFrame[i] - T_min) / T_range; 
        
        if (T_normalized > max_normalized_value) {
            max_normalized_value = T_normalized;
            hotIndex = i;
        }
    }

    // 4. Initialize target 
    struct TargetInfo target = {0, 0, 0.0f};

    // 5. Set target if threshold met
    if (max_normalized_value > normalized_detection_threshold) {
        target.x = hotIndex % MLX_PIXEL_COLS;  
        target.y = hotIndex / MLX_PIXEL_COLS;  
        target.confidence = constrain(max_normalized_value, 0.0f, 1.0f); 
    }
    
    Serial.printf("[Camera] Min:%.1f Max:%.1f Conf:%.2f @ (%d, %d)\n", 
                  T_min, T_max, target.confidence, target.x, target.y);

    return target;
}