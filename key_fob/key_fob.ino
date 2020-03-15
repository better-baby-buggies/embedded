

#include <ArduinoBLE.h>

const bool DEBUG = false;

#define DEBUG_PRINT(str) \
  do                     \
  {                      \
    if (DEBUG)           \
    {                    \
      Serial.print(str); \
    }                    \
  } while (0)

#define DEBUG_PRINTLN(str) \
  do                       \
  {                        \
    if (DEBUG)             \
    {                      \
      Serial.println(str); \
    }                      \
  } while (0)

#define STARTUP_MESSAGE "key_fob"

#define DEBUG_LED_FLASH_TIME 50

#define ALARM_DURATION 250  // milliseconds
#define ALARM_FREQ 4000     // hertz, recommended here:https://product.tdk.com/info/en/catalog/datasheets/piezoelectronic_buzzer_ps_en.pdf

/* Pins (Arduino) */
#define POWER_LED_PIN 2     // Power On LED
#define CONNECTED_LED_PIN 3 // Bluetooth Connected LED
#define HEARTBEAT_LED_PIN 4 // Heartbeat Pulse LED
#define TX_LED_PIN 5        // Data Transfer Indicator LED (Not Used)
#define RX_LED_PIN 6        // Data Received Indicator LED
#define SPEAKER_LED_PIN 7   // Alarm Emulator/Indicator LED
#define SPEAKER_PIN 12      // Alarm Emulator/Indicator LED
#define BUTTON0_PIN 8       // User facing button

// Globals
typedef enum
{
  BABY_NOT_DETECTED = 0,
  BABY_DETECTED = 1
} baby_state_t;

baby_state_t baby_state;

typedef enum
{
  ALARM_OFF,
  ALARM_ON
} alarm_state_t;

// Either HIGH or LOW
const char power_state = LOW; // Alway on
char connected_state;

// Function Declarations
void monitorBabyState(BLEDevice peripheral);
void disconnect(BLEDevice peripheral);
void alert_user(void);
void flash_pin(short pin);
void check_leds(void);

void setup()
{

  if (DEBUG)
  {
    Serial.begin(9600);
    while (!Serial)
      ;
    DEBUG_PRINTLN(STARTUP_MESSAGE);
  }

  /* Pin setup */
  pinMode(POWER_LED_PIN, OUTPUT);
  pinMode(CONNECTED_LED_PIN, OUTPUT);
  if (DEBUG)
  {
    pinMode(HEARTBEAT_LED_PIN, OUTPUT);
    pinMode(TX_LED_PIN, OUTPUT);
    pinMode(RX_LED_PIN, OUTPUT);
  }
  pinMode(BUTTON0_PIN, INPUT_PULLUP);

  // Pins should be initialized off (HIGH)
  digitalWrite(POWER_LED_PIN, HIGH);
  digitalWrite(CONNECTED_LED_PIN, HIGH);
  if (DEBUG)
  {
    digitalWrite(HEARTBEAT_LED_PIN, HIGH);
    digitalWrite(TX_LED_PIN, HIGH);
    digitalWrite(RX_LED_PIN, HIGH);
  }

  // Set inital states
  baby_state = BABY_NOT_DETECTED;
  connected_state = HIGH;

  // initialize the BLE hardware
  BLE.begin();
  //BLE.setTimeout(1000);

  DEBUG_PRINTLN("BLE Central - Baby Monitor");
}

void loop()
{
  check_leds();

  delay(100);
  DEBUG_PRINTLN("Starting Scan...");
  BLE.scanForUuid("25576995-63dc-4c59-be89-8907453e623c", false);

  // check if a peripheral has been discovered
  BLEDevice peripheral = BLE.available();

  if (peripheral)
  {
    DEBUG_PRINT("Found ");
    DEBUG_PRINT(peripheral.address());
    DEBUG_PRINT(" '");
    DEBUG_PRINT(peripheral.localName());
    DEBUG_PRINT("' ");
    DEBUG_PRINTLN(peripheral.advertisedServiceUuid());

    if (peripheral.localName() != "BabySeatMonitor")
    {
      return;
    }

    // stop scanning
    BLE.stopScan();

    monitorBabyState(peripheral);
  }
}

