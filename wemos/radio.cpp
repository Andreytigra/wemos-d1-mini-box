#include <Arduino.h>
#include <RCSwitch.h>
#include "options.h"

RCSwitch mySwitch = RCSwitch();

bool radioReceiveEnabled = false;

void setupRadio() {
  mySwitch.enableReceive(RADIO_RECEIVE_PIN);
  mySwitch.enableTransmit(RADIO_SENDER_PIN);
}

void sendRadio(int nPulseLength, int decimalCode, int bitLength, int protocol = 1) {
  Serial.println("Using protocol: " + protocol);
  
  mySwitch.setProtocol(protocol);
  mySwitch.setPulseLength(nPulseLength);
  mySwitch.send(decimalCode, bitLength);
}

void radioLoop() {
  if (mySwitch.available()) {
    Serial.println((mySwitch.getReceivedValue(), mySwitch.getReceivedBitlength(), mySwitch.getReceivedDelay(), mySwitch.getReceivedRawdata(),mySwitch.getReceivedProtocol()));
    mySwitch.resetAvailable();
  }
}