/*
  Project: 16x2 LCD – Hello World (I2C Version 1)
  File: LCD_2x16-I2C_version_1_hello_world.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/27/2025

  Description:
    Demonstrates the simplest usage of a 16x2 I2C LCD.
    Prints "Hello World" and a counter on the second row.

  Wiring:
    LCD (I2C):
      SDA → A4
      SCL → A5
      VCC → 5V
      GND → GND

  Libraries:
    - LiquidCrystal_I2C.h
    - Wire.h

  Serial Baud:
    9600 (optional)

  Notes:
    - Default I2C address is usually 0x27 or 0x3F.
    - Use an I2C scanner if display does not respond.

  Real-World Applications:
    - I2C-based sensor dashboards
    - Robotics UI panels
    - Compact display systems

  License: GPL-3.0
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.begin();
  lcd.backlight();
  lcd.print("Hello World");
}

void loop() {
  lcd.setCursor(0, 1);
  lcd.print("Saniye: ");
  lcd.print(millis() / 1000);
  delay(500);
}