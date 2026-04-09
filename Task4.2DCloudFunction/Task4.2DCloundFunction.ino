#include "thingProperties.h"

const int PIN_LIVING_ROOM = 2;
const int PIN_BATHROOM    = 3;
const int PIN_CLOSET      = 4;

void setup() {
  Serial.begin(9600);
  delay(1500);

  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  pinMode(PIN_LIVING_ROOM, OUTPUT);
  pinMode(PIN_BATHROOM,    OUTPUT);
  pinMode(PIN_CLOSET,      OUTPUT);

  digitalWrite(PIN_LIVING_ROOM, LOW);
  digitalWrite(PIN_BATHROOM,    LOW);
  digitalWrite(PIN_CLOSET,      LOW);
}

void loop() {
  ArduinoCloud.update();
}

void onLivingRoomChange() {
  digitalWrite(PIN_LIVING_ROOM, living_room ? HIGH : LOW);
  Serial.println(living_room ? "Living room: ON" : "Living room: OFF");
}

void onBathroomChange() {
  digitalWrite(PIN_BATHROOM, bathroom ? HIGH : LOW);
  Serial.println(bathroom ? "Bathroom: ON" : "Bathroom: OFF");
}

void onClosetChange() {
  digitalWrite(PIN_CLOSET, closet ? HIGH : LOW);
  Serial.println(closet ? "Closet: ON" : "Closet: OFF");
}