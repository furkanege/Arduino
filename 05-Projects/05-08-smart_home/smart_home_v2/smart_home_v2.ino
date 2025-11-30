/*
  Project: Smart Home Monitor – v2
  File: smart_home_v2.ino
  Author: Furkan Ege
  Board: Arduino Nano
  Version: 1.0
  Last Updated: 11/29/2025

  Description:
    Version 2 of the Smart Home Monitor system.
    This build focuses on field-stable UI and event-based alarms
    with manual page navigation via button.

    Features:
      - AHT10 (0x38) temperature & humidity via raw I2C read
      - BMP280 sensor for atmospheric pressure (hPa)
      - MQ (analog) air quality indicator with baseline-calibrated % mapping
      - MPU6050 (0x68) motion detection (orientation-independent / high-pass)
      - OLED SSD1306 UI with 4 manual pages (button cycle)
      - Buzzer alerts with event priority (shake > air danger)
      - Stabilized readings on-screen (EMA)

  Display Pages:
    PAGE 0 → TEMP:  C and F side-by-side + Humidity (%)
    PAGE 1 → PRESSURE: hPa (altitude removed)
    PAGE 2 → AIR PARTICLE: % (baseline-based) + status + alarm flag
    PAGE 3 → MOTION: stabilized X/Y/Z + shake YES/NO

  Wiring:
    AHT10 (0x38):
      VCC → 5V
      GND → GND
      SDA → A4
      SCL → A5

    BMP280 (0x76):
      VCC → 3.3V
      GND → GND
      SDA → A4
      SCL → A5

    MPU6050 (0x68):
      VCC → 5V
      GND → GND
      SDA → A4
      SCL → A5

    OLED SSD1306 (0x3C):
      VCC → 5V
      GND → GND
      SDA → A4
      SCL → A5

    MQ Sensor:
      AOUT → A0
      VCC  → 5V
      GND  → GND

    Button:
      D11  → Button → GND  (INPUT_PULLUP)

    Buzzer:
      D12  → + (signal)
      GND  → -

  Libraries:
    - Wire.h
    - Adafruit_SSD1306.h
    - Adafruit_GFX.h   (Dependency of SSD1306)
    - Adafruit_BMP280.h
    - math.h
    - string.h

  Serial Baud:
    9600

  Real-World Applications:
    - Home environmental awareness
    - Air quality trend monitoring (relative %)
    - Motion/shake event monitoring
    - STEM education projects
    - Sensor fusion & UI development baseline

  License: GPL-3.0
*/

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Adafruit_BMP280.h>
#include <math.h>
#include <string.h>
#define SCREEN_W 128
#define SCREEN_H 64

Adafruit_SSD1306 display(SCREEN_W, SCREEN_H, &Wire, -1);
Adafruit_BMP280 bmp;

#define MQ_PIN        A0
#define BTN_PIN       11
#define BUZZ_PIN      12
#define AHT10_ADDR    0x38
#define MPU6050_ADDR  0x68

// ---------- UI ----------
byte page = 0;
bool lastBtnState = HIGH;
unsigned long lastBtnMs = 0;
const unsigned long BTN_DEBOUNCE_MS = 35;

// ---------- Sensors presence ----------
bool hasAHT = false;
bool hasBMP = false;
bool hasMPU = false;

// ---------- AHT10 values ----------
float tempC = 25.0f;
float humP  = 50.0f;

// ---------- BMP280 values ----------
float pressHpa = 1013.0f;

// ---------- MQ baseline -> % ----------
int mq = 0;
int mqBase = 0;                  // 0%
int mqDirty = 0;                 // 100%
const int MQ_SPAN = 250;          // hassasiyet (küçük => daha hassas)
const int MQ_FLOOR_PCT = 1;       // %0 göstermeyi yasakla

// ---------- MPU6050 raw + stabilized ----------
int16_t ax = 0, ay = 0, az = 0;
static bool accInited = false;
static float axE = 0, ayE = 0, azE = 0;
const float ACC_EMA_ALPHA = 0.10f;  // daha stabil

// ---------- Motion: orientation-independent (high-pass magnitude) ----------
static float magE = 0;
static float dynE = 0;
static float noiseE = 0;
const float MAG_ALPHA   = 0.02f;
const float DYN_ALPHA   = 0.25f;
const float NOISE_ALPHA = 0.01f;
const int   MIN_QUAKE_COUNTS = 70; // daha hassas için düşür (ör. 50)
const float NOISE_MULT = 5.0f;     // false positive artarsa yükselt (ör. 6-7)
unsigned long shakeUntilMs = 0;
const unsigned long SHAKE_HOLD_MS = 300;

static inline float clampf(float v, float lo, float hi) {
  if (v < lo) return lo;
  if (v > hi) return hi;
  return v;
}

