// Water Level Sensor (YL-83 / FC-28) Basic Example
// Buzzer + Water Level Detection
// Open-source standard format by Furkan Ege

int thresholdValue = 400;      // Water level threshold
int buzzerPin = 9;             // Buzzer pin
int sensorValue;               // Raw sensor reading

void setup() {
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);
}

void loop() {
  // Read analog value from water level sensor
  sensorValue = analogRead(A0);
  Serial.print("Water Level: ");
  Serial.println(sensorValue);

  // If water level is above threshold → activate buzzer
  if (sensorValue > thresholdValue) {
    digitalWrite(buzzerPin, HIGH);
    delay(200);
    digitalWrite(buzzerPin, LOW);
    delay(200);
  } 
  else {
    digitalWrite(buzzerPin, LOW);
  }

  delay(100);
}