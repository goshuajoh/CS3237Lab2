#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "ssid";  //I almost gave everyone free wifi
const char* password = "password";  //I almost leaked my password here

WebServer server(80);

const int led = 13;  //GPIO 13 for LED
bool ledState = LOW;  //LED State

void handleRoot() {
  digitalWrite(led, HIGH);
  server.send(200, "text/plain", "Hello from ESP32!");
  digitalWrite(led, LOW);
}

void handleToggleLED() {
  ledState = !ledState;
  digitalWrite(led, ledState);

  String message = (ledState == HIGH) ? "LED On" : "LED Off";
  
  server.send(200, "text/plain", message);
}

void handleNotFound() {
  digitalWrite(led, HIGH);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, LOW);
}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "This works as well");
  });

  server.on("/toggleLED", handleToggleLED);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  delay(2);
}
