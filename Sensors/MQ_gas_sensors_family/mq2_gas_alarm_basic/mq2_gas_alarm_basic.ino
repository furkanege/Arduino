// ===============================
// Basic Gas Sensor + Buzzer Example
// ===============================
const int gasSensorPin = A0;   // Analog output from gas sensor (AO pin)
const int buzzerPin    = 9;    // Buzzer connected to digital pin 9
int gasThreshold = 400;
int gasValue = 0;

void setup() {
  pinMode(buzzerPin, OUTPUT);   
  digitalWrite(buzzerPin, LOW);
  Serial.begin(9600); // Optional: For monitoring sensor values
}

void loop() {
  // Read analog value from gas sensor
  gasValue = analogRead(gasSensorPin);
  Serial.print("Gas Value: ");
  Serial.println(gasValue);
  if (gasValue > gasThreshold) {
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    delay(100);
  } 
  else {
    digitalWrite(buzzerPin, LOW);
  }
  delay(50); // Small delay for stability
}