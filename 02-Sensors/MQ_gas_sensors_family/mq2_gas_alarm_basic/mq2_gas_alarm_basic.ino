/*
  Project: MQ-2 Gas Sensor Alarm (Basic Version)
  File: mq2_gas_alarm_basic.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/27/2025

  Description:
    Simple gas leak detection using the MQ-2 sensor. If gas concentration
    exceeds a defined threshold, buzzer activates. Ideal for smoke, LPG, LNG,
    and general gas leakage detection demos.

  Wiring:
    MQ-2:
      AO → A0
      DO → (Not used)
      VCC → 5V
      GND → GND

    Buzzer:
      + → D8
      - → GND

    LED:
      + → D7
      - → GND

  Libraries:
    - (No external libraries required)

  Serial Baud:
    9600

  Real-World Applications:
    - Home gas leak alarms
    - Fire safety systems
    - Robot gas detection modules

  License: GPL-3.0
*/

int gasSensorPin = A0;  
int buzzerPin = 8;      
int ledPin = 7;         
int gasValue = 0;
int threshold = 400;  

void setup() {
  Serial.begin(9600);
  
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  gasValue = analogRead(gasSensorPin);
  Serial.print("Gas Value: ");
  Serial.println(gasValue);

  if (gasValue > threshold) {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledPin, LOW);
  }

  delay(50);
}