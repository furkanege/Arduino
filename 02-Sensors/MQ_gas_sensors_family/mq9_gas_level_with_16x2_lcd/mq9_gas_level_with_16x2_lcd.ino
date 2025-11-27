/*
  Project: MQ-9 Gas Level Monitor (16x2 I2C LCD)
  File: mq9_gas_level_with_16x2_lcd.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.1
  Last Updated: 11/27/2025

  Description:
    Reads gas level using the MQ-9 sensor and displays:
      - Air quality percentage (0–100%)
      - Live updated values on LCD
      - Warning LED activation on high gas concentration

    MQ-9 is designed for CO, LPG, and CH4 detection.

  Wiring:
    MQ-9 Sensor:
      AO   → A0
      VCC → 5V
      GND → GND

    LED:
      + → D13 (with resistor)
      - → GND

    LCD (I2C):
      SDA → A4
      SCL → A5
      VCC → 5V
      GND → GND

  Libraries:
    - Wire.h
    - LiquidCrystal_I2C.h

  Serial Baud:
    9600

  Notes:
    - GAS_THRESHOLD is percentage-based.
    - MQ-9 requires a few minutes of preheating for accurate readings.
    - Fully converted to English for consistency with project template.

  Real-World Applications:
    - Air quality monitoring
    - CO / combustible gas leak alarms
    - Safety control panels

  License: GPL-3.0
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int MQ9_PIN = A0;    // MQ-9 sensor analog pin
const int LED_PIN = 13;    // Warning LED
const int GAS_THRESHOLD = 50;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Gas Detection");
  lcd.setCursor(0, 1);
  lcd.print("System Loading...");
  delay(2000);
  lcd.clear();
}

void loop() {
  int sensorValue = analogRead(MQ9_PIN);
  int gasLevel = map(sensorValue, 0, 1023, 0, 100);
  Serial.print("Gas Level: ");
  Serial.print(gasLevel);
  Serial.println("%");
  lcd.setCursor(0, 0);
  lcd.print("Air Quality:    ");
  lcd.setCursor(0, 1);
  lcd.print("% ");
  lcd.print(gasLevel);
  lcd.print("    "); // Clear previous characters

  if (gasLevel > GAS_THRESHOLD) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  delay(500);
}