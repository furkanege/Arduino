/*
  LDR Light Sensor + LED Control
  Clean version with Serial feedback
  - Reads light level from LDR
  - Turns LED ON/OFF based on threshold
  - Prints both sensor value and LED status
*/

#define LDR_PIN A0        // LDR connected to analog pin A0
#define LED_PIN 13        // LED connected to pin 13

int ldrValue = 0;         // Stores LDR reading
int threshold = 650;      // Light cutoff threshold

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);  // LED OFF initially
}

void loop() {
  ldrValue = analogRead(LDR_PIN);   // Read sensor
  Serial.print("LDR Value: ");
  Serial.print(ldrValue);

  if (ldrValue > threshold) {
    digitalWrite(LED_PIN, LOW);     // LED OFF
    Serial.println(" --> LED OFF");
  } else {
    digitalWrite(LED_PIN, HIGH);    // LED ON
    Serial.println(" --> LED ON");
  }
  delay(100);
}