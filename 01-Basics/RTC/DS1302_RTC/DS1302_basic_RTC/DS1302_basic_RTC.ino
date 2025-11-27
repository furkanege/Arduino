/*
  Project: DS1302 Real-Time Clock Basic Usage
  File: DS1302_basic_RTC.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/26/2025

  Description:
    Demonstrates initialization, configuration, and real-time reading from the
    DS1302 RTC module using the RtcDS1302 and ThreeWire libraries. Automatically
    loads compile-time (date & time) into the RTC if needed and prints the
    current timestamp every 5 seconds.

  Wiring (DS1302 Module):
    DS1302 DAT → D4
    DS1302 CLK → D5
    DS1302 RST → D2
    VCC → 5V
    GND → GND
    (CR2032 battery recommended for backup)

  Libraries:
    - ThreeWire.h
    - RtcDS1302.h

  Serial Baud:
    9600

  Example Serial Output:
    compiled: Nov 26 2025  21:14:03
    RTC is newer than compile time (expected).
    11/26/2025 21:14:05

  Notes:
    - Ensure CR2032 battery is installed for long-term timekeeping.
    - DS1302 supports only seconds resolution (no milliseconds).
    - Module accuracy varies; periodic manual time sync recommended.
    - Write protection must be disabled to set new time.
    - DS1302 is less accurate than DS3231 but widely used and simple.

  Real-World Applications:
    - Clock displays
    - Data loggers
    - Sensor timestamping
    - Time-based automation

  License: GPL-3.0
*/

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