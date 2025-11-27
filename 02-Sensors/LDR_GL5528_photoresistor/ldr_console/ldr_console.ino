/*
  Project: LDR Light Sensor Monitor (Console Output)
  File: ldr_console.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/27/2025

  Description:
    Reads analog light intensity values from an LDR (Light Dependent Resistor)
    and prints them to the Serial Monitor. Demonstrates simple ambient light
    sensing for IoT, automation, and robotics.

  Wiring (LDR Voltage Divider):
    LDR → 5V
    LDR → A0
    10k Resistor → A0 → GND

  Libraries:
    - (No external libraries required)

  Serial Baud:
    9600

  Example Serial Output:
    LDR Value: 843
    LDR Value: 121

  Notes:
    - Higher values typically indicate stronger light.
    - Exact reading depends on resistor choice (default 10k).
    - Ideal for threshold-based light detection.

  Real-World Applications:
    - Day/Night detection
    - Smart street lights
    - Automatic brightness control
    - Solar tracking systems

  License: GPL-3.0
*/

#define LED_PIN 9
#define LDR_PIN A0

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  int ldrValue = analogRead(LDR_PIN);  // Read LDR sensor value
  Serial.print("LDR Value: ");
  Serial.println(ldrValue);
  if (ldrValue > 500) {
    digitalWrite(LED_PIN, LOW);       // Bright environment → LED OFF
  } else {
    digitalWrite(LED_PIN, HIGH);      // Dark environment   → LED ON
  }
  delay(50);
}