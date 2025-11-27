/*
  Project: Basic Servo Movement Demo
  File: servo_basic.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/27/2025

  Description:
    Demonstrates simple servo movement between 0° and 120°.
    Ideal for beginners learning servo control with Arduino.

  Wiring:
    Servo:
      Signal → D9
      VCC    → 5V
      GND    → GND

  Libraries:
    - Servo.h

  Serial Baud:
    (Not used)

  Notes:
    - Standard hobby servos accept 0–180° values.
    - Ensure external power supply for high-torque servos.

  Real-World Applications:
    - Robotic arms
    - Door locks
    - Steering mechanisms

  License: GPL-3.0
*/

#include <Servo.h>
Servo servoMotor;

void setup() {
  servoMotor.attach(9);
  servoMotor.write(0);
  delay(1000);
  servoMotor.write(120);
  delay(1000);
  servoMotor.write(0);
  delay(1000);
  servoMotor.write(120);
  delay(1000);
}

void loop() {
  // Empty — movements executed in setup()
}