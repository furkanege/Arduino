/*
  Project: Servo Reset to Zero Position
  File: servo_reset_zero.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/27/2025

  Description:
    Automatically resets the servo to 0° and holds the position.
    Useful for calibration and mechanical home-position routines.

  Wiring:
    Servo:
      Signal → D9
      VCC    → 5V
      GND    → GND

  Libraries:
    - Servo.h

  Serial Baud:
    (Not required)

  Notes:
    - For accurate "home" position, ensure servo horn is mounted correctly.
    - Continuous rotation servos will NOT respond to angle commands.

  Applications:
    - Homing systems
    - Reset mechanisms
    - Robotics calibration

  License: GPL-3.0
*/

#include <Servo.h>
Servo myServo;

void setup() {
  myServo.attach(9);
  myServo.write(0);   // Move to 0°
  delay(500);
}

void loop() {
  myServo.write(0);   // Continuously hold zero position
  delay(10);
}