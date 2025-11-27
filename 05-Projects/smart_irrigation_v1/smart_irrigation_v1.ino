/*
  Project: Smart Irrigation System v1 (LCD + Soil Moisture + Pump Control)
  File: smart_irrigation_v1.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/27/2025

  Description:
    Smart irrigation system using:
      - Soil moisture sensor (analog)
      - Water pump (relay controlled)
      - 16x2 I2C LCD status display

    Behavior:
      - If soil moisture is dry → pump activates
      - If moisture is medium/high → pump turns OFF
      - LCD shows:
          * Pump status (ACTIVE / OFF)
          * Moisture level (HIGH / MED / LOW)

  Wiring:
    Soil Moisture Sensor:
      AO  → A0
      VCC → 5V
      GND → GND

    Water Pump Relay:
      IN  → D2
      VCC → 5V
      GND → GND
      NOTE: Many relay modules are ACTIVE-LOW.
            This code assumes:
              LOW  → Pump ON
              HIGH → Pump OFF

    LCD (I2C):
      SDA → A4
      SCL → A5
      VCC → 5V
      GND → GND

  Libraries:
    - LiquidCrystal_I2C.h
    - Wire.h

  Serial Baud:
    9600

  Notes:
    - moistureValue varies by sensor model; thresholds should be calibrated.
    - Typical readings:
        0–300   = High moisture
        300–950 = Medium moisture
        >950    = Dry soil → Pump ON
    - LCD requires a short delay after initialization.

  Real-World Applications:
    - Automated home plant watering
    - Smart garden systems
    - School IoT projects
    - Agricultural automation demos

  License: GPL-3.0
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
const int pumpPin = 2;             // Relay control pin
const int soilMoisturePin = A0;    // Soil moisture sensor AO

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  pinMode(pumpPin, OUTPUT);
  digitalWrite(pumpPin, HIGH);    // Pump OFF (active-low relay)
  lcd.setCursor(0, 0);
  lcd.print("Smart Irrigation");
  lcd.setCursor(0, 1);
  lcd.print("System Active...");
  delay(3000);
  lcd.clear();
}

void loop() {
  int moistureValue = analogRead(soilMoisturePin);
  Serial.println(moistureValue);

  if (moistureValue > 950) {
    digitalWrite(pumpPin, LOW);
    lcd.setCursor(0, 0);
    lcd.print("Pump: ACTIVE   ");
  } else {
    digitalWrite(pumpPin, HIGH);
    lcd.setCursor(0, 0);
    lcd.print("Pump: OFF      ");
  }
  lcd.setCursor(0, 1);

  if (moistureValue < 300) {
    lcd.print("Moisture: HIGH ");
  }
  else if (moistureValue < 950) {
    lcd.print("Moisture: MED  ");
  }
  else {
    lcd.print("Moisture: LOW  ");
  }
  delay(300);
}