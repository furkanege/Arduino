/*
  Project: MQ-9 Gas Sensor Alarm (CO/CH4/LPG)
  File: mq9_gas_alarm_basic.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/27/2025

  Description:
    Detects carbon monoxide and flammable gas levels using MQ-9.
    Activates LEDs and buzzer based on concentration level.

  Wiring:
    MQ-9:
      AO → A0
      DO → D2 (optional)
      VCC → 5V
      GND → GND

    LEDs:
      GREEN → D7
      YELLOW → D6
      RED → D5

    Buzzer:
      → D8

  Serial Baud:
    9600

  Real-World Applications:
    - CO monitoring
    - Fire safety modules
    - Smart alarm systems

  License: GPL-3.0
*/

#define MQ9_PIN A0
#define RED_LED 5
#define YELLOW_LED 6
#define GREEN_LED 7
#define BUZZER 8

int gasValue = 0;

void setup() {
  Serial.begin(9600);

  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
}

void loop() {
  gasValue = analogRead(MQ9_PIN);
  Serial.print("MQ-9 Value: ");
  Serial.println(gasValue);

  if (gasValue > 700) {
    digitalWrite(RED_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BUZZER, HIGH);
  }
  else if (gasValue > 400) {
    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BUZZER, LOW);
  }
  else {
    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BUZZER, LOW);
  }
}