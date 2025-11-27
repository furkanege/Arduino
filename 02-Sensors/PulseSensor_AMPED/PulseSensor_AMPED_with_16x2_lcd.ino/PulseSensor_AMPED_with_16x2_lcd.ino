// PulseSensor AMPED – Basic LCD + Buzzer Example
// Cleaned & Documented Version by Furkan Ege

#include <LiquidCrystal.h>            // LCD library
#include <PulseSensorPlayground.h>    // PulseSensor library

const int BUZZER_PIN = 8;   // Buzzer output pin
const int PULSE_PIN  = A0;  // PulseSensor signal pin
int alarmState = 0;         // Buzzer state flag (0 = off, 1 = on)
PulseSensorPlayground pulseSensor;  
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);  // LCD wiring

void setup() {
  Serial.begin(9600);
  pulseSensor.analogInput(PULSE_PIN);
  pulseSensor.blinkOnPulse(13);      // Blink LED on heartbeat (optional)
  pulseSensor.setSerial(Serial);     // Enable serial monitoring
  pulseSensor.setThreshold(550);     // Heartbeat detection threshold
  
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Pulse Sensor");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(1500);
}

void loop() {
  int bpm = pulseSensor.getBeatsPerMinute();  
  pulseSensor.sawNewSample();                // Process new sample
  if (pulseSensor.sawStartOfBeat()) {
    Serial.print("BPM: ");
    Serial.println(bpm);
    if (bpm > 100) {
      alarmState = 1;
    } else {
      alarmState = 0;
    }
  }

  digitalWrite(BUZZER_PIN, alarmState ? HIGH : LOW);
  lcd.setCursor(0, 0);
  lcd.print("Nabiz: ");
  lcd.print(bpm);
  lcd.print(" BPM   "); // Clear trailing chars
  delay(200);
}