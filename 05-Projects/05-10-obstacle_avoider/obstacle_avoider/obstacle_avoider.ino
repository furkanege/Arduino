/*
  Project: Obstacle Avoider – HC-SR04 + Dual DC Motors (L298N-style)
  File: obstacle_avoider.ino
  Author: Furkan Ege
  Board: Arduino UNO
  Version: 1.0
  Last Updated: 12/02/2025

  Description:
    Simple obstacle avoidance using an ultrasonic sensor (HC-SR04).
    The robot measures distance continuously:
      - If distance is below threshold, it pivots for 500 ms
      - Otherwise it drives forward

  Features:
    - HC-SR04 distance measurement using pulseIn()
    - Threshold-based pivot turn
    - Fixed PWM speed

  Wiring:
    HC-SR04:
      TRIG → D13
      ECHO → D12

    Motor Driver (L298N-style):
      ENA → D5
      IN1 → D6
      IN2 → D7
      IN3 → D8
      IN4 → D9
      ENB → D10

  Libraries:
    - None

  Notes:
    - Math and thresholds are preserved to keep identical behavior.

  License: GPL-3.0
*/

// ===== Hardware / Pins (values preserved) =====
const int TRIG_PIN = 13;
const int ECHO_PIN = 12;
const int ENA_PIN = 5;
const int IN1_PIN = 6;
const int IN2_PIN = 7;
const int IN3_PIN = 8;
const int IN4_PIN = 9;
const int ENB_PIN = 10;
// ===== Parameters (values preserved) =====
int speedPwm = 100;      // 0..255 PWM
long pulseDurationUs;    // pulse duration
long distanceCm;         // computed distance

// ===== Helpers =====
static inline void driveMotorsRaw(int in1, int in2, int in3, int in4, int pwmA, int pwmB) {
  digitalWrite(IN1_PIN, in1);
  digitalWrite(IN2_PIN, in2);
  digitalWrite(IN3_PIN, in3);
  digitalWrite(IN4_PIN, in4);
  analogWrite(ENA_PIN, pwmA);
  analogWrite(ENB_PIN, pwmB);
}

static inline long measureDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  pulseDurationUs = pulseIn(ECHO_PIN, HIGH);
  // Same formula preserved (float constant → long result)
  distanceCm = pulseDurationUs * 0.034 / 2;
  return distanceCm;
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(ENA_PIN, OUTPUT);
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  pinMode(ENB_PIN, OUTPUT);
  pinMode(IN3_PIN, OUTPUT);
  pinMode(IN4_PIN, OUTPUT);
}

void loop() {
  long distance = measureDistanceCm();
  // Decision logic preserved
  if (distance < 30 && distance > 1) {
    // Pivot: Motor A reverse, Motor B forward (pin levels preserved)
    driveMotorsRaw(LOW, HIGH, HIGH, LOW, speedPwm, speedPwm);
    delay(500);
  } else {
    // Forward: both forward (pin levels preserved)
    driveMotorsRaw(HIGH, LOW, HIGH, LOW, speedPwm, speedPwm);
  }
}