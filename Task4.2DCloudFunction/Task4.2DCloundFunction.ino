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

// Main function that takes a string argument to control the correct LED
void controlLight(String room) {
  if (room == "living room") {
    digitalWrite(PIN_LIVING_ROOM, living_room ? HIGH : LOW);
    Serial.println("Living room: " + String(living_room ? "ON" : "OFF"));
  } else if (room == "bathroom") {
    digitalWrite(PIN_BATHROOM, bathroom ? HIGH : LOW);
    Serial.println("Bathroom: " + String(bathroom ? "ON" : "OFF"));
  } else if (room == "closet") {
    digitalWrite(PIN_CLOSET, closet ? HIGH : LOW);
    Serial.println("Closet: " + String(closet ? "ON" : "OFF"));
  }
}

// Cloud callbacks — each calls controlLight() with the room name
void onLivingRoomChange() {
  controlLight("living room");
}

void onBathroomChange() {
  controlLight("bathroom");
}

void onClosetChange() {
  controlLight("closet");
}