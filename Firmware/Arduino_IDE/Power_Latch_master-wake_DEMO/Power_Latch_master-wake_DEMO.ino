/*
  Power latch master/wake DEMO

  The code enables interaction with the Master and Wake pins of the powerlatch
  In the setup phase, it initializes serial communication, configures the power latch to prevent accidental shutdown, 
  and sets the initial LED color to a dim green. The loop function simply waits five seconds before triggering 
  the power latch to turn off the entire module.
*/

#include <Adafruit_NeoPixel.h>

#define LED_PIN    48   // GPIO connected to RGB status led (WS2812B)
#define LED_COUNT   1

Adafruit_NeoPixel led(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);  // Create a Adafruit_NeoPixel object to control the LED

#define AUTO_TURN_OFF 14  // GPIO connected to power latch
#define BattLVL 7  // GPIO connected to battery level circuit

void setup() {
  // Power latch setup
  pinMode(AUTO_TURN_OFF, OUTPUT);
  digitalWrite(AUTO_TURN_OFF, HIGH);  // Setting the AUTO_TURN_OFF pin HIGH prevents module shutdown. Do this quickly, or the power latch may cut off power.
  
  // Start serial communication
  Serial.begin(115200);
  Serial.println("Power Latch master wake DEMO");

  // Initialize NeoPixel library
  led.begin();  // Initialize NeoPixel led object
  pixels.setPixelColor(0, pixels.Color(0, 50, 0));  // Set a color following RGB standard (R,G,B) ranging from 0 to 255 
  pixels.show();
}

void loop() {
  delay(5000);
  digitalWrite(AUTO_TURN_OFF, LOW);  // Setting the AUTO_TURN_OFF pin LOW will trigger the power latch to turn off the module.
}