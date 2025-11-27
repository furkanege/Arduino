// MQ-9 Gas Sensor Basic Example

#define GAS_SENSOR   A0     // MQ-9 analog output
#define RED_LED      2
#define GREEN_LED    3
#define BUZZER       7
int sensorValue;
int thresholdValue = 450;   // Adjustable gas threshold

void setup() {
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  sensorValue = analogRead(GAS_SENSOR);
  Serial.println(sensorValue);
  if (sensorValue > thresholdValue) {
    // Gas detected
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BUZZER, HIGH);
    digitalWrite(RED_LED, HIGH);
    delay(300);
    digitalWrite(RED_LED, LOW);
    digitalWrite(BUZZER, LOW);
    delay(300);
  } else {
    // Safe level
    digitalWrite(RED_LED, LOW);
    digitalWrite(BUZZER, LOW);
    digitalWrite(GREEN_LED, HIGH);
  }
}