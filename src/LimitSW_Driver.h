#ifndef LIMITS_SW_H
#define LIMITS_SW_H

#include <Arduino.h>
#include "TM1638Setup.h"
#include "mainProjectDefinition.h"

#define Left_SW 36
#define Right_SW 35
#define Forward_SW 14  // 33
#define BackWard_SW 15 // 34
#define HomePositoin_SW 25

#define X_LimitLED 0
#define Y_LimitLED 1
#define Z_LimitLED 0

class LimitSW
{
private:
    boolean X_newcondition = 1;
    boolean X_last_condition = 0;

    boolean Y_newcondition = 1;
    boolean Y_last_condition = 0;

    boolean Z_newcondition = 1;
    boolean Z_last_condition = 0;

public:
    void init_LimitsSW()
    {
        pinMode(Left_SW, INPUT_PULLUP);
        pinMode(Right_SW, INPUT_PULLUP);
        pinMode(BackWard_SW, INPUT_PULLUP);
        pinMode(Forward_SW, INPUT_PULLUP);
        pinMode(HomePositoin_SW, INPUT_PULLUP);

        if (digitalRead(Left_SW) == Open && digitalRead(Right_SW) == Open)
        {
            X_newcondition = 0;
            tm.setLED(X_LimitLED, X_newcondition);
        }
        else
        {
            X_newcondition = 1;
            tm.setLED(X_LimitLED, X_newcondition);
        }
        X_last_condition = X_newcondition;

        if (digitalRead(BackWard_SW) == Open && digitalRead(Forward_SW) == Open)
        {
            Y_newcondition = 0;
            tm.setLED(Y_LimitLED, Y_newcondition);
        }
        else
        {
            Y_newcondition = 1;
            tm.setLED(Y_LimitLED, Y_newcondition);
        }
        Y_last_condition = Y_newcondition;
    }

    //--------------------------When Limit Switch Pressed Logic '0' on Teensy Pin
    boolean Left_SW_Pressed()
    {
        if (digitalRead(Left_SW) == Open)
        {
            return Open;
        }
        return Close;
    }
    //--------------------------When Limit Switch Pressed Logic '0' on Teensy Pin
    boolean Right_SW_Pressed()
    {
        if (digitalRead(Right_SW) == Open)
        {
            return Open;
        }
        return Close;
    }
    //--------------------------When Limit Switch Pressed Logic '0' on Teensy Pin
    boolean BackWard_SW_Pressed()
    {
        if (digitalRead(BackWard_SW) == Open)
        {
            return Open;
        }
        return Close;
    }
    //--------------------------When Limit Switch Pressed Logic '0' on Teensy Pin
    boolean Forward_SW_Presses()
    {
        if (digitalRead(Forward_SW) == Open)
        {
            return Open;
        }
        return Close;
    }
    //---------------------------
    void LimitSWLoop(uint8_t _cMCS)
    {
        /**/
        if (digitalRead(Left_SW) == Open && digitalRead(Right_SW) == Open)
        {
            X_newcondition = 0;
        }
        else
        {
            X_newcondition = 1;
        }
        if (X_last_condition != X_newcondition)
        {
            X_last_condition = X_newcondition;
            tm.setLED(X_LimitLED, X_newcondition);
            if (X_newcondition == 1)
            {
                if (_cMCS == mManual)
                {
                    Serial.println("X_Limit Press");
                }
            }
        }
        /**/
        if (digitalRead(Forward_SW) == Open && digitalRead(BackWard_SW) == Open)
        {
            Y_newcondition = 0;
        }
        else
        {
            Y_newcondition = 1;
        }
        if (Y_last_condition != Y_newcondition)
        {
            Y_last_condition = Y_newcondition;
            tm.setLED(Y_LimitLED, Y_newcondition);
            if (Y_newcondition == 1)
            {
                if (_cMCS == mManual)
                {
                    Serial.println("Y_Limit Press");
                }
            }
        }
        /**/
        if (digitalRead(HomePositoin_SW) == Non_HomePosition)
        {
            Z_newcondition = 0;
        }
        else
        {
            Z_newcondition = 1;
        }
        if (Z_last_condition != Z_newcondition)
        {
            Z_last_condition = Z_newcondition;
            if (Z_last_condition == 1)
            {
                if (_cMCS == mManual)
                {
                    Serial.println("Z_Limit Press");
                }
            }
        }
        /**/
    }
};
LimitSW Limit_SW;

// extern LimitSW Limit_SW(Left_SW, Right_SW, BackWard_SW, Forward_SW);
#endif