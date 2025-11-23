/*
  Fire Detection System v2
  Components:
  - Flame Sensor (Digital + Analog)
  - LED Indicator
  - Buzzer Alarm
  - 5V Water Pump
  This project detects fire using a flame sensor.
  When fire is detected:
    → LED turns ON
    → Buzzer activates
    → Water pump runs for 5 seconds
*/

int flamePinDigital = 2;   // Flame Sensor Digital Output
int flamePinAnalog  = A1;  // Flame Sensor Analog Output
int ledPin = 13;           // Fire Alert LED
int buzzerPin = 12;        // Buzzer for alarm
int pumpPin = 11;          // Water Pump Relay / Motor Driver
int flameDigitalValue;     // Stores digital reading (0 or 1)
int flameAnalogValue;      // Stores analog fire intensity value
int flameThreshold = 100;  // Optional threshold (not used in digital mode)

void setup() {
  pinMode(flamePinDigital, INPUT);
  pinMode(flamePinAnalog, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(pumpPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("Fire Detection System v2 Started...");
}

void loop() {
  flameDigitalValue = digitalRead(flamePinDigital);
  flameAnalogValue = analogRead(flamePinAnalog);
  // Print sensor readings to serial monitor
  Serial.print("Fire Detected (Digital): ");
  Serial.println(flameDigitalValue);
  Serial.print("Fire Intensity (Analog): ");
  Serial.println(flameAnalogValue);

  // Digital Output LOW --> Fire detected
  if (flameDigitalValue == LOW) {
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(pumpPin, HIGH);
    delay(5000); // Run pump for 5 seconds
    digitalWrite(pumpPin, LOW);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledPin, LOW);
  } 
  else {
    // No fire detected → All systems OFF
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(pumpPin, LOW);
  }
  delay(100);
}