void monitorBabyState(BLEDevice peripheral)
{

  DEBUG_PRINTLN("Connecting ...");

  if (peripheral.connect())
  {
    DEBUG_PRINTLN("Peripheral Connected");
    connected_state = LOW; // Turn on LED
  }
  else
  {
    DEBUG_PRINTLN("Failed to connect!");
    return;
  }

  // discover peripheral attributes
  DEBUG_PRINTLN("Discovering attributes ...");
  if (!peripheral.discoverAttributes())
  {
    DEBUG_PRINTLN("Attribute discovery failed!");
    disconnect(peripheral);
    return;
  }

  DEBUG_PRINTLN("Attributes discovered");

  // retrieve the LED characteristic
  BLECharacteristic babyDetectedChar = peripheral.characteristic("25576996-63dc-4c59-be89-8907453e623c");

  // subscribe to the baby detection characteristic
  if (!babyDetectedChar)
  {
    DEBUG_PRINTLN("No baby detection characteristic found!");
    disconnect(peripheral);
    return;
  }

  // while the peripheral is connected
  while (peripheral.connected())
  {
    check_leds();
    flash_pin(HEARTBEAT_LED_PIN);
    
    // check if the value of the baby detection characteristic has been updated
    byte value = 0;
    if (babyDetectedChar.readValue(value) > 0)
    {
      // if number of bytes read is greater than 0
      flash_pin(RX_LED_PIN);           // Value was received via Bluetooth
      baby_state = (baby_state_t)value; // read into global state

      if (baby_state == BABY_DETECTED)
      {
        DEBUG_PRINTLN("Baby was Detected");
      }
      if (baby_state == BABY_NOT_DETECTED)
      {
        DEBUG_PRINTLN("Baby was not Detected");
      }
    }
    else
    {
      DEBUG_PRINTLN("No Bytes Read!!");
      disconnect(peripheral);
      return;
    }
  }
  
  disconnect(peripheral);
  return;
}

void disconnect(BLEDevice peripheral)
{
  DEBUG_PRINTLN("Disconnecting...");
  if (peripheral.disconnect())
  {
    DEBUG_PRINTLN("Peripheral disconnected successfully");
  }
  else
  {
    DEBUG_PRINTLN("Peripheral disconnected unsuccessfully");
    DEBUG_PRINT("Connected state: ");
    DEBUG_PRINTLN((bool)peripheral.connected());
  }

  connected_state = HIGH; // Turn off LED

  // Alert the User if the last baby_state recieved was BABY_DETECTED
  // since we have disconnected
  if (baby_state == BABY_DETECTED)
  {
    alert_user();
    baby_state = BABY_NOT_DETECTED;
  }
}

// This function should sound the alarm in the key fob
// This function should not return until the user presses the BUTTON0
void alert_user()
{
  DEBUG_PRINTLN("Turning on alarm");

  // Sound alarm until button press
  do
  {
    //check_leds();

    tone(SPEAKER_PIN, ALARM_FREQ, ALARM_DURATION);
    delay(ALARM_DURATION*2);
    noTone(SPEAKER_PIN);

  } while (digitalRead(BUTTON0_PIN) != LOW);
  
  DEBUG_PRINTLN("Alarm Off");
}

// Only use on debug pins
void flash_pin(short pin)
{
  digitalWrite(pin, LOW); // Turn on
  delay(DEBUG_LED_FLASH_TIME); // Wait
  digitalWrite(pin, HIGH); // Turn off
  delay(DEBUG_LED_FLASH_TIME); // Wait
}

void check_leds(void)
{
  if (digitalRead(BUTTON0_PIN) == LOW)
  {
    // Show LED state
    digitalWrite(POWER_LED_PIN, power_state);
    digitalWrite(CONNECTED_LED_PIN, connected_state);
  }
  else
  {
    // Turn off LEDs
    digitalWrite(POWER_LED_PIN, HIGH);
    digitalWrite(CONNECTED_LED_PIN, HIGH);
  }
}
