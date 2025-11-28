/*
  Project: Smart Irrigation System (v3)
  File: smart_irrigation_v3.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 3.0
  Last Updated: 11/28/2025

  Description:
    This is Version 3 of the Smart Irrigation System.
    Unlike v1 and v2, this version uses a dual-pin H-bridge style
    output for driving a water pump (IN1/IN2 control instead of relay).

    Additionally, the soil moisture value is mapped in reverse:
      Raw 1023 → 0% moisture (dry)
      Raw 0    → 100% moisture (wet)

    Features:
      - LCD display (I2C 16x2)
      - Reversed mapping for moisture (%0–100)
      - Dual-pin pump motor control (IN1/IN2)
      - Pump activation when moisture < 10%
      - "Pump started" / "Moisture sufficient" messages

  Wiring:
    Soil Moisture Sensor:
      Signal → A0
      VCC → 5V
      GND → GND

    Water Pump Motor L298N (H-Bridge style):
      IN1 → D9
      IN2 → D10
      VCC → External Motor Supply
      GND → Shared Ground

    LCD Display (I2C 0x27):
      SDA → A4
      SCL → A5
      VCC → 5V
      GND → GND

  Libraries:
    - LiquidCrystal_I2C.h
    - Wire.h

  Serial Baud:
    9600

  Real-World Applications:
    - Motor-based water irrigation systems
    - Small robotic pumps and fluid systems
    - Agriculture and school STEM automation projects

  Version Notes:
    - Main difference from v1/v2: pump is controlled via two digital pins.
    - Reversed moisture mapping (1023→0%).
    - Very strict moisture threshold (<10%).
    - LCD message texts updated.

  License: GPL-3.0
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define SOIL_MOISTURE_PIN A0
#define PUMP_IN1_PIN 9
#define PUMP_IN2_PIN 10

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("System Ready");
  delay(2000);
  lcd.clear();
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  pinMode(PUMP_IN1_PIN, OUTPUT);
  pinMode(PUMP_IN2_PIN, OUTPUT);
  digitalWrite(PUMP_IN1_PIN, LOW);
  digitalWrite(PUMP_IN2_PIN, LOW);
}

void loop() {
  int rawMoisture = analogRead(SOIL_MOISTURE_PIN);
  float moisturePercent = map(rawMoisture, 1023, 0, 0, 100);
  lcd.setCursor(0, 0);
  lcd.print("Moisture:%");
  lcd.print(moisturePercent);
  lcd.print("   ");

  if (moisturePercent < 10) {
    digitalWrite(PUMP_IN1_PIN, HIGH);
    digitalWrite(PUMP_IN2_PIN, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Watering...     ");
  } 
  else {
    digitalWrite(PUMP_IN1_PIN, LOW);
    digitalWrite(PUMP_IN2_PIN, LOW);
    lcd.setCursor(0, 1);
    lcd.print("Moisture OK     ");
  }

  Serial.print("Moisture: ");
  Serial.print(moisturePercent);
  Serial.println(" %");
  delay(1000);
}