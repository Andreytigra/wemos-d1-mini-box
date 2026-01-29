#include <Arduino.h>
#include "ir.h"
#include "radio.h"
#include "options.h"

String command;

void showHelp() {
  Serial.println("ir <protocol> <address> <command> <repeats>");
}

void commandsLoop() {
  if (Serial.available()) {
    command = Serial.readStringUntil('\n');
    command.trim();
    if (command.startsWith("ir")) {
      command.remove(0, 2);
      command.trim(); 
      Serial.println(command);
      int protocolIndex = command.indexOf(' ');
      String protocol = command.substring(0, protocolIndex);

      int addressIndex = command.indexOf(' ', protocolIndex + 1);
      uint16_t address = command.substring(protocolIndex + 1, addressIndex).toInt();

      int commandIndex = command.indexOf(' ', addressIndex + 1);
      uint16_t commandIR = command.substring(addressIndex + 1, commandIndex).toInt();

      int repeatsIndex = command.indexOf(' ', commandIndex + 1);
      int_fast8_t repeats = command.substring(commandIndex + 1, repeatsIndex).toInt();
      Serial.println("Sending...");
      sendIR(protocol, address, commandIR, repeats);
    } else if (command.startsWith("radio")) {

    } else if (command.startsWith("help")) {
      showHelp();
    } else {
      Serial.println("Unknown command. :(");
      showHelp();
    }
    Serial.print("Command: ");
    Serial.println(command);
  }
}