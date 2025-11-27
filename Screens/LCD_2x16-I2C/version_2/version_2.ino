#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.begin();
}

void loop() {
  int pos;

  // First text
  lcd.setCursor(0, 0);
  lcd.print("Hello");
  lcd.setCursor(0, 1);
  lcd.print("Everyone!");
  delay(2000);

  // Second text
  lcd.setCursor(0, 0);
  lcd.print("This is");
  lcd.setCursor(0, 1);
  lcd.print("Massachusetts");
  delay(2000);

  // Third text (scrolling)
  lcd.clear();
  lcd.noBacklight();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Welcome To");

  for (pos = 0; pos < 17; pos++) {
    lcd.setCursor(pos, 1);
    lcd.print("Our State!");
    delay(300);
    lcd.setCursor(pos, 1);
    lcd.print(" ");
  }
  lcd.clear();
}