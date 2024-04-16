#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN    48
#define LED_COUNT 1

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void rainbow(int wait) {
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    strip.rainbow(firstPixelHue);
    strip.show();
    delay(wait);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("ELPM RGB TEST");

  strip.begin();           
  strip.show();            
  strip.setBrightness(100); 
}

void loop() {
  rainbow(10);            
}

