#include <Arduino.h>
#include <esp_adc_cal.h>
#include <esp32-hal-adc.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#define PIN        48
#define NUMPIXELS   1
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define AUTO_TURN_OFF 14
#define BattLVL 7
#define PLATCH_IN 6

void setup() {
  Serial.begin(115200);
  // Power latch setup
  pinMode(AUTO_TURN_OFF, OUTPUT);
  digitalWrite(AUTO_TURN_OFF, HIGH);
  Serial.println("system started");

  Wire.begin(8, 9); // SDA | SCL

  pixels.begin(); 
  pixels.setPixelColor(0, pixels.Color(0, 50, 0));
  pixels.show(); 
}

void loop() {
  delay(5000);
  digitalWrite(AUTO_TURN_OFF, LOW);
}

