// PIR Motion Sensor (HC-SR501) – Basic Example
int pirPin = 8;      // PIR signal pin
int ledPin = 9;      // LED pin
int value = 0;       // PIR reading variable

void setup() {
  pinMode(pirPin, INPUT);     // PIR as input
  pinMode(ledPin, OUTPUT);    // LED as output
  Serial.begin(9600);         // Serial monitor
}

void loop() {
  value = digitalRead(pirPin);   // Read PIR signal
  Serial.println(value);         // Print to serial monitor
  if (value == HIGH) {           // Motion detected
    digitalWrite(ledPin, HIGH);  // Turn LED on
  } 
  else {
    digitalWrite(ledPin, LOW);   // No motion → LED off
  }
}