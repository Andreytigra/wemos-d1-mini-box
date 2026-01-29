#include <Arduino.h>
#include <IRremote.hpp>
#include "options.h"

bool IRReceiveEnabled = true;

void setupIR() {
  IrReceiver.begin(IR_RECEIVE_PIN);
  IrSender.begin(IR_SENDER_PIN); 
  IrSender.enableIROut(38);
}

void sendNECext(uint16_t address, uint16_t command, int_fast8_t repeats) {
  uint8_t addressFirst  = address & 0xFF;
  uint8_t addressSecond = (address >> 8) & 0xFF;

  uint8_t commandFirst = command & 0xFF;
  uint8_t commandSecond = (command >> 8) & 0xFF;

  uint32_t rawData = ((uint32_t)commandFirst << 24) | 
          ((uint32_t)commandSecond  << 16) |
          ((uint32_t)addressFirst << 8)  |
          ((uint32_t)addressSecond);

  uint16_t finalAdr = ((uint16_t)addressSecond << 8) | addressFirst;

  Serial.println(rawData, HEX);

  IrSender.sendNECRaw(rawData, repeats);
}

void sendIR(String protocol, uint16_t address, uint16_t command, int_fast8_t repeats, uint32_t rawData) {
  //uint32_t rawData = 0x6F905583;


  if (protocol.equalsIgnoreCase("NEC")) {
    IrSender.sendNEC(address, command, repeats);
  } else if (protocol.equalsIgnoreCase("NEC2")) {
    IrSender.sendNEC2(address, command, repeats);
  } else if (protocol.equalsIgnoreCase("Samsung")) {
    IrSender.sendSamsung(address, command, repeats);
  } else if (protocol.equalsIgnoreCase("Onkyo")) {
    IrSender.sendOnkyo(address, command, repeats);
  } else if (protocol.equalsIgnoreCase("NECext")) {
    sendNECext(address, command, repeats);
  } else if (protocol.equalsIgnoreCase("NECRaw")) {
    IrSender.sendNECRaw(rawData, repeats);
  } else {
    Serial.println("Unknown protocol. :(");
  }
  

  //IrSender.sendNEC(0xEF00, 0x3, 0); RGB strip turn on
}

void IRLoop() {
  if (IrReceiver.decode()) {
    if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
      auto tDecodedRawData = IrReceiver.decodedIRData.decodedRawData;
      Serial.print(F("Raw data received are 0x"));
      Serial.println(tDecodedRawData);
    } else {
      IrReceiver.printIRResultShort(&Serial);
      IrReceiver.printIRSendUsage(&Serial);
    }
    IrReceiver.resume();
  }
}

