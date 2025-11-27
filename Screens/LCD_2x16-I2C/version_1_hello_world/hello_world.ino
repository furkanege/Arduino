#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("LCD 16x2 Test");
  lcd.setCursor(0, 1);
  lcd.print("Hello World!");
}

void loop() {
}