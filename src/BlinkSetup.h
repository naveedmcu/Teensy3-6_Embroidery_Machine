#ifndef BLINKSETUP_H
#define BLINKSETUP_H

#include <Arduino.h>
#include "TM1638Setup.h"
#include <elapsedMillis.h>
elapsedMillis time_Led_Blink;

const int LED = 13;
volatile int blinkcode = 0;

void SuperLoop_BlinkTask();
void BlinkLEDSetup();
void blinkthread();
void ADD_thread();

void SuperLoop_BlinkTask()
{
    static uint64_t count = 0;
    count++;
    if (count >= 10)
    {
        count = 0;
        digitalWrite(LED, !digitalRead(LED));
    }
}

void blinkthread()
{
    // while (1)
    // {
    //     if (blinkcode)
    //     {
    //         for (int i = 0; i < blinkcode; i++)
    //         {
    //             digitalWrite(LED, HIGH);
    //             threads.delay(150);
    //             digitalWrite(LED, LOW);
    //             threads.delay(150);
    //         }
    //         blinkcode = 0;
    //     }
    //     threads.yield();
    // }
}
void BlinkLEDSetup()
{
    pinMode(LED, OUTPUT);
}

void TM1638_LED_Blink()
{
    while (1)
    {
        static boolean state_of_LED = 0;
        if (time_Led_Blink >= 1000)
        {
            time_Led_Blink = 0;
            state_of_LED = !state_of_LED;
            tm.setLED(0, state_of_LED);
        }

        // time_Led_Blink = 0;
        // state_of_LED = !state_of_LED;
        // tm.setLED(0, state_of_LED);
    }
}

#endif