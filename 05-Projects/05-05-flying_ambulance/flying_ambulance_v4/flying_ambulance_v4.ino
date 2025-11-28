/*
  Project: Flying Ambulance Prototype – Drive, Leg & Cabin System (v4)
  File: flying_ambulance_v4.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 4.0
  Last Updated: 11/28/2025

  Description:
    This is Version 4 of the drive controller prototype for the
    "Flying Ambulance" project.

    Features:
      - Bluetooth-based control via SoftwareSerial
      - Motor driver control (forward, backward, left, right, stop)
      - Speed selection (40%–100%)
      - Leg system with 4 servos (UP/DOWN positions)
      - Cabin door servo with front/back positions (0° / 180°)

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

    Servos:
      Leg Servo 1 → D13
      Leg Servo 2 → D12
      Leg Servo 3 → D3
      Leg Servo 4 → D2
      Cabin Servo → D4

  Libraries:
    - Arduino.h
    - Wire.h
    - SoftwareSerial.h
    - Servo.h

  Serial Baud:
    9600

  Real-World Applications:
    - Ground mobility subsystem for emergency robotics
    - Servo-actuated robotic transformation systems (legs + cabin)
    - Bluetooth-controlled robotic vehicles
    - Educational robotics demonstrations

  License: GPL-3.0
*/

#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial btConnection(0,1);

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;

float speedValue = 0;

int in1 = 7;
int in2 = 8;
int enA = 6;
int in3 = 9;
int in4 = 10;
int enB = 11;

char incomingByte;
int intValue = 0;
String strValue;

void moveForward(int power){
    int pwm = round((power * 255) / 100);
    if(power < 0){
        analogWrite(enA,abs(pwm));
        analogWrite(enB,abs(pwm));
        digitalWrite(in1,LOW);
        digitalWrite(in3,HIGH);
        digitalWrite(in2,HIGH);
        digitalWrite(in4,LOW);
    }
    else{
        analogWrite(enA,pwm);
        analogWrite(enB,pwm);
        digitalWrite(in1,HIGH);
        digitalWrite(in3,LOW);
        digitalWrite(in2,LOW);
        digitalWrite(in4,HIGH);
    }
}

void moveBackward(int power){
    int pwm = round((power * 255) / 100);
    if(power < 0){
        analogWrite(enA,abs(pwm));
        analogWrite(enB,abs(pwm));
        digitalWrite(in1,HIGH);
        digitalWrite(in3,LOW);
        digitalWrite(in2,LOW);
        digitalWrite(in4,HIGH);
    }
    else{
        analogWrite(enA,pwm);
        analogWrite(enB,pwm);
        digitalWrite(in1,LOW);
        digitalWrite(in3,HIGH);
        digitalWrite(in2,HIGH);
        digitalWrite(in4,LOW);
    }
}

void moveLeft(int power){
    int pwm = round((power * 255) / 100);
    if(power < 0){
        analogWrite(enA,abs(pwm));
        analogWrite(enB,abs(pwm));
        digitalWrite(in1,LOW);
        digitalWrite(in3,LOW);
        digitalWrite(in2,HIGH);
        digitalWrite(in4,HIGH);
    }
    else{
        analogWrite(enA,pwm);
        analogWrite(enB,pwm);
        digitalWrite(in1,HIGH);
        digitalWrite(in3,HIGH);
        digitalWrite(in2,LOW);
        digitalWrite(in4,LOW);
    }
}

void moveRight(int power){
    int pwm = round((power * 255) / 100);
    if (power < 0){
        analogWrite(enA,abs(pwm));
        analogWrite(enB,abs(pwm));
        digitalWrite(in1,HIGH);
        digitalWrite(in3,HIGH);
        digitalWrite(in2,LOW);
        digitalWrite(in4,LOW);
    }
    else{
        analogWrite(enA,pwm);
        analogWrite(enB,pwm);
        digitalWrite(in1,LOW);
        digitalWrite(in3,LOW);
        digitalWrite(in2,HIGH);
        digitalWrite(in4,HIGH);
    }
}

void legsUp(){
  servo1.write(100);              
  servo2.write(100);
  servo3.write(100);
  servo4.write(100);
}

void legsDown(){
  servo1.write(20);              
  servo2.write(20);
  servo3.write(20);
  servo4.write(20);
}

void cabinFront(){
  servo5.write(0);              
}

void cabinBack(){
  servo5.write(180);              
}

void _delay(float seconds) {
  long endTime = millis() + seconds * 1000;
  while(millis() < endTime) _loop();
}

void setup() {
  servo1.attach(13);
  servo2.attach(12);
  servo3.attach(3);
  servo4.attach(2);
  servo5.attach(4);

  pinMode(in1,OUTPUT);
  pinMode(in2,OUTPUT);
  pinMode(enA,OUTPUT);
  pinMode(in3,OUTPUT);
  pinMode(in4,OUTPUT);
  pinMode(enB,OUTPUT);

  btConnection.begin(9600);
  btConnection.flush();

  speedValue = 50;

  while(1) {
    if(btConnection.available()){
      incomingByte = btConnection.read();

      if(String(incomingByte).indexOf(String("1")) > -1){
        speedValue = 40;

      }else{
        if(String(incomingByte).indexOf(String("2")) > -1){
          speedValue = 50;

        }else{
          if(String(incomingByte).indexOf(String("3")) > -1){
            speedValue = 55;

          }else{
            if(String(incomingByte).indexOf(String("4")) > -1){
              speedValue = 60;

            }else{
              if(String(incomingByte).indexOf(String("5")) > -1){
                speedValue = 65;

              }else{
                if(String(incomingByte).indexOf(String("6")) > -1){
                  speedValue = 70;

                }else{
                  if(String(incomingByte).indexOf(String("7")) > -1){
                    speedValue = 80;

                  }else{
                    if(String(incomingByte).indexOf(String("8")) > -1){
                      speedValue = 85;

                    }else{
                      if(String(incomingByte).indexOf(String("9")) > -1){
                        speedValue = 90;

                      }else{
                        if(String(incomingByte).indexOf(String("q")) > -1){
                          speedValue = 100;

                        }else{
                          if(String(incomingByte).indexOf(String("F")) > -1){
                            moveForward(speedValue);

                          }
                          if(String(incomingByte).indexOf(String("B")) > -1){
                            moveBackward(speedValue);

                          }
                          if(String(incomingByte).indexOf(String("L")) > -1){
                            moveLeft(speedValue);

                          }
                          if(String(incomingByte).indexOf(String("R")) > -1){
                            moveRight(speedValue);

                          }

                          if(String(incomingByte).indexOf(String("U")) > -1){
                            legsUp();                        
                          }

                          if(String(incomingByte).indexOf(String("u")) > -1){
                            legsDown();
                          }

                          if(String(incomingByte).indexOf(String("W")) > -1){
                            cabinFront();                        
                          }

                          if(String(incomingByte).indexOf(String("w")) > -1){
                            cabinBack();
                          }

                          if(String(incomingByte).indexOf(String("S")) > -1){
                            analogWrite(enA,0);
                            analogWrite(enB,0);
                            digitalWrite(in1,LOW);
                            digitalWrite(in2,LOW);
                            digitalWrite(in3,LOW);
                            digitalWrite(in4,LOW);
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