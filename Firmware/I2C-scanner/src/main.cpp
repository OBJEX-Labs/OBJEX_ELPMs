#include <Arduino.h>

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#define PIN        48
#define NUMPIXELS   1 

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500 

void setup() {
  Wire.begin(8, 9); // SDA | SCL
  Serial.begin(115200);
  Serial.println("\nI2C Scanner");

  pixels.begin();
}

void loop() {
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  pixels.setPixelColor(0, pixels.Color(50, 0, 0));
  pixels.show(); 

  nDevices = 0;
  for(address = 1; address < 127; address++ ){
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0){
      Serial.print("I2C device found at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    }else if (error==4){
      Serial.print("Unknow error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  delay(2000);           
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show(); 
  delay(2000);           

}