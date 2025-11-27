/*
  Project: Potentiometer LED Brightness Control
  File: LED_PWM.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/26/2025

  Description:
    Reads an analog value from a potentiometer (0–1023) and maps it to a PWM
    output (0–255) to control LED brightness. Demonstrates basic analog input,
    mapping and PWM output on Arduino.

  Wiring:
    Potentiometer:
      - Middle Pin → A0
      - Side Pin → 5V
      - Other Side Pin → GND
    LED:
      - Anode (+) → D9 (PWM)
      - Cathode (–) → 220Ω Resistor → GND

  Libraries:
    - (No external libraries required)

  Serial Baud:
    9600

  Example Serial Output:
    Pot Value: 512  |  LED PWM: 128

  Notes:
    - Ensure LED uses a 220Ω–330Ω resistor to prevent damage.
    - Potentiometer should be wired as a voltage divider.
    - PWM on pin 9 uses ~490 Hz frequency on Arduino UNO.

  Real-World Applications:
    - Manual brightness control
    - User interfaces for embedded devices
    - Analog sensor calibration

  License: GPL-3.0
*/

int potPin = A0;     // Potentiometer connected to A0
int ledPin = 9;      // LED connected to PWM pin 9
int potValue = 0;    // Raw analog value
int ledValue = 0;    // Mapped PWM value (0–255)

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  potValue = analogRead(potPin);          // Read 0–1023
  ledValue = map(potValue, 0, 1023, 0, 255); // Convert to 0–255 for PWM
  analogWrite(ledPin, ledValue);          // Control LED brightness
  Serial.print("Pot Value: ");
  Serial.print(potValue);
  Serial.print("  |  LED PWM: ");
  Serial.println(ledValue);
  delay(20);
}