#include <Servo.h>

// --- Servo Objects ---
Servo servo13;
Servo servo12;
Servo servo11;
Servo servo10;

void setup() {
  // --- Initialize each servo and set to 0 degrees ---
  initializeServo(servo13, 13);
  initializeServo(servo12, 12);
  initializeServo(servo11, 11);
  initializeServo(servo10, 10);
}

void loop() {
  // Nothing here, servos are only zeroed on startup.
}

// ----------------------------------------------------
//  Function: Initialize a servo and move it to 0°
//  - servo: Servo object reference
//  - pin: Arduino pin to attach the servo
// ----------------------------------------------------
void initializeServo(Servo &servo, int pin) {
  servo.attach(pin);     // Attach servo to selected pin
  servo.write(0);        // Move servo to 0 degrees
  delay(1000);           // Wait 1 second for stabilization
}