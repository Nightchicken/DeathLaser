#include "StorageManager.h"
#include <SD.h>
#include <time.h> 
#include <Arduino.h> 

// Initialize the static instance pointer
StorageManager* StorageManager::instancePtr = nullptr;

// =================================================================
// 1. SINGLETON & SETUP
// =================================================================
StorageManager::StorageManager() {
    sd_card_present = false;
}

StorageManager* StorageManager::getInstance() {
    if (instancePtr == nullptr) {
        instancePtr = new StorageManager();
    }
    return instancePtr;
}

bool StorageManager::begin() {
    Serial.printf("[Storage] Initializing SD card on CS pin %d...\n", SD_CS_PIN);

    // Initialize SD card
    if (!SD.begin(SD_CS_PIN)) {
        Serial.println("[Storage] SD CARD FAILED, OR NOT PRESENT!");
        sd_card_present = false;
        return false;
    }
    
    Serial.println("[Storage] SD CARD INITIALIZED.");
    sd_card_present = true;
    
    // Create/Append to a main system log to confirm startup
    logEvent("System initialized. Storage ready.");
    return true;
}

// =================================================================
// 2. LOGGING & DETECTIONS
// =================================================================
void StorageManager::logEvent(String message) {
    if (!sd_card_present) return;

    File file = SD.open("/events.log", FILE_APPEND);
    if (file) {
        // You can add a timestamp here if you have NTP setup
        file.println(message);
        file.close();
    } else {
        Serial.println("[Storage] Error opening events.log");
    }
    // Also print to Serial for debugging
    Serial.println("[LOG] " + message);
}

void StorageManager::saveDetection(int x, int y, float confidence) {
    if (!sd_card_present) return;

    // Save to a CSV file for easy Excel/Python analysis
    File file = SD.open("/detections.csv", FILE_APPEND);
    if (file) {
        // Format: Timestamp(placeholder), X, Y, Confidence
        file.printf("%lu,%d,%d,%.2f\n", millis(), x, y, confidence);
        file.close();
    }
}

// =================================================================
// 3. THERMAL FRAME SAVER (NEW)
// =================================================================
bool StorageManager::saveThermalFrame(const float* frameData, size_t size) {
    if (!sd_card_present) {
        logEvent("ERROR: Cannot save frame, SD card not ready.");
        return false;
    }
    
    // Generate a unique filename based on millis() to avoid overwriting
    // Ideally, use real time (time_t) if NTP is active.
    char filename[32];
    snprintf(filename, sizeof(filename), "/FRAME_%lu.txt", millis());
    
    File file = SD.open(filename, FILE_WRITE);
    if (!file) {
        logEvent("ERROR: Failed to open frame file: " + String(filename));
        return false;
    }

    // Write all 768 temperature values
    for (size_t i = 0; i < size; ++i) {
        file.print(frameData[i], 2); 
        if (i < size - 1) {
            file.print(","); // Comma delimiter
        }
    }
    
    file.close();
    logEvent("Saved thermal frame: " + String(filename));
    return true;
}