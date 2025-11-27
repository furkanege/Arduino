/*
  Project: Ultrasonic Distance Alert System (LEDs + Buzzer)
  File: distance_with_leds_and_buzzer.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/26/2025

  Description:
    Uses an HC-SR04 ultrasonic sensor to measure distance and provides
    visual (LED) and audible (buzzer) alerts based on proximity thresholds.
    Demonstrates basic robotics safety logic and real-time obstacle
    detection feedback systems.

  Wiring:
    HC-SR04:
      - TRIG → D13
      - ECHO → D12
      - VCC → 5V
      - GND → GND

    LEDs:
      - Green LED → D8
      - Yellow LED → D9
      - Red LED → D10
      (All LEDs through 220Ω resistors)

    Buzzer:
      - + → D11
      - - → GND

  Libraries:
    - (No external libraries required)

  Serial Baud:
    9600

  Example Serial Output:
    Distance: 14 cm → ALERT: TOO CLOSE!

  Notes:
    - HC-SR04 accuracy improves in quiet environments.
    - LED color thresholds can be adjusted per project.
    - Buzzer can be modified for continuous or pulsed alert patterns.

  Real-World Applications:
    - Robot proximity alerts
    - Parking assist systems
    - Obstacle detection alarms
    - Industrial safety systems

  License: GPL-3.0
*/

#define TRIG_PIN 13
#define ECHO_PIN 12
#define GREEN_LED 8
#define YELLOW_LED 9
#define RED_LED 10
#define BUZZER_PIN 11

long duration;
int distance;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  // Trigger pulse
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration / 2) / 29.1;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Distance logic
  if (distance > 30) {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }
  else if (distance <= 30 && distance > 15) {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }
  else {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, HIGH);

    // Pulsed buzzer alert
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
  }

  delay(200); // Slight delay for stable readings
}