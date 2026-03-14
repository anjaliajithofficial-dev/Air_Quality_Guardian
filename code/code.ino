#include <Wire.h>

#include <LiquidCrystal_I2C.h>

#include <DHT.h>


// Pin Definitions

#define DHTPIN 4       // GPIO4 for DHT11

#define MQ135_PIN 34   // GPIO34 for MQ135 (Analog Input)


// DHT Sensor Type

#define DHTTYPE DHT11


// Initialize DHT sensor

DHT dht(DHTPIN, DHTTYPE);


// Initialize LCD (0x27 is common I2C address, change if needed)

LiquidCrystal_I2C lcd(0x27, 16, 2); // 16x2 display


// Calibration for MQ135

#define RLOAD 10.0

#define RZERO 76.63

#define PARA 116.6020682

#define PARB 2.769034857


// Variables

float temperature = 0;

float humidity = 0;

float air_quality = 0;


void setup() {

  Serial.begin(115200);

 

  // Initialize DHT sensor

  dht.begin();

 

  // Initialize LCD

  lcd.init();

  lcd.backlight();

 

  // Configure MQ135 pin

  pinMode(MQ135_PIN, INPUT);

 

  // Display startup message

  lcd.setCursor(0, 0);

  lcd.print("ESP32 Sensor");

  lcd.setCursor(0, 1);

  lcd.print("Initializing...");

  delay(2000);

  lcd.clear();
Serial.println("End of setup");
}


void loop() {

  // Read DHT11 values

  readDHT11();

 Serial.println("DHT Data read");

  // Read MQ135 values

  readMQ135();

 Serial.println("Air sensor ");

  // Display on LCD

  displayOnLCD();

 Serial.println("LCD ");

  // Display on Serial Monitor for debugging

  displaySerial();

 

  delay(2000); // Wait 2 seconds between readings

}


void readDHT11() {

  // Reading temperature or humidity takes about 250 milliseconds!

  humidity = dht.readHumidity();

  temperature = dht.readTemperature();

 

  // Check if any reads failed

  if (isnan(humidity) || isnan(temperature)) {

    Serial.println("Failed to read from DHT sensor!");

    humidity = 0;

    temperature = 0;

  }

}


void readMQ135() {

  // Read analog value from MQ135

  int sensorValue = analogRead(MQ135_PIN);

 

  // Convert to voltage (ESP32 ADC is 12-bit, 0-4095)

  float voltage = sensorValue * (3.3 / 4095.0);

 

  // Calculate resistance of the sensor

  float rs = (3.3 - voltage) / voltage * RLOAD;

 

  // Calculate ppm (approximate for CO2)

  air_quality = PARA * pow((rs / RZERO), -PARB);

 

  // Alternative simpler calculation (for general air quality indication)

  // air_quality = map(sensorValue, 0, 4095, 0, 100);

}


void displayOnLCD() {

  // Clear LCD

  lcd.clear();

 

  // First line: Temperature and Humidity

  lcd.setCursor(0, 0);

  lcd.print("T:");

  lcd.print(temperature, 1);

  lcd.print("C H:");

  lcd.print(humidity, 0);

  lcd.print("%");

 

  // Second line: Air Quality

  lcd.setCursor(0, 1);

  lcd.print("AQ:");

  lcd.print(air_quality, 0);

  lcd.print(" PPM");

}


void displaySerial() {

  Serial.print("Temperature: ");

  Serial.print(temperature);

  Serial.print(" °C, Humidity: ");

  Serial.print(humidity);

  Serial.print(" %, Air Quality: ");

  Serial.print(air_quality);

  Serial.println(" PPM");

}