static inline void buzzerOff() { noTone(BUZZ_PIN); }
static inline bool isShakeActive() { return millis() < shakeUntilMs; }

// Default font: 6px per char (5+1), 8px height.
static inline void printCenter(const char* text, int y, uint8_t size) {
  display.setTextSize(size);
  int16_t x = (SCREEN_W - (int)strlen(text) * 6 * size) / 2;
  if (x < 0) x = 0;
  display.setCursor(x, y);
  display.print(text);
}

bool checkI2C(byte addr) {
  Wire.beginTransmission(addr);
  return (Wire.endTransmission() == 0);
}

// ---------- AHT10 (raw I2C) ----------
void readAHT10() {
  Wire.beginTransmission(AHT10_ADDR);
  Wire.write(0xAC);
  Wire.write(0x33);
  Wire.write(0x00);
  Wire.endTransmission();
  delay(80);

  Wire.requestFrom(AHT10_ADDR, 6);
  if (Wire.available() != 6) return;

  byte data[6];
  for (int i = 0; i < 6; i++) data[i] = Wire.read();

  if (data[0] & 0x80) return; // busy

  uint32_t h_raw = ((uint32_t)data[1] << 12) |
                   ((uint32_t)data[2] << 4)  |
                   (data[3] >> 4);

  uint32_t t_raw = (((uint32_t)data[3] & 0x0F) << 16) |
                   ((uint32_t)data[4] << 8)          |
                   data[5];

  humP  = (h_raw * 100.0f) / 1048576.0f;
  tempC = ((t_raw * 200.0f) / 1048576.0f) - 50.0f;

  humP  = clampf(humP,  0.0f, 100.0f);
  tempC = clampf(tempC, -40.0f, 85.0f);
}

// ---------- MPU6050 ----------
void initMPU6050() {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x6B); // PWR_MGMT_1
  Wire.write(0);    // wake
  Wire.endTransmission();
}

void readMPU6050Raw() {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x3B); // ACCEL_XOUT_H
  Wire.endTransmission();

  Wire.requestFrom(MPU6050_ADDR, 6);
  if (Wire.available() == 6) {
    ax = (Wire.read() << 8) | Wire.read();
    ay = (Wire.read() << 8) | Wire.read();
    az = (Wire.read() << 8) | Wire.read();
  }
}

void updateMotion() {
  if (!hasMPU) return;

  readMPU6050Raw();

  if (!accInited) {
    axE = ax; ayE = ay; azE = az;
    magE = sqrtf((float)ax * ax + (float)ay * ay + (float)az * az);
    dynE = 0; noiseE = 0;
    accInited = true;
    return;
  }

  // Stable XYZ for display
  axE = (1.0f - ACC_EMA_ALPHA) * axE + ACC_EMA_ALPHA * (float)ax;
  ayE = (1.0f - ACC_EMA_ALPHA) * ayE + ACC_EMA_ALPHA * (float)ay;
  azE = (1.0f - ACC_EMA_ALPHA) * azE + ACC_EMA_ALPHA * (float)az;

  // Orientation-independent vibration
  float mag = sqrtf((float)ax * ax + (float)ay * ay + (float)az * az);

  magE = (1.0f - MAG_ALPHA) * magE + MAG_ALPHA * mag;
  float dyn = fabsf(mag - magE);            // high-pass
  dynE = (1.0f - DYN_ALPHA) * dynE + DYN_ALPHA * dyn;

  if (!isShakeActive()) {
    noiseE = (1.0f - NOISE_ALPHA) * noiseE + NOISE_ALPHA * dynE;
  }

  float th = noiseE * NOISE_MULT;
  if (th < (float)MIN_QUAKE_COUNTS) th = (float)MIN_QUAKE_COUNTS;

  if (dynE > th) {
    shakeUntilMs = millis() + SHAKE_HOLD_MS;
    tone(BUZZ_PIN, 1200, 160); // event beep
  }
}

// ---------- MQ baseline calibration ----------
void calibrateMQBaseline() {
  long sum = 0;
  for (int i = 0; i < 220; i++) {
    sum += analogRead(MQ_PIN);
    delay(5);
  }
  mqBase = (int)(sum / 220);
  mqDirty = constrain(mqBase + MQ_SPAN, mqBase + 1, 1023);
}

int calcAirPct(int raw) {
  if (mqDirty <= mqBase) return MQ_FLOOR_PCT;
  raw = constrain(raw, mqBase, mqDirty);
  long pct = (long)(raw - mqBase) * 100L / (long)(mqDirty - mqBase);
  int ip = (int)constrain((int)pct, 0, 100);
  if (ip == 0) ip = MQ_FLOOR_PCT;
  return ip;
}

