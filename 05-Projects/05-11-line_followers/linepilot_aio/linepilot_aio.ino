/*
  Project: LinePilot – All-in-One (Rule / PD / PID)
  File: linepilot_all_in_one_exact.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 12/02/2025

  Description:
    This file is an "exact merge" of:
      - linepilot_rule_based.ino
      - linepilot_pd.ino
      - linepilot_pid.ino

    No internal logic is refactored; each original file content is kept inside
    a compile-time mode block.

  How to Use:
    1) Uncomment exactly ONE mode macro below.
    2) Compile & upload.

  License: GPL-3.0
*/

// ======================
// MODE SELECTION (choose exactly ONE)
// ======================
// #define LINEPILOT_MODE_RULE_BASED
#define LINEPILOT_MODE_PD
// #define LINEPILOT_MODE_PID

#if (defined(LINEPILOT_MODE_RULE_BASED) + defined(LINEPILOT_MODE_PD) + defined(LINEPILOT_MODE_PID)) != 1
  #error "Select exactly ONE: LINEPILOT_MODE_RULE_BASED / LINEPILOT_MODE_PD / LINEPILOT_MODE_PID"
#endif


#if defined(LINEPILOT_MODE_RULE_BASED)

/*
  Project: LinePilot – Rule-Based Line Follower (L298N + 3 Digital Sensors)
  File: linepilot_rule_based.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 12/02/2025

  Description:
    Clean-room rule-based line follower for:
      - L298N dual H-bridge motor driver
      - 2 DC motors
      - 3 digital line sensors (Left/Center/Right)

    The robot follows the line using simple decision rules and
    uses "last seen direction" memory to recover when the line is lost.

  Features:
    - 3 digital sensors (L/C/R)
    - Rule-based decisions (no PID)
    - Last-seen memory for line reacquisition
    - Configurable line polarity (black line on white ground, or inverse)

  Wiring:
    L298N:
      ENA → D5 (PWM)
      IN1 → D6
      IN2 → D7
      IN3 → D8
      IN4 → D9
      ENB → D10 (PWM)

    Line Sensors (digital):
      Left   → D2
      Center → D3
      Right  → D4

  Libraries:
    - None

  Serial Baud:
    9600

  License: GPL-3.0
*/

// ======================
// User Settings
// ======================
static const bool LINE_IS_BLACK = true;  // true: black line on white ground, false: white line on black
static const int BASE_SPEED_PWM = 170;   // 0..255
static const int TURN_SPEED_PWM = 170;   // 0..255
static const int SEARCH_SPEED_PWM = 120; // 0..255 (line lost w/ no memory)

// If your sensor module outputs 1 on black (instead of common 0), flip this:
static const bool SENSOR_OUTPUT_IS_BLACK_HIGH = false;

// ======================
// Pin Map - L298N
// ======================
static const int PIN_ENA = 5;  // PWM (Motor A)
static const int PIN_IN1 = 6;
static const int PIN_IN2 = 7;

static const int PIN_IN3 = 8;
static const int PIN_IN4 = 9;
static const int PIN_ENB = 10; // PWM (Motor B)

// Sensors
static const int PIN_SENSOR_LEFT   = 2;
static const int PIN_SENSOR_CENTER = 3;
static const int PIN_SENSOR_RIGHT  = 4;

// ======================
// Types / State
// ======================
enum class LastSeen : uint8_t { None, Left, Right };
static LastSeen lastSeen = LastSeen::None;

// ======================
// Helpers - Sensor Logic
// ======================
static inline int digitalSeesBlack(int rawDigital) {
  // rawDigital is 0/1 from module.
  // Normalize to "black" = 1.
  if (SENSOR_OUTPUT_IS_BLACK_HIGH) {
    return (rawDigital == 1) ? 1 : 0;
  }
  return (rawDigital == 0) ? 1 : 0;
}

