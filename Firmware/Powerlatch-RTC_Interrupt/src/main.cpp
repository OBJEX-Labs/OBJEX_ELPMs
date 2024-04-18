#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Melopero_RV3028.h>

Melopero_RV3028 rtc;

#define PIN        48
#define NUMPIXELS   1
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define AUTO_TURN_OFF 14
#define BattLVL 7
#define PLATCH_IN 6

void setup() {
  Serial.begin(115200);

  // Power lacth setup
  pinMode(AUTO_TURN_OFF, OUTPUT);
  digitalWrite(AUTO_TURN_OFF, HIGH);

  Wire.begin(8, 9); // SDA | SCL

  pixels.begin(); 
  pixels.setPixelColor(0, pixels.Color(0, 50, 0));
  pixels.show(); 

  rtc.initI2C();
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
    
    //ANCHOR - ELPM POWER OFF!
    digitalWrite(AUTO_TURN_OFF, LOW);
}


void loop() {
  /*
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
  
  delay(5000);
  digitalWrite(AUTO_TURN_OFF, LOW);
  */
}

