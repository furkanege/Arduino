// User Settings
static const bool LINE_IS_BLACK = true;  // true: black line on white ground, false: white line on black
static const int BASE_SPEED_PWM = 170;   // 0..255
static const int TURN_SPEED_PWM = 170;   // 0..255
static const int SEARCH_SPEED_PWM = 120; // 0..255 (line lost w/ no memory)
// If your sensor module outputs 1 on black (instead of common 0), flip this:
static const bool SENSOR_OUTPUT_IS_BLACK_HIGH = false;
// Pin Map - L298N
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
// Types / State
enum class LastSeen : uint8_t { None, Left, Right };
static LastSeen lastSeen = LastSeen::None;
// Helpers - Sensor Logic
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

// Helpers - Motor Control
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

// Arduino
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