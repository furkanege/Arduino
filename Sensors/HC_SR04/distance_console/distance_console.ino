// ------------------------------------------------------------
// HC-SR04 Ultrasonic Distance Measurement
// ------------------------------------------------------------

int trigPin = 13;   // Trigger pin (sends ultrasonic pulse)
int echoPin = 12;   // Echo pin (receives returned pulse)
long duration;      // Stores pulse travel time (microseconds)
long distance;      // Calculated distance (centimeters)

void setup() {
  pinMode(trigPin, OUTPUT);  // TRIG is an output
  pinMode(echoPin, INPUT);   // ECHO is an input
  Serial.begin(9600);        // Start serial communication
}

void loop() {
  // --- Send ultrasonic pulse ---
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);          // Stabilization delay
  digitalWrite(trigPin, HIGH);   // Start 10µs pulse
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);    // End pulse
  // --- Measure echo return time ---
  duration = pulseIn(echoPin, HIGH);
  // --- Convert time to distance ---
  // Sound speed = 343 m/s → 29.1 µs per cm
  // Distance = (duration / 29.1) / 2 (go + return)
  distance = (duration / 29.1) / 2;
  // --- Output result to Serial Monitor ---
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(500); // Update every 0.5 seconds
}