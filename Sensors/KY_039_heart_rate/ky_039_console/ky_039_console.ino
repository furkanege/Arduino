// KY-039 Pulse Sensor (Basic Version)
// Reads analog values, averages them, and lights up an LED if above threshold.

int analogPin = A0;        // KY-039 signal pin
int LED = 13;              // Built-in LED for feedback
int numSamples = 10;       // Number of samples to average
int threshold = 600;       // Pulse detection threshold
int sensorValues[10];      // Array to hold sampled values

void setup() {
  Serial.begin(9600);      // Initialize Serial Monitor
  pinMode(LED, OUTPUT);    // LED as output
}

void loop() {
  int sum = 0;

  // Read multiple samples for stability
  for (int i = 0; i < numSamples; i++) {
    sensorValues[i] = analogRead(analogPin);
    sum += sensorValues[i];
    delay(10);             // Small delay between samples
  }

  int average = sum / numSamples;  // Calculate average value
  Serial.println(average);         // Print averaged value

  // LED ON if pulse value above threshold
  if (average > threshold) {
    digitalWrite(LED, HIGH);
  } 
  else {
    digitalWrite(LED, LOW);
  }
}