#ifndef FRAME_SETUP_H
#define FRAME_SETUP_H
#include <Arduino.h>
#include "mainProjectDefinition.h"
#include "MotorsSetup.h"
#include "LimitSW_Driver.h"
#include "mainProjectVar.h"
#include "ZmotorSetup.h"

typedef enum
{
    undefineSpeed = 0,
    JumpingStichSpeed = 1,
    StichingStichSpeed,
    Stiching_F_R_StichSpeed,
    ManualFrameMoveSpeed,
    HomingFrameMoveSpeed,
    /**/
    ManualNeedleMoveSpeed,
    HomingNeedleMoveSpeed
    /**/
} FS;
FS FrameSpeed;
/**/
/**/
#define microStep 4
#define distancePerRotation 20 // 20 mm
#define stepsPerRotation 200
#define RotationtoLiner 4
//#define RotationtoLinear  microStep*stepsPerRotation/stepsPerRotation
/**/
#define ManualFreeFrameSpeed 2600
#define ManualFreeFrameAccel 350000
// #define ManualFreeFrameSpeed 5000  //// 16 MICROSTEP
// #define ManualFreeFrameAccel 150000 //// 16 MICROSTEP
// #define ManualFreeFrameSpeed 2000 ////tEST WITH  RotationtoLiner 1
// #define ManualFreeFrameAccel 22000////tEST WITH  RotationtoLiner 1
// #define ManualFreeFrameSpeed 9000
// #define ManualFreeFrameAccel 440000
/**/
#define ManualFrameSpeed 2600  // USE WHEN SHITCHIN // 20 81 Gear
#define ManualFrameAccel 50000 // USE WHEN SHITCHIN // 20 81 Gear
// #define ManualFrameSpeed 2500   // USE WHEN SHITCHIN // //tEST WITH  RotationtoLiner 4
// #define ManualFrameAccel 100000 // USE WHEN SHITCHIN // //tEST WITH  RotationtoLiner 4
// #define ManualFrameSpeed 1000   // USE WHEN SHITCHIN////tEST WITH  RotationtoLiner 1
// #define ManualFrameAccel 20000 // USE WHEN SHITCHIN ////tEST WITH  RotationtoLiner 1
/**/
// #define StichFrameSpeed 9000*2
#define StichFrameSpeed 2600   // 20 81 Gear  // 3500 TEST BUT LESS TORQUE
#define StichFrameAccel 350000 // 20 81 Gear 400000
//#define StichFrameSpeed 25000  //tEST WITH  RotationtoLiner 8
//#define StichFrameAccel 480000 //tEST WITH  RotationtoLiner 8
// #define StichFrameSpeed 4000 ////tEST WITH  RotationtoLiner 4
// #define StichFrameAccel 200000 //tEST WITH  RotationtoLiner 4
// #define StichFrameSpeed 3500 ////tEST WITH  RotationtoLiner 1
// #define StichFrameAccel 30000////tEST WITH  RotationtoLiner 1
/**/
#define Stich_F_R_FrameSpeed 2600   //// 20 81 Gear
#define Stich_F_R_FrameAccel 150000 //// 20 81 Gear
// #define Stich_F_R_FrameSpeed 2000 //tEST WITH  RotationtoLiner 4
// #define Stich_F_R_FrameAccel 90000//tEST WITH  RotationtoLiner 4
// #define Stich_F_R_FrameSpeed 800////tEST WITH  RotationtoLiner 1
// #define Stich_F_R_FrameAccel 22500////tEST WITH  RotationtoLiner 1
/**/
#define JumpFrameSpeed 2600   // 20 81 Gear
#define JumpFrameAccel 100000 // 20 81 Gear
// #define JumpFrameSpeed 7000 //tEST WITH  RotationtoLiner 4
// #define JumpFrameAccel 80000 //tEST WITH  RotationtoLiner 4
// #define JumpFrameSpeed 2000////tEST WITH  RotationtoLiner 1
// #define JumpFrameAccel 10000////tEST WITH  RotationtoLiner 1
/**/
#define FramHomeSpeed 2600  // 20 81 Gear
#define FramHomeAccel 50000 // 20 81 Gear
// #define FramHomeSpeed 5000 //TEST WITH  RotationtoLiner 4
// #define FramHomeAccel 100000 //TEST WITH  RotationtoLiner 4
// #define FramHomeSpeed 5000////tEST WITH  RotationtoLiner 1
// #define FramHomeAccel 100000////tEST WITH  RotationtoLiner 1
/**/
class FrameMove
{
private:
    int8 Y_LastStatus = Stop;
    int8 X_LastStatus = Stop;
    volatile sint32 X_MotorPuls = 0;
    volatile sint32 Y_MotorPuls = 0;

public:
    void X_Move(sint16 _Position)
    {
        // X_MotorPuls = _Position * microStep;
        X_MotorPuls = _Position * RotationtoLiner;
#ifdef StichPulsDisplay
        Serial.println("*XPlusData*");
        Serial.println(X_MotorPuls, DEC);
        Serial.println("******");
#endif
    }
    void Y_Move(sint16 _Position)
    {
        // Y_MotorPuls = _Position * microStep;
        Y_MotorPuls = _Position * RotationtoLiner;
#ifdef StichPulsDisplay
        Serial.println("*YPlusData*");
        Serial.println(Y_MotorPuls, DEC);
        Serial.println("******");
#endif
    }
    void X_Move_One_MM(sint16 _Position)
    {
        _Position = _Position * 10;
        X_MotorPuls = _Position * RotationtoLiner;
    }
    void Y_Move_One_MM(sint16 _Position)
    {
        _Position = _Position * 10;
        Y_MotorPuls = _Position * RotationtoLiner;
    }

