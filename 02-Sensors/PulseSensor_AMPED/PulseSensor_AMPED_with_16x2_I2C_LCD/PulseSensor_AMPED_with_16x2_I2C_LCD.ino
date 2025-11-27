/*
  Project: PulseSensor AMPED Heartbeat Monitor (I2C LCD + Buzzer)
  File: PulseSensor_AMPED_with_16x2_I2C_LCD.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.1
  Last Updated: 11/27/2025

  Description:
    Reads heart rate (BPM) using the PulseSensor AMPED module and:
      - Displays BPM on a 16x2 I2C LCD
      - Activates a buzzer when BPM exceeds threshold
      - Optionally blinks LED on heartbeat

  Wiring:
    PulseSensor:
      Signal → A0
      VCC    → 5V
      GND    → GND

    LCD (I2C):
      SDA → A4
      SCL → A5
      VCC → 5V
      GND → GND

    Buzzer:
      + → D8
      - → GND

  Libraries:
    - LiquidCrystal_I2C.h
    - PulseSensorPlayground.h

  Serial Baud:
    9600

  Notes:
    - pulseSensor.setThreshold(550) may require tuning.
    - BPM alarm threshold = 100 by default.
    - pulseSensor.sawNewSample() MUST run frequently.

  License: GPL-3.0
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <PulseSensorPlayground.h>

const int BUZZER_PIN = 8;
const int PULSE_PIN  = A0;
PulseSensorPlayground pulseSensor;
LiquidCrystal_I2C lcd(0x27, 16, 2);
int alarmState = 0;

void setup() {
  Serial.begin(9600);
  pulseSensor.analogInput(PULSE_PIN);
  pulseSensor.blinkOnPulse(13);     // Optional LED blink
  pulseSensor.setSerial(Serial);
  pulseSensor.setThreshold(550);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pulse Sensor");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(1500);
}

void loop() {
  int bpm = pulseSensor.getBeatsPerMinute();
  pulseSensor.sawNewSample();

  if (pulseSensor.sawStartOfBeat()) {
    Serial.print("BPM: ");
    Serial.println(bpm);
    alarmState = (bpm > 100) ? 1 : 0;
  }

  digitalWrite(BUZZER_PIN, alarmState ? HIGH : LOW);
  lcd.setCursor(0, 0);
  lcd.print("BPM: ");
  lcd.print(bpm);
  lcd.print("     "); // Clear old chars
  delay(200);
}