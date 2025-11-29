/*
  Project: Environment Monitoring System
  File: environment_monitor.ino
  Author: Furkan Ege
  Board: Arduino Uno
  Version: 1.0
  Last Updated: 11/29/2025

  Description:
    Second major version of the Environment Monitoring System.
    This system measures temperature, humidity, gas level,
    flame detection, and includes a touch-controlled LCD toggle.

    Features:
      - DHT11 sensor for temperature & humidity
      - MQ-9 gas sensor with air-quality classification
      - Flame sensor with emergency alarm mode
      - Touch sensor to enable/disable LCD screen
      - 16x2 I2C LCD display (0x27 address)
      - Breathing LED effect in fire mode
      - Buzzer alarm for flame detection
      - LCD refreshes without flicker

  Wiring:

    DHT11:
      VCC → 5V
      GND → GND
      DATA → D2

    MQ-9 Gas Sensor:
      AOUT → A0
      VCC  → 5V
      GND  → GND

    Flame Sensor:
      DO → D3
      VCC → 5V
      GND → GND

    Touch Sensor:
      OUT → D4
      VCC → 5V
      GND → GND

    Buzzer:
      + → D8
      - → GND

    LED:
      + → D9 (PWM)
      - → GND

    16x2 I2C LCD (0x27):
      SDA → A4
      SCL → A5
      VCC → 5V
      GND → GND

  Libraries:
    - Wire.h
    - LiquidCrystal_I2C.h
    - DHT.h

  Serial Baud:
    9600

  Real-World Applications:
    - Classroom/workshop safety system
    - Indoor environmental awareness
    - Gas and fire early warning
    - STEM/IoT education projects

  License: GPL-3.0
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#define DHTPIN       2
#define DHTTYPE      DHT11
#define MQ9_PIN      A0
#define FLAME_PIN    3
#define TOUCH_PIN    4
#define BUZZER_PIN   8
#define LED_PIN      9
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);
bool lcd_enabled = true;
unsigned long lastTouch = 0;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  dht.begin();
  pinMode(MQ9_PIN, INPUT);
  pinMode(FLAME_PIN, INPUT);
  pinMode(TOUCH_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  lcd.setCursor(0, 0);
  lcd.print("System Ready");
  delay(2000);
  lcd.clear();
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int gasValue = analogRead(MQ9_PIN);
  int flameValue = digitalRead(FLAME_PIN);
  int touchValue = digitalRead(TOUCH_PIN);

  if (touchValue == HIGH && millis() - lastTouch > 500) {
    lcd_enabled = !lcd_enabled;
    lastTouch = millis();
  }
  if (flameValue == LOW) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WARNING");
    lcd.setCursor(0, 1);
    lcd.print("FIRE DETECTED!");
    for (int i = 0; i < 255; i++) {
      analogWrite(LED_PIN, i);
      delay(3);
    }
    for (int i = 255; i > 0; i--) {
      analogWrite(LED_PIN, i);
      delay(3);
    }
    tone(BUZZER_PIN, 1000, 200);
    delay(100);
    return;
  }

  noTone(BUZZER_PIN);
  analogWrite(LED_PIN, 0);
  lcd.clear();

  if (lcd_enabled) {
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("Hum: ");
    lcd.print(humidity);
    lcd.print("%");
    lcd.setCursor(10, 1);
    if (gasValue < 200)      lcd.print("Clean");
    else if (gasValue < 500) lcd.print("Mid");
    else                     lcd.print("Dirty");
  }
  delay(1000);
}