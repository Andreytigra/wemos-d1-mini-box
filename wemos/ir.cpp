#include <Arduino.h>
#include <IRremote.hpp>
#include "options.h"

bool IRReceiveEnabled = true;

void setupIR() {
  IrReceiver.begin(IR_RECEIVE_PIN);
  IrSender.begin(IR_SENDER_PIN); 
  IrSender.enableIROut(38);
}

void sendIR(String protocol, uint16_t address, uint16_t command, int_fast8_t repeats) {

  if (protocol.equalsIgnoreCase("NEC")) {
    IrSender.sendNEC(address, command, repeats);
  } else if (protocol.equalsIgnoreCase("NEC2")) {
    IrSender.sendNEC2(address, command, repeats);
  } else if (protocol.equalsIgnoreCase("Samsung")) {
    IrSender.sendSamsung(address, command, repeats);
  } else if (protocol.equalsIgnoreCase("Onkyo") || protocol.equalsIgnoreCase("NECext")) {
    IrSender.sendOnkyo(address, command, repeats);
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

