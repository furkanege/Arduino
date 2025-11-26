/*
  Basic LDR Reader + LED Output
  Simple version for Serial Monitor testing
*/

#define LED_PIN 3        // LED connected to pin 3

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  int lightValue = analogRead(A0);   // Read LDR value
  Serial.print("LDR: ");
  Serial.println(lightValue);
  // LED logic
  if (lightValue > 900) {
    digitalWrite(LED_PIN, LOW);      // LED OFF
  }
  else if (lightValue < 850) {
    digitalWrite(LED_PIN, HIGH);     // LED ON
  }
  delay(50);
}