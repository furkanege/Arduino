/*
  Project: Flying Ambulance Prototype – Drive & Servo System (v2)
  File: flying_ambulance_v2.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 2.0
  Last Updated: 11/28/2025

  Description:
    Version 2 of the "Flying Ambulance" ground drive and leg system.
    Adds 4 synchronized leg servos and a cabin servo on top of the v1 drive system.

  Features:
    - Bluetooth-based control via SoftwareSerial
    - DC motor control: forward, backward, left, right, stop
    - Speed presets (40%–100%)
    - 4-leg servo system with smooth sweeping between min/max angles
    - Cabin servo output pins (A0/A1) controlled over Bluetooth

  Bluetooth Commands:
    '1'..'9', 'q' : Set speed level (40–100)
    'F'           : Move forward
    'B'           : Move backward
    'L'           : Move left
    'R'           : Move right
    'S'           : Stop motors

    'W'           : analogWrite(A0, 180)
    'w'           : analogWrite(A0, 0)
    'U'           : analogWrite(A1, 180) + move legs from maxDegree → minDegree
    'u'           : analogWrite(A1, 0)   + move legs from minDegree → maxDegree

  Wiring:
    Bluetooth (HC-05 / HC-06):
      RX  → D1 (SoftwareSerial TX)
      TX  → D0 (SoftwareSerial RX)
      VCC → 5V
      GND → GND

    Motor Driver (L298N or compatible):
      ENA → D6 (PWM)
      IN1 → D7
      IN2 → D8
      IN3 → D9
      IN4 → D10
      ENB → D11 (PWM)

    Servos:
      Leg Servo 1 → D13
      Leg Servo 2 → D12
      Leg Servo 3 → D3
      Leg Servo 4 → D2
      Cabin Servo → D4

    Extra Outputs:
      A0 → Cabin power (W/w)
      A1 → Leg power  (U/u)

  Libraries:
    - Arduino.h
    - Wire.h
    - SoftwareSerial.h
    - Servo.h

  License: GPL-3.0
*/

#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial btConnection(1, 0);  // RX, TX (same as your original)
const int servoCount = 4;
Servo legServos[servoCount];
Servo cabinServo;
const int maxDegree = 120;
const int minDegree = 10;
float speedValue = 0;
int motorIn1 = 7;
int motorIn2 = 8;
int motorEnA = 6;
int motorIn3 = 9;
int motorIn4 = 10;
int motorEnB = 11;
char byteData;
int intData = 0;
String strData = "";

void moveForward(int power) {
  int pwmSpeed = round((power * 255) / 100);
  if (power < 0) {
    analogWrite(motorEnA, abs(pwmSpeed));
    analogWrite(motorEnB, abs(pwmSpeed));
    digitalWrite(motorIn1, LOW);
    digitalWrite(motorIn3, HIGH);
    digitalWrite(motorIn2, HIGH);
    digitalWrite(motorIn4, LOW);
  } else {
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
  } else {
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
  } else {
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
  } else {
    analogWrite(motorEnA, pwmSpeed);
    analogWrite(motorEnB, pwmSpeed);
    digitalWrite(motorIn1, LOW);
    digitalWrite(motorIn3, LOW);
    digitalWrite(motorIn2, HIGH);
    digitalWrite(motorIn4, HIGH);
  }
}

void _loop();

void _delay(float seconds) {
  long endTime = millis() + seconds * 1000;
  while (millis() < endTime) _loop();
}

void delayBasedOnDegree(int degree) {
  if (degree <= 15) {
    delay(10);
  } else if (degree <= 20) {
    delay(5);
  } else if (degree <= 110) {
    delay(1);
  } else if (degree <= 115) {
    delay(5);
  } else {
    delay(10);
  }
}

void setup() {
  legServos[0].attach(13);
  legServos[1].attach(12);
  legServos[2].attach(3);
  legServos[3].attach(2);
  cabinServo.attach(4);
  pinMode(motorIn1, OUTPUT);
  pinMode(motorIn2, OUTPUT);
  pinMode(motorEnA, OUTPUT);
  pinMode(motorIn3, OUTPUT);
  pinMode(motorIn4, OUTPUT);
  pinMode(motorEnB, OUTPUT);
  btConnection.begin(9600);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);

  btConnection.flush();
  speedValue = 50;

  while (1) {
    if (btConnection.available()) {
      byteData = btConnection.read();

      if (String(byteData).indexOf(String("1")) > -1) {
        speedValue = 40;
      } else {
        if (String(byteData).indexOf(String("2")) > -1) {
          speedValue = 50;
        } else {
          if (String(byteData).indexOf(String("3")) > -1) {
            speedValue = 55;
          } else {
            if (String(byteData).indexOf(String("4")) > -1) {
              speedValue = 60;
            } else {
              if (String(byteData).indexOf(String("5")) > -1) {
                speedValue = 65;
              } else {
                if (String(byteData).indexOf(String("6")) > -1) {
                  speedValue = 70;
                } else {
                  if (String(byteData).indexOf(String("7")) > -1) {
                    speedValue = 80;
                  } else {
                    if (String(byteData).indexOf(String("8")) > -1) {
                      speedValue = 85;
                    } else {
                      if (String(byteData).indexOf(String("9")) > -1) {
                        speedValue = 90;
                      } else {
                        if (String(byteData).indexOf(String("q")) > -1) {
                          speedValue = 100;
                        } else {
                          if (String(byteData).indexOf(String("W")) > -1) {
                            analogWrite(A0, 180);
                          }
                          if (String(byteData).indexOf(String("w")) > -1) {
                            analogWrite(A0, 0);
                          }
                          if (String(byteData).indexOf(String("U")) > -1) {
                            analogWrite(A1, 180);
                          }
                          if (String(byteData).indexOf(String("u")) > -1) {
                            analogWrite(A1, 0);
                          }
                          if (String(byteData).indexOf(String("F")) > -1) {
                            moveForward(speedValue);
                          }
                          if (String(byteData).indexOf(String("B")) > -1) {
                            moveBackward(speedValue);
                          }
                          if (String(byteData).indexOf(String("L")) > -1) {
                            moveLeft(speedValue);
                          }
                          if (String(byteData).indexOf(String("R")) > -1) {
                            moveRight(speedValue);
                          }
                          if (String(byteData).indexOf(String("S")) > -1) {
                            analogWrite(motorEnA, 0);
                            analogWrite(motorEnB, 0);
                            digitalWrite(motorIn1, LOW);
                            digitalWrite(motorIn2, LOW);
                            digitalWrite(motorIn3, LOW);
                            digitalWrite(motorIn4, LOW);
                          }

                          if (String(byteData).indexOf(String("U")) > -1) {
                            for (int degree = maxDegree; degree >= minDegree; degree--) {
                              for (int i = 0; i < servoCount; i++) {
                                legServos[i].write(degree);
                                delayBasedOnDegree(degree);
                              }
                            }
                          }

                          if (String(byteData).indexOf(String("u")) > -1) {
                            for (int degree = minDegree; degree <= maxDegree; degree++) {
                              for (int i = 0; i < servoCount; i++) {
                                legServos[i].write(degree);
                                delayBasedOnDegree(degree);
                              }
                            }
                          }

                          if (String(byteData).indexOf(String("W")) > -1) {
                          }

                          if (String(byteData).indexOf(String("w")) > -1) {
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
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