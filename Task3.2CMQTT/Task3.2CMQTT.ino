
#include <WiFiNINA.h>
#include <PubSubClient.h>
#include "arduino_secrets.h"

//WiFi credentials (from Sketch Secrets) ---
const char* WIFI_SSID     = SECRET_SSID;
const char* WIFI_PASSWORD = SECRET_PASS;

//MQTT broker
const char* MQTT_BROKER    = "broker.hivemq.com";
const int   MQTT_PORT      = 1883;
const char* MQTT_CLIENT_ID = "arduino-kyle-001";
const char* TOPIC_WAVE     = "ES/Wave";
const char* TOPIC_PAT      = "ES/Pat";
const char* YOUR_NAME      = "Kyle";

//Pin definitions
#define TRIG_PIN  9
#define ECHO_PIN  10
#define LED1_PIN  4
#define LED2_PIN  5

//Detection thresholds
#define WAVE_NEAR_THRESHOLD  20
#define WAVE_FAR_THRESHOLD   35
#define PAT_THRESHOLD        8
#define PAT_HOLD_TIME        600
#define WAVE_TIMEOUT         1500
#define COOLDOWN_TIME        1500

//Global objects
WiFiClient   wifiClient;
PubSubClient mqttClient(wifiClient);

//Gesture state
bool  handInZone      = false;
long  handEnteredTime = 0;
bool  wavePending     = false;
long  waveStartTime   = 0;
long  lastGestureTime = 0;


// WiFi
void connectWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(WIFI_SSID);

  while (WiFi.begin(WIFI_SSID, WIFI_PASSWORD) != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// MQTT Callback - runs when message received
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("Message received [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(message);

  if (String(topic) == TOPIC_WAVE) {
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, HIGH);
    Serial.println("Lights ON");
  }

  if (String(topic) == TOPIC_PAT) {
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    Serial.println("Lights OFF");
  }
}

// MQTT Connect
void connectMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("Connecting to MQTT broker...");

    if (mqttClient.connect(MQTT_CLIENT_ID)) {
      Serial.println(" connected!");
      mqttClient.subscribe(TOPIC_WAVE);
      mqttClient.subscribe(TOPIC_PAT);
      Serial.println("Subscribed to ES/Wave and ES/Pat");
    } else {
      Serial.print(" failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" | retrying in 3s...");
      delay(3000);
    }
  }
}


// Ultrasonic sensor
float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return 999;

  return duration * 0.034 / 2;
}

// Gesture detection
// Wave = hand moves in then out quickly
// Pat  = hand held very close for a moment
void detectGesture(float distance) {
  long now = millis();

  if (now - lastGestureTime < COOLDOWN_TIME) return;

  // Hand enters zone
  if (!handInZone && distance < WAVE_NEAR_THRESHOLD) {
    handInZone      = true;
    handEnteredTime = now;
    wavePending     = true;
    waveStartTime   = now;
    Serial.println("Hand detected - waiting for gesture...");
  }

  // Hand stays in zone
  if (handInZone && distance < WAVE_NEAR_THRESHOLD) {
    // PAT: held very close long enough
    if (distance < PAT_THRESHOLD && (now - handEnteredTime) >= PAT_HOLD_TIME) {
      Serial.println("PAT detected! Publishing to ES/Pat...");
      mqttClient.publish(TOPIC_PAT, YOUR_NAME);
      lastGestureTime = now;
      handInZone      = false;
      wavePending     = false;
    }
    // Wave timeout
    if (wavePending && (now - waveStartTime) > WAVE_TIMEOUT) {
      wavePending = false;
    }
  }

  // Hand leaves zone -> WAVE
  if (handInZone && distance >= WAVE_FAR_THRESHOLD) {
    if (wavePending && (now - waveStartTime) <= WAVE_TIMEOUT) {
      Serial.println("WAVE detected! Publishing to ES/Wave...");
      mqttClient.publish(TOPIC_WAVE, YOUR_NAME);
      lastGestureTime = now;
    }
    handInZone  = false;
    wavePending = false;
  }
}

// Setup
void setup() {
  Serial.begin(9600);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);

  connectWiFi();

  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  mqttClient.setCallback(mqttCallback);

  connectMQTT();

  Serial.println("System ready! Wave or Pat to control lights.");
}


// Loop
void loop() {
  if (!mqttClient.connected()) {
    connectMQTT();
  }

  mqttClient.loop();

  float distance = getDistance();
  detectGesture(distance);

  delay(100);
}