// Potentiometer LED Brightness Control
// Reads analog value from potentiometer and controls LED brightness using PWM.

int potPin = A0;     // Potentiometer connected to A0
int ledPin = 9;      // LED connected to PWM pin 9
int potValue = 0;    // Raw analog value
int ledValue = 0;    // Mapped PWM value (0–255)

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  potValue = analogRead(potPin);          // Read 0–1023
  ledValue = map(potValue, 0, 1023, 0, 255); // Convert to 0–255 for PWM
  analogWrite(ledPin, ledValue);          // Control LED brightness
  Serial.print("Pot Value: ");
  Serial.print(potValue);
  Serial.print("  |  LED PWM: ");
  Serial.println(ledValue);
  delay(20);
}