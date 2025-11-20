#include "StorageManager.h"
#include <FS.h>
#include <SPIFFS.h>

StorageManager::StorageManager()
  : initialized(false)
{
}

bool StorageManager::begin() {
  if (!SPIFFS.begin(true)) {
    Serial.println("[Storage] SPIFFS mount failed.");
    initialized = false;
    return false;
  }

  initialized = true;
  Serial.println("[Storage] SPIFFS initialized.");
  return true;
}

void StorageManager::logEvent(const String &message) {
  if (!initialized) return;

  File logFile = SPIFFS.open("/events.log", FILE_APPEND);
  if (!logFile) {
    Serial.println("[Storage] Failed to open events.log for appending.");
    return;
  }

  String line = getTimestamp() + " - " + message + "\n";
  logFile.print(line);
  logFile.close();
}

bool StorageManager::saveDetection(const TargetInfo &target) {
  if (!initialized) return false;

  File detFile = SPIFFS.open("/detections.csv", FILE_APPEND);
  if (!detFile) {
    Serial.println("[Storage] Failed to open detections.csv.");
    return false;
  }

  // Simple CSV: timestamp,x,y,confidence
  detFile.print(getTimestamp());
  detFile.print(",");
  detFile.print(target.x);
  detFile.print(",");
  detFile.print(target.y);
  detFile.print(",");
  detFile.println(target.confidence);

  detFile.close();
  return true;
}

void StorageManager::update() {
  if (!initialized) return;
  // For later: rotate logs, check free space, etc.
  rotateLogsIfNeeded();
}

String StorageManager::getTimestamp() {
  // TODO: If you have NTP or RTC, return real time.
  // For now, just use millis().
  unsigned long ms = millis();
  return String(ms);
}

void StorageManager::rotateLogsIfNeeded() {
  // TODO: implement max-size log rotation if needed.
  // For now, do nothing.
}
