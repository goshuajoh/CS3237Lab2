#include <Wire.h>

uint8_t address = 0x39; // The I2C address for APDS9960
uint8_t MSByte = 0, LSByte = 0;
uint16_t regValue = 0;

void setup() {
  Wire.begin();
  Serial.begin(9600);

  // Enable Power and ALS (0x80 is the ENABLE register, 0x03 enables PON and AEN)
  Wire.beginTransmission(address);
  Wire.write(0x80);
  Wire.write(0x03);
  Wire.endTransmission();
  
  delay(500);
}

uint16_t readRegister(uint8_t reg) {
  Wire.beginTransmission(address);
  Wire.write(reg);
  Wire.endTransmission();
  
  Wire.requestFrom(address, 1);
  if(Wire.available()){
    return Wire.read();
  }
  return 0;
}

void loop() {
  uint16_t clear = (readRegister(0x95) << 8) | readRegister(0x94);
  uint16_t red = (readRegister(0x97) << 8) | readRegister(0x96);
  uint16_t green = (readRegister(0x99) << 8) | readRegister(0x98);
  uint16_t blue = (readRegister(0x9B) << 8) | readRegister(0x9A);

  Serial.print("Clear: "); Serial.println(clear);
  Serial.print("Red: "); Serial.println(red);
  Serial.print("Green: "); Serial.println(green);
  Serial.print("Blue: "); Serial.println(blue);

  delay(1000); // Wait 1 second before next reading
}
