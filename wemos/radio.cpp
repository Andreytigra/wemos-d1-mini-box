#include <Arduino.h>
#include <RCSwitch.h>
#include "options.h"
#include "web.h"

RCSwitch mySwitch = RCSwitch();

bool radioReceiveEnabled = true;

void setupRadio() {
  mySwitch.enableReceive(RADIO_RECEIVE_PIN);
  mySwitch.enableTransmit(RADIO_SENDER_PIN);
}

void handleToggleRadio() {
  radioReceiveEnabled = !radioReceiveEnabled;
  
  if (radioReceiveEnabled) {
    setupRadio();
    Serial.println("Radio scanning enabled");
    sendText("Radio scanning enabled");
  } else {
    mySwitch.disableReceive();
    mySwitch.disableTransmit();
    Serial.println("Radio scanning disabled");
    sendText("Radio scanning disabled");
  }
}

void sendRadio(int nPulseLength, int decimalCode, int bitLength, int protocol = 1) {
  Serial.print("Using protocol: ");
  Serial.println(protocol);

  Serial.println("Sending radio.");
  sendText("Sending radio.");
  
  mySwitch.setProtocol(protocol);
  mySwitch.setPulseLength(nPulseLength);
  mySwitch.send(decimalCode, bitLength);
}

void radioLoop() {
  if (mySwitch.available()) {
    
    Serial.print("Decimal Code: ");
    Serial.print(mySwitch.getReceivedValue());
    Serial.print(" Bit Length: ");
    Serial.print(mySwitch.getReceivedBitlength());
    Serial.print(" Pulse Length: ");
    Serial.print(mySwitch.getReceivedDelay());
    Serial.print(" Protocol: ");
    Serial.println(mySwitch.getReceivedProtocol());

    String text;

    text += "Decimal Code:" + String(mySwitch.getReceivedValue());
    text += " Bit Length:" + String(mySwitch.getReceivedBitlength());
    text += " Pulse Length:" + String(mySwitch.getReceivedDelay());
    text += " Protocol:" + String(mySwitch.getReceivedProtocol());

    sendText(text);

    mySwitch.resetAvailable();
  }
}