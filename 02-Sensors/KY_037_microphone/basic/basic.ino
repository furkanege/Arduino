// Microphone Sound Detection (KY-037 / KY-038)
// LED reacts when sound intensity goes above threshold.

const int LED = 13;        // Indicator LED
const int Sensor = A0;     // Analog output of microphone module
int level;                 // Sound level reading
const int threshold = 640; // Sound threshold

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);      // For monitoring sound levels
}

void loop() {
  level = analogRead(Sensor);  // Read microphone
  Serial.println(level);       // Print value to Serial Monitor
  if (level > threshold) {     // Loud sound detected
    digitalWrite(LED, HIGH);   // LED ON
    delay(5000);               // Stay ON for 5 sec
    digitalWrite(LED, LOW);    // LED OFF
  }
  else {
    digitalWrite(LED, LOW);    // Quiet environment → LED OFF
  }
}