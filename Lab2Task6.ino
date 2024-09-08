#include <Arduino.h>
#include <SparkFun_APDS9960.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>

#define USE_SERIAL Serial

WiFiMulti wifiMulti;

SparkFun_APDS9960 apds = SparkFun_APDS9960();
const char* serverName = "http://192.168.0.1:3237/data"; // Use your computer's IP address

void setup() {
  USE_SERIAL.begin(115200);

  // Connect to WiFi
  wifiMulti.addAP("25LentorGoh", "Oct0b3r10!");

  // Wait for connection
  while (wifiMulti.run() != WL_CONNECTED) {
    delay(1000);
    USE_SERIAL.println("Connecting to WiFi...");
  }

  USE_SERIAL.println("Connected to WiFi!");

  // Initialize the gesture sensor
  if (apds.enableGestureSensor(true)) {
    Serial.println("Gesture sensor is now running");
  } else {
    Serial.println("Something went wrong during gesture sensor init!");
  }
}

void loop() {
  String gestureData = "";
  
  if (apds.isGestureAvailable()) {
    switch (apds.readGesture()) {
      case DIR_UP:
        Serial.println("UP gesture detected");
        gestureData = "UP";
        break;
      case DIR_DOWN:
        Serial.println("DOWN gesture detected");
        gestureData = "DOWN";
        break;
      case DIR_LEFT:
        Serial.println("LEFT gesture detected");
        gestureData = "LEFT";
        break;
      case DIR_RIGHT:
        Serial.println("RIGHT gesture detected");
        gestureData = "RIGHT";
        break;
      case DIR_NEAR:
        Serial.println("NEAR gesture detected");
        gestureData = "NEAR";
        break;
      case DIR_FAR:
        Serial.println("FAR gesture detected");
        gestureData = "FAR";
        break;
      default:
        Serial.println("Unknown gesture");
        gestureData = "UNKNOWN";
        break;
    }

    // Send gesture data to Flask server
    sendGestureData(gestureData);
  }

  delay(5000);  // Wait for 5 seconds before checking again
}

void sendGestureData(String gesture) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Build the request URL
    String serverPath = String(serverName) + "?gesture=" + gesture;

    http.begin(serverPath.c_str());
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Server Response: " + response);
    } else {
      Serial.println("Error in sending GET request");
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}
