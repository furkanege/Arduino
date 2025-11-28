/*
  Project: Smart Trash Can v2 (Ultrasonic + Sound Trigger)
  File: smart_trash_can_v2.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/27/2025

  Description:
    Enhanced smart trash can with two triggers:
      - Ultrasonic proximity detection (HC-SR04)
      - Sound (clap/noise) detection via microphone sensor

    The lid opens when:
      1) Distance < openDistance threshold  OR
      2) Sound amplitude > soundThreshold

    Lid remains open for 1 second after trigger,
    then closes automatically.

  Wiring:
    HC-SR04 Ultrasonic:
      Trig → D9
      Echo → D8
      VCC  → 5V
      GND  → GND

    Microphone:
      AO  → A0
      VCC → 5V
      GND → GND

    Servo:
      Signal → D10
      VCC    → 5V (or external supply recommended)
      GND    → GND

  Libraries:
    - Servo.h

  Serial Baud:
    9600

  Notes:
    - soundThreshold must be calibrated based on environment noise.
    - If servo twitches, use a dedicated 5V supply.

  Real-World Applications:
    - Touchless waste systems
    - Gesture/sound interactive devices
    - Accessibility devices

  License: GPL-3.0
*/

#include <Servo.h>
const int trigPin  = 9;
const int echoPin  = 8;
const int servoPin = 10;
const int soundPin = A0;
Servo lidServo;
long duration;
float distance;
int soundValue = 0;
const float openDistance = 25.0;  // cm
const int openAngle      = 110;
const int closedAngle    = 0;
const int soundThreshold = 600;   // Adjust experimentally

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
  duration = pulseIn(echoPin, HIGH, 30000);
  if (duration == 0)
    return 999.0;
  return (duration / 2.0) / 29.1;
}

void loop() {
  distance = getDistanceCm();
  soundValue = analogRead(soundPin);
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm | Sound: ");
  Serial.println(soundValue);
  bool openByDistance = distance < openDistance;
  bool openBySound    = soundValue > soundThreshold;
  if (openByDistance || openBySound) {
    lidServo.write(openAngle);
    delay(1000);  // keep lid open for 1 sec
  } else {
    lidServo.write(closedAngle);
  }
  delay(100);
}