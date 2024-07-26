/*
  Power latch RTC Interrupt

  This code controls an RTC (Real-Time Clock) IC and the status RGB LED. 
  Initializes a single LED and sets its color to green; sets the date and time on the RTC, 
  then configures an alarm to trigger at one minute past the current time. 
  Finally, sets the AUTO_TURN_OFF pin LOW, this trigger the power latch to turn off the module,
  waiting for the interrupt from the RTC to turn on again the module.
*/

#include <Adafruit_NeoPixel.h>
#include <Melopero_RV3028.h>

Melopero_RV3028 rtc;

#define PIN        48  // GPIO connected to RGB status led (WS2812B)
#define LED_COUNT   1
Adafruit_NeoPixel led(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);  // Create a Adafruit_NeoPixel object to control the LED

#define AUTO_TURN_OFF 14  // GPIO connected to power latch
#define BattLVL 7  // GPIO connected to battery level circuit

void setup() {
  // Power lacth setup
  pinMode(AUTO_TURN_OFF, OUTPUT);
  digitalWrite(AUTO_TURN_OFF, HIGH);  // Setting the AUTO_TURN_OFF pin HIGH prevents module shutdown. Do this quickly, or the power latch may cut off power.
  
  // Start serial communication
  Serial.begin(115200);
  Serial.println("Power latch RTC Interrupt");

  // Initialize NeoPixel library
  led.begin();  // Initialize NeoPixel led object
  led.setPixelColor(0, led.Color(0, 50, 0));  // Set a color following RGB standard (R,G,B) ranging from 0 to 255 
  led.show();
  
  Wire.begin(8, 9); // SDA | SCL
  
  rtc.initI2C();

  // Set the device to use the 24hour format (default) instead of the 12 hour format
  rtc.set24HourMode();

  // Set the date and time:
  rtc.setTime(2024, 11, 1, 1, 17, 34, 0);
  // year, month, weekday, date, hour, minute, second
  // Note: time is always set in 24h format
  // Note: month value ranges from 1 (Jan) to 12 (Dec)
  // Note: date value ranges from 1 to 31

  // set the interrupt to trigger every day at 1 minute from now
  // since we set the hour to 15:20 we don't have to check if the 
  // minute value exceeds 59...
  rtc.setDateModeForAlarm(false);
  rtc.enableAlarm(
    0, //weekdayOrDate not important
    rtc.getHour(), //hour
    rtc.getMinute() + 1, //minute
    false, //dateAlarm
    true, //hourAlarm
    true, //minuteAlarm 
    true //generateInterrupt
  );

  Serial.print(rtc.getHour());
  Serial.print(":");
  Serial.print(rtc.getMinute());
  Serial.print(":");
  Serial.print(rtc.getSecond());
  Serial.print("   ");
  Serial.print(rtc.getDate());
  Serial.print("/");
  Serial.print(rtc.getMonth());
  Serial.print("/");
  Serial.println(rtc.getYear());
  
  delay(1000);
  digitalWrite(AUTO_TURN_OFF, LOW);
}

void loop() {
  
}