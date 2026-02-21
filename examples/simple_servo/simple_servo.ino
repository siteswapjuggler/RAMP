#include <Arduino.h>
#include <Servo.h>
#include <Ramp.h>

#define SERVO_PIN      9
#define MIN_ANGLE     30
#define MAX_ANGLE     90
#define PAUSE       1000
#define DURATION    5000

Servo myservo;
rampInt  myAngle;

int  previousAngle = 0;

void setup() {
  myservo.attach(SERVO_PIN);
  myAngle.go(MIN_ANGLE);
}


void loop() {
  int newAngle = myAngle.update();
  if (newAngle != previousAngle) {
    myservo.write(myAngle.update());
  }

  if (myAngle.isFinished()) {
    delay(PAUSE);
    if (myAngle.getTarget() == MAX_ANGLE) {
      myAngle.go(MIN_ANGLE, DURATION);
    } else {
      myAngle.go(MAX_ANGLE, DURATION);
    }
  }
}
