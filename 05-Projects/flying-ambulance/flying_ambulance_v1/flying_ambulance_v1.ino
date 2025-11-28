/*
  Project: Flying Ambulance Prototype – Drive System (v1)
  File: project_v1.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/28/2025

  Description:
    This is Version 1 of the drive controller prototype for the
    "Flying Ambulance" project.

    Features:
      - Bluetooth-based control via SoftwareSerial
      - Motor driver control (forward, backward, left, right, stop)
      - Speed selection (10%–100%)
      - No servo movement in this version (legs/cabin not yet implemented)

  Wiring:
    Bluetooth Module (HC-05 / HC-06):
      RX → D0 (SoftwareSerial TX)
      TX → D1 (SoftwareSerial RX)
      VCC → 5V
      GND → GND

    Motor Driver (L298N or compatible):
      ENA → D6   (PWM)
      IN1 → D7
      IN2 → D8
      IN3 → D9
      IN4 → D10
      ENB → D11  (PWM)

  Libraries:
    - Arduino.h
    - Wire.h
    - SoftwareSerial.h

  Serial Baud:
    9600

  Real-World Applications:
    - Ground mobility subsystem for emergency robotics
    - Bluetooth-based remote control vehicles
    - Educational robotics demonstrations

  License: GPL-3.0
*/

#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
SoftwareSerial btConnection(0, 1);

float speedValue = 0;
float legServo1 = 0;
float legServo2 = 0;
float legServo3 = 0;
float legServo4 = 0;
float cabinServo = 0;
float legMin = 0;
float legMax = 0;
int motorIn1 = 7;
int motorIn2 = 8;
int motorEnA = 6;
int motorIn3 = 9;
int motorIn4 = 10;
int motorEnB = 11;

char byteData;
int intData = 0;
String strData;

void moveForward(int power) {
    int pwmSpeed = round((power * 255) / 100);

    if (power < 0) {
        analogWrite(motorEnA, abs(pwmSpeed));
        analogWrite(motorEnB, abs(pwmSpeed));
        digitalWrite(motorIn1, LOW);
        digitalWrite(motorIn3, HIGH);
        digitalWrite(motorIn2, HIGH);
        digitalWrite(motorIn4, LOW);
    }
    else {
        analogWrite(motorEnA, pwmSpeed);
        analogWrite(motorEnB, pwmSpeed);
        digitalWrite(motorIn1, HIGH);
        digitalWrite(motorIn3, LOW);
        digitalWrite(motorIn2, LOW);
        digitalWrite(motorIn4, HIGH);
    }
}

void moveBackward(int power) {
    int pwmSpeed = round((power * 255) / 100);

    if (power < 0) {
        analogWrite(motorEnA, abs(pwmSpeed));
        analogWrite(motorEnB, abs(pwmSpeed));
        digitalWrite(motorIn1, HIGH);
        digitalWrite(motorIn3, LOW);
        digitalWrite(motorIn2, LOW);
        digitalWrite(motorIn4, HIGH);
    }
    else {
        analogWrite(motorEnA, pwmSpeed);
        analogWrite(motorEnB, pwmSpeed);
        digitalWrite(motorIn1, LOW);
        digitalWrite(motorIn3, HIGH);
        digitalWrite(motorIn2, HIGH);
        digitalWrite(motorIn4, LOW);
    }
}

void moveLeft(int power) {
    int pwmSpeed = round((power * 255) / 100);

    if (power < 0) {
        analogWrite(motorEnA, abs(pwmSpeed));
        analogWrite(motorEnB, abs(pwmSpeed));
        digitalWrite(motorIn1, LOW);
        digitalWrite(motorIn3, LOW);
        digitalWrite(motorIn2, HIGH);
        digitalWrite(motorIn4, HIGH);
    }
    else {
        analogWrite(motorEnA, pwmSpeed);
        analogWrite(motorEnB, pwmSpeed);
        digitalWrite(motorIn1, HIGH);
        digitalWrite(motorIn3, HIGH);
        digitalWrite(motorIn2, LOW);
        digitalWrite(motorIn4, LOW);
    }
}

void moveRight(int power) {
    int pwmSpeed = round((power * 255) / 100);

    if (power < 0) {
        analogWrite(motorEnA, abs(pwmSpeed));
        analogWrite(motorEnB, abs(pwmSpeed));
        digitalWrite(motorIn1, HIGH);
        digitalWrite(motorIn3, HIGH);
        digitalWrite(motorIn2, LOW);
        digitalWrite(motorIn4, LOW);
    }
    else {
        analogWrite(motorEnA, pwmSpeed);
        analogWrite(motorEnB, pwmSpeed);
        digitalWrite(motorIn1, LOW);
        digitalWrite(motorIn3, LOW);
        digitalWrite(motorIn2, HIGH);
        digitalWrite(motorIn4, HIGH);
    }
}

void _delay(float seconds) {
    long endTime = millis() + seconds * 1000;
    while (millis() < endTime) _loop();
}

void setup() {
    pinMode(motorIn1, OUTPUT);
    pinMode(motorIn2, OUTPUT);
    pinMode(motorEnA, OUTPUT);
    pinMode(motorIn3, OUTPUT);
    pinMode(motorIn4, OUTPUT);
    pinMode(motorEnB, OUTPUT);
    btConnection.begin(9600);
    btConnection.flush();

    speedValue = 50;

    while (1) {
        if (btConnection.available()) {
            byteData = btConnection.read();
            if (String(byteData).indexOf("1") > -1) speedValue = 10;
            else if (String(byteData).indexOf("2") > -1) speedValue = 20;
            else if (String(byteData).indexOf("3") > -1) speedValue = 30;
            else if (String(byteData).indexOf("4") > -1) speedValue = 40;
            else if (String(byteData).indexOf("5") > -1) speedValue = 50;
            else if (String(byteData).indexOf("6") > -1) speedValue = 60;
            else if (String(byteData).indexOf("7") > -1) speedValue = 70;
            else if (String(byteData).indexOf("8") > -1) speedValue = 80;
            else if (String(byteData).indexOf("9") > -1) speedValue = 90;
            else if (String(byteData).indexOf("q") > -1) speedValue = 100;
            else {
                if (String(byteData).indexOf("F") > -1) moveForward(speedValue);
                if (String(byteData).indexOf("B") > -1) moveBackward(speedValue);
                if (String(byteData).indexOf("L") > -1) moveLeft(speedValue);
                if (String(byteData).indexOf("R") > -1) moveRight(speedValue);
                if (String(byteData).indexOf("S") > -1) {
                    analogWrite(motorEnA, 0);
                    analogWrite(motorEnB, 0);
                    digitalWrite(motorIn1, LOW);
                    digitalWrite(motorIn2, LOW);
                    digitalWrite(motorIn3, LOW);
                    digitalWrite(motorIn4, LOW);
                }
            }
        }
        _loop();
    }
}

void _loop() {
}

void loop() {
    _loop();
}