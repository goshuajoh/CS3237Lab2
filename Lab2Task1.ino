#include <ESP32Servo.h>

Servo myservo;

//different positions
const int LEFT = 0;
const int MIDDLE = 90;
const int RIGHT = 180;

int servoPin;
int state = 0;

#define BUTTON_PIN 4

volatile bool pressed = false;
volatile unsigned long lastPressTime = 0;
const unsigned long debounceDelay = 500;

// ISR to handle button press with debounce
void IRAM_ATTR handleButtonPress() {
    unsigned long currentTime = millis();
    if ((currentTime - lastPressTime) > debounceDelay) {
        lastPressTime = currentTime;
        pressed = true;  // Set flag to process in the loop
    }
}

void setup() {
    Serial.begin(115200);

    // Configure button pin
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(BUTTON_PIN, handleButtonPress, FALLING);  // Attach interrupt to button pin

    // Allow allocation of all timers
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);

    // Set up!
    myservo.setPeriodHertz(50);
    myservo.attach(18, 1000, 2000);
    myservo.write(MIDDLE);
}

void loop() {
    if (pressed) {
        pressed = false;  // Reset the pressed flag

        // Move the servo to the next position in sequence
        if (state == 0) {
          myservo.write(LEFT);
          delay(1000);
          state = 1;
        } 
        else if (state == 1) {
          myservo.write(MIDDLE);
          delay(1000);
          state = 2;
        } 
        else if (state == 2) {
          myservo.write(RIGHT);
          delay(1000);
          state = 0;
        }
    }
    delay(100);
}