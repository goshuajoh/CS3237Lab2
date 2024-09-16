#include <Wire.h>
#include <SparkFun_APDS9960.h>
#include <ESP32Servo.h>

SparkFun_APDS9960 apds = SparkFun_APDS9960();
Servo myservo;

int servoPin;

void setup() {
  Serial.begin(115200);
  if (apds.init()) {
    Serial.println("init");
  } else {
    Serial.println("Error 1!");
  }

  if (apds.enableGestureSensor(true)) {
    Serial.println("running");
  } else {
    Serial.println("Error 2!");
  }
  servoPin = 18;
  myservo.setPeriodHertz(50);
  myservo.attach(servoPin, 1000, 2000);
}

void loop() {
  if (apds.isGestureAvailable()) {
    switch (apds.readGesture()) {
      case DIR_UP:
        Serial.println("UP gesture detected");
        myservo.write(0);
        delay(1000);
        break;
      case DIR_DOWN:
        Serial.println("DOWN gesture detected");
        myservo.write(180);
        delay(1000);
        break;
      case DIR_LEFT:
        Serial.println("LEFT gesture detected");
        myservo.write(45);
        delay(1000);
        break;
      case DIR_RIGHT:
        Serial.println("RIGHT gesture detected");
        myservo.write(135);
        delay(1000);
        break;
      case DIR_NEAR:
        Serial.println("NEAR gesture detected");
        myservo.write(90);
        delay(1000);
        break;
      case DIR_FAR:
        Serial.println("FAR gesture detected");
        myservo.write(0);
        delay(1000);
        myservo.write(180);
        delay(1000);
        myservo.write(0);
        delay(1000);
        break;
      default:
        Serial.println("Unknown gesture");
        break;
    }
  }

  delay(100);
}
