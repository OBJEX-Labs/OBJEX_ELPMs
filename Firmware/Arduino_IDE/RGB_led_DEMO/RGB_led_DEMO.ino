/*
  RGB Led DEMO
  
  This code controls the builtin RGB status led of the OBJEX ELPM module. 
  It uses the Adafruit_NeoPixel library to manage the LED. The code defines 
  a function called "rainbow" that cycles the LED through a rainbow color spectrum. 
  The "loop" function continuously calls the "rainbow" function with a delay 
  of 10 milliseconds, creating a smooth rainbow animation.
*/

#include <Adafruit_NeoPixel.h>

#define LED_PIN    48   // GPIO connected to RGB status led (WS2812B)
#define LED_COUNT 1

Adafruit_NeoPixel led(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);  // Create a Adafruit_NeoPixel object to control the LED

void rainbow(int wait) {
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    led.rainbow(firstPixelHue);
    led.show();
    delay(wait);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("ELPM RGB TEST");

  led.begin();  // Initialize NeoPixel led object
  led.show();   // Turn OFF LED
  led.setBrightness(100); 
}

void loop() {
  rainbow(10);  // Run the rainbow effect        
}