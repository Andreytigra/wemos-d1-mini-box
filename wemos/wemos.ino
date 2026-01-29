#include <Arduino.h>
#include "radio.h"
#include "ir.h"
#include "web.h"
#include "commands.h"

void setup() {
  Serial.begin(115200);
  wifiSetup();
  Serial.printf("Free heap: %u bytes\n", ESP.getFreeHeap());
  delay(2000);
  setupRadio();
  setupIR();
}

bool asyncWait(unsigned long intervalMs) {
  static unsigned long lastTime = 0;
  unsigned long now = millis();

  if (now - lastTime >= intervalMs) {
    lastTime = now;
    return true;   
  }
  return false;    
}


void loop() {
  wifiLoop();

  if (radioReceiveEnabled) {
    radioLoop();
  }

  if (IRReceiveEnabled) {
    IRLoop();
  }

  commandsLoop(); 
}
