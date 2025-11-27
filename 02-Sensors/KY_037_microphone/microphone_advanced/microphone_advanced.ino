/*
  Project: Microphone Sensor Advanced Analyzer (Analog Audio)
  File: microphone_advanced.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/26/2025

  Description:
    Reads analog sound amplitude from a microphone sensor (KY-037/KY-038),
    displays the live waveform values on Serial Monitor and triggers a
    buzzer alarm when audio exceeds a customizable threshold.
    Demonstrates analog signal analysis and peak detection.

  Wiring (Microphone Module – Analog Mode):
    AO  → A0
    DO  → (Not used)
    VCC → 5V
    GND → GND

  Buzzer:
    + → D9
    - → GND

  Libraries:
    - (No external libraries required)

  Serial Baud:
    9600

  Example Serial Output:
    Mic: 132
    Mic: 810
    ALERT! High sound detected.

  Notes:
    - Analog microphone output is noisy; smoothing or averaging helps.
    - Threshold must be calibrated for the environment.
    - Avoid powering strong motors nearby (EM noise).

  Real-World Applications:
    - Noise monitoring
    - Audio-reactive LEDs
    - Industrial noise alarms
    - Smart home automation (sound-triggered events)

  License: GPL-3.0
*/

int micPin = A0;        // Analog microphone output
int buzzerPin = 9;      // Buzzer
int micValue = 0;       // Raw microphone reading
int threshold = 600;    // Sound limit (adjust as needed)

void setup() {
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  micValue = analogRead(micPin);
  Serial.print("Mic: ");
  Serial.println(micValue);

  if (micValue > threshold) {
    Serial.println("ALERT! High sound detected.");
    digitalWrite(buzzerPin, HIGH);
    delay(50);
    digitalWrite(buzzerPin, LOW);
  }

  delay(5);
}