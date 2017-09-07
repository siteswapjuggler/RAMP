#include <Ramp.h>						// include library

ramp myRamp;							// new ramp object (ramp<byte> by default)
long previousTime; 						// previous time

void setup() {
  Serial.begin(9600);					// begin Serial communication
  Serial.print("Value set to: ");		// print start value
  Serial.println(myRamp.go(0));			// set a value
  myRamp.go(255, 1000, BOUNCE_INOUT);	// set interpolation (value, time, interpolation mode)
}

void loop() {
	long actualTime = millis();						// 
	if (actualTime - previousTime > INTERVAL) {		// 
		Serial.println(myRamp.update());			// print updated value
		previousTime = actualTime;					// 
	}
}