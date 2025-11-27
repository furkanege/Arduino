/*
  Project: DHT11 Temperature & Humidity Monitor (LCD 16x2 Display)
  File: dht_11_with_16x2_lcd.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/26/2025

  Description:
    Reads temperature and humidity values from the DHT11 sensor and displays
    them on a 16x2 LCD (I2C). Demonstrates environmental monitoring using
    a common LCD interface for visual feedback.

  Wiring:
    DHT11:
      - Signal → D2
      - VCC → 5V
      - GND → GND

    LCD 16x2 (I2C):
      - SDA → A4
      - SCL → A5
      - VCC → 5V
      - GND → GND

  Libraries:
    - DHT Sensor Library by Adafruit
    - Adafruit Unified Sensor
    - LiquidCrystal_I2C Library

  Serial Baud:
    (Not used)

  Example LCD Output:
    Temp: 24°C
    Humi: 56%

  Notes:
    - DHT11 reads must be spaced at least 1 second apart.
    - Ensure correct I2C address (often 0x27 or 0x3F).
    - Use external pull-ups if your LCD module lacks them.

  Real-World Applications:
    - Indoor climate display
    - Smart greenhouse control
    - Classroom/office environment monitoring

  License: GPL-3.0
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C LCD with common address

void setup() {
  dht.begin();
  lcd.init();
  lcd.backlight();
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print((char)223);  // Degree symbol
  lcd.print("C  ");

  lcd.setCursor(0, 1);
  lcd.print("Humi: ");
  lcd.print(humidity);
  lcd.print("%   ");

  delay(800); // Smooth LCD refresh
}