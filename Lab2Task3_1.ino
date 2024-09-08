uint8_t address = 0x39; uint8_t MSByte = 0, LSByte = 0;
uint16_t regValue = 0;
#include <Wire.h>import smbus

def read_RGBC_data():
    bus = smbus.SMBus(1)  # Use appropriate bus number
    i2c_address = 0x39  # APDS-9960 I2C address
    
    # Enable the RGBC functionality
    bus.write_byte_data(i2c_address, 0x80, 0x03)

    # Read RGBC data registers
    clear_low = bus.read_byte_data(i2c_address, 0x94)
    clear_high = bus.read_byte_data(i2c_address, 0x95)
    red_low = bus.read_byte_data(i2c_address, 0x96)
    red_high = bus.read_byte_data(i2c_address, 0x97)
    green_low = bus.read_byte_data(i2c_address, 0x98)
    green_high = bus.read_byte_data(i2c_address, 0x99)
    blue_low = bus.read_byte_data(i2c_address, 0x9A)
    blue_high = bus.read_byte_data(i2c_address, 0x9B)

    # Combine low and high bytes
    clear = (clear_high << 8) | clear_low
    red = (red_high << 8) | red_low
    green = (green_high << 8) | green_low
    blue = (blue_high << 8) | blue_low

    # Print out the results
    print(f"Clear: {clear}, Red: {red}, Green: {green}, Blue: {blue}")

read_RGBC_data()

void setup() {
Wire.begin();
Serial.begin(9600); Wire.beginTransmission(address); Wire.write(0x80); Wire.write(0x03); Wire.endTransmission();
delay(500); }
void loop() { Wire.beginTransmission(address); Wire.write(0x94); Wire.endTransmission();
Wire.requestFrom(address, 1); if(Wire.available()){
LSByte = Wire.read(); }
Wire.beginTransmission(address); Wire.write(0x95); Wire.endTransmission();
Wire.requestFrom(address, 1); if(Wire.available()){
MSByte = Wire.read(); }
// What is this result?
regValue = (MSByte<<8) + LSByte;
Serial.print("Register Value: "); Serial.println(regValue);
// Wait 1 second before next reading delay(1000);
}