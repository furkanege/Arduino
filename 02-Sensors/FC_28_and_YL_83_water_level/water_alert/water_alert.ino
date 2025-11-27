/*
  Project: Water Level Sensor Alert System (YL-83 / FC-28)
  File: water_alert.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/26/2025

  Description:
    Reads moisture/water presence using a YL-83 (FC-28) analog water level
    sensor and triggers a buzzer alarm when the reading exceeds a threshold.
    Demonstrates basic threshold sensing for leak detection or tank overflow
    applications.

  Wiring:
    Water Level Sensor (Analog):
      - Signal → A0
      - VCC → 5V
      - GND → GND
    
    Buzzer:
      - + → D9
      - - → GND

  Libraries:
    - (No external libraries required)

  Serial Baud:
    9600

  Example Serial Output:
    Raw Sensor: 512
    ALERT: Water detected!

  Notes:
    - YL-83/FC-28 sensors vary by module; threshold may require calibration.
    - Sensor values often range 0–700 depending on conductivity.
    - Avoid long-term water contact; sensor corrodes over time.
    - If the buzzer is too loud, use a small series resistor.

  Real-World Applications:
    - Water leak detection
    - Plant watering monitor
    - Tank overflow alert systems
    - Basement flood alarms

  License: GPL-3.0
*/

int thresholdValue = 400;      // Water level threshold
int buzzerPin = 9;             // Buzzer pin
int sensorValue;               // Raw sensor reading

void setup() {
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
}

void loop() {
  // Read analog value from water level sensor
  sensorValue = analogRead(A0);
  Serial.print("Raw Sensor: ");
  Serial.println(sensorValue);

  // If water level is above threshold → activate buzzer
  if (sensorValue > thresholdValue) {
    digitalWrite(buzzerPin, HIGH);
    delay(200);
    digitalWrite(buzzerPin, LOW);
    delay(200);
  } 
  else {
    digitalWrite(buzzerPin, LOW);
  }
  delay(100);
}