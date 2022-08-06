#ifndef THREADBREAKSETUP_H
#define THREADBREAKSETUP_H
#include <Arduino.h>
#define threadpin 24 // 37 past used and now 24 was used mainshaftused only
uint8_t NumberofStichPasswhileThreadBreak = 0;

void ThreadBreakIOinit()
{
    pinMode(threadpin, INPUT_PULLUP); // logic '0' when thread break
}
#endif