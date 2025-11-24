#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Sensör ve LED için pin tanımları
const int MQ9_PIN = A0;    // MQ-9 gaz sensörü analog pini
const int LED_PIN = 13;    // Uyarı LED pini

// Gaz seviyesinin kritik olacağı eşik (% cinsinden)
const int GAS_THRESHOLD = 50;

// LCD ekran ayarları (I2C adresi 0x27, 16 sütun, 2 satır)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // Seri haberleşmeyi başlat (ekran ve hata ayıklama amaçlı)
  Serial.begin(9600);

  // LED pinini çıkış (OUTPUT) olarak ayarla
  pinMode(LED_PIN, OUTPUT);

  // LCD başlatma
  lcd.init();       // Bazı kütüphanelerde "lcd.begin(16, 2);" yerine "lcd.init();" kullanılır
  lcd.backlight();  // LCD arka ışığını aç

  // Hoş geldiniz mesajı
  lcd.setCursor(0, 0);
  lcd.print("Gaz Tespit Sistemi");
  delay(2000); // 2 saniye bekleyerek mesajın okunmasını sağla

  // LCD ekranı temizle
  lcd.clear();
}

void loop() {
  // 1) Sensör değerini oku
  int sensorDegeri = analogRead(MQ9_PIN);

  // 2) Sensör değerini (0-1023) aralığından, (0-100) aralığına çevir
  int gazSeviyesi = map(sensorDegeri, 0, 1023, 0, 100);

  // 3) Gaz seviyesini seri monitöre yazdır
  Serial.print("Gaz Seviyesi: ");
  Serial.print(gazSeviyesi);
  Serial.println("%");

  // 4) LCD ekranını güncelle
  lcd.setCursor(0, 0);
  lcd.print("Hava Temizligi:");
  lcd.setCursor(0, 1);
  lcd.print("% ");
  lcd.print(gazSeviyesi);
  lcd.print("   "); // Önceki verileri silmek için birkaç boşluk bırak

  // 5) Eşik değeri aşılırsa LED’i yak, aşılmazsa söndür
  if (gazSeviyesi > GAS_THRESHOLD) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  // 6) Biraz bekleyip döngüye tekrar başla (0.5 saniye)
  delay(500);
}