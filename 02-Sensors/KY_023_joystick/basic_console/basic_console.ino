// ============================================
//   JOYSTICK – BASIC 2-AXIS + BUTTON EXAMPLE
// ============================================
// Joystick pins
const int xPin = A0;     // X-axis (Analog)
const int yPin = A1;     // Y-axis (Analog)
const int buttonPin = 2; // Joystick button (Digital)
int xValue = 0;
int yValue = 0;
int buttonState = 0;

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);  // Joystick button is active LOW
}

void loop() {
  // Read axis values
  xValue = analogRead(xPin);
  yValue = analogRead(yPin);
  buttonState = digitalRead(buttonPin); // LOW = pressed
  // Print values to Serial Monitor
  Serial.print("X: ");
  Serial.print(xValue);
  Serial.print(" | Y: ");
  Serial.print(yValue);
  Serial.print(" | Button: ");
  Serial.println(buttonState == LOW ? "PRESSED" : "RELEASED");
  delay(100);
}