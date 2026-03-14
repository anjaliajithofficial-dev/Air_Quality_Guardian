#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <math.h>

// -------------------- Pin Definitions --------------------
#define DHTPIN 4        // GPIO4 for DHT11
#define MQ135_PIN 34    // GPIO34 for MQ135 (Analog Input)

// -------------------- DHT Sensor --------------------
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// -------------------- LCD --------------------
LiquidCrystal_I2C lcd(0x27, 16, 2); // 16x2 LCD with I2C

// -------------------- MQ135 Calibration --------------------
#define RLOAD 10.0
#define RZERO 76.63
#define PARA 116.6020682
#define PARB 2.769034857

// -------------------- Variables --------------------
float temperature = 0;    // Last valid temperature
float humidity = 0;       // Last valid humidity
float air_quality = 0;    // PPM value from MQ135

// -------------------- Setup --------------------
void setup() {
  Serial.begin(115200);

  // Initialize DHT sensor
  dht.begin();

  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Configure MQ135 pin
  pinMode(MQ135_PIN, INPUT);

  // Startup message
  lcd.setCursor(0, 0);
  lcd.print("ESP32 Sensor");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
}

// -------------------- Loop --------------------
void loop() {
  readDHT11();    // Read temperature & humidity
  readMQ135();    // Read air quality

  displayOnLCD(); // Show values on LCD
  displaySerial(); // Print values to Serial Monitor

  delay(2000);    // Wait 2 seconds between readings
}

// -------------------- Read DHT11 --------------------
void readDHT11() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Retry once if failed
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor! Retrying...");
    delay(1000);
    h = dht.readHumidity();
    t = dht.readTemperature();
  }

  // Update values if successful
  if (!isnan(h)) humidity = h;
  if (!isnan(t)) temperature = t;

  // If still failed, keep last valid values
  if (isnan(h) || isnan(t)) {
    Serial.println("DHT read failed again, keeping last valid values.");
  }
}

// -------------------- Read MQ135 --------------------
void readMQ135() {
  int sensorValue = analogRead(MQ135_PIN);

  // Convert to voltage (ESP32 ADC is 12-bit, 0-4095)
  float voltage = sensorValue * (3.3 / 4095.0);

  // Calculate sensor resistance
  float rs = (3.3 - voltage) / voltage * RLOAD;

  // Approximate ppm for CO2
  air_quality = PARA * pow((rs / RZERO), -PARB);
}

// -------------------- Display on LCD --------------------
void displayOnLCD() {
  lcd.clear();

  // Line 1: Temperature & Humidity
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperature, 1);
  lcd.print("C H:");
  lcd.print(humidity, 0);
  lcd.print("%");

  // Line 2: Air Quality
  lcd.setCursor(0, 1);
  lcd.print("AQ:");
  lcd.print(air_quality, 0);
  lcd.print(" PPM");
}

// -------------------- Display on Serial --------------------
void displaySerial() {
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.print(" %, Air Quality: ");
  Serial.print(air_quality);
  Serial.println(" PPM");
}