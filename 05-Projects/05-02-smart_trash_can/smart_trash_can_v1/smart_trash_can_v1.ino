/*
  Project: Smart Trash Can v1 (Ultrasonic Auto-Lid)
  File: smart_trash_can_v1.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/27/2025

  Description:
    Hands-free smart trash can using:
      - HC-SR04 ultrasonic distance sensor
      - Servo motor for lid automation

    Behavior:
      - If an object is detected closer than a threshold (default: 25 cm)
          → opens lid (servo rotates to openAngle)
      - Otherwise → lid remains closed

  Wiring:
    Ultrasonic Sensor (HC-SR04):
      Trig → D9
      Echo → D8
      VCC  → 5V
      GND  → GND

    Servo:
      Signal → D10
      VCC    → 5V (or external 5V recommended)
      GND    → GND

  Libraries:
    - Servo.h

  Serial Baud:
    9600

  Notes:
    - Servo may require external power for stable torque.
    - Ultrasonic distance measurement timeout prevents lockups.

  Real-World Applications:
    - Contactless trash cans (hygiene)
    - Public facilities
    - IoT/automation demos

  License: GPL-3.0
*/

#include <Servo.h>
const int trigPin  = 9;
const int echoPin  = 8;
const int servoPin = 10;
Servo lidServo;
long duration;
float distance;
const float openDistance = 25.0;   // cm
const int openAngle = 110;
const int closedAngle = 0;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  lidServo.attach(servoPin);
  lidServo.write(closedAngle);
  Serial.begin(9600);
}

float getDistanceCm() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH, 30000);  // 30ms timeout
  if (duration == 0) return 999.0;           // No echo / out-of-range
  return (duration / 2.0) / 29.1;
}

void loop() {
  distance = getDistanceCm();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  if (distance < openDistance) {
    lidServo.write(openAngle);
  } else {
    lidServo.write(closedAngle);
  }
  delay(150);
}