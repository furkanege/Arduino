#include <Servo.h>
#define TRIG_PIN 13
#define ECHO_PIN 12
#define SERVO_PIN 10
const int MicSensor = A0;
int soundValue;
Servo myservo;
const int thresholdValue = 640;

void setup() {
  myservo.attach(SERVO_PIN);
  myservo.write(0);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  soundValue = analogRead(MicSensor);
  long duration, distance;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration / 2) / 29.1;
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  Serial.println(soundValue);

  if (distance > 10) {
    myservo.write(180);
    delay(1000);
  }

  if (soundValue > thresholdValue) {
    myservo.write(180);
    delay(1000);
  } 
  else {
    myservo.write(10);
  }
}