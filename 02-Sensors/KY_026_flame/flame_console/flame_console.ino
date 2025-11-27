/*
  Project: Flame Sensor Console Reader (Analog Fire Detection)
  File: flame_console.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/26/2025

  Description:
    Reads raw analog values from a flame sensor (IR fire sensor) and prints
    them to the Serial Monitor. Useful for understanding sensor behavior,
    threshold tuning and basic fire/light detection applications.

  Wiring (Flame Sensor Module):
    AO → A0     (Analog output)
    DO → D2     (Not used in this example)
    VCC → 5V
    GND → GND

  Libraries:
    - (No external libraries required)

  Serial Baud:
    9600

  Example Serial Output:
    Flame Value: 782
    Flame Value: 134

  Notes:
    - In most modules: Lower analog value = stronger flame detected.
    - Avoid direct sunlight interference.
    - Analog threshold typically needs calibration for your environment.

  Real-World Applications:
    - Fire detection systems
    - Gas stove/flame monitoring
    - Safety automation
    - Robotics fire detection challenge projects

  License: GPL-3.0
*/

int flamePin = A0;  // Analog output from flame sensor
int analogVal = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  analogVal = analogRead(flamePin);
  Serial.print("Flame Value: ");
  Serial.println(analogVal);
  delay(100);
}