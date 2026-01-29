#ifndef IR_H
#define IR_H

void setupIR();
void sendIR(String protocol, uint16_t address, uint16_t command, int_fast8_t repeats, uint32_t rawData = 0);
void IRLoop();

extern bool IRReceiveEnabled;

#endif