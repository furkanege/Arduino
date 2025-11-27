#include <Servo.h>
Servo servoMotor;

void setup() {
  servoMotor.attach(9);    // Attach servo to pin 9
  servoMotor.write(0);     // Move to 0°
  delay(1000);
  servoMotor.write(120);   // Move to 120°
  delay(1000);
  servoMotor.write(0);     // Move to 0°
  delay(1000);
  servoMotor.write(120);   // Move to 120°
  delay(1000);
}

void loop() {
}