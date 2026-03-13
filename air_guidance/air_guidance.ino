//#include <DHTesp.h>
#include "DHT.h"
#define DHTPIN 4     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

//#define DHTPIN 3

int gasSensor = 1;
int buzzer = 18;

//DHTesp dht;

void setup() {
  Serial.begin(115200);
  pinMode(buzzer, OUTPUT);

 // dht.setup(DHTPIN, DHTesp::DHT22);
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature(); 
  int gasValue = analogRead(gasSensor);

  //TempAndHumidity data = dht.getTempAndHumidity();

  Serial.print("Gas Value: ");
  Serial.println(gasValue);

  Serial.print("Temperature: ");
  Serial.println(t);

  Serial.print("Humidity: ");
  Serial.println(h);
  

  if (gasValue > 2500) {
    tone(buzzer, 1000);
  } else {
    noTone(buzzer);
  }

  delay(2000);
}