#include <Arduino.h>                        // needed for PlatformIO
#include <Ramp.h>                           // include library
ramp myRamp;                                // new ramp object (ramp<unsigned char> by default)

void setup() {
  Serial.begin(9600);                       // begin Serial communication
  
  Serial.print("Value start at: ");         //
  Serial.println(myRamp.getValue());        // ramp object instantiate with a value of O

  Serial.println("Starting interpolation"); //  
  myRamp.go(255, 30000);                    // start interpolation (value to go to, duration in ms)
}

void loop() {
  Serial.print("Actual value is: ");        // 
  Serial.println(myRamp.update());          // update() return the actual interpolation value
  delay(100);                               //
}
