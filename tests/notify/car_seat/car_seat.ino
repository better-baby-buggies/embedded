// From: https://github.com/arduino-libraries/ArduinoBLE/pull/15#issuecomment-530084737

/*
  Callback LED

  This example creates a BLE peripheral with service that contains a
  characteristic to control an LED. The callback features of the
  library are used.

  The circuit:
  - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
    Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.

  You can use a generic BLE central app, like LightBlue (iOS and Android) or
  nRF Connect (Android), to interact with the services and characteristics
  created in this sketch.

  This example code is in the public domain.
*/

#include <ArduinoBLE.h>
#include <Ticker.h>

BLEService babyService("25576995-63dc-4c59-be89-8907453e623c");
BLEUnsignedCharCharacteristic babyDetectedChar("25576996-63dc-4c59-be89-8907453e623c1", BLERead | BLENotify);

// Timer for in between checks
mbed::Ticker ticker();

#define CONNECTED_PIN LED_BUILTIN
#define DELAY_TIME 200

enum baby_state {
  BABY_NOT_DETECTED = 0,
  BABY_DETECTED = 1
};

baby_state prev_state;

void setup()
{
  // Setup Connected Pin
  pinMode(CONNECTED_PIN, OUTPUT);
  
#ifdef DEBUG
  // Setup debuging serial
  Serial.begin(9600);
  while (!Serial);
#endif

  // Setup BLE device
  if (!BLE.begin()) {

#ifdef DEBUG
    Serial.println("Starting BLE failed!");
#endif

    while (1);
  }

  // set the local name peripheral advertises
  BLE.setLocalName("BabySeatMonitor");
  BLE.setAdvertisedService(babyService);
  babyService.addCharacteristic(babyDetectedChar);
  BLE.addService(babyService);

  // assign event handlers for connected, disconnected to peripheral
  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  babyDetectedChar.writeValue(BABY_NOT_DETECTED);
  prev_state = BABY_NOT_DETECTED;

  // Set up timer to callback soon
  ticker.attach_us(tickerHandler, DELAY_TIME); // TODO: Fix build errors

  BLE.advertise();

#ifdef DEBUG
  Serial.println("Bluetooth device active, waiting for connections...");
#endif
}

void loop() {
  // poll for BLE events
  BLE.poll(10000);
}

void blePeripheralConnectHandler(BLEDevice central) {
  // central connected event handler
#ifdef DEBUG
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
#endif
}

void blePeripheralDisconnectHandler(BLEDevice central) {
  // central disconnected event handler
#ifdef DEBUG
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
#endif
}

void tickerHandler() {
#ifdef DEBUG
  Serial.print("Ticker time ran up");
#endif
  static baby_state state;

  // Check if the baby is in the car seat
  state = check_baby();
      
#ifdef DEBUG
  if (state == BABY_DETECTED) {
    Serial.println("Baby is detected");
  } else if (state == BABY_NOT_DETECTED) {
    Serial.println("Baby is NOT detected");
  }
#endif

  if (prev_state != state) {
    babyDetectedChar.writeValue(state);
    prev_state = state;
  }

  // Set up timer to callback soon
  ticker.attach_us(tickerHandler, DELAY_TIME); // TODO: Fix build errors, 
                                              // or create new timer solution (Low power?)
}

// Check if the baby is present
baby_state check_baby() {
  // TODO
  return BABY_DETECTED;
}

