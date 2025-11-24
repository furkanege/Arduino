// ===========================================================
//   KY-039 Basic Heart Rate Monitor (LCD + Buzzer)
//   Simple peak detection, basic thresholding,
//   moving text "Place Your Finger"
//   Author: Furkan Ege
// ===========================================================

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define sensorThreshold 700.0    // Minimum value to detect finger contact
#define pulseSamples 20          // Sample count for averaging
#define peakSamples 5            // Consecutive rises to confirm a peak
LiquidCrystal_I2C lcd(0x27, 16, 2);
int sensorPin = A0;
int buzzer = 12;
char initString[] = "Place Finger";
float currentRead;
float lastRead;
int riseCount = 0;
int fallCount = 0;
bool peakDetected = false;

// -----------------------------------------------------------
// Setup
// -----------------------------------------------------------
void setup() {
  pinMode(sensorPin, INPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.print("Place Finger");
  lcd.setCursor(0, 1);
  lcd.print(" on Sensor");
}

// -----------------------------------------------------------
// Main Loop
// -----------------------------------------------------------
void loop() {
  while (analogRead(sensorPin) < sensorThreshold) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(initString);
    for (int i = 0; i < 16; i++) {
      lcd.scrollDisplayLeft();
      delay(200);
    }
  }

  float heartBeat;
  int sum = 0;
  for (int i = 0; i < pulseSamples; i++) {
    sum += analogRead(sensorPin);
  }

  heartBeat = sum / pulseSamples;
  Serial.println(heartBeat);
  currentRead = heartBeat;
  if (currentRead > lastRead) {
    riseCount++;
    if (riseCount > peakSamples) {
      if (!peakDetected) {
        digitalWrite(buzzer, HIGH);
        lcd.clear();
        lcd.print("Heart Rate:");
        lcd.setCursor(0, 1);
        lcd.print((int)heartBeat);
        peakDetected = true;
      }
      riseCount = 0;
      fallCount = 0;
    }
  }
  if (currentRead < lastRead) {
    fallCount++;
    if (fallCount > peakSamples) {
      riseCount = 0;
      fallCount = 0;
      peakDetected = false;
    }
  }
  lastRead = currentRead;
  digitalWrite(buzzer, LOW);
  lcd.clear();
  lcd.print("Heart Rate:");
  lcd.setCursor(0, 1);
  lcd.print((int)(heartBeat / 10));
  delay(5);
}