#include <ThreeWire.h>
#include <RtcDS1302.h>

ThreeWire myWire(4, 5, 2);   // DAT, CLK, RST pin sırası
RtcDS1302<ThreeWire> Rtc(myWire);

void setup() {
  Serial.begin(9600);
  Serial.print("compiled: ");
  Serial.print(__DATE__);
  Serial.print(" ");
  Serial.println(__TIME__);
  Rtc.Begin();
  // Derleme tarihini RTC'ye yüklemek için
  RtcDateTime compiled(__DATE__, __TIME__);
  printDateTime(compiled);
  Serial.println();
  if (!Rtc.IsDateTimeValid()) {
    Serial.println("RTC lost confidence in the DateTime!");
    Rtc.SetDateTime(compiled);
  }
  if (Rtc.GetIsWriteProtected()) {
    Serial.println("RTC was write protected, enabling writing now");
    Rtc.SetIsWriteProtected(false);
  }
  if (!Rtc.GetIsRunning()) {
    Serial.println("RTC was not running, starting now");
    Rtc.SetIsRunning(true);
  }
  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled) {
    Serial.println("RTC is older than compile time! Updating...");
    Rtc.SetDateTime(compiled);
  } 
  else if (now > compiled) {
    Serial.println("RTC is newer than compile time (expected).");
  } 
  else {
    Serial.println("RTC is exactly the same as compile time!");
  }
}

void loop() {
  RtcDateTime now = Rtc.GetDateTime();
  printDateTime(now);
  Serial.println();
  if (!now.IsValid()) {
    Serial.println("RTC lost confidence in the DateTime!");
  }
  delay(5000);
}

void printDateTime(const RtcDateTime& dt) {
  char datestring[20];
  snprintf_P(datestring,
             sizeof(datestring),
             PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
             dt.Month(),
             dt.Day(),
             dt.Year(),
             dt.Hour(),
             dt.Minute(),
             dt.Second());
  Serial.print(datestring);
}