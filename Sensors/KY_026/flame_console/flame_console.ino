/*
  Basic Flame Sensor (Fire Detection) Project
  Author: Furkan Ege
  Description:
  - Reads flame detection from a digital pin
  - Reads flame intensity from an analog pin
  - Turns LED ON when flame is NOT detected
  - Turns LED OFF when flame IS detected
*/

int ledPin = 13;          // LED indicator pin
int digitalPin = 3;       // Flame sensor digital output
int analogPin = A1;       // Flame sensor analog output
int digitalVal = 0;       // Stores digital reading
int analogVal = 0;        // Stores analog reading

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(digitalPin, INPUT);
  Serial.begin(9600);
  Serial.println("Flame Sensor Test Started...");
}

void loop() {
  // --- Digital read (0 = flame detected, 1 = no flame) ---
  digitalVal = digitalRead(digitalPin);
  if (digitalVal == HIGH) {
    // No flame detected → LED ON
    digitalWrite(ledPin, HIGH);
  } else {
    // Flame detected → LED OFF
    digitalWrite(ledPin, LOW);
  }
  // --- Analog read (intensity measurement) ---
  analogVal = analogRead(analogPin);
  Serial.print("Analog Value: ");
  Serial.println(analogVal);

  delay(100);
}