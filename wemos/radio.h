#ifndef RADIO_H
#define RADIO_H

void setupRadio();
void sendRadio(int nPulseLength, int decimalCode, int bitLength, int protocol);
void radioLoop();

extern bool radioReceiveEnabled;

#endif