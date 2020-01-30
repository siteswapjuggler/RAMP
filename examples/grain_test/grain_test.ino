#include <Ramp.h>

int lastValue;
rampInt myRamp;

void setup() {
  Serial.begin(2000000);
  myRamp.setGrain(1);
  myRamp.go(5000,0,LINEAR);
  myRamp.go(0, 15);
  Serial.println();
}

void loop() {
  int val = myRamp.update();
  if (val != lastValue) {
    Serial.print(val);
    Serial.print("\t|\t");
    Serial.print(myRamp.getCompletion());
    Serial.print(" %");
    Serial.println();
  }
  lastValue = val;
}
