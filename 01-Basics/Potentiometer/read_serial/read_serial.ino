/*
  Project: Potentiometer Serial Monitor Reader
  File: read_serial.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/26/2025

  Description:
    Reads the raw analog value (0–1023) from a potentiometer connected to A0
    and prints it directly to the Serial Monitor. Useful for calibration,
    debugging and understanding analog input behavior.

  Wiring:
    Potentiometer:
      - Middle Pin → A0
      - Side Pin → 5V
      - Other Side Pin → GND

  Libraries:
    - (No external libraries required)

  Serial Baud:
    9600

  Example Serial Output:
    Potentiometer Value: 734

  Notes:
    - The ADC on Arduino UNO is 10-bit (0–1023).
    - Potentiometer must be wired as a voltage divider.
    - Ideal for reading any analog sensor before processing or mapping.

  Real-World Applications:
    - Sensor calibration
    - User input reading
    - Debugging analog circuits

  License: GPL-3.0
*/

int potPin = A0;     // Potentiometer connected to A0
int potValue = 0;    // Raw analog value

void setup() {
  Serial.begin(9600);
}

void loop() {
  potValue = analogRead(potPin);
  Serial.print("Potentiometer Value: ");
  Serial.println(potValue);
  delay(50);
}