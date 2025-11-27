/*
  Project: MQ-2 Gas Alarm with Fan
  File: mq2_gas_alarm_with_fan.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/27/2025

  Description:
    Uses the MQ-2 gas sensor to detect gas concentration and:
      - Activates a warning LED
      - Activates a buzzer
      - Activates an exhaust fan for a fixed duration

    Demonstrates a basic automatic gas evacuation and alert system.

  Wiring:
    MQ-2 Sensor:
      - AO → A0
      - VCC → 5V
      - GND → GND

    LED:
      - + → D13 (with resistor)
      - - → GND

    Buzzer:
      - + → D12
      - - → GND

    Fan (transistor or relay required):
      - IN → D11
      - External supply recommended (Arduino 5V cannot drive motors directly!)

  Libraries:
    - (No external libraries required)

  Serial Baud:
    9600

  Notes:
    - gasThreshold must be calibrated depending on the environment.
    - MQ-2 requires heating time to stabilize.
    - This is a corrected version: duplicate code blocks removed.

  Real-World Applications:
    - Gas leak detection
    - Automatic evacuation systems
    - Fire/gas safety demos

  License: GPL-3.0
*/

int mq2Pin = A0;      // MQ2 Analog Output
int ledPin = 13;      // Warning LED
int buzzerPin = 12;   // Buzzer
int fanPin = 11;      // Exhaust Fan
int mq2Val = 0;          // Raw sensor value
int gasThreshold = 100;  // Adjustable threshold

void setup() {
  pinMode(mq2Pin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(fanPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  mq2Val = analogRead(mq2Pin);
  Serial.print("Gaz Değeri: ");
  Serial.println(mq2Val);
  if (mq2Val > gasThreshold) {
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(fanPin, HIGH);
    delay(5000);          // Fan runs for 5 seconds
    digitalWrite(fanPin, LOW);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledPin, LOW);
  }
  else {
    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledPin, LOW);
    digitalWrite(fanPin, LOW);
  }
  
  delay(100);
}