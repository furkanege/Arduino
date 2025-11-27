/*
  Project: Ultrasonic Distance Measurement (Serial Output)
  File: distance_console.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/26/2025

  Description:
    Measures distance using an HC-SR04 ultrasonic sensor and prints the
    measured value to the Serial Monitor. Demonstrates basic ultrasonic
    timing, echo measurement and distance calculation for robotics and obstacle detection.

  Wiring (HC-SR04):
    VCC → 5V
    GND → GND
    TRIG → D13
    ECHO → D12

  Libraries:
    - (No external libraries required)

  Serial Baud:
    9600

  Example Serial Output:
    Distance: 27 cm

  Notes:
    - Accurate readings require stable echo timing.
    - HC-SR04 minimum range ~2 cm, maximum ~400 cm.
    - Avoid placing sensor too close to ultrasonic-reflective surfaces.

  Real-World Applications:
    - Obstacle detection (robots)
    - Parking assist systems
    - Distance logging
    - Automated measurement devices

  License: GPL-3.0
*/

#define TRIG_PIN 13
#define ECHO_PIN 12

long duration;
int distance;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  // Trigger pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Echo reading
  duration = pulseIn(ECHO_PIN, HIGH);

  // Convert to cm
  distance = (duration / 2) / 29.1;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(500); // Update every 0.5 seconds
}