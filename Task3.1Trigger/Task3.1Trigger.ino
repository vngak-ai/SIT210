#include <Wire.h>
#include <BH1750.h>
#include <WiFiNINA.h>

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

char host[] = "maker.ifttt.com";
String key = SECRET_KEY;

BH1750 lightMeter;

float threshold = 1000;
bool isLight = false;

void connectWiFi() {
  Serial.print("Connecting WiFi...");
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    delay(2000);
    Serial.print(".");
  }
  Serial.println("Connected!");
}

float readLux() {
  float lux = lightMeter.getLightIntensity();
  Serial.print("Lux: ");
  Serial.println(lux);
  return lux;
}

void sendEvent(String eventName, float lux) {
  WiFiSSLClient client;

  if (client.connect(host, 443)) {
    String url = "/trigger/" + eventName + "/with/key/" + key +
                 "?value1=" + String(lux);

    client.println("GET " + url + " HTTP/1.1");
    client.println("Host: maker.ifttt.com");
    client.println("Connection: close");
    client.println();

    Serial.println("Sent: " + eventName);
    delay(5000);
  }

  client.stop();
}

void checkLightState(float lux) {
  if (lux > threshold && isLight == false) {
    sendEvent("sunlight_on", lux);
    isLight = true;
  }
  else if (lux <= threshold && isLight == true) {
    sendEvent("sunlight_off", lux);
    isLight = false;
  }
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  lightMeter.begin();

  connectWiFi();

  float lux = lightMeter.getLightIntensity();
  if (lux > threshold) isLight = true;
}

void loop() {
  float lux = readLux();
  checkLightState(lux);
  delay(3000);
}