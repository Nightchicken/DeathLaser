#include <Arduino.h>
#include <Servo.h>

// ----- Servo Setup -----
Servo servo_bottom;
Servo servo_top;

// ----- Function Declaration -----
void patrol();

void setup() {
  servo_bottom.attach(9, 500, 2500);
  servo_top.attach(8, 500, 2500);

  Serial.begin(9600);
  Serial.println("Starting patrol...");
}

void loop() {
  patrol();
}

// ----- Function Definition -----
void patrol() {
  for (int bottomPos = 0; bottomPos <= 180; bottomPos += 20) {
    servo_bottom.write(bottomPos);
    delay(15);

    for (int topPos = 0; topPos <= 180; topPos++) {
      servo_top.write(topPos);
      delay(10);
    }
  }

  for (int bottomPos = 180; bottomPos >= 0; bottomPos -= 20) {
    servo_bottom.write(bottomPos);
    delay(15);

    for (int topPos = 0; topPos <= 180; topPos++) {
      servo_top.write(topPos);
      delay(10);
    }
  }
}


