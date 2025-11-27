/*
  Project: Automatic Fire Detection & Suppression System (Flame Sensor + Water Pump)
  File: flame_with_water_pump.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/26/2025

  Description:
    Detects the presence of fire using a flame sensor and automatically
    activates a water pump through a relay module when the fire threshold
    is exceeded. Demonstrates closed-loop safety automation.

  Wiring:
    Flame Sensor:
      AO → A0
      DO → (optional)
      VCC → 5V
      GND → GND

    Relay Module:
      IN → D8
      VCC → 5V
      GND → GND

    Water Pump:
      Pump + → Relay NO
      Pump - → External Power GND
      External power + → Relay COM

  Libraries:
    - (No external libraries required)

  Serial Baud:
    9600

  Example Serial Output:
    Fire Level: 155
    FIRE DETECTED! Activating pump...

  Notes:
    - Many sensors: LOWER analog value = STRONGER flame signal.
    - Pump MUST be powered from external supply (not from Arduino).
    - Relay modules are typically LOW-activated.
    - Strong electromagnetic pumps may require diode or LC filtering.

  Real-World Applications:
    - Automatic fire extinguishing robot
    - Safety suppression systems
    - Laboratory fire monitoring
    - Smart home fire detection modules

  License: GPL-3.0
*/

int flamePin = A0;      // Flame sensor analog output
int relayPin = 8;       // Relay controlling water pump
int value = 0;          // Sensor reading
int threshold = 300;    // Lower = stronger flame (adjust as needed)

void setup() {
  Serial.begin(9600);
  pinMode(flamePin, INPUT);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // Relay HIGH = OFF (common modules)
}

void loop() {
  value = analogRead(flamePin);
  Serial.print("Fire Level: ");
  Serial.println(value);

  // Flame detected (LOW value means strong IR flame signal)
  if (value < threshold) {
    Serial.println("FIRE DETECTED! Activating pump...");
    digitalWrite(relayPin, LOW);  // Activate pump
    delay(800);
    digitalWrite(relayPin, HIGH); // OFF
  }
  else {
    digitalWrite(relayPin, HIGH);
  }
  delay(100);
}