// ------------------------------------------------------------
// DHT11 Basic Test
// Reads humidity and temperature from DHT11 sensor
// ------------------------------------------------------------

#include <DHT.h>
#define DHTPIN 2         // Digital pin where DHT11 is connected
#define DHTTYPE DHT11    // Sensor type
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println("DHT11 Sensor Test");
  dht.begin();           // Initialize the sensor
}

void loop() {
  delay(2000);           // DHT11 requires 2 seconds between readings
  float humidity = dht.readHumidity();        // Humidity (%)
  float temperature = dht.readTemperature();  // Temperature (Celsius)
  // Check for failed readings
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Error: Failed to read from DHT11 sensor!");
    return;
  }

  // --- Serial Output ---
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%   |   Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
}