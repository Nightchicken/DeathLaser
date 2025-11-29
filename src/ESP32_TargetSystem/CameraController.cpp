#include "CameraController.h"
#include <Wire.h> // Direct write to pins
#include <memory> // Smart pointers
#include <fstream>
#define SERIAL_DATA 0 //temp numbers
#define SERIAL_CLOCK 0 //temp numbers
#define I2C_DEV_ADDRES 0x33 //default number read value needs to be 128 + dev address, write value is less than 128
#define FRAME_LENGTH_BYTES 24*32


CameraController* CameraController::instancePtr = nullptr;

CameraController::~CameraController(){
  delete instancePtr;
}

CameraController::CameraController(){
  isConfigured = configureCamera();
  if (!isConfigured) {
    Serial.println("[Camera] Failed to configure camera.");
  }
  else{
    Serial.println("[Camera] Initialized.");
  }
}

CameraController* CameraController::getInstance(){
  if (instancePtr == nullptr){
    instancePtr = new CameraController();
  }
  return instancePtr;
}

void CameraController::testOutput(){
// NOTE: file output, store all the pixel data from camera frame
  std::ofstream outputFile;
  outputFile.open("testOutput.txt");
  if (outputFile.is_open()){
    outputFile << getFrame() << std::endl;
  }
  else{
    Serial.println("Write to file failed");
  }
  outputFile.close();
}

bool CameraController::configureCamera(){
  Wire.begin(I2C_DEV_ADDRES);
  return true;
}

float* CameraController::getFrame() {
  // Request a frame worth of bytes from the I2C device
  Wire.requestFrom(I2C_DEV_ADDRES, (uint8_t)FRAME_LENGTH_BYTES);

  int pixel = 0;
  while (Wire.available() && pixel < FRAME_LENGTH_BYTES) {
    uint8_t raw = Wire.read();        // read 1 byte
    frame[pixel] = static_cast<float>(raw);  // store as float
    ++pixel;                          // IMPORTANT: advance index
  }

  // Optional delay for debugging / slow polling – you can remove or tweak this
  delay(500);

  // Return pointer to the internal buffer
  return frame;
}


TargetInfo CameraController::captureAndDetectTarget(float temp){
  // NOTE: average sensor data, return location of hotspot with that temp
  // This function needs to be O(N^2)
  struct TargetInfo target{};
  if (!isConfigured) {
    return target;
  }
}

