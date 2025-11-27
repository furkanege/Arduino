// Potentiometer Serial Monitor Reader
// Reads raw potentiometer value and prints it to the Serial Monitor.

int potPin = A0;     // Potentiometer connected to A0
int potValue = 0;    // Raw analog value

void setup() {
  Serial.begin(9600);
}

void loop() {
  potValue = analogRead(potPin);
  Serial.print("Potentiometer Value: ");
  Serial.println(potValue);
  delay(50);
}