

#include <ArduinoBLE.h>

const bool DEBUG = true;

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

#define ALARM_DELAY 100 // milliseconds
#define ALARM_FREQ 4000 // hertz, recommended here:https://product.tdk.com/info/en/catalog/datasheets/piezoelectronic_buzzer_ps_en.pdf

/* Pins (Arduino) */
#define POWER_LED_PIN 2     // Power On LED
#define CONNECTED_LED_PIN 3 // Bluetooth Connected LED
#define HEARTBEAT_LED_PIN 4 // Heartbeat Pulse LED
#define TX_LED_PIN 5        // Data Transfer Indicator LED (Not Used)
#define RX_LED_PIN 6        // Data Received Indicator LED
#define SPEAKER_LED_PIN 7   // Alarm Emulator/Indicator LED
#define SPEAKER_PIN 8       // Alarm Emulator/Indicator LED
#define BUTTON0_PIN 12      // User facing button

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

alarm_state_t alarm_state;

// Either HIGH or LOW
const char power_state = LOW; // Alway on
char connected_state;

// Function Declarations
void button_triggered(void);
void alert_user(void);
void toggle_pin(char pin);

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
  alarm_state = ALARM_OFF;
  connected_state = HIGH;

  // Setup button trigger
  attachInterrupt(digitalPinToInterrupt(BUTTON0_PIN), button_triggered, FALLING);

  // initialize the BLE hardware
  BLE.begin();

  DEBUG_PRINTLN("BLE Central - Baby Monitor");

  // start scanning for peripherals
  BLE.scanForUuid("25576995-63dc-4c59-be89-8907453e623c");
}

void loop()
{
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

    // peripheral disconnected, start scanning again
    BLE.scanForUuid("25576995-63dc-4c59-be89-8907453e623c");
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
    peripheral.disconnect();
    return;
  }

  DEBUG_PRINTLN("Attributes discovered");

  // retrieve the LED characteristic
  BLECharacteristic babyDetectedChar = peripheral.characteristic("25576996-63dc-4c59-be89-8907453e623c");

  // subscribe to the baby detection characteristic
  DEBUG_PRINTLN("Subscribing to baby detection characteristic ...");
  if (!babyDetectedChar)
  {
    DEBUG_PRINTLN("No baby detection characteristic found!");
    peripheral.disconnect();
    return;
  }
  else if (!babyDetectedChar.canSubscribe())
  {
    DEBUG_PRINTLN("Baby detection characteristic is not subscribable!");
    peripheral.disconnect();
    return;
  }
  else if (!babyDetectedChar.subscribe())
  {
    DEBUG_PRINTLN("Subscription failed!");
    peripheral.disconnect();
    return;
  } 
  DEBUG_PRINTLN("Subscribed");

  // while the peripheral is connected
  while (peripheral.connected())
  {
    toggle_pin(HEARTBEAT_LED_PIN);
    // check if the value of the baby detection characteristic has been updated
    if (babyDetectedChar.valueUpdated())
    {
      byte value = 0;

      if (babyDetectedChar.readValue(value) > 0)
      {
        // if number of bytes read is greater than 0
        toggle_pin(RX_LED_PIN);           // Value was received via Bluetooth
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
    }
  }

  DEBUG_PRINTLN("Peripheral disconnected");
  connected_state = HIGH; // Turn on LED

  // Alert the User if the last baby_state recieved was BABY_DETECTED
  // since we have disconnected
  if (baby_state == BABY_DETECTED)
  {
    alert_user();
  }
}

// This function should sound the alarm in the key fob
// This function should not return until the user presses the BUTTON0
void alert_user()
{
  alarm_state = ALARM_ON;

  // Sound alarm until button press
  while (alarm_state == ALARM_ON)
  {
    tone(SPEAKER_PIN, ALARM_FREQ, ALARM_DELAY);
    if (DEBUG)
    {
      tone(SPEAKER_LED_PIN, ALARM_FREQ, ALARM_DELAY);
    }
  }
}

// Only use on debug pins
void toggle_pin(char pin)
{
  if (DEBUG)
  {
    digitalWrite(pin, (digitalRead(pin) == HIGH) ? LOW : HIGH);
  }
}

void button_triggered(void)
{
  DEBUG_PRINTLN("Calling button_triggered");
  
  // Turn off the alarm if it is on
  alarm_state = ALARM_OFF;

  // Show External LEDs
  DEBUG_PRINT("Setting LEDs: Power: ");
  DEBUG_PRINT(power_state);
  DEBUG_PRINT("Connected: ");
  DEBUG_PRINTLN(connected_state);
  digitalWrite(POWER_LED_PIN, power_state);
  digitalWrite(CONNECTED_LED_PIN, connected_state);

  delay(1000);

  // Turn off LEDs
  digitalWrite(POWER_LED_PIN, HIGH);
  digitalWrite(CONNECTED_LED_PIN, HIGH);
}
