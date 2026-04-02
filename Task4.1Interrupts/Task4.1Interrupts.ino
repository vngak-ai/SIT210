#include <Wire.h>
#include <BH1750.h>

// ========== PIN DEFINITIONS ==========
#define PIR_PIN 2        
#define SWITCH_PIN 3     
#define LED1_PIN 4       
#define LED2_PIN 5       

// ========== GLOBAL VARIABLES ==========
BH1750 lightMeter;

volatile bool motionDetected = false;
volatile bool switchPressed = false;

volatile unsigned long lastMotionTime = 0;
volatile unsigned long lastSwitchTime = 0;

bool lightsOn = false;

const float DARK_THRESHOLD = 50.0;
const unsigned long MOTION_DEBOUNCE = 2000;
const unsigned long SWITCH_DEBOUNCE = 300;

// ========== INTERRUPT SERVICE ROUTINES ==========

void pirISR() {
  unsigned long currentTime = millis();
  if (currentTime - lastMotionTime > MOTION_DEBOUNCE) {
    motionDetected = true;
    lastMotionTime = currentTime;
  }
}

void switchISR() {
  unsigned long currentTime = millis();
  if (currentTime - lastSwitchTime > SWITCH_DEBOUNCE) {
    switchPressed = true;
    lastSwitchTime = currentTime;
  }
}

// ========== SETUP ==========

void setup() {
  Serial.begin(9600);
  
  Wire.begin();
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
    Serial.println("[BH1750] Initialized successfully");
  } else {
    Serial.println("[BH1750] ERROR - Check wiring!");
  }
  
  pinMode(PIR_PIN, INPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  
  attachInterrupt(digitalPinToInterrupt(PIR_PIN), pirISR, RISING);
  attachInterrupt(digitalPinToInterrupt(SWITCH_PIN), switchISR, FALLING);
  
  for (int i = 30; i > 0; i--) {
    if (i % 5 == 0 || i <= 3) {
      Serial.print("  ");
      Serial.print(i);
      Serial.println("s...");
    }
    delay(1000);
  }
  
  Serial.println("\n✓ System Ready!\n");
  Serial.println("═══════════════════════════════════\n");
}

// ========== MAIN LOOP ==========

void loop() {
  if (motionDetected) {
    motionDetected = false;
    handleMotion();
  }
  
  if (switchPressed) {
    switchPressed = false;
    handleSwitch();
  }
  
  delay(10);
}

// ========== EVENT HANDLERS ==========

void handleMotion() {
  float lux = lightMeter.readLightLevel();
  
  Serial.print("┌─ [PIR] Motion detected");
  Serial.println();
  Serial.print("│  Light level: ");
  Serial.print(lux);
  Serial.print(" lux ");
  
  if (lux < DARK_THRESHOLD) {
    Serial.println("(DARK)");
    
    if (!lightsOn) {
      turnOnLights();
      Serial.println("│  Action: Lights ON (Auto)");
    } else {
      Serial.println("│  Action: Lights already ON");
    }
  } else {
    Serial.println("(BRIGHT)");
    Serial.println("│  Action: No action (Too bright)");
  }
  
  Serial.println("└─");
  Serial.println();
}

void handleSwitch() {
  Serial.print("┌─ [SWITCH] Manual control");
  Serial.println();
  
  if (lightsOn) {
    turnOffLights();
    Serial.println("│  Action: Lights OFF");
  } else {
    turnOnLights();
    Serial.println("│  Action: Lights ON");
  }
  
  Serial.println("└─");
  Serial.println();
}

// ========== LIGHT CONTROL FUNCTIONS ==========

void turnOnLights() {
  digitalWrite(LED1_PIN, HIGH);
  digitalWrite(LED2_PIN, HIGH);
  lightsOn = true;
}

void turnOffLights() {
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  lightsOn = false;
}