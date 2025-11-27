/*
  Project: Joystick Module Basic Reader (X/Y Axis + Button)
  File: Joystick_basic_console.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/26/2025

  Description:
    Reads X and Y analog axis values and the push-button state from a standard
    2-axis joystick module. Outputs all readings to the Serial Monitor. Useful
    for robotics control, game input and manual actuator control systems.

  Wiring (Standard Joystick Module):
    X-Axis → A0
    Y-Axis → A1
    SW (Button) → D2
    VCC → 5V
    GND → GND

  Libraries:
    - (No external libraries required)

  Serial Baud:
    9600

  Example Serial Output:
    X: 523 | Y: 490 | Button: RELEASED
    X: 1023 | Y: 0 | Button: PRESSED

  Notes:
    - Joystick button is active LOW (pressed = 0).
    - Joystick axes typically output 0–1023 centered near ~512.
    - Use smoothing/averaging for stable robot control.
    - Ideal for servo control, motor control or menu input.

  Real-World Applications:
    - Robot teleoperation
    - Manual servo/motor control
    - Game controller input
    - CNC/manual axis control

  License: GPL-3.0
*/

// Joystick pins
const int xPin = A0;     // X-axis (Analog)
const int yPin = A1;     // Y-axis (Analog)
const int buttonPin = 2; // Joystick button (Digital)

int xValue = 0;
int yValue = 0;
int buttonState = 0;

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);  // Joystick button is active LOW
}

void loop() {
  // Read axis values
  xValue = analogRead(xPin);
  yValue = analogRead(yPin);
  buttonState = digitalRead(buttonPin); // LOW = pressed

  // Print values to Serial Monitor
  Serial.print("X: ");
  Serial.print(xValue);
  Serial.print(" | Y: ");
  Serial.print(yValue);
  Serial.print(" | Button: ");
  Serial.println(buttonState == LOW ? "PRESSED" : "RELEASED");

  delay(100);
}