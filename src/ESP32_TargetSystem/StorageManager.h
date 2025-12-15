#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <Arduino.h>

// Assuming the CS pin for the SD Card is defined here or elsewhere (e.g., in ESP32_TargetSystem.ino)
// Since the compiler complained, let's define it here for global access.
#define SD_CS_PIN 5 

// Forward declarations to avoid circular dependencies and get necessary types
struct TargetInfo; // Forward declaration (TargetInfo is defined in CameraController.h)


class StorageManager {
public:
    // Singleton Access
    static StorageManager* getInstance();
    StorageManager(const StorageManager& obj) = delete; // Disable copy
    
    // Core SD Card Functions
    bool begin();
    void logEvent(String message);
    
    // Data Saving Functions
    void saveDetection(int x, int y, float confidence); // <-- Adjusted to match CPP
    bool saveThermalFrame(const float* frameData, size_t size);

private:
    // Private Constructor for Singleton pattern
    StorageManager();
    
    // Singleton instance pointer (REQUIRED static declaration)
    static StorageManager* instancePtr; 

    // Internal state (REQUIRED private member declaration)
    bool sd_card_present; 
};

#endif