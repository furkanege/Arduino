/*
  Project: DHT11 Temperature & Humidity Monitor (Serial Output)
  File: dht_11_console.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/26/2025

  Description:
    Reads temperature and humidity values from the DHT11 sensor and prints
    them to the Serial Monitor. Demonstrates simple environmental data
    acquisition for IoT, logging and robotics systems.

  Wiring:
    DHT11 Signal → D2
    VCC → 5V
    GND → GND

  Libraries:
    - DHT Sensor Library by Adafruit
    - Adafruit Unified Sensor (recommended)

  Serial Baud:
    9600

  Example Serial Output:
    Temperature: 24 °C
    Humidity: 56 %

  Notes:
    - DHT11 should not be read more often than once every ~1 second.
    - Typical sensor accuracy: ±2°C, ±5% RH.
    - Use a pull-up resistor on data line if module does not include one.

  Real-World Applications:
    - Smart home temperature/humidity monitoring
    - HVAC systems
    - IoT data logging
    - Weather stations

  License: GPL-3.0
*/


#include <DHT.h>
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  delay(1000);
}