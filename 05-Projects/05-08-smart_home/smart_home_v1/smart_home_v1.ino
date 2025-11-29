/*
  Project: Smart Home Monitor – v1
  File: smart_home_v1.ino
  Author: Furkan Ege
  Board: Arduino Nano
  Version: 1.0
  Last Updated: 11/29/2025

  Description:
    Version 1 of the Smart Home Monitor system.
    This system displays temperature, atmospheric pressure,
    and gas levels on an SSD1306 OLED screen and over Serial.

    Features:
      - BMP280 sensor for temperature & pressure
      - MQ gas sensor with threshold-based alerting
      - OLED display with 3 auto-switching pages
      - Page cycling every 2 seconds
      - Serial output logging (gas, temp, pressure)
      - Automatic sensor fallback using last valid readings

  Wiring:
    BMP280 (0x76):
      VCC → 3.3V
      GND → GND
      SDA → A4
      SCL → A5

    OLED SSD1306 (0x3C):
      VCC → 5V
      GND → GND
      SDA → A4
      SCL → A5

    MQ Gas Sensor:
      AOUT → A0
      VCC  → 5V
      GND  → GND

  Libraries:
    - Wire.h
    - Adafruit_SSD1306.h
    - Adafruit_GFX.h   (Dependency of SSD1306)
    - Adafruit_BMP280.h

  Serial Baud:
    9600

  Real-World Applications:
    - Home environmental awareness
    - Gas leak monitoring
    - STEM education projects
    - Sensor fusion & UI development baseline

  License: GPL-3.0
*/

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BMP280.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

Adafruit_BMP280 bmp;

#define MQ_PIN A0
#define GAS_THRESHOLD 300

int page = 0;
unsigned long lastSwitch = 0;

float lastTemp = 0;
float lastPress = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED NOT FOUND!");
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("System Booting...");
  display.display();

  if (!bmp.begin(0x76)) {
    Serial.println("BMP280 INIT FAILED!");
    display.setCursor(0,10);
    display.println("BMP280 INIT ERR!");
    display.display();
    while (1);
  }

  delay(1500);
  display.clearDisplay();
}

void loop() {
  int mq_raw = analogRead(MQ_PIN);
  float temperature = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100.0;

  if (!isnan(temperature) && !isnan(pressure)) {
    lastTemp = temperature;
    lastPress = pressure;
  }

  Serial.print("Gas: "); Serial.print(mq_raw);
  Serial.print(" | Temp: "); Serial.print(lastTemp);
  Serial.print(" C | Pressure: "); Serial.println(lastPress);

  if (millis() - lastSwitch > 2000) {
    page = (page + 1) % 3;
    lastSwitch = millis();
  }

  display.clearDisplay();

  if (page == 0) {
    display.setTextSize(2);
    display.setCursor(0,0);
    display.println("ENV");
    display.setTextSize(1);
    display.setCursor(0,25);
    display.print("Temp: ");
    display.print(lastTemp);
    display.println(" C");
    display.setCursor(0,40);
    display.print("Pressure: ");
    display.print(lastPress);
    display.println(" hPa");
  }

  if (page == 1) {
    display.setTextSize(2);
    display.setCursor(0,0);
    display.println("GAS");
    display.setTextSize(1);
    display.setCursor(0,25);
    display.print("Raw: ");
    display.println(mq_raw);
    display.setCursor(0,40);
    if (mq_raw > GAS_THRESHOLD)
      display.println("⚠ ALERT! HIGH GAS");
    else
      display.println("Status: Normal");
  }

  if (page == 2) {
    display.setTextSize(1);
    display.setCursor(0,0);
    display.println("=== SUMMARY ===");
    display.setCursor(0,15);
    display.print("Temp: ");
    display.print(lastTemp);
    display.println(" C");
    display.setCursor(0,30);
    display.print("Press: ");
    display.print(lastPress);
    display.println(" hPa");
    display.setCursor(0,45);
    display.print("Gas: ");
    display.print(mq_raw);
    if (mq_raw > GAS_THRESHOLD) display.print(" (ALERT!)");
  }

  display.display();
  delay(100);
}