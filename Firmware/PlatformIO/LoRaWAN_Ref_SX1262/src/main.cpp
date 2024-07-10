#include <Arduino.h>
#include "config.h"

// include the library
#include <RadioLib.h>

int LORA_SS = 5;	 // LORA SPI CS
int LORA_SCLK = 45;	 // LORA SPI CLK
int LORA_MISO = 35;	 // LORA SPI MISO
int LORA_MOSI = 36;	 // LORA SPI MOSI

void setup() {
  Serial.begin(115200);
  SPI.begin(LORA_SCLK, LORA_MISO, LORA_MOSI, LORA_SS);

  while (!Serial);  // Wait for serial to be initalised
  delay(5000);  // Give time to switch to the serial monitor
  Serial.println(F("\nSetup"));

  int16_t state = 0;  // return value for calls to RadioLib

  Serial.println(F("Initalise the radio"));
  if (radio.setDio2AsRfSwitch() != RADIOLIB_ERR_NONE) {
    Serial.println(F("Failed to set DIO2 as RF switch!"));
    while (true);
  }
  state = radio.begin(868.0, 125.0, 9, 7, RADIOLIB_SX126X_SYNC_WORD_PRIVATE, 10, 8, 0, true);
  //state = radio.begin(868.0, 125.0, 9, 7, RADIOLIB_SX126X_SYNC_WORD_PRIVATE, 10, 8, 1.6, false); //EBYTE MODULE
  debug(state != RADIOLIB_ERR_NONE, F("Initalise radio failed"), state, true);

  // Override the default join rate
  // uint8_t joinDR = 3;

  Serial.println(F("Join ('login') to the LoRaWAN Network"));
  state = node.beginOTAA(joinEUI, devEUI, nwkKey, appKey, true);
  debug(state < RADIOLIB_ERR_NONE, F("Join failed"), state, false);

  // Print the DevAddr
  Serial.print("[LoRaWAN] DevAddr: ");
  Serial.println((unsigned long)node.getDevAddr(), HEX);

  // Disable the ADR algorithm (on by default which is preferable)
  node.setADR(false);

  // Set a fixed datarate & make it persistent (not normal)
  node.setDatarate(4);

  // Enable CSMA which tries to minimize packet loss by searching 
  // for a free channel before actually sending an uplink 
  node.setCSMA(6, 2, true);

  // Manages uplink intervals to the TTN Fair Use Policy
  node.setDutyCycle(true, 1250);

  // Enable the dwell time limits - 400ms is the limit for the US
  node.setDwellTime(true, 400);

  Serial.println(F("Ready!\n"));
} // setup


