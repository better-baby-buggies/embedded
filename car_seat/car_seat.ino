// This file is based on code retrived from https://rootsaid.com/arduino-ble-example/

#include <ArduinoBLE.h>

BLEService babyService("25576995-63dc-4c59-be89-8907453e623c");
BLEUnsignedCharCharacteristic babyDetectedChar("25576996-63dc-4c59-be89-8907453e623c1", BLERead | BLENotify);

//#define DEBUG

#define SENSOR_PIN A0 // Weight Sensor Input
#define CONNECTED_PIN 13 // Bluetooth Connected LED
#define SENSOR_LED 2 // Weight Sensor Indicator LED
#define TX_LED 0 // Data Transfer Indicator LED
#define RX_LED 1 // Data Received Indicator LED **Necessary?**
#define HEARTBEAT_LED 3// Heartbeat Pulse LED

#define DELAY_TIME 200 // Delays between checking for updates
#define HEARTBEAT_FLASH_TIME 40 // Longer delay to notice hearbeat indicator

enum baby_state {
    BABY_NOT_DETECTED = 0,
    BABY_DETECTED = 1
};

baby_state prev_state;

void setup()
{
    // Setup Connected Pin
    pinMode(CONNECTED_PIN, OUTPUT);
    pinMode(SENSOR_LED, OUTPUT);

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

    // Beginning values for setup
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
        // Signal Bluetooth connection active
        digitalWrite(CONNECTED_PIN, HIGH);

        while (central.connected())
        {
            state = check_baby();

#ifdef DEBUG
            if (state == BABY_DETECTED) {
                Serial.println("Baby is detected");
            }
            else if (state == BABY_NOT_DETECTED) {
                Serial.println("Baby is NOT detected");
            }
#endif
            // If value changed since last read
            if (prev_state != state) {
                // Set global bluetooth characteristic
                babyDetectedChar.writeValue(state);
                //digitalWrite(TX_LED, HIGH); // Value should be transmitted via Bluetooth
                prev_state = state;
            }
            // Indicate heartbeat pulse with delay for visual flash
            //digitalWrite(HEARTBEAT_LED, HIGH);
            //delay(HEARTBEAT_FLASH_TIME);
            //digitalWrite(HEARTBEAT_LED, LOW);
            // Delay checking of sensor
            delay(DELAY_TIME);
        }
    }

#ifdef DEBUG
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
#endif
    // No longer connected via bluetooth
    digitalWrite(CONNECTED_PIN, LOW);
}


// Check if the baby is present
baby_state check_baby() {
    baby_state baby;
    int sensor_value = 0;
    sensor_value = analogRead(SENSOR_PIN);

    // Check if voltage read is greater than ~1.5 V (R=10k)
    if (sensor_value > 465) {
        baby = BABY_DETECTED;
        //digitalWrite(SENSOR_LED, HIGH);
    }
    else {
        baby = BABY_NOT_DETECTED;
        //digitalWrite(SENSOR_LED, LOW);
    }
    return baby;
}
