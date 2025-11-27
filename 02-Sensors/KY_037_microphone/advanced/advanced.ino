/*
  Microphone Sensor (Analog) – Advanced v2
  Features:
  - Noise filtering (moving average)
  - Peak detection
  - LED + Buzzer alert
  - Clear serial monitoring
  Author: Furkan Ege
*/

const int micPin = A0;      // Microphone analog output
const int ledPin = 13;      // LED indicator
const int buzzerPin = 12;   // Buzzer for loud alerts

// Threshold values
int soundThreshold = 300;   // Normal sound alert
int loudThreshold  = 500;   // Very loud sound alert (buzzer)
const int filterSamples = 10;  // Moving average filter buffer
int buffer[filterSamples];
int indexPos = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  // Initialize buffer
  for (int i = 0; i < filterSamples; i++) {
    buffer[i] = 0;
  }
}

void loop() {
  // Read raw microphone value
  int raw = analogRead(micPin);
  // ---- Moving Average Filter ----
  buffer[indexPos] = raw;
  indexPos = (indexPos + 1) % filterSamples;
  long sum = 0;
  for (int i = 0; i < filterSamples; i++) {
    sum += buffer[i];
  }
  int filtered = sum / filterSamples;
  // ---- Serial Output ----
  Serial.print("Raw: ");
  Serial.print(raw);
  Serial.print(" | Filtered: ");
  Serial.println(filtered);
  // ---- Sound Logic ----
  if (filtered > soundThreshold) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
  if (filtered > loudThreshold) {
    digitalWrite(buzzerPin, HIGH);
    delay(50);
    digitalWrite(buzzerPin, LOW);
  }
  delay(5);
}