// This file is based on code retrived from https://rootsaid.com/arduino-ble-example/

#include <ArduinoBLE.h>

BLEService babyService("25576995-63dc-4c59-be89-8907453e623c");
BLEUnsignedCharCharacteristic babyDetectedChar("25576996-63dc-4c59-be89-8907453e623c1", BLERead | BLENotify);

//#define DEBUG

#define CONNECTED_PIN LED_BUILTIN
#define DELAY_TIME 200 // Delays between checking for updates

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

  BLE.setLocalName("BabySeatMonitor");
  BLE.setAdvertisedService(babyService);
  babyService.addCharacteristic(babyDetectedChar);
  BLE.addService(babyService);

  babyDetectedChar.writeValue(BABY_NOT_DETECTED);
  prev_state = BABY_NOT_DETECTED;

  BLE.advertise();

#ifdef DEBUG
  Serial.println("Bluetooth device active, waiting for connections...");
#endif
}

void loop() 
{
  BLEDevice central = BLE.central();
  baby_state state;

  if (central)
  {
#ifdef DEBUG
    Serial.print("Connected to central: ");
    Serial.println(central.address());
#endif
    
    digitalWrite(CONNECTED_PIN, HIGH);

    while (central.connected())
    {
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
      delay(DELAY_TIME);
    }
  }
  digitalWrite(CONNECTED_PIN, LOW);

#ifdef DEBUG
  Serial.print("Disconnected from central: ");
  Serial.println(central.address());
#endif
}


// Check if the baby is present
baby_state check_baby() {
  // TODO
  return BABY_DETECTED;
}
