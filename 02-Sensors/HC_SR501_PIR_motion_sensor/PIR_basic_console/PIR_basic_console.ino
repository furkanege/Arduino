/*
  Project: PIR Motion Sensor Basic Console Output
  File: PIR_basic_console.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/26/2025

  Description:
    Reads digital motion data from the HC-SR501 PIR motion sensor and prints
    the result to the Serial Monitor. If motion is detected, an LED is turned
    on. Simple demonstration of digital sensors and motion detection logic.

  Wiring (HC-SR501 PIR Sensor):
    PIR OUT  → D8
    VCC      → 5V
    GND      → GND

  LED:
    + (Anode) → D9 (with 220Ω resistor)
    - (Cathode) → GND

  Libraries:
    - (No external libraries required)

  Serial Baud:
    9600

  Example Serial Output:
    0
    1
    1
    0

  Notes:
    - PIR sensors require ~30–60 seconds warm-up for stable readings.
    - Output is HIGH when motion is detected.
    - Avoid placing PIR near hot airflow or sunlight for false-trigger reduction.

  Real-World Applications:
    - Security systems
    - Automatic lighting
    - Smart home automation
    - Human-presence detection

  License: GPL-3.0
*/

// PIR Motion Sensor (HC-SR501) – Basic Example
int pirPin = 8;      // PIR signal pin
int ledPin = 9;      // LED pin
int value = 0;       // PIR reading variable

void setup() {
  pinMode(pirPin, INPUT);     // PIR as input
  pinMode(ledPin, OUTPUT);    // LED as output
  Serial.begin(9600);         // Serial monitor
}

void loop() {
  value = digitalRead(pirPin);   // Read PIR signal
  Serial.println(value);         // Print to serial monitor
  if (value == HIGH) {           // Motion detected
    digitalWrite(ledPin, HIGH);  // Turn LED on
  } 
  else {
    digitalWrite(ledPin, LOW);   // No motion → LED off
  }
}
