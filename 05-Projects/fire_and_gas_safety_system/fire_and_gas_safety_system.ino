int flamePinDigital = 2; // Alev Sensörü'nün dijital pin'i
int flamePinAnalog = A1; // Alev Sensörü'nün analog pin'i
int mq2Pin = A0; // MQ2 Gaz Sensörü
int ledPin = 13; // LED
int buzzerPin = 12; // Buzzer
int fanPin = 10; // Pervane
int waterPumpPin = 11; // Su pompası

int flameValDigital; // Alev Sensörü'nün dijital değeri
int flameValAnalog; // Alev Sensörü'nün analog değeri
int mq2Val; // Gaz Sensörü değeri

int gasThreshold = 100; // Gaz eşik değeri
int flameThresold = 100; // Ateş eşik değeri

void setup() {
  pinMode(flamePinDigital, INPUT);
  pinMode(flamePinAnalog, INPUT);
  pinMode(mq2Pin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(fanPin, OUTPUT); // Pervane için çıkış ayarlaması
  pinMode(waterPumpPin, OUTPUT); // Su pompası için çıkış ayarlaması
  Serial.begin(9600); // Serial monitör başlatılıyor.
}

void loop() {
  flameValDigital = digitalRead(flamePinDigital); // Alev Sensörü'nün dijital değeri okunuyor.
  flameValAnalog = analogRead(flamePinAnalog); // Alev Sensörü'nün analog değeri okunuyor.
  mq2Val = analogRead(mq2Pin); // Gaz Sensörü okunuyor.
  // Sensör değerlerini Serial monitöre yazdırma
  Serial.print("Ateş Tespiti (Dijital): ");
  Serial.println(flameValDigital);
  Serial.print("Ateş Yoğunluğu (Analog): ");
  Serial.println(flameValAnalog);
  Serial.print("Gaz Değeri: ");
  Serial.println(mq2Val);
  // Eğer alev tespit edilirse veya gaz seviyesi eşiği aşarsa
  if (flameValDigital == LOW || mq2Val > gasThreshold) {
    digitalWrite(ledPin, HIGH); // LED'i yak.
    digitalWrite(buzzerPin, HIGH); // Buzzer'ı çalıştır.
    if (flameValDigital == LOW) {
      digitalWrite(waterPumpPin, HIGH); // Ateş algılandığında su pompasını başlat
      delay(15000); // 15 saniye bekle
      digitalWrite(waterPumpPin, LOW); // Su pompasını durdur
    }
    if (mq2Val > gasThreshold) {
      digitalWrite(fanPin, HIGH); // Gaz algılandığında pervaneyi başlat
      delay(15000); // 15 saniye bekle
      digitalWrite(fanPin, LOW); // Pervaneyi durdur
    }
    delay(100);
    digitalWrite(buzzerPin, LOW); // Buzzer'ı durdur.
    digitalWrite(ledPin, LOW); // LED'i söndür.
  } 
  else {
    digitalWrite(buzzerPin, LOW); // Buzzer'ı durdur.
    digitalWrite(ledPin, LOW); // LED'i söndür.
    digitalWrite(fanPin, LOW); // Pervaneyi durdur.
    digitalWrite(waterPumpPin, LOW); // Su pompasını durdur.
  }
  delay(100);
}