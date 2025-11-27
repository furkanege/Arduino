/*
  Project: 16x2 I2C LCD – Scrolling "HELLO WORLD!" Demo
  File: LCD_2x16-I2C_version_3.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/27/2025

  Description:
    Demonstrates horizontal manual scrolling on the second line of a
    16x2 I2C LCD while keeping a static "WELCOME" text on the first line.

    Behavior:
      - Line 1 shows: "WELCOME" centered-ish at column 3
      - Line 2 scrolls the message "HELLO WORLD!" from left to right

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
    (Not used)

  Notes:
    - I2C address set to 0x27 (change if required).
    - Uses manual write + erase for smooth scrolling.
    - Avoids scrollDisplayLeft() for strict positional control.

  Real-World Applications:
    - Welcome screens
    - Menu systems
    - Educational LCD demos

  License: GPL-3.0
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
const char SCROLL_TEXT[] = "HELLO WORLD!";

void setup() {
  lcd.init();
  lcd.backlight();
}

void loop() {
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("WELCOME");

  int textLen = strlen(SCROLL_TEXT);

  for (int pos = 0; pos <= 16; pos++) {
    lcd.setCursor(pos, 1);
    lcd.print(SCROLL_TEXT);
    delay(300);
    lcd.setCursor(pos, 1);
    for (int i = 0; i < textLen; i++) {
      lcd.print(" ");
    }
  }
}