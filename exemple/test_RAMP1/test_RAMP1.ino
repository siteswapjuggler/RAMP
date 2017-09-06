#include <Ramp.h>

ramp<byte> test(0);                               // déclaration de l'objet

rByte* RGB = new rByte[3];                        // plus d'espace de stockage moins de mémoire dynamique
//rByte RGB[3];                                   // moins d'espace de stockage plus de mémoire dynamique

void setup() {
  Serial.begin(115200);                           // déclaration du port série

  long t = millis();
  long v, u = millis();
  
  test.go(255, 1000, BOUNCE_INOUT);               // démarrer l'interpolation

  while (!test.isFinished()) {
    delay(10);
    Serial.println(test.update());
  }

  /*while (!test.isFinished()) {
    v = millis();
    if (v - u > 10) {
      Serial.println(test.update());
      u = v;
    }
  }

  Serial.println("Done in "+String(millis()-t));*/
}

void loop() {
}

