#ifndef MAIN_SHAFT_ENCODER_H
#define MAIN_SHAFT_ENCODER_H
#include <Arduino.h>
#include "mainProjectDefinition.h"
#define ENCODER_USE_INTERRUPTS
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#define EncoderPinA 33
#define EncoderPinB 34
#define EncoderPinZ 16 // 16 WAS USED NEW IS 28
volatile uint8_t EncoderResettoZero = 0;
uint8_t MainShaftHome = No;
Encoder myEnc(EncoderPinA, EncoderPinB);

long position = -999;
long absPosition = -999;

void mainShaftReset();
void RunTimeEncoderRead();
void initMainShaft();

void mainShaftReset()
{
    myEnc.write(0);
    EncoderResettoZero = Yes;
    digitalWrite(TestPin1, !digitalRead(TestPin1));
}
void initMainShaft()
{
    pinMode(EncoderPinZ, INPUT_PULLUP);
    // allow time for a passive R-C filter to charge
    // through the pullup resistors, before reading
    // the initial state
    delayMicroseconds(2000);
    // pinMode(EncoderPinA, INPUT);
    // pinMode(EncoderPinB, INPUT);
    //attachInterrupt(digitalPinToInterrupt(EncoderPinZ), mainShaftReset, RISING);
    attachInterrupt(EncoderPinZ, mainShaftReset, RISING);
    NVIC_SET_PRIORITY(IRQ_PORTA, 0);
}

void RunTimeEncoderRead()
{

    long newPos = myEnc.read();
    if (newPos != position)
    {
        if (MainShaftHome == Yes)
        {
            position = newPos;
            absPosition = abs(position);
            if (absPosition >= 2886 && absPosition <= 2950 && NippleJustUp == No)
            {
                if (MainShaftHome == Yes)
                {
                    MainShaftRoundCompleteCounter++;
                    digitalWrite(TestPin1, 1);
                }
#ifdef debugEncoderfunction
                Serial.println("NJU"); // NIPPLE JUST UP
#endif
                //digitalWrite(TestPin1, 1);
                NippleJustUp = Yes;
            }
            else if (absPosition > 2950)
            {
                NippleJustUp = No;
            }

            if (absPosition >= 104 && absPosition <= 140 && needleComingDown == No)
            {
                StichStaus = FM.XYZ_StichStatus();
                if (StichStaus == WaitXY)
                {
                    Serial.println("FD"); //Frame Delay
                                          //DTS = doPauseStiching;
                    FrameDelayed = Yes;
                }
                else
                {
                    digitalWrite(TestPin1, 0);
                }
                needleComingDown = Yes;
            }
            else if (absPosition > 140)
            {
                needleComingDown = No;
            }
            if (absPosition >= 500 && absPosition <= 540 && needleJustTouch == No)
            {
                if (MainShaftHome == Yes)
                {
                    MainShaftRoundCompleteCounter2 = 1;
                    needleTestpoint1 = 1;
                    StichStaus = FM.XYZ_StichStatus();
                    if (StichStaus == WaitXY)
                    {
                        Serial.println("FD"); //Frame Delay
                                              //DTS = doPauseStiching;
                    }
                    else
                    {
                        digitalWrite(TestPin1, 0);
                    }
                }
#ifdef debugEncoderfunction
                Serial.println("NJT"); // NEEDLE JUST TOUSH
#endif
                //digitalWrite(TestPin1, 1);
                needleJustTouch = Yes;
            }
            else if (absPosition > 540)
            {
                needleJustTouch = No;
                //digitalWrite(TestPin1, 0);
            }
            //         if (absPosition >= 250 && absPosition <= 270 && needleJustTouch == No)
            //         {
            //             if (MainShaftHome == Yes)
            //             {
            //                 MainShaftRoundCompleteCounter2 = 1;
            //             }
            // #ifdef debugEncoderfunction
            //             Serial.println("NJT"); // NEEDLE JUST TOUSH
            // #endif
            //             //digitalWrite(TestPin1, 1);
            //             needleJustTouch = Yes;
            //         }
            //         else if (absPosition > 270)
            //         {
            //             needleJustTouch = No;
            //             //digitalWrite(TestPin1, 0);
            //         }

            //         if (absPosition >= 2610 && absPosition <= 590 && needleInShuttle == No)
            //         {
            //             if (MainShaftHome == Yes)
            //             {
            //                 MainShaftRoundCompleteCounter3 = 1;
            //             }
            // #ifdef debugEncoderfunction
            //             Serial.println("NIS"); // NEEDLE IN Shuttle
            // #endif
            //             //digitalWrite(TestPin1, 1);
            //             needleInShuttle = Yes;
            //         }
            //         else if (absPosition > 590)
            //         {
            //             needleInShuttle = No;
            //             //digitalWrite(TestPin1, 0);
            //         }
        }
        if (EncoderResettoZero == Yes)
        {
            EncoderResettoZero = No;
            MainShaftHome = Yes;
            position = 0;
#ifdef debugEncoderfunction
            Serial.println("Zero");
#endif
        }
#ifdef debugEncoderreading
        Serial.println(position);
#endif
    }
}
#endif