// ================================
// Setup
// ================================
void setup() {
  Serial.begin(9600);
  Wire.begin();

  pinMode(BTN_PIN, INPUT_PULLUP);
  pinMode(BUZZ_PIN, OUTPUT);
  digitalWrite(BUZZ_PIN, LOW);

  if (display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    display.setTextColor(SSD1306_WHITE);
    display.setTextWrap(false); // TEMP taşmasında alta kaymayı engeller

    display.clearDisplay();
    printCenter("SMART", 0, 2);
    printCenter("HOME", 16, 2);
    printCenter("by FURKAN EGE", 34, 1);
    printCenter("2025", 44, 1);
    printCenter("Boston, MA", 54, 1);
    display.display();
    delay(1600);
  }

  hasAHT = checkI2C(AHT10_ADDR);
  hasBMP = bmp.begin(0x76);

  hasMPU = checkI2C(MPU6050_ADDR);
  if (hasMPU) initMPU6050();

  calibrateMQBaseline();
}

// ================================
// Loop
// ================================
void loop() {
  // Button page cycle (debounced edge)
  bool btn = digitalRead(BTN_PIN);
  unsigned long now = millis();
  if (btn != lastBtnState && (now - lastBtnMs) > BTN_DEBOUNCE_MS) {
    lastBtnMs = now;
    if (btn == LOW) page = (page + 1) % 4;
    lastBtnState = btn;
  }

  // Read sensors
  if (hasAHT) readAHT10();
  if (hasBMP) pressHpa = bmp.readPressure() / 100.0f;

  mq = analogRead(MQ_PIN);
  updateMotion();

  // UI
  display.clearDisplay();

  // -------- PAGE 0: TEMP --------
  if (page == 0) {
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println(F("TEMP"));

    if (hasAHT) {
      int tC = (int)roundf(tempC);
      int tF = (int)roundf((tempC * 9.0f / 5.0f) + 32.0f);

      char buf[20];
      int n = snprintf(buf, sizeof(buf), "%dC %dF", tC, tF);

      // Uzarsa size 2'ye düş, asla satır kaydırma yok (wrap kapalı)
      if (n <= 7) printCenter(buf, 22, 3);
      else        printCenter(buf, 26, 2);

      display.setTextSize(1);
      display.setCursor(0, 56);
      display.print(F("Humidity: "));
      display.print(humP, 1);
      display.print(F("%"));
    } else {
      display.setTextSize(2);
      printCenter("NO SENSOR", 28, 2);
    }

    if (!isShakeActive()) buzzerOff();
  }

  // -------- PAGE 1: PRESSURE (ALTITUDE REMOVED) --------
  else if (page == 1) {
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println(F("PRESSURE"));

    if (hasBMP) {
      char pbuf[16];
      dtostrf(pressHpa, 0, 1, pbuf);
      printCenter(pbuf, 22, 3);
      printCenter("hPa", 50, 1);
    } else {
      display.setTextSize(2);
      printCenter("NO SENSOR", 28, 2);
    }

    if (!isShakeActive()) buzzerOff();
  }

  // -------- PAGE 2: AIR PARTICLE (%) --------
  else if (page == 2) {
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println(F("AIR"));

    display.setTextSize(1);
    display.setCursor(0, 16);
    display.println(F("PARTICLE"));

    int airPct = calcAirPct(mq);

    char abuf[8];
    snprintf(abuf, sizeof(abuf), "%d%%", airPct);
    printCenter(abuf, 26, 3);

    bool alarm = false;

    display.setTextSize(2);
    display.setCursor(0, 50);
    if (airPct < 35)       display.print(F("CLEAN"));
    else if (airPct < 60)  display.print(F("MED"));
    else if (airPct < 80)  display.print(F("DIRTY"));
    else { display.print(F("DANGER")); alarm = true; }

    display.setTextSize(1);
    display.setCursor(84, 0);
    display.print(F("V:"));
    display.print(airPct);
    display.print(F("%"));

    display.setCursor(84, 10);
    display.print(F("Alarm:"));
    display.print(alarm ? F("Y") : F("N"));

    // Buzzer priority: shake > air alarm
    if (!isShakeActive()) {
      if (alarm) tone(BUZZ_PIN, 1500, 180);
      else buzzerOff();
    }
  }

  // -------- PAGE 3: MOTION --------
  else {
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println(F("MOTION"));

    if (hasMPU && accInited) {
      display.setTextSize(1);
      display.setCursor(0, 18);
      display.print(F("X: ")); display.println((int)axE);
      display.setCursor(0, 30);
      display.print(F("Y: ")); display.println((int)ayE);
      display.setCursor(0, 42);
      display.print(F("Z: ")); display.println((int)azE);

      bool shake = isShakeActive();
      display.setTextSize(2);
      display.setCursor(20, 50);
      display.println(shake ? F("YES!") : F("NO"));

      if (!shake) buzzerOff();
    } else {
      display.setTextSize(2);
      printCenter("NO SENSOR", 28, 2);
      buzzerOff();
    }
  }

  display.display();
  delay(60);
}