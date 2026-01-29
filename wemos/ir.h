#ifndef IR
#define IR

void setupIR();
void sendIR(String protocol, uint16_t address, uint16_t command, int_fast8_t repeats);
void IRLoop();

extern bool IRReceiveEnabled;

#endif