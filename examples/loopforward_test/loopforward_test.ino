#include <Arduino.h>
#include <Ramp.h>

rampInt r;
int oldVal = -1;

void setup() {
  Serial.begin(115200);
  r.setGrain(0);
  r.go(255, 5000, LINEAR, LOOPFORWARD);
}

void loop() {
  int newVal = r.update();
  if (newVal != oldVal) {
      Serial.print(r.getPosition());
      Serial.print(" ");
      Serial.print(newVal);
      Serial.println();
      oldVal = newVal;
    }
}