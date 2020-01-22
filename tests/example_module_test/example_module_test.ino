#include <example_device_driver.h>
#include <OtherGuy.h>
#include <parameters.h>


int num = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(SERIAL_BAUD_RATE); // This is defined in parameters.h
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  Serial.write(num);
  num = addOneToNum(num);
}
