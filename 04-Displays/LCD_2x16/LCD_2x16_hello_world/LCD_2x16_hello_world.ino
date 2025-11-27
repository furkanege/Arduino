/*
  Project: 16x2 LCD – Hello World (Parallel Mode)
  File: LCD_2x16_hello_world.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/27/2025

  Description:
    Demonstrates basic usage of a 16x2 parallel LCD using the LiquidCrystal library.
    Prints "Hello World" and a simple counter that updates every second.

  Wiring:
    LCD – Parallel (4-bit Mode):
      RS → D7
      E  → D8
      D4 → D9
      D5 → D10
      D6 → D11
      D7 → D12
      VCC → 5V
      GND → GND
      VO → Potentiometer middle pin (for contrast)

  Libraries:
    - LiquidCrystal.h (built-in)

  Serial Baud:
    9600 (optional, not used)

  Notes:
    - Contrast must be adjusted with potentiometer (typically 10k).
    - LCD requires ~1–2 seconds after power-up for stable operation.

  Real-World Applications:
    - Menu systems
    - Debug displays
    - Sensor readout screens

  License: GPL-3.0
*/

#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup() {
  lcd.begin(16, 2);
  lcd.print("Hello World!");
}

void loop() {
  lcd.setCursor(0, 1);
  lcd.print("Counter: ");
  lcd.print(millis() / 1000);
  delay(500);
}