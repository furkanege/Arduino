/*
  Project: 4-Servo Synchronized Variable-Speed Sweep
  File: servo_4sync_variable_speed_sweep.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/28/2025

  Description:
    Controls 4 servo motors simultaneously and moves them between
    10° and 120° using a variable-speed delay profile.
    No features modified—original behavior preserved.

  Wiring:
    Servo 1 → D13
    Servo 2 → D12
    Servo 3 → D3
    Servo 4 → D2

  Libraries:
    - Servo.h

  License: GPL-3.0
*/

#include <Servo.h>
const int servoCount = 4;     
Servo Servos[servoCount];     
const int maxDegree = 120;    
const int minDegree = 10;     

void setup() {
  Servos[0].attach(13);
  Servos[1].attach(12);
  Servos[2].attach(3);
  Servos[3].attach(2);
}

void delayBasedOnDegree(int degree) {
  if (degree <= 15)      delay(10);
  else if (degree <= 20) delay(5);
  else if (degree <= 110) delay(1);
  else if (degree <= 115) delay(5);
  else delay(10);
}

void loop() {
  for (int degree = minDegree; degree <= maxDegree; degree++) {
    for (int i = 0; i < servoCount; i++) {
      Servos[i].write(degree);
      delayBasedOnDegree(degree);
    }
  }

  for (int degree = maxDegree; degree >= minDegree; degree--) {
    for (int i = 0; i < servoCount; i++) {
      Servos[i].write(degree);
      delayBasedOnDegree(degree);
    }
  }
}