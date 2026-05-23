#include <ArduinoBLE.h>
#include <Wire.h>
#include <BH1750.h>

// Pin definitions
const int LED_BATHROOM = 3;
const int LED_HALLWAY  = 5;
const int FAN_PIN      = 6;

// Threshold: below 10 lux means the room is dark
const float LIGHT_THRESHOLD = 10.0;

BH1750 lightMeter;

// BLE service and characteristic
BLEService lightService("19B10000-E8F2-537E-4F6C-D104768A1214");
BLEByteCharacteristic commandChar(
  "19B10001-E8F2-537E-4F6C-D104768A1214",
  BLERead | BLEWrite
);

void setup() {
  Serial.begin(9600);

  pinMode(LED_BATHROOM, OUTPUT);
  pinMode(LED_HALLWAY, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);

  // Initialize BH1750 over I2C
  Wire.begin();
  if (!lightMeter.begin()) {
    Serial.println("BH1750 init failed!");
    while (1);
  }

  // Initialize BLE
  if (!BLE.begin()) {
    Serial.println("BLE init failed!");
    while (1);
  }

  BLE.setLocalName("LindaLightSystem");
  BLE.setAdvertisedService(lightService);
  lightService.addCharacteristic(commandChar);
  BLE.addService(lightService);
  commandChar.writeValue(0);

  BLE.advertise();
  Serial.println("BLE ready, waiting for connection...");
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Connected to: ");
    Serial.println(central.address());

    while (central.connected()) {
      if (commandChar.written()) {
        byte cmd = commandChar.value();
        handleCommand(cmd);
      }
    }
    Serial.println("Disconnected");
  }
}

// Command codes:
// 1 = lights on, 2 = lights off, 3 = fan on, 4 = fan off
void handleCommand(byte cmd) {
  float lux = lightMeter.readLightLevel();
  Serial.print("Command: "); Serial.print(cmd);
  Serial.print(" | Lux: "); Serial.println(lux);

  switch (cmd) {
    case 1: // Lights on - only if room is dark
      if (lux < LIGHT_THRESHOLD) {
        digitalWrite(LED_BATHROOM, HIGH);
        digitalWrite(LED_HALLWAY, HIGH);
        Serial.println("Lights ON (room is dark)");
      } else {
        Serial.print("Lights NOT on - room is bright (");
        Serial.print(lux); Serial.println(" lux)");
      }
      break;

    case 2: // Lights off
      digitalWrite(LED_BATHROOM, LOW);
      digitalWrite(LED_HALLWAY, LOW);
      Serial.println("Lights OFF");
      break;

    case 3: // Fan on
      digitalWrite(FAN_PIN, HIGH);
      Serial.println("Fan ON");
      break;

    case 4: // Fan off
      digitalWrite(FAN_PIN, LOW);
      Serial.println("Fan OFF");
      break;
  }
}