    //----------------------------------------------------------
    void Y_FrameFreeMove(int8 Y_Move_Status)
    {
        if (Y_Move_Status == Move)
        {
            if (Y_LastStatus == Stop)
            {
                Y_LastStatus = Move;
                if (FrameSpeed == StichingStichSpeed || FrameSpeed == Stiching_F_R_StichSpeed)
                {
                    Yaxis
                        .setMaxSpeed(ManualFrameSpeed)      // steps/s
                        .setAcceleration(ManualFrameAccel); // steps/s
                    Xaxis
                        .setMaxSpeed(ManualFrameSpeed)      // steps/s
                        .setAcceleration(ManualFrameAccel); // steps/s
                    FrameSpeed = ManualFrameMoveSpeed;
                    Serial.println("PauseStichingSpeedY");
                }
                // if (cMCS == mManual)
                // {
                else if (FrameSpeed != ManualFrameMoveSpeed)
                {
                    Yaxis
                        .setMaxSpeed(ManualFreeFrameSpeed)      // steps/s
                        .setAcceleration(ManualFreeFrameAccel); // steps/s
                    Xaxis
                        .setMaxSpeed(ManualFreeFrameSpeed)      // steps/s
                        .setAcceleration(ManualFreeFrameAccel); // steps/s
                    FrameSpeed = ManualFrameMoveSpeed;
                    Serial.println("ManualSpeedXY");
                }
                // }
                Yaxis.setTargetRel(Y_MotorPuls);
                controllerY.moveAsync(Yaxis);
                Serial.println("Start_Y");
#ifdef ON_OFF_withfreeMove
                ON_All_motor();
#endif
            }
        }
        else if (Y_Move_Status == Stop)
        {
            if (Y_LastStatus == Move)
            {
                controllerY.stopAsync();
                while (controllerY.isRunning())
                {
                }
                Y_LastStatus = Initiat_Stop;
                Serial.println("Initiat_Stop_Y");
            }
            if (Y_LastStatus == Initiat_Stop)
            {
                if (controllerY.isRunning())
                {
                    //Do nothing
                }
                else
                {
                    Yaxis.setPosition(0);
                    Y_LastStatus = Stop;
                    Serial.println("Stop_Y");
#ifdef ON_OFF_withfreeMove
                    OFF_All_motor();
#endif
                }
            }
        }
        else if (Y_Move_Status == MStop)
        {
            if (Y_LastStatus == Move)
            {
                controllerY.stopAsync(); // starts deceleration
                while (controllerY.isRunning())
                {
                }
                Y_LastStatus = Initiat_Stop;
                Serial.println("Initiat_MStop_Y");
            }
            if (Y_LastStatus == Initiat_Stop)
            {
                if (controllerY.isRunning())
                {
                    // Do nothing
                }
                else
                {
                    Yaxis.setPosition(0);
                    Y_LastStatus = Stop;
                    Serial.println("MStop_Y");
#ifdef ON_OFF_withfreeMove
                    OFF_All_motor();
#endif
                }
            }
        }
    }
    //--------------------------------------------------------------------
    void X_FrameFreeMove(int8 X_Move_Status)
    {
        if (X_Move_Status == Move)
        {
            if (X_LastStatus == Stop)
            {
                X_LastStatus = Move;
                if (FrameSpeed == StichingStichSpeed || FrameSpeed == Stiching_F_R_StichSpeed)
                {
                    Yaxis
                        .setMaxSpeed(ManualFrameSpeed)      // steps/s
                        .setAcceleration(ManualFrameAccel); // steps/s
                    Xaxis
                        .setMaxSpeed(ManualFrameSpeed)      // steps/s
                        .setAcceleration(ManualFrameAccel); // steps/s
                    FrameSpeed = ManualFrameMoveSpeed;
                    Serial.println("PauseStichingSpeedY");
                }
                // if (_cMCS == mManual)
                // {
                // }
                else if (FrameSpeed != ManualFrameMoveSpeed)
                {
                    Yaxis
                        .setMaxSpeed(ManualFreeFrameSpeed)      // steps/s
                        .setAcceleration(ManualFreeFrameAccel); // steps/s
                    Xaxis
                        .setMaxSpeed(ManualFreeFrameSpeed)      // steps/s
                        .setAcceleration(ManualFreeFrameAccel); // steps/s
                    FrameSpeed = ManualFrameMoveSpeed;
                    Serial.println("ManualSpeedXY");
                }
                // }
                Xaxis.setTargetRel(X_MotorPuls);
                controllerX.moveAsync(Xaxis);
                Serial.println("Start_X");
#ifdef ON_OFF_withfreeMove
                ON_All_motor();
#endif
            }
        }
        else if (X_Move_Status == Stop)
        {
            if (X_LastStatus == Move)
            {
                controllerX.stopAsync(); // starts deceleration
                while (controllerX.isRunning())
                {
                }
                X_LastStatus = Initiat_Stop;
                Serial.println("Initiat_Stop_X");
            }
            if (X_LastStatus == Initiat_Stop)
            {
                if (controllerX.isRunning())
                {
                    //Do nothing
                }
                else
                {
                    Xaxis.setPosition(0);
                    X_LastStatus = Stop;
                    Serial.println("Stop_X");
#ifdef ON_OFF_withfreeMove
                    OFF_All_motor();
#endif
                }
            }
        }
        else if (X_Move_Status == MStop)
        {
            if (X_LastStatus == Move)
            {
                controllerX.stopAsync(); // starts deceleration
                while (controllerX.isRunning())
                {
                }
                X_LastStatus = Initiat_Stop;
                Serial.println("Initiat_MStop_X");
            }
            if (X_LastStatus == Initiat_Stop)
            {
                if (controllerX.isRunning())
                {
                    // Do nothing
                }
                else
                {
                    Xaxis.setPosition(0);
                    X_LastStatus = Stop;
                    Serial.println("MStop_X");
#ifdef ON_OFF_withfreeMove
                    OFF_All_motor();
#endif
                }
            }
        }
    }
    //--------------------------------------------------------------------
    void XYZ_StichMove()
    {
        // Serial.println(Y_MotorPuls, DEC);
        // Serial.println(X_MotorPuls, DEC);
        // Serial.println(Z_MotorPuls, DEC);
        // Yaxis.setTargetRel(Y_MotorPuls);
        // Xaxis.setTargetRel(X_MotorPuls);
        // controllerXY.moveAsync(Xaxis, Yaxis);
        if (Y_MotorPuls != 0 && X_MotorPuls != 0)
        {
            Yaxis.setTargetRel(Y_MotorPuls);
            Xaxis.setTargetRel(X_MotorPuls);
            controllerXY.moveAsync(Xaxis, Yaxis);
        }
        else
        {
            if (Y_MotorPuls != 0)
            {
                Yaxis.setTargetRel(Y_MotorPuls);
                controllerXY.moveAsync(Yaxis);
            }
            else if (X_MotorPuls != 0)
            {
                Xaxis.setTargetRel(X_MotorPuls);
                controllerXY.moveAsync(Xaxis);
            }
            else
            {
                // do nothing because both are Zero Pluses
            }
        }
        if (Z_MotorPuls != 0)
        {
            Zaxis.setTargetRel(Z_MotorPuls);
            controllerZ.moveAsync(Zaxis);
        }
    }
    void XY_StichMove()
    {
        // Serial.println(Y_MotorPuls, DEC);
        // Serial.println(X_MotorPuls, DEC);
        if (Y_MotorPuls != 0 && X_MotorPuls != 0)
        {
            Yaxis.setTargetRel(Y_MotorPuls);
            Xaxis.setTargetRel(X_MotorPuls);
            controllerXY.moveAsync(Xaxis, Yaxis);
        }
        else
        {
            if (Y_MotorPuls != 0)
            {
                Yaxis.setTargetRel(Y_MotorPuls);
                controllerXY.moveAsync(Yaxis);
            }
            else if (X_MotorPuls != 0)
            {
                Xaxis.setTargetRel(X_MotorPuls);
                controllerXY.moveAsync(Xaxis);
            }
            else
            {
                // do nothing because both are Zero Pluses
            }
        }
    }
    //--------------------------------------------------------------------
    uint8_t XYZ_StichStatus()
    {
        if (controllerXY.isRunning())
        {
            return WaitXY;
        }
        else if (controllerZ.isRunning())
        {
            return WaitZ;
        }
        else
        {
            if (controllerXY.isOk() && controllerZ.isOk())
            {
                return AvailableToMove;
            }
            else
            {
                return Wait;
            }
        }
        return Wait;
    }
    // uint8_t XY_StichStatus()
    // {
    //     if (controllerXY.isRunning())
    //     {
    //         return WaitXY;
    //     }
    //     else
    //     {
    //         if (controllerXY.isOk())
    //         {
    //             return AvailableToMove;
    //         }
    //         else
    //         {
    //             return Wait;
    //         }
    //     }
    //     return Wait;
    // }
    // uint8_t Z_StichStatus()
    // {
    //     if (controllerZ.isRunning())
    //     {
    //         return WaitZ;
    //     }
    //     else
    //     {
    //         if (ontrollerZ.isOk())
    //         {
    //             return AvailableToMove;
    //         }
    //         else
    //         {
    //             return Wait;
    //         }
    //     }
    //     return Wait;
    // }
    //--------------------------------------------------------------------
    /**/
    //Setup_StichFrameSpeed
    /**/
    void Setup_StichFrameSpeed()
    {
        NM.Setup_StichNeedleSpeed();
        if (FrameSpeed != StichingStichSpeed)
        {
            FrameSpeed = StichingStichSpeed;
#ifdef debugSetup_StichFrameSpeed
            Serial.println("Frame Speed Stiching");
#endif
            // Zaxis
            //     .setMaxSpeed(StichNeedleSpeed)      // steps/s HBS507 200,000 max step Accepted
            //     .setAcceleration(StichNeedleAccel); // steps/s^2
            Yaxis
                .setMaxSpeed(StichFrameSpeed)      // steps/s
                .setAcceleration(StichFrameAccel); // steps/s^2
            Xaxis
                .setMaxSpeed(StichFrameSpeed)      // steps/s
                .setAcceleration(StichFrameAccel); // steps/s^2
        }
    }
    /**/
    //Setup_Stich_F_R_FrameSpeed
    /**/
    void Setup_Stich_F_R_FrameSpeed()
    {
        if (FrameSpeed != Stiching_F_R_StichSpeed)
        {
            FrameSpeed = Stiching_F_R_StichSpeed;
#ifdef debugSetup_StichFrameSpeed
            Serial.println("Frame_F_R_Speed Stiching");
#endif
            // Zaxis
            //     .setMaxSpeed(StichNeedleSpeed)      // steps/s HBS507 200,000 max step Accepted
            //     .setAcceleration(StichNeedleAccel); // steps/s^2
            Yaxis
                .setMaxSpeed(Stich_F_R_FrameSpeed)      // steps/s
                .setAcceleration(Stich_F_R_FrameAccel); // steps/s^2
            Xaxis
                .setMaxSpeed(Stich_F_R_FrameSpeed)      // steps/s
                .setAcceleration(Stich_F_R_FrameAccel); // steps/s^2
        }
    }
    //--------------------------------------------------------------------
    /**/
    //Setup_JumpFrameSpeed
    /**/
    void Setup_JumpFrameSpeed()
    {
        if (FrameSpeed != JumpingStichSpeed)
        {
            FrameSpeed = JumpingStichSpeed;
#ifdef debugSetup_JumpFrameSpeed
            Serial.println("Frame Speed Jumping");
#endif
            Yaxis
                .setMaxSpeed(JumpFrameSpeed)      // steps/s
                .setAcceleration(JumpFrameAccel); // steps/s^2
            Xaxis
                .setMaxSpeed(JumpFrameSpeed)      // steps/s
                .setAcceleration(JumpFrameAccel); // steps/s^2
        }
    }
    //--------------------------------------------------------------------
    void ManualFrameMove()
    {
        if (X_MotorPuls >= 0)
        {
            if (digitalRead(Left_SW) == Open)
            {
                Xaxis.setTargetRel(X_MotorPuls);
                Serial.println(X_MotorPuls, DEC);
            }
            else
            {
                Xaxis.setTargetRel(0);
            }
        }
        else if (X_MotorPuls <= 0)
        {
            if (digitalRead(Right_SW) == Open)
            {
                Xaxis.setTargetRel(X_MotorPuls);
                Serial.println(X_MotorPuls, DEC);
            }
            else
            {
                Xaxis.setTargetRel(0);
            }
        }
        //----------------------------------------------
        if (Y_MotorPuls >= 0)
        {
            if (digitalRead(BackWard_SW) == Open)
            {
                if (!controllerY.isRunning())
                {
                    Yaxis.setTargetRel(Y_MotorPuls);
                    controllerY.moveAsync(Yaxis);
                    Serial.println(Y_MotorPuls, DEC);
                }
            }
            else
            {
                controllerX.stopAsync(); // starts deceleration
                Y_MotorPuls = 0;
            }
        }
        else if (Y_MotorPuls <= 0)
        {
            if (digitalRead(Forward_SW) == Open)
            {
                if (!controllerY.isRunning())
                {
                    Yaxis.setTargetRel(Y_MotorPuls);
                    controllerY.moveAsync(Yaxis);
                    Serial.println(Y_MotorPuls, DEC);
                }
            }
            else
            {
                controllerX.stopAsync(); // starts deceleration
                Y_MotorPuls = 0;
            }
        }
        //--------------------------------------
        // controllerX.moveAsync(Xaxis);

        if (controllerX.isRunning() || controllerY.isRunning())
        {
            if (digitalRead(Forward_SW) == Open)
            {
                //controllerY.emergencyStop();
                controllerY.stopAsync(); // starts deceleration
                Y_MotorPuls = 0;
            }
            if (digitalRead(Forward_SW) == Open)
            {
                // controllerY.emergencyStop();
                controllerY.stopAsync(); // starts deceleration
                Y_MotorPuls = 0;
            }
            if (digitalRead(Left_SW) == Open)
            {
                //controllerX.emergencyStop();
                controllerX.stopAsync(); // starts deceleration
            }
            if (digitalRead(Right_SW) == Open)
            {
                // controllerX.emergencyStop();
                controllerX.stopAsync(); // starts deceleration
            }
        }
        // X_MotorPuls = 0;
        // Y_MotorPuls = 0;
    }
    void SetSpeedXZERO()
    {
        if (FrameSpeed != HomingFrameMoveSpeed)
        {
            Serial.println("SetSpeed XY ZERO");
            Xaxis
                .setMaxSpeed(FramHomeSpeed)      // steps/s
                .setAcceleration(FramHomeAccel); // steps/s
            Yaxis
                .setMaxSpeed(FramHomeSpeed)      // steps/s
                .setAcceleration(FramHomeAccel); // steps/s
            FrameSpeed = HomingFrameMoveSpeed;
        }
    }
    void SetSpeedYZERO()
    {
        if (FrameSpeed != HomingFrameMoveSpeed)
        {
            Serial.println("SetSpeed XY ZERO");
            Xaxis
                .setMaxSpeed(FramHomeSpeed)      // steps/s
                .setAcceleration(FramHomeAccel); // steps/s
            Yaxis
                .setMaxSpeed(FramHomeSpeed)      // steps/s
                .setAcceleration(FramHomeAccel); // steps/s
            FrameSpeed = HomingFrameMoveSpeed;
        }
    }
};
FrameMove FM;
#endif