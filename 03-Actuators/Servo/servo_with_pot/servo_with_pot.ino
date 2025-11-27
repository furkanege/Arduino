/*
  Project: Servo Control with Potentiometer
  File: servo_with_pot.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/27/2025

  Description:
    Reads a potentiometer (0–1023) and converts it into a
    proportional servo angle (0–180°). Provides smooth manual
    servo control.

  Wiring:
    Potentiometer:
      Left Pin → 5V
      Right Pin → GND
      Middle Pin → A0

    Servo:
      Signal → D9
      VCC    → 5V
      GND    → GND

  Libraries:
    - Servo.h

  Serial Baud:
    9600 (optional for debugging)

  Notes:
    - Use a 10k pot for stable readings.
    - Ensure servo has separate power if it draws high current.

  Real-World Applications:
    - Manual servo testing
    - Robotic joints
    - DIY pan-tilt mechanisms

  License: GPL-3.0
*/

#include <Servo.h>

const int potPin = A0;
const int servoPin = 9;
Servo myServo;
int potValue = 0;
int angle = 0;

void setup() {
  myServo.attach(servoPin);
  Serial.begin(9600);
}

void loop() {
  potValue = analogRead(potPin);
  angle = map(potValue, 0, 1023, 0, 180);
  myServo.write(angle);
  Serial.print("Pot: ");
  Serial.print(potValue);
  Serial.print(" | Angle: ");
  Serial.println(angle);
  delay(10);
}