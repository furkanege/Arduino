/*
  Project: KY-039 Pulse Sensor Basic Serial Reader
  File: ky_039_console.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/26/2025

  Description:
    Reads raw analog input from the KY-039 pulse sensor and displays the
    fluctuating waveform values on the Serial Monitor. Useful for studying
    pulse waveform patterns and experimenting with basic heart-beat detection.

  Wiring (KY-039 Pulse Sensor):
    Signal → A0
    VCC    → 5V
    GND    → GND

  LED (Heartbeat Indicator):
    + → D13
    - → GND

  Libraries:
    - (No external libraries required)

  Serial Baud:
    9600

  Example Serial Output:
    Pulse Value: 132
    Pulse Value: 187

  Notes:
    - KY-039 is highly sensitive; keep finger still during measurement.
    - Environmental light affects waveform stability.
    - This basic version does NOT calculate BPM.

  Real-World Applications:
    - Pulse waveform visualization
    - Biomedical sensor experimentation
    - Heartbeat-triggered interactions

  License: GPL-3.0
*/

int pulsePin = A0;
int led = 13;

void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int value = analogRead(pulsePin);
  Serial.print("Pulse Value: ");
  Serial.println(value);

  if (value > 550) {
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
  }
}