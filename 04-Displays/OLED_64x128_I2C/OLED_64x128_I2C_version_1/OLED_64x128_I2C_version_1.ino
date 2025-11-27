/*
  Project: 128x64 OLED I2C Display – Basic Text Demo
  File: OLED_64x128_I2C_version_1.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/27/2025

  Description:
    Demonstrates simple text output on a 128x64 SSD1306 OLED display
    using the Adafruit GFX + SSD1306 libraries.

    Behavior:
      - Initializes screen
      - Displays two lines of text
      - Holds static message on screen

  Wiring:
    OLED Display (SSD1306 I2C):
      SDA → A4
      SCL → A5
      VCC → 5V
      GND → GND

  Libraries:
    - Adafruit GFX Library
    - Adafruit SSD1306 Library
    - Wire.h

  Display:
    Resolution: 128 × 64
    Address:    0x3C

  Notes:
    - If your display does not show anything, double-check I2C address.
    - Some SSD1306 modules use 0x3D instead of 0x3C.
    - OLEDs do not require backlight; pixels illuminate individually.

  Real-World Applications:
    - Wearable devices
    - Sensor dashboards
    - Compact UI systems

  License: GPL-3.0
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1        // Reset pin (not used on most I2C OLEDs)
#define OLED_ADDR 0x3C       // Common I2C address

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    for (;;) ;  // If display fails, stop here
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("OLED Display Ready!");
  display.setCursor(0, 16);
  display.println("Hello Massachusetts!");
  display.display();
}

void loop() {
  // Screen stays static; nothing here
}