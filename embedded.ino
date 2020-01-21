
// #define KEY_FOB
// #define CAR_SEAT
// #define TEST_1

#ifdef KEY_FOB
#include "src/key_fob.h"
#endif

#ifdef CAR_SEAT
#include "src/car_seat.h"
#endif

#ifdef TEST_1
#include "src/tests/test_1.h"
#endif

void setup() {
  // put your setup code here, to run once:
  _setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  _loop();
}
