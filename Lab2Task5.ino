#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include "DHT.h"  //Use DHT sensor library from Lab 1

WiFiMulti wifiMulti;

const char* ssid = "SSID";
const char* password = "Password!";

const char* laptopAt = "Ipaddress"; //Laptop IP address

// DHT sensor setup
#define DHTPIN 4     //conneted to GPIO pin 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {

    Serial.begin(115200);
    dht.begin();

    wifiMulti.addAP(ssid, password);

    while (wifiMulti.run() != WL_CONNECTED) {
      delay(500);
    }
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() == WL_CONNECTED){
    Serial.println("Sending...");

    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    Serial.print("Raw temperature: ");
    Serial.println(temperature);
    Serial.print("Raw humidity: ");
    Serial.println(humidity);

    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Failed to read!");
      return;
    }

    int temperatureInt = (int)temperature;
    int humidityInt = (int)humidity;

    HTTPClient http;

    String url = laptopAt;
    url += "/data?data1=" + String(temperatureInt) + "&data2=" + String(humidityInt);

    http.begin(url);
    int returnCode = http.GET();   //Perform GET request

    if (returnCode > 0){
      String payload = http.getString();
      Serial.println(payload);
    }
    http.end();

  } else {
    Serial.println("WiFi disconnected");
  }
  
  delay(5000);
}
