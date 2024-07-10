/*
  LoRaWAN DEMO
  
  This code sets up and operates a LoRaWAN communication system. It initializes serial communication and configures 
  the SPI pins for the LoRa module. In the setup() function, it initializes the LoRa radio and attempts to join the 
  LoRaWAN network using Over-The-Air Activation (OTAA) with provided credentials. The loop() function periodically
  sends an uplink packet containing a string and an incrementing count value, and attempts to receive a downlink response.
  Debug messages and status updates are printed to the serial monitor throughout the process.
*/


#include "config.h"
 
int LORA_SS = 5; // LORA SPI CS
int LORA_SCLK = 45; // LORA SPI CLK
int LORA_MISO = 35; // LORA SPI MISO
int LORA_MOSI = 36; // LORA SPI MOSI

int count = 0;
 
void setup() {
  // Start serial communication
  Serial.begin(115200);
  Serial.println("LoRaWAN DEMO");

  SPI.begin(LORA_SCLK, LORA_MISO, LORA_MOSI, LORA_SS);

  Serial.println(F("\nSetup ... "));
  Serial.println(F("Initalise the radio"));

  //radio.setDio2AsRfSwitch(true);
  if (radio.setDio2AsRfSwitch() != RADIOLIB_ERR_NONE) {
    Serial.println(F("Failed to set DIO2 as RF switch!"));
    while (true);
  }

  int state = radio.begin(868.0, 125.0, 9, 7, RADIOLIB_SX126X_SYNC_WORD_PRIVATE, 10, 8, 0, false); 
  debug(state != RADIOLIB_ERR_NONE, F("Initalise radio failed"), state, true);

  Serial.println(F("Join ('login') to the LoRaWAN Network"));
  state = node.beginOTAA(joinEUI, devEUI, nwkKey, appKey, false);
  debug(state < RADIOLIB_ERR_NONE, F("Join failed"), state, false);
  Serial.println(F("Ready!\n"));
 }

 void loop() {
  Serial.println(F("Sending uplink")); 
  Serial.print(F("[LoRaWAN] Sending uplink packet ... "));
  String strUp = "Hello!" + String(count++);
  String strDown;
  int state = node.sendReceive(strUp, 10, strDown);
  debug((state != RADIOLIB_ERR_RX_TIMEOUT) && (state != RADIOLIB_ERR_NONE), F("Error in sendReceive"), state, false);

  Serial.print(F("Uplink complete, next in "));
  Serial.print(uplinkIntervalSeconds);
  Serial.println(F(" seconds"));
 }