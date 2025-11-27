#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  lcd.begin();
}

void loop() {
  int i;
  lcd.setCursor(3,0);
  lcd.print("WELCOME");
  for (i = 0; i < 17; i++) {
    lcd.setCursor(i,1);
    lcd.print("HELLO WORLD!");
    delay(300);
    lcd.setCursor(i,1);
    lcd.print(" ");
  }
}