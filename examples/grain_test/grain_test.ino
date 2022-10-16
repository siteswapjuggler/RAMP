#include <Arduino.h>                          // needed for PlatformIO
#include <Ramp.h>                             // include library

int lastValue;                                // global variable
rampInt myRamp;                               // new int ramp object

void setup() {
  Serial.begin(115200);                       // begin Serial communication
  myRamp.setGrain(1);                         // set grain to 1 ms
  myRamp.go(5000);                            // set value to directly to 5000
  myRamp.go(0, 15, LINEAR, LOOPFORWARD);      // go to 0 in 15 ms
}

void loop() {
  int val = myRamp.update();                  // store updated value
  if (val != lastValue) {                     // print updated value and completion percentage
    Serial.print(val);
    Serial.print("\t|\t");
    Serial.print(myRamp.getCompletion());
    Serial.print(" %");
    Serial.println();
  }
  lastValue = val;
}
