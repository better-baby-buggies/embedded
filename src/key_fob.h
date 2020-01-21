#include <Arduino.h>

#include "parameters.h"

void _setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void _loop() {
  // put your main code here, to run repeatedly:
  Serial.read();
}