static inline int sensorOnLine(int rawDigital) {
  // Convert sensor reading to "on line" = 1
  const int isBlack = digitalSeesBlack(rawDigital);
  // If line is black, onLine == isBlack; else onLine == !isBlack
  return LINE_IS_BLACK ? isBlack : (isBlack == 0 ? 1 : 0);
}

// ======================
// Helpers - Motor Control
// ======================
static inline void setMotorA(int pwmSigned) {
  // Motor A: ENA + IN1/IN2
  pwmSigned = constrain(pwmSigned, -255, 255);

  if (pwmSigned < 0) {
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, HIGH);
    analogWrite(PIN_ENA, abs(pwmSigned));
  } else {
    digitalWrite(PIN_IN1, HIGH);
    digitalWrite(PIN_IN2, LOW);
    analogWrite(PIN_ENA, pwmSigned);
  }
}

static inline void setMotorB(int pwmSigned) {
  // Motor B: ENB + IN3/IN4
  pwmSigned = constrain(pwmSigned, -255, 255);

  if (pwmSigned < 0) {
    digitalWrite(PIN_IN3, LOW);
    digitalWrite(PIN_IN4, HIGH);
    analogWrite(PIN_ENB, abs(pwmSigned));
  } else {
    digitalWrite(PIN_IN3, HIGH);
    digitalWrite(PIN_IN4, LOW);
    analogWrite(PIN_ENB, pwmSigned);
  }
}

static inline void drive(int leftPwm, int rightPwm) {
  // Convention: Motor A = Left, Motor B = Right (change if wiring differs)
  setMotorA(leftPwm);
  setMotorB(rightPwm);
}

static inline void stopMotors() {
  analogWrite(PIN_ENA, 0);
  analogWrite(PIN_ENB, 0);
}

static inline void driveForward(int pwm) {
  drive(pwm, pwm);
}

static inline void pivotLeft(int pwm) {
  // left reverse, right forward
  drive(-pwm, pwm);
}

static inline void pivotRight(int pwm) {
  // left forward, right reverse
  drive(pwm, -pwm);
}

// ======================
// Arduino
// ======================
void setup() {
  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);

  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);
  pinMode(PIN_ENB, OUTPUT);

  pinMode(PIN_SENSOR_LEFT, INPUT);
  pinMode(PIN_SENSOR_CENTER, INPUT);
  pinMode(PIN_SENSOR_RIGHT, INPUT);

  Serial.begin(9600);
  stopMotors();
}

void loop() {
  const int leftOnLine   = sensorOnLine(digitalRead(PIN_SENSOR_LEFT));
  const int centerOnLine = sensorOnLine(digitalRead(PIN_SENSOR_CENTER));
  const int rightOnLine  = sensorOnLine(digitalRead(PIN_SENSOR_RIGHT));

  // Decision rules:
  // 1) Center sees line -> go forward
  // 2) Left sees line   -> pivot left, remember left
  // 3) Right sees line  -> pivot right, remember right
  // 4) Wide line/intersection -> go forward
  // 5) Lost line -> pivot toward lastSeen, else slow forward search

  if (centerOnLine == 1 && leftOnLine == 0 && rightOnLine == 0) {
    driveForward(BASE_SPEED_PWM);
  } else if (leftOnLine == 1 && centerOnLine == 0) {
    pivotLeft(TURN_SPEED_PWM);
    lastSeen = LastSeen::Left;
  } else if (rightOnLine == 1 && centerOnLine == 0) {
    pivotRight(TURN_SPEED_PWM);
    lastSeen = LastSeen::Right;
  } else if (centerOnLine == 1 && (leftOnLine == 1 || rightOnLine == 1)) {
    driveForward(BASE_SPEED_PWM);
  } else {
    // Line lost
    if (lastSeen == LastSeen::Left) {
      pivotLeft(TURN_SPEED_PWM);
    } else if (lastSeen == LastSeen::Right) {
      pivotRight(TURN_SPEED_PWM);
    } else {
      driveForward(SEARCH_SPEED_PWM);
    }
  }
}

