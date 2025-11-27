/*
  Project: Microphone Sound Detection (Digital Output – KY-037/KY-038)
  File: microphone_basic.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/26/2025

  Description:
    Reads digital HIGH/LOW output from a KY-037 or KY-038 microphone module
    and turns an LED on when loud sound or clap is detected. Demonstrates
    simple noise-triggered digital sensing.

  Wiring (KY-037 / KY-038):
    DO  → D8   (Digital sound detection)
    AO  → (Not used in this sketch)
    VCC → 5V
    GND → GND

  LED:
    + → D7 (with 220Ω resistor)
    - → GND

  Libraries:
    - (No external libraries required)

  Serial Baud:
    9600

  Example Serial Output:
    Sound Detected: 1
    Sound Detected: 0

  Notes:
    - Digital pin goes HIGH when sound exceeds onboard potentiometer threshold.
    - Adjust module sensitivity with onboard screw potentiometer.
    - Digital detection is not volume-accurate—just “loud/quiet”.

  Real-World Applications:
    - Clap switch
    - Noise alarm
    - Robot microphone trigger
    - Smart lighting activation

  License: GPL-3.0
*/

int mic = 8;           // Microphone DO pin
int LED = 7;           // LED pin
int soundVal = 0;      // Sound reading

void setup() {
  pinMode(mic, INPUT);
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  soundVal = digitalRead(mic);
  Serial.print("Sound Detected: ");
  Serial.println(soundVal);
  if (soundVal == HIGH) {
    digitalWrite(LED, HIGH);   // Loud sound → LED ON
  } else {
    digitalWrite(LED, LOW);    // Quiet environment → LED OFF
  }
}