#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include <DHT.h>
#include <Wire.h>
#include <BH1750.h>

// Sensor definitions
#define DHTPIN 2
#define DHTTYPE DHT22

// WiFi credentials
char ssid[] = "8Uganda";
char pass[] = "Lmamlmam2025";

WiFiClient client;

// ThingSpeak channel information
unsigned long channelID = 3299817;
const char* writeAPIKey = "0H84YSA01ZSRVCKN";

// Sensor objects
DHT dht(DHTPIN, DHTTYPE);
BH1750 lightMeter;

// Data variables
float temperature;
float lightLevel;

void setup()
{
  Serial.begin(9600);

  // Start DHT22
  dht.begin();

  // Start I2C and BH1750
  Wire.begin();
  lightMeter.begin();

  // Connect to WiFi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
  }

  // Start ThingSpeak
  ThingSpeak.begin(client);
}

// Read temperature from DHT22
float readTemperature()
{
  return dht.readTemperature();
}

// Read light level from BH1750 in lux
float readLight()
{
  return lightMeter.readLightLevel();
}

// Send data to ThingSpeak
void sendToThingSpeak(float temp, float light)
{
  ThingSpeak.setField(1, temp);
  ThingSpeak.setField(2, light);
  ThingSpeak.writeFields(channelID, writeAPIKey);
}

// Print data to Serial Monitor
void sendToSerial(float temp, float light)
{
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" C");

  Serial.print("Light: ");
  Serial.print(light);
  Serial.println(" lux");
}

void loop()
{
  temperature = readTemperature();
  lightLevel = readLight();

  if (!isnan(temperature) && lightLevel >= 0)
  {
    sendToSerial(temperature, lightLevel);
    sendToThingSpeak(temperature, lightLevel);
  }
  else
  {
    Serial.println("Sensor reading failed.");
  }

  delay(30000);
}