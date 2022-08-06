#ifndef BREAKMOTOR_H
#define BREAKMOTOR_H
#include <Arduino.h>
#include "mainProjectVar.h"
#include "MotorsSetup.h"

#define bmicroStep 4
BreakMotor BAction = BreakMotorSpeedSet;
BreakMotorAction BMA = bUndefine;

void f_BreakONOFF(MachineCurrentStatus _cMCS);
void SetSpeedBreakMotor();
//---------------------------------------
// DO Break MOTOR Position State Machine
//---------------------------------------
#define BreakMotorSpeed 6000
#define BreakMotorAccel 100000
void SetSpeedBreakMotor()
{
    Serial.println("SetSpeed BreakMotor");
    Baxis
        .setMaxSpeed(BreakMotorSpeed)      // steps/s
        .setAcceleration(BreakMotorAccel); // steps/s
}
void f_BreakONOFF(MachineCurrentStatus _cMCS)
{
    sint32 FreeMove;
    sint8 breakround = 45;
    switch (BAction)
    {
    case BreakMotorSpeedSet:
    {
        BAction = BreakUndefine;
        SetSpeedBreakMotor();
        break;
    }
    case BreakON:
        if (BMA != bEngaged)
        {
            FreeMove = breakround * bmicroStep;
            Baxis.setTargetRel(FreeMove);
            if (_cMCS == mManual) // use controllerXY when cMCS in mManual
            {
                controllerXY.moveAsync(Baxis);
                BMA = bEngaged;
                BAction = BreakDone;
// while (controllerXY.isRunning())
// {
// }
#ifdef debugf_BreakONOFF
                Serial.println("BreakON mManual with ControllerXY");
#endif
            }
            else if (_cMCS == mEndDesign) // use controllerXY when cMCS in mEndDesign
            {
                controllerX.moveAsync(Baxis);
                BMA = bEngaged;
                BAction = BreakDone;
#ifdef debugf_BreakONOFF
                Serial.println("BreakON");
#endif
            }
            else if (_cMCS == mStichDone) // use controllerX when cMCS in mStichLoad
            {
                controllerX.moveAsync(Baxis);
                BMA = bEngaged;
                BAction = BreakDone;
// while (controllerX.isRunning())
// {
// }
#ifdef debugf_BreakONOFF
                Serial.println("BreakON mStichDone");
#endif
            }
            else if (_cMCS == mPauseStich || _cMCS == mStopStich || _cMCS == mColourChange) // use controllerX when cMCS in mStichLoad
            {
                controllerX.moveAsync(Baxis);
                BMA = bEngaged;
                BAction = BreakDone;
// while (controllerX.isRunning())
// {
// }
#ifdef debugf_BreakONOFF
                Serial.println("BreakON");
#endif
            }
            else
            {
                BAction = BreakError;
                BMA = bUndefine;
            }
        }
        break;
    case BreakOFF:
        if (BMA != bDisengaged)
        {
            FreeMove = -breakround * bmicroStep;
            Baxis.setTargetRel(FreeMove);
            if (_cMCS == mManual) // use controllerXY when cMCS in mManual
            {
#ifdef debugf_BreakONOFF
                Serial.println("BreakOFF mManual ControllerXY");
#endif
                controllerXY.moveAsync(Baxis);
                BMA = bDisengaged;
                BAction = BreakDone;
                // while (controllerXY.isRunning())
                // {
                // }
            }
//             else if (_cMCS == mEndDesign) // use controllerXY when cMCS in mEndDesign
//             {
//                 controllerX.moveAsync(Baxis);
//                 BMA = bDisengaged;
//                 BAction = BreakDone;
// // while (controllerXY.isRunning())
// // {
// // }
// #ifdef debugf_BreakONOFF
//                 Serial.println("BreakOFF mEndDesign");
// #endif
//             }
            else if (_cMCS == mStichDone) // use controllerX when cMCS in mStichLoad
            {
#ifdef debugf_BreakONOFF
                Serial.println("BreakOFF mStichDone");
#endif
                controllerX.moveAsync(Baxis);
                BMA = bDisengaged;
                BAction = BreakDone;
            }
//             else if (_cMCS == mPauseStich || _cMCS == mStopStich || _cMCS == mColourChange) // use controllerX when cMCS in mStichLoad
//             {
//                 controllerX.moveAsync(Baxis);
//                 BMA = bDisengaged;
//                 BAction = BreakDone;
// // while (controllerX.isRunning())
// // {
// // }
// #ifdef debugf_BreakONOFF
//                 Serial.println("BreakOFF ");
// #endif
//             }
            else
            {
                BAction = BreakError;
                BMA = bUndefine;
            }
        }
        break;
    case BreakStop:
        if (BMA != bStop)
        {
            if (_cMCS == mManual) // use controllerXY when cMCS in mManual
            {
                controllerXY.emergencyStop();
                BMA = bStop;
            }
            else if (_cMCS == mStiching || _cMCS == mStich) // use controllerX when cMCS in mStichLoad
            {
                controllerX.emergencyStop();
                BMA = bStop;
            }
            else if (_cMCS == mPauseStich || _cMCS == mStopStich || _cMCS == mJumpDone || _cMCS == mColourChange || _cMCS == mJump) // use controllerX when cMCS in mStichLoad
            {
                controllerX.emergencyStop();
                BMA = bStop;
            }
            else
            {
                BAction = BreakError;
                BMA = bUndefine;
            }
        }
        break;
    case BreakDisable:
        if (BMA != bEnable)
        {
            B_Stepper_DIS();
            BMA = bEnable;
#ifdef debugf_BreakONOFF
            Serial.println("bEnable");
#endif
        }
        break;
    case BreakEnable:
        if (BMA != bDisable)
        {
            B_Stepper_EN();
            BMA = bDisable;
#ifdef debugf_BreakONOFF
            Serial.println("bDisable");
#endif
        }
        break;
    case BreakError:
#ifdef debugf_BreakONOFF
        Serial.println("BreakError");
#endif
        Serial.println(_cMCS);
        BAction = BreakUndefine;
        break;
    case BreakDone:
#ifdef debugf_BreakONOFF
        Serial.println("BreakDone");
#endif
        BAction = BreakUndefine;
        break;
    default:
        break;
    }
}
#endif