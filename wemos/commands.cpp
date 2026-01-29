#include <Arduino.h>
#include "ir.h"
#include "radio.h"
#include "options.h"

String command;

void showHelp() {
  Serial.println("ir <protocol> <address> <command> <repeats>");
  Serial.println("ir <protocol> <raw> <repeats>");
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

      if (protocol.equalsIgnoreCase("NECRaw")) {
        int rawIndex = command.indexOf(' ', protocolIndex + 1);
        uint32_t raw = strtoul(command.substring(protocolIndex + 1, rawIndex).c_str(), NULL, 32);

        int repeatsIndex = command.indexOf(' ', rawIndex + 1);
        int_fast8_t repeats = command.substring(rawIndex + 1, repeatsIndex).toInt();

        sendIR(protocol, 0, 0, repeats, raw);

        return;
      }
      
      int addressIndex = command.indexOf(' ', protocolIndex + 1);
      uint16_t address = strtoul(command.substring(protocolIndex + 1, addressIndex).c_str(), NULL, 16);
    
      int commandIndex = command.indexOf(' ', addressIndex + 1);
      uint16_t commandIR = strtoul(command.substring(addressIndex + 1, commandIndex).c_str(), NULL, 16);

      Serial.println("Command is: ");
      Serial.print(commandIR);

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