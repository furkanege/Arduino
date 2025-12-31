/*
  Project: LinePilot – PID Line Follower (TB6612FNG + QTR-8)
  File: linepilot_pid.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 12/02/2025

  Description:
    Clean-room PID line follower using:
      - TB6612FNG motor driver (2 DC motors)
      - QTR-8 sensor array (RC type via QTRSensors library)
      - PID control with anti-windup (integral clamp)
    Optional obstacle sensor block is provided as commented code.

  Features:
    - QTR-8 position (0..7000)
    - Start-up calibration with oscillation
    - PID control (P + I + D)
    - Integral clamp (anti-windup)
    - Optional obstacle stop logic (commented)

  Wiring:
    QTR-8 (RC):
      Sensors → D2, D4, D5, D6, D7, D8, D9, D10

    TB6612FNG:
      PWMA → D3  (PWM)
      AIN1 → A0
      AIN2 → A1
      PWMB → D11 (PWM)
      BIN1 → A2
      BIN2 → A3
      STBY → D13

    Optional Obstacle Sensor:
      OUT → A4  (digital input)

  Libraries:
    - QTRSensors.h

  Serial Baud:
    9600

  License: GPL-3.0
*/

#include <QTRSensors.h>
// User Settings
static const float KP = 0.07f;
static const float KI = 0.00008f;
static const float KD = 1.80f;
static const int LEFT_BASE_PWM  = 165;
static const int RIGHT_BASE_PWM = 165;
static const int PWM_MIN = -220;
static const int PWM_MAX =  220;
// Track polarity
static const bool INVERT_POSITION = false;
// Anti-windup clamp
static const long INTEGRAL_MIN = -20000L;
static const long INTEGRAL_MAX =  20000L;
// Optional obstacle sensor pin
// static const int PIN_OBSTACLE = A4; // digital input
// Pin Map - TB6612FNG
static const int PIN_PWMA = 3;    // PWM (Left motor)
static const int PIN_AIN1 = A0;
static const int PIN_AIN2 = A1;
static const int PIN_PWMB = 11;   // PWM (Right motor)
static const int PIN_BIN1 = A2;
static const int PIN_BIN2 = A3;
static const int PIN_STBY = 13;
// QTR-8 (RC)
static const unsigned char QTR_PINS[8] = { 2, 4, 5, 6, 7, 8, 9, 10 };
QTRSensorsRC qtr(QTR_PINS, 8, 2500, QTR_NO_EMITTER_PIN);
unsigned int qtrValues[8];
// State
static int lastError = 0;
static long integral = 0;
// Motor Helpers
static inline void tb6612Enable() {
  digitalWrite(PIN_STBY, HIGH);
}

static inline void setTb6612Motor(int pwmPin, int in1Pin, int in2Pin, int speedSigned) {
  speedSigned = constrain(speedSigned, -255, 255);
  if (speedSigned < 0) {
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, HIGH);
    analogWrite(pwmPin, abs(speedSigned));
  } else {
    digitalWrite(in1Pin, HIGH);
    digitalWrite(in2Pin, LOW);
    analogWrite(pwmPin, speedSigned);
  }
}

static inline void driveMotors(int leftPwm, int rightPwm) {
  leftPwm  = constrain(leftPwm,  PWM_MIN, PWM_MAX);
  rightPwm = constrain(rightPwm, PWM_MIN, PWM_MAX);
  setTb6612Motor(PIN_PWMA, PIN_AIN1, PIN_AIN2, leftPwm);
  setTb6612Motor(PIN_PWMB, PIN_BIN1, PIN_BIN2, rightPwm);
}

static inline void stopMotors() {
  analogWrite(PIN_PWMA, 0);
  analogWrite(PIN_PWMB, 0);
}

// QTR Calibration
static void calibrateQtrWithOscillation() {
  const int swingPwm = 120;
  for (int i = 0; i < 220; i++) {
    const bool swingRight = (i % 40) < 20;
    if (swingRight) {
      driveMotors(+swingPwm, -swingPwm);
    } else {
      driveMotors(-swingPwm, +swingPwm);
    }
    qtr.calibrate();
    delay(5);
  }
  stopMotors();
  delay(300);
}

// Arduino
void setup() {
  pinMode(PIN_PWMA, OUTPUT);
  pinMode(PIN_AIN1, OUTPUT);
  pinMode(PIN_AIN2, OUTPUT);
  pinMode(PIN_PWMB, OUTPUT);
  pinMode(PIN_BIN1, OUTPUT);
  pinMode(PIN_BIN2, OUTPUT);
  pinMode(PIN_STBY, OUTPUT);
  // Optional obstacle sensor
  // pinMode(PIN_OBSTACLE, INPUT);
  Serial.begin(9600);
  tb6612Enable();
  stopMotors();
  calibrateQtrWithOscillation();
}

void loop() {
  // Optional obstacle stop logic (uncomment to use)
  /*
  if (digitalRead(PIN_OBSTACLE) == HIGH) {
    stopMotors();
    delay(10);
    return;
  }
  */

  unsigned int position = qtr.readLine(qtrValues); // 0..7000
  if (INVERT_POSITION) {
    position = 7000 - position;
  }
  const int error = (int)position - 3500;
  integral += error;
  integral = constrain(integral, INTEGRAL_MIN, INTEGRAL_MAX);
  const int derivative = error - lastError;
  lastError = error;
  const int correction = (int)(KP * error + KI * (float)integral + KD * derivative);
  const int leftPwm  = LEFT_BASE_PWM  - correction;
  const int rightPwm = RIGHT_BASE_PWM + correction;
  driveMotors(leftPwm, rightPwm);
}