/*
  Project: KY-035 Hall Effect Sensor Console Reader
  File: hall_effect_console.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/26/2025

  Description:
    Reads analog magnetic field intensity using the KY-035 analog Hall effect
    sensor and prints results to the Serial Monitor. Useful for testing magnet
    presence, magnetic force changes and sensor calibration.

  Wiring (KY-035 Hall Sensor):
    AOUT → A0
    DOUT → (Not used in this example)
    VCC  → 5V
    GND  → GND

  Libraries:
    - (No external libraries required)

  Serial Baud:
    9600

  Example Serial Output:
    Magnetic Field Value: 312
    Magnetic Field Value: 721

  Notes:
    - KY-035 outputs higher analog values when magnetic field intensity increases.
    - Use strong neodymium magnets for clear variation.
    - Keep away from high-current wires to avoid interference.
    - Analog output typically ranges between ~100–900 depending on magnet.

  Real-World Applications:
    - Magnetic field detection
    - Rotational speed sensing (with magnets)
    - Contactless switching
    - Robotics magnetic navigation experiments

  License: GPL-3.0
*/

const int hallSensorPin = A0;   // KY-035 analog output connected to A0
int hallValue = 0;              // Variable to store sensor reading

void setup() {
  Serial.begin(9600);           // Start serial communication
  Serial.println("KY-035 Hall Effect Sensor Test Started");
}

void loop() {
  // Read analog magnetic field value
  hallValue = analogRead(hallSensorPin);
  // Print value to Serial Monitor
  Serial.print("Magnetic Field Value: ");
  Serial.println(hallValue);
  delay(300);  // Small delay for stability
}