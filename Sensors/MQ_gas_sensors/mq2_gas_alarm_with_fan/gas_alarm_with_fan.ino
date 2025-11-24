int mq2Pin = A0; // MQ2 Gaz Sensörü
int ledPin = 13; // LED
int buzzerPin = 12; // Buzzer
int fanPin = 11; // Pervane

int mq2Val; // Gaz Sensörü değeri

int gasThreshold = 100; // Gaz eşik değeri

void setup() {
  pinMode(mq2Pin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(fanPin, OUTPUT); // Pervane için çıkış ayarlaması
  Serial.begin(9600); // Serial monitör başlatılıyor.
}

void loop() {
  mq2Val = analogRead(mq2Pin); // Gaz Sensörü okunuyor.

  // Sensör değerlerini Serial monitöre yazdırma
  Serial.print("Gaz Değeri: ");
  Serial.println(mq2Val);
  
  // Eğer gaz seviyesi eşiği aşarsa
  if (mq2Val > gasThreshold) {
    digitalWrite(ledPin, HIGH); // LED'i yak.
    digitalWrite(buzzerPin, HIGH); // Buzzer'ı çalıştır.
    digitalWrite(fanPin, HIGH); // Gaz algılandığında pervaneyi başlat
    delay(5000); // 5 saniye bekle
    digitalWrite(fanPin, LOW); // Pervaneyi durdur
    delay(100);
    digitalWrite(buzzerPin, LOW); // Buzzer'ı durdur.
    digitalWrite(ledPin, LOW); // LED'i söndür.
  } 
  else {
    digitalWrite(buzzerPin, LOW); // Buzzer'ı durdur.
    digitalWrite(ledPin, LOW); // LED'i söndür.
    digitalWrite(fanPin, LOW); // Pervaneyi durdur.
  }
  delay(100);
}




int mq2Pin = A0; // MQ2 Gaz Sensörü
int ledPin = 13; // LED
int buzzerPin = 12; // Buzzer
int fanPin = 11; // Pervane
int mq2Val; // Gaz Sensörü değeri
int gasThreshold = 100; // Gaz eşik değeri

void setup() {
  pinMode(mq2Pin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(fanPin, OUTPUT); // Pervane için çıkış ayarlaması
  Serial.begin(9600); // Serial monitör başlatılıyor.
}

void loop() {
  mq2Val = analogRead(mq2Pin); // Gaz Sensörü okunuyor.
  Serial.print("Gaz Değeri: ");
  Serial.println(mq2Val);
    if (mq2Val > gasThreshold) {
    digitalWrite(ledPin, HIGH); // LED'i yak.
    digitalWrite(buzzerPin, HIGH); // Buzzer'ı çalıştır.
    digitalWrite(fanPin, HIGH); // Gaz algılandığında pervaneyi başlat
    delay(5000); // 5 saniye bekle
    digitalWrite(fanPin, LOW); // Pervaneyi durdur
    delay(100);
    digitalWrite(buzzerPin, LOW); // Buzzer'ı durdur.
    digitalWrite(ledPin, LOW); // LED'i söndür.
  } 
  else {
    digitalWrite(buzzerPin, LOW); // Buzzer'ı durdur.
    digitalWrite(ledPin, LOW); // LED'i söndür.
    digitalWrite(fanPin, LOW); // Pervaneyi durdur.
  }
  delay(100);
}

