/*
  Project: 16x2 I2C LCD – Text & Scrolling Demo
  File: LCD_2x16-I2C_version_2.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.1
  Last Updated: 11/27/2025

  Description:
    Shows three different messages on a 16x2 I2C LCD:
      - "Hello / Everyone!"
      - "This is / Massachusetts"
      - "Welcome To" + scrolling "Our State!" on the second line

    Demonstrates static text, backlight control and manual horizontal scrolling.

  Wiring:
    LCD (I2C):
      SDA → A4
      SCL → A5
      VCC → 5V
      GND → GND

  Libraries:
    - Wire.h
    - LiquidCrystal_I2C.h

  Serial Baud:
    (Not used)

  Notes:
    - I2C address is set to 0x27 (change if needed).
    - Scrolling uses manual writing/erasing instead of scrollDisplayLeft()
      to keep full control over the text.

  Real-World Applications:
    - Simple status screens
    - Information banners
    - UI/message demos

  License: GPL-3.0
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
const char MSG1_LINE1[] = "Hello";
const char MSG1_LINE2[] = "Everyone!";
const char MSG2_LINE1[] = "This is";
const char MSG2_LINE2[] = "Massachusetts";
const char MSG3_TOP[]   = "Welcome To";
const char MSG3_SCROLL[] = "Our State!";

void showStaticMessage(const char* line1, const char* line2, unsigned long ms) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
  delay(ms);
}

void scrollBottomLine(const char* topText, const char* msg, unsigned long stepDelay) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(topText);
  int len = strlen(msg);
  int maxPos = 16 - len;
  if (maxPos < 0) {
    maxPos = 0; // safety, though msg should be <= 16 chars
  }

  for (int pos = 0; pos <= maxPos; pos++) {
    lcd.setCursor(pos, 1);
    lcd.print(msg);
    delay(stepDelay);
    lcd.setCursor(pos, 1);
    for (int i = 0; i < len; i++) {
      lcd.print(' ');
    }
  }
}

void setup() {
  lcd.init();
  lcd.backlight();
}

void loop() {
  showStaticMessage(MSG1_LINE1, MSG1_LINE2, 2000);
  showStaticMessage(MSG2_LINE1, MSG2_LINE2, 2000);
  lcd.noBacklight();
  delay(150);
  lcd.backlight();
  scrollBottomLine(MSG3_TOP, MSG3_SCROLL, 300);
}