#endif


#if defined(LINEPILOT_MODE_PD)

/*
  Project: LinePilot – PD Line Follower (TB6612FNG + QTR-8)
  File: linepilot_pd.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 12/02/2025

  Description:
    Clean-room PD line follower using:
      - TB6612FNG motor driver (2 DC motors)
      - QTR-8 sensor array (RC type via QTRSensors library)
      - PD control: correction = Kp*error + Kd*derivative

    Optional obstacle sensor block is provided as commented code.

  Features:
    - QTR-8 position (0..7000)
    - Start-up calibration with in-place oscillation
    - PD control (stable, easy to tune)
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

// ======================
// User Settings
// ======================
static const float KP = 0.09f;
static const float KD = 0.80f;

static const int LEFT_BASE_PWM  = 170;   // 0..255
static const int RIGHT_BASE_PWM = 170;   // 0..255

static const int PWM_MIN = -220;
static const int PWM_MAX =  220;

// Track polarity (set false for black line on white ground in most cases)
static const bool INVERT_POSITION = false;

// Optional obstacle sensor pin
// static const int PIN_OBSTACLE = A4; // digital input

// ======================
// Pin Map - TB6612FNG
// ======================
static const int PIN_PWMA = 3;    // PWM (Left motor)
static const int PIN_AIN1 = A0;
static const int PIN_AIN2 = A1;

static const int PIN_PWMB = 11;   // PWM (Right motor)
static const int PIN_BIN1 = A2;
static const int PIN_BIN2 = A3;

static const int PIN_STBY = 13;

// ======================
// QTR-8 (RC) pins
// ======================
static const unsigned char QTR_PINS[8] = { 2, 4, 5, 6, 7, 8, 9, 10 };
QTRSensorsRC qtr(QTR_PINS, 8, 2500, QTR_NO_EMITTER_PIN);
unsigned int qtrValues[8];

// ======================
// State
// ======================
static int lastError = 0;

// ======================
// Motor Helpers
// ======================
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

// ======================
// QTR Calibration
// ======================
static void calibrateQtrWithOscillation() {
  const int swingPwm = 120;

  for (int i = 0; i < 220; i++) {
    const bool swingRight = (i % 40) < 20;
    if (swingRight) {
      // pivot right
      driveMotors(+swingPwm, -swingPwm);
    } else {
      // pivot left
      driveMotors(-swingPwm, +swingPwm);
    }

    qtr.calibrate();
    delay(5);
  }

  stopMotors();
  delay(300);
}

// ======================
// Arduino
// ======================
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
  const int derivative = error - lastError;
  lastError = error;

  const int correction = (int)(KP * error + KD * derivative);

  const int leftPwm  = LEFT_BASE_PWM  - correction;
  const int rightPwm = RIGHT_BASE_PWM + correction;

  driveMotors(leftPwm, rightPwm);
}

#endif


#if defined(LINEPILOT_MODE_PID)

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

// ======================
// User Settings
// ======================
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

// ======================
// Pin Map - TB6612FNG
// ======================
static const int PIN_PWMA = 3;    // PWM (Left motor)
static const int PIN_AIN1 = A0;
static const int PIN_AIN2 = A1;

static const int PIN_PWMB = 11;   // PWM (Right motor)
static const int PIN_BIN1 = A2;
static const int PIN_BIN2 = A3;

static const int PIN_STBY = 13;

// ======================
// QTR-8 (RC)
// ======================
static const unsigned char QTR_PINS[8] = { 2, 4, 5, 6, 7, 8, 9, 10 };
QTRSensorsRC qtr(QTR_PINS, 8, 2500, QTR_NO_EMITTER_PIN);
unsigned int qtrValues[8];

// ======================
// State
// ======================
static int lastError = 0;
static long integral = 0;

// ======================
// Motor Helpers
// ======================
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

// ======================
// QTR Calibration
// ======================
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

// ======================
// Arduino
// ======================
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

#endif