/*
  KY-035 Hall Effect Sensor – Basic Magnetic Field Reading
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