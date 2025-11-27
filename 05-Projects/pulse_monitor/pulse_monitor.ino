// ------------------------------------------------------------
// Pulse Sensor with OLED BPM Display
// Cleaned and documented version (English comments)
// Original project by Furkan Ege
// ------------------------------------------------------------
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// ---------------- OLED CONFIG ----------------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// ---------------- TIMING VARIABLES ----------------
unsigned long previousMillisGetHR = 0;
unsigned long previousMillisResultHR = 0;
const long intervalGetHR = 20;      // Pulse sampling interval
const long intervalResultHR = 10000; // BPM refresh interval (10 sec)
// ---------------- PULSE SENSOR CONFIG ----------------
int PulseSensorSignal;
const int PulseSensorPin = A0;
const int LEDFeedbackPin = A1;
int UpperThreshold = 520;   // Heartbeat detection upper threshold
int LowerThreshold = 510;   // Heartbeat detection lower threshold
int heartbeatCount = 0;
bool thresholdState = true;
int BPMValue = 0;
// ---------------- GRAPH VARIABLES ----------------
int x = 0;
int y = 0;
int lastX = 0;
int lastY = 0;
// ---------------- HEART ICON ----------------
const unsigned char Heart_Icon [] PROGMEM = {
  0x00, 0x00, 0x18, 0x30, 0x3c, 0x78, 0x7e, 0xfc, 0xff, 0xfe, 0xff, 0xfe, 0xee, 0xee, 0xd5, 0x56, 
  0x7b, 0xbc, 0x3f, 0xf8, 0x1f, 0xf0, 0x0f, 0xe0, 0x07, 0xc0, 0x03, 0x80, 0x01, 0x00, 0x00, 0x00
};
// ------------------------------------------------------------
// SETUP
// ------------------------------------------------------------
void setup() {
  pinMode(LEDFeedbackPin, OUTPUT);
  Serial.begin(9600);
  // Initialize OLED Display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Infinite loop if OLED fails
  }
  display.display();
  delay(1000);
  // Welcome Screen
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
  // Initial BPM Screen
  display.clearDisplay();
  display.drawBitmap(0, 47, Heart_Icon, 16, 16, WHITE);
  display.drawLine(0, 43, 127, 43, WHITE);
  display.setTextSize(2);
  display.setCursor(20, 48);
  display.print(": BPM 0");
  display.display();
  Serial.println("Pulse measurement starting...");
}
// ------------------------------------------------------------
// LOOP
// ------------------------------------------------------------
void loop() {
  GetHeartRate();
}
// ------------------------------------------------------------
// HEART RATE READER
// ------------------------------------------------------------
void GetHeartRate() {
  unsigned long currentMillisGetHR = millis();
  // ---------------- SAMPLE PULSE SIGNAL ----------------
  if (currentMillisGetHR - previousMillisGetHR >= intervalGetHR) {
    previousMillisGetHR = currentMillisGetHR;
    PulseSensorSignal = analogRead(PulseSensorPin);
    // Rising edge detection
    if (PulseSensorSignal > UpperThreshold && thresholdState == true) {
      heartbeatCount++;
      thresholdState = false;
      digitalWrite(LEDFeedbackPin, HIGH);
    }
    // Reset state
    if (PulseSensorSignal < LowerThreshold) {
      thresholdState = true;
      digitalWrite(LEDFeedbackPin, LOW);
    }
    DrawGraph();
  }
  // ---------------- UPDATE BPM EVERY 10 SECONDS ----------------
  unsigned long currentMillisResultHR = millis();
  if (currentMillisResultHR - previousMillisResultHR >= intervalResultHR) {
    previousMillisResultHR = currentMillisResultHR;
    BPMValue = heartbeatCount * 6; // Because 10s * 6 = 60s
    heartbeatCount = 0;
    Serial.print("BPM: ");
    Serial.println(BPMValue);
    // Update OLED
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
// ------------------------------------------------------------
// GRAPH DISPLAY
// ------------------------------------------------------------
void DrawGraph() {
  if (x > 127) {
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