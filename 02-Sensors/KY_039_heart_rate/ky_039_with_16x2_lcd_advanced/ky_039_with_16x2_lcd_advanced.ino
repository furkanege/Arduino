// ===============================================================
//  KY-039 Advanced Pulse Sensor Project
//  Features:
//   - Beat detection algorithm
//   - Averaged BPM calculation
//   - Custom LCD heart icon
//   - Heartbeat LED + Buzzer feedback
//   - Noise filtering with rolling sample buffer
//  Author: Furkan Ege
// ===============================================================

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
const int PulseWire = A0;
const int LED13 = 13;
const int BuzzerPin = 8;
const int sampleInterval = 1;      // Sampling interval (ms)
const int displayInterval = 1000;  // LCD refresh interval
const int numSamples = 5;          // Rolling average sample count
const int minNabiz = 50;           
const int maxNabiz = 200;
const float thresholdMultiplier = 0.5;
int nabiz = 0;
unsigned long lastBeatTime = 0;
int threshold = 512;
float avgNabiz = 0.0;
int nabizSamples[numSamples] = {0};

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ---------------------------------------------------------------
//   Setup
// ---------------------------------------------------------------
void setup() {
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Nabziniz: ");
  pinMode(LED13, OUTPUT);
  pinMode(BuzzerPin, OUTPUT);
  Serial.begin(9600);
  createHeart();
}

// ---------------------------------------------------------------
//   Main Loop
// ---------------------------------------------------------------
void loop() {
  int rawValue = analogRead(PulseWire);
  if (detectBeat(rawValue, threshold)) {
    nabiz = 60000 / (millis() - lastBeatTime);
    lastBeatTime = millis();
    float avgThreshold = avgNabiz * thresholdMultiplier;

    if (nabiz > minNabiz && nabiz < maxNabiz &&
        (nabiz > avgThreshold * 1.05 || nabiz < avgThreshold * 0.95)) {

      for (int i = numSamples - 1; i > 0; i--) {
        nabizSamples[i] = nabizSamples[i - 1];
      }
      nabizSamples[0] = nabiz;
    } 
    else {
      nabiz = nabizSamples[0];
    }

    float total = 0.0;
    for (int i = 0; i < numSamples; i++) total += nabizSamples[i];
    avgNabiz = total / numSamples;

    static unsigned long lastDisplayTime = 0;
    if (millis() - lastDisplayTime >= displayInterval) {
      lcd.setCursor(10, 0);
      lcd.print("   ");
      lcd.setCursor(10, 0);
      lcd.print(nabiz);
      Serial.print("Nabiz: ");
      Serial.println(nabiz);
      Serial.print("Ortalama Nabiz: ");
      Serial.println(avgNabiz);
      lastDisplayTime = millis();
    }

    digitalWrite(LED13, HIGH);
    static unsigned long lastHeartBlink = 0;
    if (millis() - lastHeartBlink >= 500) {
      lcd.setCursor(14, 0);
      lcd.write(byte(0));
      delay(250);
      lcd.setCursor(14, 0);
      lcd.print(" ");
      lastHeartBlink = millis();
    }
  } 
  else {
    digitalWrite(LED13, LOW);
  }
  playHeartbeatSound();
  delay(sampleInterval);
}

// ---------------------------------------------------------------
//  Heartbeat Sound
// ---------------------------------------------------------------
void playHeartbeatSound() {
  static unsigned long lastHeartbeatTime = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - lastHeartbeatTime >= 800) {
    digitalWrite(BuzzerPin, HIGH);
    delay(50);
    digitalWrite(BuzzerPin, LOW);
    lastHeartbeatTime = currentMillis;
  }
}

// ---------------------------------------------------------------
//  Create Custom LCD Heart Icon
// ---------------------------------------------------------------
void createHeart() {
  byte heart[8] = {
    B00000,
    B01010,
    B11111,
    B11111,
    B11111,
    B01110,
    B00100,
    B00000
  };
  lcd.createChar(0, heart);
}

// ---------------------------------------------------------------
//  Beat Detection Algorithm
// ---------------------------------------------------------------
bool detectBeat(int value, int threshold) {
  static int previousValue = 0;
  static bool beat = false;
  if (value > threshold && !beat) {
    beat = true;
    previousValue = value;
    return true;
  } 
  else if (value < threshold && beat) {
    beat = false;
  }

  previousValue = value;
  return false;
}