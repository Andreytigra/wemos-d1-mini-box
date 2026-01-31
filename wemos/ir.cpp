#include <Arduino.h>
#include <IRremote.hpp>
#include "options.h"
#include "web.h"

bool IRReceiveEnabled = true;

void setupIR() {
  IrReceiver.begin(IR_RECEIVE_PIN, DISABLE_LED_FEEDBACK);
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

  IrSender.sendNECRaw(rawData, repeats);
}

void handleToggleIR() {
  IRReceiveEnabled = !IRReceiveEnabled;
  
  if (IRReceiveEnabled) {
    IrReceiver.enableIRIn();
    Serial.println("IR scanning enabled");
    sendText("IR scanning enabled");
  } else {
    IrReceiver.disableIRIn();
    Serial.println("IR scanning disabled");
    sendText("IR scanning disabled");
  }
}

void sendIR(String protocol, uint16_t address, uint16_t command, int_fast8_t repeats, uint32_t rawData) {
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
}

void IRLoop() {
  if (IrReceiver.decode()) {
    String text;
    if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
      auto tDecodedRawData = IrReceiver.decodedIRData.decodedRawData;

      text += "Raw data received are 0x" + String(tDecodedRawData);

      Serial.print(F("Raw data received are 0x"));
      Serial.println(tDecodedRawData);
    } else {
      
      text += "Protocol:" + String(getProtocolString(IrReceiver.decodedIRData.protocol));
      text += " Address:" + String(IrReceiver.decodedIRData.address);
      text += " Command:" + String(IrReceiver.decodedIRData.command);

      IrReceiver.printIRResultShort(&Serial);
      IrReceiver.printIRSendUsage(&Serial);
    }
    sendText(text);
    IrReceiver.resume();
  }
}

