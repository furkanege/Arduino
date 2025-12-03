/*
  Project: LinePilot – Rule-Based Line Follower (3 Digital Sensors)
  File: linepilot_rule_based.ino
  Author: Furkan Ege
  Board: Arduino UNO
  Version: 1.0
  Last Updated: 12/02/2025

  Description:
    A simple, robust rule-based line follower using 3 digital line sensors:
      - Left, Center, Right (digital 0/1)
    The robot drives forward when centered and pivots to reacquire the line
    when offset. If the line is lost, it pivots toward the last known direction.

  Features:
    - 3 digital sensors (L/C/R)
    - Rule-based motor decisions (no PID)
    - Last-seen direction memory for recovery
    - Configurable line polarity (black line on white ground or vice versa)

  Wiring:
    Line Sensors (digital):
      Right → D7
      Center → D6
      Left  → D5

    Motor Driver (DIR1/DIR2 + PWM):
      Left motor:
        DIR1 → A4
        DIR2 → A3
        PWM  → D9
      Right motor:
        DIR1 → A1
        DIR2 → A2
        PWM  → D10

  Serial Baud:
    9600

  License: GPL-3.0
*/

// User Settings
static const bool LINE_IS_BLACK = true; // true: black line on white ground, false: white line on black ground
static const int BASE_SPEED_PWM = 180;  // 0..255
static const int TURN_SPEED_PWM = 180;  // 0..255
// Pin Map
static const int PIN_SENSOR_RIGHT  = 7;
static const int PIN_SENSOR_CENTER = 6;
static const int PIN_SENSOR_LEFT   = 5;
static const int PIN_MOTOR_L_DIR1 = A4;
static const int PIN_MOTOR_L_DIR2 = A3;
static const int PIN_MOTOR_L_PWM  = 9;
static const int PIN_MOTOR_R_DIR1 = A1;
static const int PIN_MOTOR_R_DIR2 = A2;
static const int PIN_MOTOR_R_PWM  = 10;
// Types / State
enum class LastSeen : uint8_t { None, Left, Right };
static LastSeen lastSeen = LastSeen::None;
// Helpers
static inline int normalizeDigitalLineReading(int rawDigital) {
  // rawDigital is 0/1 from sensor module.
  // Convert to "onLine" = 1 when sensor sees the line.
  // For many IR modules: black often returns 0 (less reflect) and white returns 1.
  // If your module is inverted, flip LINE_IS_BLACK or invert logic here.
  const int seenBlack = (rawDigital == 0) ? 1 : 0;
  const int seenWhite = (rawDigital == 1) ? 1 : 0;
  return LINE_IS_BLACK ? seenBlack : seenWhite;
}

static inline void setLeftMotorForward(int pwm) {
  digitalWrite(PIN_MOTOR_L_DIR1, HIGH);
  digitalWrite(PIN_MOTOR_L_DIR2, LOW);
  analogWrite(PIN_MOTOR_L_PWM, pwm);
}

static inline void setLeftMotorReverse(int pwm) {
  digitalWrite(PIN_MOTOR_L_DIR1, LOW);
  digitalWrite(PIN_MOTOR_L_DIR2, HIGH);
  analogWrite(PIN_MOTOR_L_PWM, pwm);
}

static inline void setRightMotorForward(int pwm) {
  digitalWrite(PIN_MOTOR_R_DIR1, HIGH);
  digitalWrite(PIN_MOTOR_R_DIR2, LOW);
  analogWrite(PIN_MOTOR_R_PWM, pwm);
}

static inline void setRightMotorReverse(int pwm) {
  digitalWrite(PIN_MOTOR_R_DIR1, LOW);
  digitalWrite(PIN_MOTOR_R_DIR2, HIGH);
  analogWrite(PIN_MOTOR_R_PWM, pwm);
}

static inline void driveForward(int pwm) {
  setLeftMotorForward(pwm);
  setRightMotorForward(pwm);
}

static inline void pivotLeft(int pwm) {
  // Rotate left in place: left reverse, right forward
  setLeftMotorReverse(pwm);
  setRightMotorForward(pwm);
}

static inline void pivotRight(int pwm) {
  // Rotate right in place: left forward, right reverse
  setLeftMotorForward(pwm);
  setRightMotorReverse(pwm);
}

static inline void stopMotors() {
  analogWrite(PIN_MOTOR_L_PWM, 0);
  analogWrite(PIN_MOTOR_R_PWM, 0);
}

void setup() {
  pinMode(PIN_SENSOR_RIGHT, INPUT);
  pinMode(PIN_SENSOR_CENTER, INPUT);
  pinMode(PIN_SENSOR_LEFT, INPUT);
  pinMode(PIN_MOTOR_L_DIR1, OUTPUT);
  pinMode(PIN_MOTOR_L_DIR2, OUTPUT);
  pinMode(PIN_MOTOR_L_PWM, OUTPUT);
  pinMode(PIN_MOTOR_R_DIR1, OUTPUT);
  pinMode(PIN_MOTOR_R_DIR2, OUTPUT);
  pinMode(PIN_MOTOR_R_PWM, OUTPUT);
  Serial.begin(9600);
  stopMotors();
}

void loop() {
  const int rOnLine = normalizeDigitalLineReading(digitalRead(PIN_SENSOR_RIGHT));
  const int cOnLine = normalizeDigitalLineReading(digitalRead(PIN_SENSOR_CENTER));
  const int lOnLine = normalizeDigitalLineReading(digitalRead(PIN_SENSOR_LEFT));

  // Decision rules:
  // - Center on line → forward
  // - Left on line   → pivot left (recenter)
  // - Right on line  → pivot right
  // - Line lost      → pivot toward last seen direction

  if (cOnLine == 1 && lOnLine == 0 && rOnLine == 0) {
    driveForward(BASE_SPEED_PWM);
    // don't change lastSeen here
  } else if (lOnLine == 1 && cOnLine == 0) {
    pivotLeft(TURN_SPEED_PWM);
    lastSeen = LastSeen::Left;
  } else if (rOnLine == 1 && cOnLine == 0) {
    pivotRight(TURN_SPEED_PWM);
    lastSeen = LastSeen::Right;
  } else if (cOnLine == 1 && (lOnLine == 1 || rOnLine == 1)) {
    // "wide line" or intersection: prefer forward
    driveForward(BASE_SPEED_PWM);
  } else {
    // Line lost: attempt recovery using last seen direction
    if (lastSeen == LastSeen::Left) {
      pivotLeft(TURN_SPEED_PWM);
    } else if (lastSeen == LastSeen::Right) {
      pivotRight(TURN_SPEED_PWM);
    } else {
      // No history: slow forward search
      driveForward(120);
    }
  }
}