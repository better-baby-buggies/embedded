

#include <ArduinoBLE.h>

//#define DEBUG

enum baby_state {
  BABY_NOT_DETECTED = 0,
  BABY_DETECTED = 1
};

baby_state state;


void setup() {
  
#ifdef DEBUG
  Serial.begin(9600);
  while (!Serial);
#endif

  // initialize the BLE hardware
  BLE.begin();

#ifdef DEBUG
  Serial.println("BLE Central - LED control");
#endif

  // start scanning for peripherals
  BLE.scanForUuid("25576995-63dc-4c59-be89-8907453e623c");
}

void loop() {
  // check if a peripheral has been discovered
  BLEDevice peripheral = BLE.available();

  if (peripheral) {

#ifdef DEBUG
    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();
#endif

    if (peripheral.localName() != "BabySeatMonitor") {
      return;
    }

    // stop scanning
    BLE.stopScan();

    monitorBabyState(peripheral);

    // peripheral disconnected, start scanning again
    BLE.scanForUuid("25576995-63dc-4c59-be89-8907453e623c");
  }
}

void monitorBabyState(BLEDevice peripheral) {

#ifdef DEBUG
  Serial.println("Connecting ...");
#endif

  if (peripheral.connect()) {
#ifdef DEBUG
    Serial.println("Connected");
#endif
  } else {
#ifdef DEBUG
    Serial.println("Failed to connect!");
#endif
    return;
  }

  // discover peripheral attributes
#ifdef DEBUG
  Serial.println("Discovering attributes ...");
#endif
  if (!peripheral.discoverAttributes()) {
#ifdef DEBUG
    Serial.println("Attribute discovery failed!");
#endif
    peripheral.disconnect();
    return;
  }

#ifdef DEBUG
  Serial.println("Attributes discovered");
#endif

  // retrieve the LED characteristic
  BLECharacteristic babyDetectedChar = peripheral.characteristic("25576996-63dc-4c59-be89-8907453e623c");

  // subscribe to the baby detection characteristic
#ifdef DEBUG
  Serial.println("Subscribing to baby detection characteristic ...");
#endif
  if (!babyDetectedChar) {
#ifdef DEBUG
    Serial.println("No baby detection characteristic found!");
#endif
    peripheral.disconnect();
    return;
  } else if (!babyDetectedChar.canSubscribe()) {
#ifdef DEBUG
    Serial.println("Baby detection characteristic is not subscribable!");
#endif
    peripheral.disconnect();
    return;
  } else if (!babyDetectedChar.subscribe()) {
#ifdef DEBUG
    Serial.println("Subscription failed!");
#endif
    peripheral.disconnect();
    return;
  }
#ifdef DEBUG
  Serial.println("Subscribed");
#endif

  // while the peripheral is connected
  while (peripheral.connected()) {

    // check if the value of the baby detection characteristic has been updated
    if (babyDetectedChar.valueUpdated()) {
      byte value = 0;

      if (babyDetectedChar.readValue(value) > 0) {
        // if number of bytes read is greater than 0

        state = value; // read into global variable

#ifdef DEBUG
        if (state == BABY_DETECTED) {
          Serial.println("Baby was Detected");
        }
        if (state == BABY_NOT_DETECTED) {
          Serial.println("Baby was not Detected");
        }
#endif
      }
    }
  }

#ifdef DEBUG
  Serial.println("Peripheral disconnected");
#endif

  // Alert the User if the last state recieved was BABY_DETECTED
  // since we have disconnected
  if (state == BABY_DETECTED) {
    alert_user();
  }
}

// This function should sound the alarm in the key fob
// This function should not return until the user presses the button
void alert_user() {
  // TODO
  // Sound alarm until button press
}
