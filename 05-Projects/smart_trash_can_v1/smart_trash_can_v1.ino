#include <Servo.h>
#define TRIG_PIN 13
#define ECHO_PIN 12
#define SERVO_PIN 10
#define SERVO_PIN2 11

Servo servo1;
Servo servo2;

void setup() {
  servo1.attach(SERVO_PIN);
  servo2.attach(SERVO_PIN2);
  servo1.write(0);
  servo2.write(0);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  long duration, distance;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration / 2) / 29.1;
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  if (distance > 30) {
    servo1.write(180);
    servo2.write(180);
    delay(1000);
  } else {
    servo1.write(5);
    servo2.write(5);
  }
}