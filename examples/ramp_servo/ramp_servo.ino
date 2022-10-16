#include <Arduino.h>                    // needed for PlatformIO

#define SERVO_PIN      9                // Servo motor PWM control pin
#define JOYSTICK_PIN  A0                // Joystick analog input
#define SPEED         60                // degree/s

#include <Servo.h>
Servo myservo;                          // create servo object to control a servo

#include <Ramp.h>
rampInt  myAngle;                       // create a byte ramp object to interpolate servo position

byte  previousAngle = 0;

void updateServo() {
  int val = myAngle.update();           // update ramp value
  Serial.println(myAngle.getValue());   // prompt angle value
  myservo.write(myAngle.getValue());    // transmit it to the servo
}

void updateJoystick(bool init = false) {
  int  val = map(analogRead(JOYSTICK_PIN), 0, 1023, 0, 180);              // read and map joystick value
  uint32_t duration = 1000.* (float)abs(myAngle.getValue()-val)/SPEED;    // calculate ramp duration according to speed
  
  if (init) {                                                             // start with going to the initial position
    myAngle.go(val);
    updateServo();
    delay(duration);
    return;
  } 
  
  if (abs(val-previousAngle) >=2) {
    myAngle.go(val, duration);                                            // set next ramp interpolation
    previousAngle = val;                                                  // save previous val
  }
}

void setup() {
  Serial.begin(115200);
  myservo.attach(SERVO_PIN);      // attaches the servo on pin 9 to the servo object
  updateJoystick(true);           // time to get to the first position
}


void loop() {
  updateJoystick();               // update joystick value
  updateServo();                  // update servo according to joystick value
  delay(10);                      // lazy way to limit update to 100 Hz
}
