#include <Arduino.h>
/*
  RadioLib LoRaWAN Starter Example

  This example joins a LoRaWAN network and will send
  uplink packets. Before you start, you will have to
  register your device at https://www.thethingsnetwork.org/
  After your device is registered, you can run this example.
  The device will join the network and start uploading data.

  Running this examples REQUIRES you to check "Resets DevNonces"
  on your LoRaWAN dashboard. Refer to the network's 
  documentation on how to do this.

  For default module settings, see the wiki page
  https://github.com/jgromes/RadioLib/wiki/Default-configuration

  For full API reference, see the GitHub Pages
  https://jgromes.github.io/RadioLib/

  For LoRaWAN details, see the wiki page
  https://github.com/jgromes/RadioLib/wiki/LoRaWAN

*/

#include "config.h"

int LORA_SS = 5;	 // LORA SPI CS
int LORA_SCLK = 45;	 // LORA SPI CLK
int LORA_MISO = 35;	 // LORA SPI MISO
int LORA_MOSI = 36;	 // LORA SPI MOSI

void setup() {
  Serial.begin(115200);

  SPI.begin(LORA_SCLK, LORA_MISO, LORA_MOSI, LORA_SS);

  Serial.println(F("\nSetup ... "));
  Serial.println(F("Initalise the radio"));

  //radio.setDio2AsRfSwitch(true);
  if (radio.setDio2AsRfSwitch() != RADIOLIB_ERR_NONE) {
    Serial.println(F("Failed to set DIO2 as RF switch!"));
    while (true);
  }

  //int state = radio.begin();
  int state = radio.begin(868.0, 125.0, 9, 7, RADIOLIB_SX126X_SYNC_WORD_PRIVATE, 10, 8, 0, true);
  debug(state != RADIOLIB_ERR_NONE, F("Initalise radio failed"), state, true);

  Serial.println(F("Join ('login') to the LoRaWAN Network"));
  state = node.beginOTAA(joinEUI, devEUI, nwkKey, appKey, false);

  debug(state < RADIOLIB_ERR_NONE, F("Join failed"), state, false);

  Serial.println(F("Ready!\n"));
}

int count = 0;

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
  
  // Wait until next uplink - observing legal & TTN FUP constraints
  delay(5000);
  //delay(uplinkIntervalSeconds * 1000UL);  // delay needs milli-seconds
}