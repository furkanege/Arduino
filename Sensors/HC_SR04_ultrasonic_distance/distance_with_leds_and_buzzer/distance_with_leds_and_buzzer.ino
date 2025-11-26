/* 
  HC-SR04 Distance Sensor + Traffic LEDs + Buzzer
  Version: V2 (Based on Fritzing circuit)
  Author: Furkan Ege
  Description:
  - Measures distance using HC-SR04 ultrasonic sensor
  - Shows distance status using 3 LEDs (Green–Yellow–Red)
  - Activates buzzer when distance is too close
*/

#define TRIG_PIN 13      // HC-SR04 Trigger pin
#define ECHO_PIN 12      // HC-SR04 Echo pin
#define LED_GREEN 8      // Safe distance
#define LED_YELLOW 7     // Warning distance
#define LED_RED 6        // Danger distance
#define BUZZER_PIN 5     // Buzzer pin
long duration;
int distance;

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  // Turn everything OFF at startup
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_RED, LOW);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  // --- Send Ultrasonic Pulse ---
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  // --- Read Echo Response ---
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration / 29.1) / 2;   // Convert time to distance (cm)
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  // --- LED & Buzzer Logic ---
  if (distance > 50) {  
    // SAFE
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  } else if (distance > 20 && distance <= 50) {
    // WARNING
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_RED, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  } else if (distance <= 20) {
    // DANGER – BUZZER ON
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
  }
  delay(200); // Slight delay for stable readings
}