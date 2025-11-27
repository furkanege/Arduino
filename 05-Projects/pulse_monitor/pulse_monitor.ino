/*
  Project: Pulse Sensor – OLED BPM + Real-Time Graph Monitor
  File: pulse_monitor.ino
  Author: Furkan Ege
  Board: Arduino UNO R3
  Version: 1.0
  Last Updated: 11/27/2025

  Description:
    Advanced heart rate monitoring system using:
      - PulseSensor analog input
      - Real-time waveform graph (up to 128 px width)
      - BPM calculation every 10 seconds
      - Heartbeat icon + OLED UI
      - LED feedback pulse indicator

    Features:
      - Rising-edge detection via dual-threshold method
      - Two-timer architecture:
           * 20 ms sampling (50 Hz)
           * 10 sec BPM refresh window
      - OLED graph auto-clear and continuous drawing
      - Accurate BPM formula:
           BPM = heartbeats_in_10_seconds * 6

  Wiring:
    Pulse Sensor:
      Signal → A0
      VCC    → 5V
      GND    → GND

    LED (optional heartbeat indicator):
      + → A1
      - → GND

    OLED SSD1306 (128x64 I2C):
      SDA → A4
      SCL → A5
      VCC → 5V
      GND → GND

  Libraries:
    - SPI.h
    - Wire.h
    - Adafruit_GFX.h
    - Adafruit_SSD1306.h

  Serial Baud:
    9600

  Notes:
    - Upper/Lower threshold must be tuned per finger & sensor model.
    - Graph uses display.writeLine() to plot live waveform data.
    - OLED auto-clears top graph region when reaching right edge.

  Real-World Applications:
    - Student biomedical demonstrations
    - Portable health devices
    - Fitness/heart-rate monitors
    - Bio-signal visualization

  License: GPL-3.0
*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

unsigned long previousMillisGetHR = 0;
unsigned long previousMillisResultHR = 0;
const long intervalGetHR = 20;       // Sampling every 20ms (~50 Hz)
const long intervalResultHR = 10000; // BPM every 10 seconds
int PulseSensorSignal = 0;
const int PulseSensorPin = A0;
const int LEDFeedbackPin = A1;
int UpperThreshold = 520;
int LowerThreshold = 510;
int heartbeatCount = 0;
bool thresholdState = true;
int BPMValue = 0;
int x = 0;
int y = 0;
int lastX = 0;
int lastY = 0;

const unsigned char Heart_Icon [] PROGMEM = {
  0x00,0x00,0x18,0x30,0x3c,0x78,0x7e,0xfc,
  0xff,0xfe,0xff,0xfe,0xee,0xee,0xd5,0x56,
  0x7b,0xbc,0x3f,0xf8,0x1f,0xf0,0x0f,0xe0,
  0x07,0xc0,0x03,0x80,0x01,0x00,0x00,0x00
};

void setup() {
  pinMode(LEDFeedbackPin, OUTPUT);
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;) ;
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 12);
  display.print("     Please wait");
  display.setCursor(0, 22);
  display.print("  while measuring");
  display.setCursor(0, 32);
  display.print("   heart rate...");
  display.display();
  delay(3000);
  display.clearDisplay();
  display.drawBitmap(0, 47, Heart_Icon, 16, 16, WHITE);
  display.drawLine(0, 43, 127, 43, WHITE);
  display.setTextSize(2);
  display.setCursor(20, 48);
  display.print(": BPM 0");
  display.display();
  Serial.println("Pulse measurement starting...");
}

void loop() {
  GetHeartRate();
}

void GetHeartRate() {
  unsigned long currentMillisGetHR = millis();
  if (currentMillisGetHR - previousMillisGetHR >= intervalGetHR) {
    previousMillisGetHR = currentMillisGetHR;
    PulseSensorSignal = analogRead(PulseSensorPin);
    if (PulseSensorSignal > UpperThreshold && thresholdState == true) {
      heartbeatCount++;
      thresholdState = false;
      digitalWrite(LEDFeedbackPin, HIGH);
    }

    if (PulseSensorSignal < LowerThreshold) {
      thresholdState = true;
      digitalWrite(LEDFeedbackPin, LOW);
    }
    DrawGraph();
  }

  unsigned long currentMillisResultHR = millis();

  if (currentMillisResultHR - previousMillisResultHR >= intervalResultHR) {
    previousMillisResultHR = currentMillisResultHR;
    BPMValue = heartbeatCount * 6; // 10s window → multiply by 6
    heartbeatCount = 0;
    Serial.print("BPM: ");
    Serial.println(BPMValue);
    display.fillRect(20, 48, 108, 18, BLACK);
    display.drawBitmap(0, 47, Heart_Icon, 16, 16, WHITE);
    display.drawLine(0, 43, 127, 43, WHITE);
    display.setTextSize(2);
    display.setCursor(20, 48);
    display.print(": BPM ");
    display.print(BPMValue);
    display.display();
  }
}

void DrawGraph() {
  if (x > 127) {
    // Clear ONLY the graph area (top 43px)
    display.fillRect(0, 0, 128, 42, BLACK);
    x = 0;
    lastX = 0;
  }
  int ySignal = PulseSensorSignal;
  if (ySignal > 1200) ySignal = 1200;
  if (ySignal < 350)  ySignal = 350;
  int mappedY = map(ySignal, 350, 1200, 0, 80);
  y = 40 - mappedY;
  display.writeLine(lastX, lastY, x, y, WHITE);
  display.display();
  lastX = x;
  lastY = y;
  x++;
}