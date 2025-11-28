/*
  Project: Fire and Gas Safety System
  File: fire_and_gas_safety_system.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/27/2025

  Description:
    Combined fire and gas safety system using:
      - Flame sensor (digital + analog)
      - MQ-2 gas sensor (analog)
      - Warning LED and buzzer
      - Exhaust fan for gas evacuation
      - Water pump for fire response

    Behavior:
      - If flame is detected → LED + buzzer + water pump ON.
      - If gas level exceeds threshold → LED + buzzer + fan ON.
      - When everything is normal → all actuators OFF.

  Wiring:
    Flame Sensor:
      DO → D2
      AO → A1
      VCC → 5V
      GND → GND

    MQ-2 Gas Sensor:
      AO → A0
      VCC → 5V
      GND → GND

    LED:
      + → D13 (with resistor)
      - → GND

    Buzzer:
      + → D12
      - → GND

    Fan (via transistor/relay):
      Control → D10

    Water Pump (via transistor/relay):
      Control → D11

  Libraries:
    - (No external libraries required)

  Serial Baud:
    9600

  Notes:
    - gasThreshold and flameThreshold must be tuned for your environment.
    - Fan and pump MUST be driven via transistor/relay, not directly from Arduino.

  Real-World Applications:
    - Basic home safety demo
    - Classroom safety systems
    - Fire/gas training setups

  License: GPL-3.0
*/

const int flamePinDigital = 2;
const int flamePinAnalog  = A1;
const int mq2Pin          = A0;
const int ledPin          = 13;
const int buzzerPin       = 12;
const int fanPin          = 10;
const int waterPumpPin    = 11;
int flameValDigital = 0;
int flameValAnalog  = 0;
int mq2Val          = 0;
int gasThreshold    = 200;  // adjust by test
int flameThreshold  = 300;  // adjust by test

void setup() {
  pinMode(flamePinDigital, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(fanPin, OUTPUT);
  pinMode(waterPumpPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(fanPin, LOW);
  digitalWrite(waterPumpPin, LOW);
  Serial.begin(9600);
}

void loop() {
  flameValDigital = digitalRead(flamePinDigital);
  flameValAnalog  = analogRead(flamePinAnalog);
  mq2Val          = analogRead(mq2Pin);
  Serial.print("FlameD: ");
  Serial.print(flameValDigital);
  Serial.print(" | FlameA: ");
  Serial.print(flameValAnalog);
  Serial.print(" | Gas: ");
  Serial.println(mq2Val);
  bool fireDetected = (flameValDigital == LOW) || (flameValAnalog > flameThreshold);
  bool gasDetected  = (mq2Val > gasThreshold);

  if (fireDetected) {
    // Fire has priority: water pump + alarm
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(waterPumpPin, HIGH);
    digitalWrite(fanPin, LOW);  // Fan optional for fire case
  }
  else if (gasDetected) {
    // Gas only: fan + alarm
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(fanPin, HIGH);
    digitalWrite(waterPumpPin, LOW);
  }
  else {
    // Normal state
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(fanPin, LOW);
    digitalWrite(waterPumpPin, LOW);
  }
  delay(100);
}