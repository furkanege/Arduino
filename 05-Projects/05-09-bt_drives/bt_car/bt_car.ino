/*
  Project: Bluetooth RC Car Driver
  File: bt_car.ino
  Author: Furkan Ege
  Board: Arduino Nano / UNO
  Version: 1.0
  Last Updated: 12-01-2025

  Description:
    Bluetooth-controlled 2WD motor driver with speed levels, AUX outputs and a safety failsafe.
    If Bluetooth data stops (disconnect / out of range) the motors are automatically stopped.

  Hardware:
    - Bluetooth module (HC-05/HC-06) via SoftwareSerial
    - Dual H-Bridge motor driver (e.g., L298N / similar)
    - 2x DC motors
    - AUX1: PWM output (e.g., headlight dimming)
    - AUX2: Digital on/off output

  Pinout (Arduino perspective):
    Bluetooth:
      RX <- D3  (SoftwareSerial RX)
      TX -> D4  (SoftwareSerial TX)

    Motor A:
      IN1 = D7
      IN2 = D8
      ENA (PWM) = D5

    Motor B:
      IN3 = D9
      IN4 = D10
      ENB (PWM) = D6

    AUX:
      AUX1_PWM   = D11 (PWM, 0..255)
      AUX2_ONOFF = D12 (digital)

  Commands (typical Bluetooth RC apps):
    Driving:
      F = Forward
      B = Backward
      L = Turn left (in-place)
      R = Turn right (in-place)
      S = Stop

    Speed:
      1..9 = predefined speed %
      q    = 100%

    AUX:
      W = AUX1 on (PWM 180)
      w = AUX1 off (PWM 0)
      U = AUX2 on
      u = AUX2 off
      
  Safety:
    - If no Bluetooth byte is received for BT_FAILSAFE_MS, motors are stopped.
*/

#include <Arduino.h>
#include <SoftwareSerial.h>
SoftwareSerial btSerial(3, 4); // RX, TX (Arduino perspective)
const uint8_t MOTOR_A_IN1 = 7;
const uint8_t MOTOR_A_IN2 = 8;
const uint8_t MOTOR_A_EN  = 5;   // PWM
const uint8_t MOTOR_B_IN3 = 9;
const uint8_t MOTOR_B_IN4 = 10;
const uint8_t MOTOR_B_EN  = 6;   // PWM
const uint8_t AUX1_PWM = 11; // PWM (e.g., headlight brightness)
const uint8_t AUX2_ONOFF = 12; // digital on/off
int speedPercent = 50;   // 0..100
int speedPwm = 128;  // 0..255 (derived)
char command = 0;
const unsigned long BT_FAILSAFE_MS = 500;
unsigned long lastRxMs = 0;

static int pwmFromPercent(int percent) {
  percent = constrain(percent, 0, 100);
  return (percent * 255) / 100;
}

static void setSpeedPercent(int percent) {
  speedPercent = constrain(percent, 0, 100);
  speedPwm = pwmFromPercent(speedPercent);
}

static void stopMotors() {
  analogWrite(MOTOR_A_EN, 0);
  analogWrite(MOTOR_B_EN, 0);
  digitalWrite(MOTOR_A_IN1, LOW);
  digitalWrite(MOTOR_A_IN2, LOW);
  digitalWrite(MOTOR_B_IN3, LOW);
  digitalWrite(MOTOR_B_IN4, LOW);
}

static void driveForward(int powerPercent) {
  int pwm = pwmFromPercent(powerPercent);
  analogWrite(MOTOR_A_EN, pwm);
  analogWrite(MOTOR_B_EN, pwm);
  digitalWrite(MOTOR_A_IN1, HIGH);
  digitalWrite(MOTOR_A_IN2, LOW);
  digitalWrite(MOTOR_B_IN3, LOW);
  digitalWrite(MOTOR_B_IN4, HIGH);
}

static void driveBackward(int powerPercent) {
  int pwm = pwmFromPercent(powerPercent);
  analogWrite(MOTOR_A_EN, pwm);
  analogWrite(MOTOR_B_EN, pwm);
  digitalWrite(MOTOR_A_IN1, LOW);
  digitalWrite(MOTOR_A_IN2, HIGH);
  digitalWrite(MOTOR_B_IN3, HIGH);
  digitalWrite(MOTOR_B_IN4, LOW);
}

static void turnLeftInPlace(int powerPercent) {
  int pwm = pwmFromPercent(powerPercent);
  analogWrite(MOTOR_A_EN, pwm);
  analogWrite(MOTOR_B_EN, pwm);
  digitalWrite(MOTOR_A_IN1, LOW);
  digitalWrite(MOTOR_A_IN2, HIGH);
  digitalWrite(MOTOR_B_IN3, LOW);
  digitalWrite(MOTOR_B_IN4, HIGH);
}

static void turnRightInPlace(int powerPercent) {
  int pwm = pwmFromPercent(powerPercent);
  analogWrite(MOTOR_A_EN, pwm);
  analogWrite(MOTOR_B_EN, pwm);
  digitalWrite(MOTOR_A_IN1, HIGH);
  digitalWrite(MOTOR_A_IN2, LOW);
  digitalWrite(MOTOR_B_IN3, HIGH);
  digitalWrite(MOTOR_B_IN4, LOW);
}

static void setAux1Pwm(uint8_t value0_255) {
  analogWrite(AUX1_PWM, value0_255);
}

static void setAux2On(bool on) {
  digitalWrite(AUX2_ONOFF, on ? HIGH : LOW);
}

static void handleSpeedCommand(char c) {
  switch (c) {
    case '1': setSpeedPercent(40);  return;
    case '2': setSpeedPercent(50);  return;
    case '3': setSpeedPercent(55);  return;
    case '4': setSpeedPercent(60);  return;
    case '5': setSpeedPercent(65);  return;
    case '6': setSpeedPercent(70);  return;
    case '7': setSpeedPercent(80);  return;
    case '8': setSpeedPercent(85);  return;
    case '9': setSpeedPercent(90);  return;
    case 'q': setSpeedPercent(100); return;
    default: return;
  }
}

static void handleDriveCommand(char c) {
  switch (c) {
    case 'F': driveForward(speedPercent);     break;
    case 'B': driveBackward(speedPercent);    break;
    case 'L': turnLeftInPlace(speedPercent);  break;
    case 'R': turnRightInPlace(speedPercent); break;
    case 'S': stopMotors();                   break;
    case 'W': setAux1Pwm(180); break;
    case 'w': setAux1Pwm(0);   break;
    case 'U': setAux2On(true);  break;
    case 'u': setAux2On(false); break;
    default: break;
  }
}

void setup() {
  pinMode(MOTOR_A_IN1, OUTPUT);
  pinMode(MOTOR_A_IN2, OUTPUT);
  pinMode(MOTOR_A_EN,  OUTPUT);
  pinMode(MOTOR_B_IN3, OUTPUT);
  pinMode(MOTOR_B_IN4, OUTPUT);
  pinMode(MOTOR_B_EN,  OUTPUT);
  pinMode(AUX1_PWM, OUTPUT);
  pinMode(AUX2_ONOFF, OUTPUT);
  btSerial.begin(9600);
  setSpeedPercent(50);
  stopMotors();
  setAux1Pwm(0);
  setAux2On(false);
  lastRxMs = millis();
}

void loop() {
  while (btSerial.available()) {
    command = (char)btSerial.read();
    lastRxMs = millis();
    handleSpeedCommand(command);
    handleDriveCommand(command);
  }
  if ((millis() - lastRxMs) > BT_FAILSAFE_MS) {
    stopMotors();
  }
}