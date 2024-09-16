#include <Wire.h>
#include <SparkFun_APDS9960.h>
#include <ESP32Servo.h>

SparkFun_APDS9960 apds = SparkFun_APDS9960();
Servo myservo;

int servoPin;

void setup() {
  Serial.begin(115200);

  // Initialize APDS-9960 (configure I2C and initial settings)
  if (apds.init()) {
    Serial.println("APDS-9960 initialization complete");
  } else {
    Serial.println("Something went wrong during APDS-9960 init!");
  }

  // Start running the APDS-9960 gesture sensor engine
  if (apds.enableGestureSensor(true)) {
    Serial.println("Gesture sensor is now running");
  } else {
    Serial.println("Something went wrong during gesture sensor init!");
  }
  servoPin = 18;
  myservo.setPeriodHertz(50);  // Standard 50 Hz for servos
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
// Function to move the servo to the specified position
void moveServoTo(int position) {
      myservo.write(position);
      delay(1000);  // Give the servo time to move to the new position
}
