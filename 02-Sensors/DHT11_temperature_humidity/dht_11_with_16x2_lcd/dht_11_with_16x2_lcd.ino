// ------------------------------------------------------------
// DHT11 Temperature & Humidity LCD Display
// ------------------------------------------------------------

#include <DHT.h>                  // DHT sensor library
#include <LiquidCrystal_I2C.h>    // I2C LCD library
#define DHTPIN 2                  // Pin where DHT11 signal pin is connected
#define DHTTYPE DHT11             // Sensor model: DHT11

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C LCD (address 0x27, 16 columns, 2 rows)

void setup() {
  Serial.begin(9600);
  Serial.println("DHT11 Temperature & Humidity Monitor");
  lcd.init();          // Initialize LCD (correct method)
  lcd.backlight();     // Turn on backlight
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp/Humidity");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(1500);
  dht.begin();         // Start DHT sensor
}

void loop() {
  float temp = dht.readTemperature();     // Celsius
  float hum  = dht.readHumidity();        // Percentage
  // Handle sensor error
  if (isnan(temp) || isnan(hum)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error!");
    lcd.setCursor(0, 1);
    lcd.print("Check wiring!");
    Serial.println("DHT11 read error!");
    delay(1000);
    return;
  }

  // ---- SERIAL OUTPUT ----
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" C");
  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.println(" %");
  // ---- LCD OUTPUT ----
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.print((char)223);  // Degree symbol
  lcd.print("C   ");
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(hum);
  lcd.print("%   ");
  delay(800);  // Smooth refresh
}