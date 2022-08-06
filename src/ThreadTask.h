#ifndef THREAD_TASK_H
#define THREAD_TASK_H

#include <Arduino.h>
// #include "TeensyThreads.h"
#include "BlinkSetup.h"

void AddBlinkTask();
void AddTM1638BlinkTask();

void AddBlinkTask()
{
    // threads.addThread(blinkthread);
}
void AddTM1638BlinkTask()
{
    // threads.addThread(TM1638_LED_Blink, 1);
}

#endif