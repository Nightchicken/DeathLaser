#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <Arduino.h>
#include "CameraController.h"   // for TargetInfo

class StorageManager {
public:
    StorageManager();

    // Mount filesystem, create dirs if needed
    bool begin();

    // Write a simple log line with timestamp
    void logEvent(const String &message);

    // Save target detection information
    bool saveDetection(const TargetInfo &target);

    // Periodic housekeeping (log rotation, flushing)
    void update();

private:
    bool initialized;

    String getTimestamp();
    void rotateLogsIfNeeded();
};

#endif // STORAGE_MANAGER_H
