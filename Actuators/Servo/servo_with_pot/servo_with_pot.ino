#include <Servo.h>
Servo myServo;

const int potPin = A0;   // Potentiometer input
const int servoPin = 9;  // Servo output

int potValue = 0;
int angle = 0;

void setup() {
  myServo.attach(servoPin);
}

void loop() {
  potValue = analogRead(potPin);        // Read potentiometer (0–1023)
  angle = map(potValue, 0, 1023, 0, 180); // Convert to servo angle (0–180)
  myServo.write(angle); // Set servo angle
  delay(10);
}