/*
  Project: KY-039 Pulse Sensor with LCD (Basic Version)
  File: ky_039_with_16x2_lcd_basic.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/26/2025

  Description:
    Simple KY-039 heart rate monitor using a 16x2 I2C LCD and buzzer.
    Includes:
      - Basic threshold-based peak detection
      - Moving "Place Finger" text animation
      - Sample averaging for stability
      - LCD live pulse visualization

  Wiring:
    KY-039 Pulse Sensor:
      - Signal → A0
      - VCC → 5V
      - GND → GND

    LCD 16x2 (I2C):
      - SDA → A4
      - SCL → A5
      - VCC → 5V
      - GND → GND

    Buzzer:
      - + → D12
      - - → GND

  Libraries:
    - Wire.h
    - LiquidCrystal_I2C.h

  Serial Baud:
    9600

  Example LCD Output:
    Heart Rate:
    82

  Notes:
    - KY-039 requires very steady finger placement.
    - This BASIC version does NOT calculate BPM.
    - Only raw pulse amplitude visualized.

  Real-World Applications:
    - Pulse waveform education
    - Biomedical demos
    - Basic heart rate indicator projects

  License: GPL-3.0
*/

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