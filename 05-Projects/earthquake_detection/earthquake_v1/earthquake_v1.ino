/*
  Project: Earthquake Detection System (v1)
  File: earthquake_v1.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/28/2025

  Description:
    This is Version 1 of the Earthquake Detection System.
    The MPU6050 accelerometer is used to measure sudden changes
    in acceleration magnitude. If the difference between the
    current magnitude and the previous magnitude exceeds a threshold,
    an earthquake alert is triggered.

    Features:
      - MPU6050 motion detection (acceleration magnitude)
      - Threshold-based earthquake detection
      - LCD alert display
      - Buzzer alarm
      - LED indicator
      - Serial monitor output

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
    - Educational demonstration of motion sensing
    - Simple earthquake simulation systems
    - Shake/impact detector projects

  Version Notes:
    - This baseline version uses a simple delta-acceleration threshold.
    - No smoothing, filtering or RMS analysis is applied.
    - v2 will include improved algorithms.

  License: GPL-3.0
*/

#include <Wire.h>
#include <LiquidCrystal.h>
#include <MPU6050.h>
#include <math.h>
MPU6050 mpu;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
double xAccel, yAccel, zAccel;
float newAccelMag = 0;
float oldAccelMag = 0;
double diff = 0; 
float threshold = 0.4;
int buzzer = 10;
int LED = 13;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);  
  lcd.print("Sistem Hazir...");
  lcd.setCursor(0, 1);  
  lcd.print("Bekleyin...");
  delay(2000);
  lcd.clear();
  pinMode(LED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(LED, LOW);
  digitalWrite(buzzer, LOW);
  Serial.println("Sistem Hazir");
}

void loop() {
  xAccel = mpu.getAccelerationX() / 16384.0;
  yAccel = mpu.getAccelerationY() / 16384.0;
  zAccel = mpu.getAccelerationZ() / 16384.0;
  newAccelMag = sqrt(pow(xAccel, 2) + pow(yAccel, 2) + pow(zAccel, 2));
  diff = abs(newAccelMag - oldAccelMag);
  oldAccelMag = newAccelMag;

  Serial.print("Accel Mag: ");
  Serial.print(newAccelMag);
  Serial.print(" | Diff: ");
  Serial.println(diff);

  if (diff > threshold) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Earthquake!");
    lcd.setCursor(0, 1);
    lcd.print("Shaking...");
    digitalWrite(LED, HIGH);
    digitalWrite(buzzer, HIGH);
    tone(buzzer, 1000);
    delay(2000);
    noTone(buzzer);
    digitalWrite(buzzer, LOW);
    digitalWrite(LED, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Monitoring...");
  }
  delay(100);
}