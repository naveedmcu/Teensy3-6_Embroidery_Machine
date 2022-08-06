#ifndef TM1638_SETUP_H
#define TM1638_SETUP_H

#include <Arduino.h>
#include <elapsedMillis.h>
#include "mainProjectDefinition.h"
#include "TM1638_Driver.h"

elapsedMillis TimeTask;
elapsedMillis time_task1;
elapsedMillis time_TM1638_BlinkLED;

#define TM1638_LEDBlinkPin 3
// GPIO I/O pins on the Arduino connected to strobe, clock, data,
//pick on any I/O you want.
#define STROBE_TM 28 // 28 WAS USED NEW IS 16
#define CLOCK_TM 29
#define DIO_TM 30

TM1638plus tm(STROBE_TM, CLOCK_TM, DIO_TM);

// Some vars and defines for the tests.
#define myTestDelay 1000
#define myTestDelay1 1000
uint8_t testcount = 1;

volatile uint32_t display_byte = 0;
uint8_t All_Motor_OFF = Yes;
volatile uint8_t Update_Led = No;

void MACHINE_ON_LED_tm1638(boolean LED_bit);
void ValueTest();

void TM1638_Task()
{
    static uint8_t Task_init = No;
    while (1)
    {
        if (Task_init == No)
        {
            tm.TM1638_init();
            tm.brightness(0x08);
            Task_init = Yes;
        }
        else
        {
            if (time_task1 > 50)
            {
                time_task1 = 0;
                tm.displayIntNum(display_byte, true); //00000048
            }
        }
        //---------------------------------------------------Task Running LED
        ValueTest();
    }
}

void TM1638Setup()
{
    tm.TM1638_init();
    tm.displayText("00000000");
}

void MACHINE_ON_LED_tm1638(boolean LED_State)
{
    tm.setLED(TM1638_LEDBlinkPin, LED_State);
}
void TM1635_LED_Blink()
{
    static boolean status_LED = 0;
    if (time_TM1638_BlinkLED >= 500)
    {
        time_TM1638_BlinkLED = 0;
        status_LED = !status_LED;
        tm.setLED(TM1638_LEDBlinkPin, status_LED);
    }
}
void ValueTest()
{
    if (TimeTask >= 200)
    {
        TimeTask = 0;
        display_byte++;
    }
}

#endif