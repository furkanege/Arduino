#include <LiquidCrystal_I2C.h>   // LCD I2C library
LiquidCrystal_I2C lcd(0x27, 16, 2);
const int pumpPin = 2;           // Water pump relay/module pin
const int soilMoisturePin = A0;  // Soil moisture sensor analog pin

void setup() {
  Serial.begin(9600);
  // LCD initialization
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  // Pump setup
  pinMode(pumpPin, OUTPUT);
  digitalWrite(pumpPin, HIGH);  // Pump OFF (assuming LOW = active)
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("Smart Irrigation");
  lcd.setCursor(0, 1);
  lcd.print("System Active...");
  delay(3000);
  lcd.clear();
}

void loop() {
  int moistureValue = analogRead(soilMoisturePin);  // Read soil moisture
  Serial.println(moistureValue);
  // ----- Pump Control -----
  if (moistureValue > 950) {  // Soil is dry
    digitalWrite(pumpPin, LOW);  // Turn pump ON
    lcd.setCursor(0, 0);
    lcd.print("Pump: ACTIVE   ");
  } 
  else {
    digitalWrite(pumpPin, HIGH); // Turn pump OFF
    lcd.setCursor(0, 0);
    lcd.print("Pump: OFF      ");
  }
  // ----- Moisture Level Display -----
  lcd.setCursor(0, 1);
  if (moistureValue < 300) {
    lcd.print("Moisture: HIGH ");
  }
  else if (moistureValue < 950) {
    lcd.print("Moisture: MED  ");
  }
  else {
    lcd.print("Moisture: LOW  ");
  }
  delay(300);
}