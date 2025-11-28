/*
  Project: Earthquake Detection System (v2 - Advanced Algorithm)
  File: earthquake_v2.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 2.0
  Last Updated: 11/28/2025

  Description:
    This is Version 2 of the Earthquake Detection System with a
    significantly improved detection algorithm.

    Enhancements over v1:
      - Gravity calibration (automatic offset removal)
      - Moving Average Filter (5 samples)
      - RMS vibration measurement
      - Shock counter (requires multiple consecutive shake events)
      - Duration check (ignores short impulses)
      - Greatly reduced false positives

    Features:
      - MPU6050 accelerometer/gyro integration
      - LCD real-time display
      - Audible buzzer alarm
      - LED earthquake indicator

  Wiring:
    MPU6050:
      VCC → 5V
      GND → GND
      SDA → A4
      SCL → A5

    LCD Display (16x2):
      RS → D12
      EN → D11
      D4 → D5
      D5 → D4
      D6 → D3
      D7 → D2
      VCC → 5V
      GND → GND

    Indicators:
      LED  → D13
      Buzzer → D10

  Libraries:
    - Wire.h
    - LiquidCrystal.h
    - MPU6050.h

  Serial Baud:
    9600

  Real-World Applications:
    - Prototype early earthquake-warning systems
    - Vibration analysis projects
    - Educational seismology tools

  License: GPL-3.0
*/

#include <Wire.h>
#include <LiquidCrystal.h>
#include <MPU6050.h>
#include <math.h>
MPU6050 mpu;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
float offsetX = 0, offsetY = 0, offsetZ = 0;
float filterBuffer[5];
int filterIndex = 0;
float rmsBuffer[10];
int rmsIndex = 0;
float thresholdRMS = 0.20;  
int shockNeeded = 3;
int shockCount = 0;
int buzzer = 10;
int LED = 13;

void calibrateSensor() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Calibrating...");
  lcd.setCursor(0, 1);
  lcd.print("Don't move!");
  float sumX = 0, sumY = 0, sumZ = 0;

  for (int i = 0; i < 100; i++) {
    sumX += mpu.getAccelerationX() / 16384.0;
    sumY += mpu.getAccelerationY() / 16384.0;
    sumZ += mpu.getAccelerationZ() / 16384.0;
    delay(10);
  }

  offsetX = sumX / 100.0;
  offsetY = sumY / 100.0;
  offsetZ = sumZ / 100.0;
  delay(500);
  lcd.clear();
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();
  lcd.begin(16, 2);
  pinMode(LED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(LED, LOW);
  digitalWrite(buzzer, LOW);
  calibrateSensor();
  lcd.setCursor(0, 0);
  lcd.print("Monitoring...");
}

float applyMovingAverage(float val) {
  filterBuffer[filterIndex] = val;
  filterIndex = (filterIndex + 1) % 5;
  float sum = 0;
  for (int i = 0; i < 5; i++) sum += filterBuffer[i];
  return sum / 5.0;
}

float computeRMS(float val) {
  rmsBuffer[rmsIndex] = val * val;
  rmsIndex = (rmsIndex + 1) % 10;
  float sum = 0;
  for (int i = 0; i < 10; i++) sum += rmsBuffer[i];
  return sqrt(sum / 10.0);
}

void triggerAlarm() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("EARTHQUAKE!");
  lcd.setCursor(0, 1);
  lcd.print("Shaking...");

  for (int i = 0; i < 5; i++) {
    digitalWrite(LED, HIGH);
    tone(buzzer, 1000);
    delay(200);
    noTone(buzzer);
    digitalWrite(LED, LOW);
    delay(200);
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Monitoring...");
}

void loop() {
  float ax = mpu.getAccelerationX() / 16384.0 - offsetX;
  float ay = mpu.getAccelerationY() / 16384.0 - offsetY;
  float az = mpu.getAccelerationZ() / 16384.0 - offsetZ;
  float magnitude = sqrt(ax * ax + ay * ay + az * az);
  float filtered = applyMovingAverage(magnitude);
  float rmsValue = computeRMS(filtered);
  Serial.print("RMS: ");
  Serial.println(rmsValue);

  if (rmsValue > thresholdRMS) {
    shockCount++;
  } else {
    shockCount = max(0, shockCount - 1);
  }

  if (shockCount >= shockNeeded) {
    triggerAlarm();
    shockCount = 0;
  }
  delay(50);
}