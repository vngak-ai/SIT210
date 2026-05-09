/*
 * BH1750 Light Sensor Test
 * SIT210 Smart Plant Disease Detection System
 * Board: Arduino Nano 33 IoT
 * Sensor: BH1750 (I2C, address 0x23)
 * Author: Kyle (Nguyen Anh Khoa Vo)
 */

#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter(0x23);  // ADDR pin = GND → 0x23
                          // (nếu ADDR nối VCC thì đổi thành 0x5C)

void setup() {
  Serial.begin(9600);
  while (!Serial);  // chờ Serial Monitor mở

  Wire.begin();

  Serial.println(F("BH1750 Test - SIT210 Project"));
  Serial.println(F("----------------------------"));

  // CONTINUOUS_HIGH_RES_MODE:
  //  - Độ phân giải 1 lux
  //  - Thời gian đo ~120ms
  //  - Đo liên tục
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println(F("BH1750 initialised successfully"));
  } else {
    Serial.println(F("BH1750 init FAILED - check wiring / address"));
    while (1);  // halt
  }
}

void loop() {
  if (lightMeter.measurementReady()) {
    float lux = lightMeter.readLightLevel();

    Serial.print(F("Light: "));
    Serial.print(lux);
    Serial.print(F(" lx  -> "));

    // Phân loại cho plant monitoring
    if (lux < 0) {
      Serial.println(F("Read error"));
    } else if (lux < 50) {
      Serial.println(F("Very dark"));
    } else if (lux < 200) {
      Serial.println(F("Low light (indoor shade)"));
    } else if (lux < 1000) {
      Serial.println(F("Medium light"));
    } else if (lux < 10000) {
      Serial.println(F("Bright (overcast outdoor)"));
    } else {
      Serial.println(F("Direct sunlight"));
    }
  }
  delay(1000);
}