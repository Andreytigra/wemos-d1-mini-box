#ifndef IR_H
#define IR_H

void setupIR();
void sendIR(String protocol, uint16_t address, uint16_t command, int_fast8_t repeats = 0, uint32_t rawData = 0);
void IRLoop();
void handleToggleIR();

extern bool IRReceiveEnabled;

#endif