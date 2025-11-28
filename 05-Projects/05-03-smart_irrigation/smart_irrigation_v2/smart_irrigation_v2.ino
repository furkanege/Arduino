/*
  Project: Smart Irrigation System (v2)
  File: smart_irrigation_v2.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 2.0
  Last Updated: 11/28/2025

  Description:
    This is Version 2 of the Smart Irrigation System.
    Unlike v1, this version converts the raw soil moisture value
    into a percentage using map() and constrain().

    Features:
      - LCD display (I2C 16x2)
      - Soil moisture reading converted to percentage (%0–100)
      - Pump activation based on % moisture threshold
      - Display of real-time soil moisture percentage
      - Relay control for water pump (LOW = active)

  Wiring:
    Soil Moisture Sensor:
      Signal → A0
      VCC → 5V
      GND → GND

    Water Pump Relay:
      IN → D6
      VCC → 5V
      GND → GND

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
    - Indoor plant irrigation
    - Smart greenhouse automation
    - Educational robotics and STEM projects
    - Automatic moisture-based watering systems

  Version Notes:
    - v2 uses map() to convert the sensor reading (1024→290) into %0–100.
    - Motor ON/OFF is determined by moisture percentage (>30% OFF, ≤30% ON).
    - This version adds more user-friendly readings on the LCD.

  License: GPL-3.0
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define RELAY_PIN 6

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C LCD at address 0x27
int soilSensorPin = A0;
int soilMoistureValue;

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  pinMode(soilSensorPin, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, 1);
  lcd.setCursor(0, 0);
  lcd.print("SST Robotics");
  lcd.setCursor(0, 1);
  lcd.print("Auto Irrigation");
  delay(2000);
}

void loop() {
  soilMoistureValue = analogRead(soilSensorPin);

  soilMoistureValue = map(soilMoistureValue, 1024, 290, 0, 100);
  soilMoistureValue = constrain(soilMoistureValue, 0, 100);

  Serial.print("Soil Moisture: ");
  Serial.print(soilMoistureValue);
  Serial.println(" %");
  lcd.setCursor(0, 0);
  lcd.print("Soil: ");
  lcd.print(soilMoistureValue);
  lcd.print("%   ");

  if (soilMoistureValue > 30 && soilMoistureValue <= 100) {
    digitalWrite(RELAY_PIN, 1);
    lcd.setCursor(0, 1);
    lcd.print("Pump: OFF         ");
  } else {
    digitalWrite(RELAY_PIN, 0);
    lcd.setCursor(0, 1);
    lcd.print("Pump: ON          ");
  }
  delay(1000);
}