void loop() {
  int state = RADIOLIB_ERR_NONE;

  // set battery fill level - the LoRaWAN network server
  // may periodically request this information
  // 0 = external power source
  // 1 = lowest (empty battery)
  // 254 = highest (full battery)
  // 255 = unable to measure
  uint8_t battLevel = 146;
  node.setDeviceStatus(battLevel);


  // Read some inputs
  uint8_t Digital1 = 10;
  uint16_t Analog1 = 12;

  // Build payload byte array
  uint8_t uplinkPayload[3];
  uplinkPayload[0] = Digital1;
  uplinkPayload[1] = highByte(Analog1);   // See notes for high/lowByte functions
  uplinkPayload[2] = lowByte(Analog1);

  uint8_t downlinkPayload[10];  // Make sure this fits your plans!
  size_t  downlinkSize;         // To hold the actual payload size rec'd

  // you can also retrieve additional information about an uplink or 
  // downlink by passing a reference to LoRaWANEvent_t structure
  LoRaWANEvent_t uplinkDetails;
  LoRaWANEvent_t downlinkDetails;
  
  uint8_t Port = 10;

  // Retrieve the last uplink frame counter
  uint32_t fcntUp = node.getFcntUp();
  // Send a confirmed uplink every 64th frame
  // and also request the LinkCheck and DeviceTime MAC commands
  if(fcntUp % 64 == 0) {
    Serial.println(F("[LoRaWAN] Requesting LinkCheck and DeviceTime"));
    node.sendMacCommandReq(RADIOLIB_LORAWAN_MAC_LINK_CHECK);
    node.sendMacCommandReq(RADIOLIB_LORAWAN_MAC_DEVICE_TIME);
    state = node.sendReceive(uplinkPayload, sizeof(uplinkPayload), Port, downlinkPayload, &downlinkSize, true, &uplinkDetails, &downlinkDetails); 
  } else {
    state = node.sendReceive(uplinkPayload, sizeof(uplinkPayload), Port, downlinkPayload, &downlinkSize);    
  }
  debug((state != RADIOLIB_LORAWAN_NO_DOWNLINK) && (state != RADIOLIB_ERR_NONE), F("Error in sendReceive"), state, false);

  // Check if downlink was received
  if(state != RADIOLIB_LORAWAN_NO_DOWNLINK) {
    // Did we get a downlink with data for us
    if (downlinkSize > 0) {
      Serial.println(F("Downlink data: "));
      arrayDump(downlinkPayload, downlinkSize);
    } else {
      Serial.println(F("<MAC commands only>"));
    }

    // print RSSI (Received Signal Strength Indicator)
    Serial.print(F("[LoRaWAN] RSSI:\t\t"));
    Serial.print(radio.getRSSI());
    Serial.println(F(" dBm"));

    // print SNR (Signal-to-Noise Ratio)
    Serial.print(F("[LoRaWAN] SNR:\t\t"));
    Serial.print(radio.getSNR());
    Serial.println(F(" dB"));

    // print frequency error
    Serial.print(F("[LoRaWAN] Frequency error:\t"));
    Serial.print(radio.getFrequencyError());
    Serial.println(F(" Hz"));

    // print extra information about the event
    Serial.println(F("[LoRaWAN] Event information:"));
    Serial.print(F("[LoRaWAN] Confirmed:\t"));
    Serial.println(downlinkDetails.confirmed);
    Serial.print(F("[LoRaWAN] Confirming:\t"));
    Serial.println(downlinkDetails.confirming);
    Serial.print(F("[LoRaWAN] Datarate:\t"));
    Serial.println(downlinkDetails.datarate);
    Serial.print(F("[LoRaWAN] Frequency:\t"));
    Serial.print(downlinkDetails.freq, 3);
    Serial.println(F(" MHz"));
    Serial.print(F("[LoRaWAN] Output power:\t"));
    Serial.print(downlinkDetails.power);
    Serial.println(F(" dBm"));
    Serial.print(F("[LoRaWAN] Frame count:\t"));
    Serial.println(downlinkDetails.fcnt);
    Serial.print(F("[LoRaWAN] Port:\t\t"));
    Serial.println(downlinkDetails.port);

    uint8_t margin = 0;
    uint8_t gwCnt = 0;
    if(node.getMacLinkCheckAns(&margin, &gwCnt) == RADIOLIB_ERR_NONE) {
      Serial.print(F("[LoRaWAN] LinkCheck margin:\t"));
      Serial.println(margin);
      Serial.print(F("[LoRaWAN] LinkCheck count:\t"));
      Serial.println(gwCnt);
    }

    uint32_t networkTime = 0;
    uint8_t fracSecond = 0;
    if(node.getMacDeviceTimeAns(&networkTime, &fracSecond, true) == RADIOLIB_ERR_NONE) {
      Serial.print(F("[LoRaWAN] DeviceTime Unix:\t"));
      Serial.println(networkTime);
      Serial.print(F("[LoRaWAN] DeviceTime second:\t1/"));
      Serial.println(fracSecond);
    }
  
  }

  // wait before sending another packet
  uint32_t minimumDelay = uplinkIntervalSeconds * 1000UL;
  uint32_t interval = node.timeUntilUplink();     // calculate minimum duty cycle delay (per FUP & law!)
  uint32_t delayMs = max(interval, minimumDelay); // cannot send faster than duty cycle allows

  Serial.print(F("[LoRaWAN] Next uplink in "));
  Serial.print(delayMs/1000);
  Serial.println(F("s"));

  delay(delayMs);
}  