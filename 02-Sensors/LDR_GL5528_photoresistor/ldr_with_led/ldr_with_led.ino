/*
  Project: LDR Light Sensor with LED Auto-Control
  File: ldr_with_led.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/27/2025

  Description:
    Uses an LDR to detect brightness and automatically controls an LED.
    Bright → LED OFF  
    Dark → LED ON  
    Ideal for smart lighting and automation demos.

  Wiring (LDR Voltage Divider):
    LDR → 5V
    LDR → A0
    10k Resistor → A0 → GND

  LED:
    + → D9 (with 220Ω resistor)
    - → GND

  Libraries:
    - (No external libraries required)

  Serial Baud:
    9600

  Example Serial Output:
    LDR Value: 143 → LED OFF
    LDR Value: 392 → LED ON

  Notes:
    - Threshold can be tuned based on environment (default: 500).
    - Direct sunlight may saturate the sensor.
    - Indoor lighting varies between 100–700.

  Real-World Applications:
    - Automatic room lighting
    - Solar-based systems
    - Security & door lighting
    - Smart IoT brightness-triggered events

  License: GPL-3.0
*/

#define LDR_PIN A0
#define LED_PIN 9
const int threshold = 500;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  int ldrValue = analogRead(LDR_PIN);
  Serial.print("LDR Value: ");
  Serial.print(ldrValue);
  if (ldrValue > threshold) {
    digitalWrite(LED_PIN, LOW);  // Bright → LED OFF
    Serial.println(" --> LED OFF");
  } else {
    digitalWrite(LED_PIN, HIGH); // Dark → LED ON
    Serial.println(" --> LED ON");
  }

  delay(100);
}