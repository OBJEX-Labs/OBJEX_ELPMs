#include <Arduino.h>
#include <WiFi.h>

#define AUTO_TURN_OFF 14

const char* ssid = "";
const char* password = "";

void setup(){
    Serial.begin(115200);
    Serial.println("STARTUP");
    // Power lacth setup
    pinMode(AUTO_TURN_OFF, OUTPUT);
    digitalWrite(AUTO_TURN_OFF, HIGH);

    WiFi.mode(WIFI_STA); 
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting");

    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(500);
    }

    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());

    Serial.print("RSSI: ");
    Serial.println(WiFi.RSSI());
}

void loop(){
  delay(1000);
  digitalWrite(AUTO_TURN_OFF, LOW);
}