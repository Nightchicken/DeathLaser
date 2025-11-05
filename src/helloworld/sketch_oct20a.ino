#include <Servo.h>

Servo servo_bottom;
Servo servo_top;

void setup() {
  servo_bottom.attach(9, 500, 2500);
  servo_top.attach(8, 500, 2500);
}

void loop() {
  patrol();
}

void patrol() {
  // Sweep bottom servo from 0 to 180
  for (int bottomPos = 0; bottomPos <= 180; bottomPos+=20) {
    servo_bottom.write(bottomPos);
    delay(15);

    // Sweep top servo from 0 to 180
    for (int topPos = 0; topPos <= 180; topPos++) {
      servo_top.write(topPos);
      delay(10);
    }

  // Sweep bottom servo back from 180 to 0
  for (int bottomPos = 180; bottomPos >= 0; bottomPos-=20) {
    servo_bottom.write(bottomPos);
    delay(15);

    // Repeat top servo sweep
    for (int topPos = 0; topPos <= 180; topPos++) {
      servo_top.write(topPos);
      delay(10);
    }
  }
  }
}
