#ifndef Z_MOTOR_SETUP_H
#define Z_MOTOR_SETUP_H
#include <Arduino.h>
#include <elapsedMillis.h>
#include "debugSerial.h"
#include <math.h>
#include "mainProjectDefinition.h"
#include "MotorsSetup.h"
#include "LimitSW_Driver.h"
#include "mainProjectVar.h"
#include "AmotorSetup.h"
/**/
// Servo motor test free move
// 65000
// 400000
/**/
/**/
#define ZmotorHomeSPEED 1000         //
#define ZmotorHomeAcceleration 50000 //
/**/
// #define NeedleFreeSpeed 10000
// #define NeedleFreeAccel 200000
#define NeedleFreeSpeed 7000   //  13500 simple stepper
#define NeedleFreeAccel 375000 //  375000 simple stepper
/**/
//#define StichNeedleSpeed 90000 // servo motor
//#define StichNeedleAccel 290000 // servo motor
#define StichNeedleSpeed 7500   //15000 // 53000 only Simple Stepper motor  // 7000
#define StichNeedleAccel 420000 //220000 only Simple Stepper motor
/**/
volatile sint32 Z_MotorPuls = 0;
volatile sint8 vZstatus = 0;
enum
{
    vundefine = 1,
    vInit_Stop,
    vInit_Start
};

void ZStopEmergencyStop()
{
    controllerZ.emergencyStop();
    vZstatus = vInit_Stop;
}

class needleMove
{
private:
    elapsedMillis waitingforHomePosition = 0;
    uint8_t Zstatus = nodefine;
    float oldZ_MotorPuls = 0;
    int8 Z_LastStatus = Stop;
/*360/4000=0.09dgree per puls    
    */
// #define Z_OneDegreePuls (1 / 0.009)
#define GearRatio 5.0 // as Zmotor Microstepping
#define ZmotorMicroStepping 5.0
#define fullRoundPulses 200.0
#define halfRoundPulses 100.0
    // #define onePlusDegree (360 / fullRoundPulses)
    // #define plusesperDegree (1 / onePlusDegree)
    float plusesperDegree = fullRoundPulses / 360.0;
    float plusesper180Degree = 180.0 * plusesperDegree;
    typedef enum
    {
        JumpingNeedleSpeed = 1,
        StichingNeedleSpeed,
        /**/
        ManualNeedleMoveSpeed,
        HomingNeedleMoveSpeed
        /**/
    } FS;
    FS NeedleSpeed;

public:
    float CurrentDegree = 0;
    enum
    {
        nodefine,
        Init_Stop,
        Init_Move,
        HomingDone,
        HomingError,
        ForceStop
    };
    /**/
    void Z_StichMove()
    {
        // Serial.println(Z_MotorPuls, DEC);
        if (Z_MotorPuls != 0)
        {
            Zaxis.setTargetRel(Z_MotorPuls);
            controllerZ.moveAsync(Zaxis);
        }
    }
    /**/
    float get_atan(uint8_t Y_Distance, uint8_t X_Distance)
    {
        float rad_1;
        float result;
        rad_1 = ((float)Y_Distance / (float)X_Distance);
        result = atan(rad_1);
        rad_1 = result * 57.295779;
        //  Serial.println(rad_1);
        rad_1 = round(rad_1);
        return rad_1;
    }
    void Z_Move(float_t _Degree)
    {
        float newZ_MotorPuls;
        sint32 differentZ_MotorPuls = 0;
        sint32 resultZ_MotorPuls = 0;
        sint32 absresultZ_MotorPuls = 0;
        CurrentDegree = _Degree;
        newZ_MotorPuls = _Degree * plusesperDegree;
        newZ_MotorPuls = round(newZ_MotorPuls);
        differentZ_MotorPuls = newZ_MotorPuls - oldZ_MotorPuls;

        absresultZ_MotorPuls = abs(differentZ_MotorPuls);
        if (absresultZ_MotorPuls > halfRoundPulses)
        {
            if (newZ_MotorPuls > oldZ_MotorPuls)
            {
                resultZ_MotorPuls = absresultZ_MotorPuls - fullRoundPulses;
            }
            else
            {
                resultZ_MotorPuls = fullRoundPulses - absresultZ_MotorPuls;
            }
        }
        else if (absresultZ_MotorPuls < halfRoundPulses)
        {
            resultZ_MotorPuls = differentZ_MotorPuls;
        }
        else if (absresultZ_MotorPuls == halfRoundPulses)
        {
            resultZ_MotorPuls = differentZ_MotorPuls;
        }
#ifdef Debug_Z_Move
        Serial.print("differentZ_MotorPuls:");
        Serial.println(differentZ_MotorPuls, DEC);

        Serial.print("oldZ_MotorPuls:");
        Serial.println(oldZ_MotorPuls, DEC);

        Serial.print("newZ_MotorPuls:");
        Serial.println(newZ_MotorPuls, DEC);

        Serial.print("resultZ_MotorPuls:");
        Serial.println(resultZ_MotorPuls, DEC);
#endif
        oldZ_MotorPuls = newZ_MotorPuls;
        Z_MotorPuls = resultZ_MotorPuls;
        Z_MotorPuls = Z_MotorPuls * ZmotorMicroStepping;
#ifdef StichPulsDisplay
        Serial.println("*ZPlusData*");
        Serial.println(Z_MotorPuls, DEC);
        Serial.println("******");
#endif
        SM.A_Move(_Degree);
    }

    void InitialPosition270Degree()
    {
        // oldZ_MotorPuls = 6000;
        // Zaxis.setTargetRel(6000);
        Z_degree = 270;
        Z_Move(Z_degree);
        Zaxis.setTargetRel(Z_MotorPuls);
        controllerZ.moveAsync(Zaxis);
        SM.A_StichFollowMove(A_MotorPuls);
        while (controllerZ.isRunning())
        {
            ;
        }
        while (controllerY.isRunning())
        {
            ;
        }
    }

    float GET_DEGREE(uint8_t _function, uint8_t _Y_Distance, uint8_t _X_Distance)
    {
        static char function_id = 0;
        float Z_Degree = 0;
        float rad;
        uint8_t function;
        uint8_t Y_Distance;
        uint8_t X_Distance;
        function = _function;
        Y_Distance = _Y_Distance;
        X_Distance = _X_Distance;

        function_id = function & 0x0F;
        if (function == 128 || function == 129 || function == 136 || function == 137 || function == 138)
        {
            if (X_Distance == 0)
            {
                Z_Degree = 90.0;
            }
            else if (Y_Distance == 0)
            {
                Z_Degree = 0.0;
            }
            else
            {
                Z_Degree = get_atan(Y_Distance, X_Distance);
            }
        }
        //____________________________//
        else if (function == 160 || function == 161 || function == 168 || function == 169 || function == 170)
        {
            if (X_Distance == 0)
            {
                Z_Degree = 90.0;
            }
            else if (Y_Distance == 0)
            {
                Z_Degree = 180.0;
            }
            else
            {
                rad = get_atan(Y_Distance, X_Distance);
                Z_Degree = 180.0 - rad;
            }
        }
        //____________________________//
        else if (function == 224 || function == 225 || function == 232 || function == 233 || function == 234)
        {
            if (X_Distance == 0)
            {
                Z_Degree = 270.0;
            }
            else if (Y_Distance == 0)
            {
                Z_Degree = 180.0;
            }
            else
            {
                rad = get_atan(Y_Distance, X_Distance);
                Z_Degree = rad + 180.0;
            }
        }
        //____________________________//
        else if (function == 192 || function == 193 || function == 200 || function == 201 || function == 202)
        {
            if (X_Distance == 0)
            {
                Z_Degree = 270.0;
            }
            else if (Y_Distance == 0)
            {
                Z_Degree = 360.0;
            }
            else
            {
                rad = get_atan(Y_Distance, X_Distance);
                Z_Degree = 360.0 - rad;
            }
        }
        if (function_id == 128)
        {
        }
        // Serial.println(Z_Degree);
        return Z_Degree;
    }
    /**/
    void ZmotorHomingSpeedSet()
    {
        if (NeedleSpeed != HomingNeedleMoveSpeed)
        {
            Serial.println("Set Homing Speed ZZERO");
            Zaxis
                .setMaxSpeed(ZmotorHomeSPEED)             // steps/s
                .setAcceleration(ZmotorHomeAcceleration); // steps/s
            NeedleSpeed = HomingNeedleMoveSpeed;
        }
    }
    /**/
    uint8_t Z_Axis_Homing()
    {
        if (Zstatus == HomingError)
        {
            Zstatus = nodefine;
            Serial.println("Z was ERROR!!");
        }
        if (Zstatus == ForceStop)
        {
            Zstatus = nodefine;
            Serial.println("Z was ForceStop!!");
        }
        if (Zstatus == HomingDone)
        {
            Zstatus = nodefine;
            Serial.println("Z WAS homming!!");
        }
        if (Zstatus == nodefine)
        {
            vZstatus = vundefine;
            ZmotorHomingSpeedSet();
            attachInterrupt(digitalPinToInterrupt(HomePositoin_SW), ZStopEmergencyStop, FALLING);
            waitingforHomePosition = 0;
            Serial.println("Z HomingStart");
        }
        if (digitalRead(HomePositoin_SW) == HomePosition || vZstatus == vInit_Stop)
        {
            if (Zstatus != Init_Stop)
            {
                detachInterrupt(digitalPinToInterrupt(HomePositoin_SW));
                controllerZ.emergencyStop();
                Serial.println("Z Init_Stop");
                Zaxis.setPosition(0); // set start position of counter use after homming position
                Zstatus = HomingDone;
                oldZ_MotorPuls = 0;
                CurrentDegree = 0;
                Serial.println("Z HomingDone");
            }
        }
        else if (Zstatus == nodefine || Zstatus == Init_Move)
        {
            if (Zstatus != Init_Move)
            {
                Zaxis.setTargetRel(16000);
                controllerZ.moveAsync(Zaxis);
                Zstatus = Init_Move;
                waitingforHomePosition = 0;
                Serial.println("Z Running!!");
            }
            if (Zstatus == Init_Move)
            {
                if (waitingforHomePosition > 5000)
                {
                    controllerZ.emergencyStop();
                    waitingforHomePosition = 0;
                    Serial.println("Z Homing Error");
                    Zstatus = HomingError;
                }
            }
        }

        if (Zstatus == Init_Stop)
        {
            Zstatus = nodefine;
            controllerZ.emergencyStop();
            Serial.println("Z Force Stopped");
        }

        return Zstatus;
    }
    void StopZmotorEmergency()
    {
        if (Zstatus != ForceStop)
        {
            Zstatus = ForceStop;
            controllerZ.emergencyStop();
            Serial.println("Z Force Stopped");
        }
    }
    //--------------------------------------------------------------------
    void Setup_StichNeedleSpeed()
    {
        if (NeedleSpeed != StichingNeedleSpeed)
        {
            NeedleSpeed = StichingNeedleSpeed;
#ifdef debugSetup_StichFrameSpeed
            Serial.println("NEEDLE Speed Stiching");
#endif
            Zaxis
                .setMaxSpeed(StichNeedleSpeed)      // steps/s HBS507 200,000 max step Accepted
                .setAcceleration(StichNeedleAccel); // steps/s^2
        }
        SM.Setup_StichShuttleSpeed();
    }
    void Z_SimpleMove(sint32 _Degree)
    {
        _Degree = _Degree * GearRatio;
        Z_MotorPuls = _Degree;
        // Z_MotorPuls = Z_OneDegreePuls * _Degree;
    }
    void Z_Move_One_MM(sint32 _Degree)
    {
        _Degree = _Degree * GearRatio;
        Z_MotorPuls = _Degree;
    }
    // void Z_StichMove()
    // {
    //     // Serial.println(Z_MotorPuls, DEC);
    //     Zaxis.setTargetRel(Z_MotorPuls);
    //     controllerZ.moveAsync(Zaxis);
    // }
    //----------------------------------------------------------
    void Z_FrameFreeMove(int8 Z_Move_Status)
    {
        if (Z_Move_Status == Move)
        {
            if (Z_LastStatus == Stop)
            {
                Z_LastStatus = Move;
                if (NeedleSpeed != ManualNeedleMoveSpeed)
                {
                    Zaxis
                        .setMaxSpeed(NeedleFreeSpeed)      // steps/s
                        .setAcceleration(NeedleFreeAccel); // steps/s
                    NeedleSpeed = ManualNeedleMoveSpeed;
#ifdef debugSetup_StichFrameSpeed
                    Serial.println("NEEDLE_FREE_MoveSpeed");
#endif
                }
                Zaxis.setTargetRel(Z_MotorPuls);
                controllerZ.moveAsync(Zaxis);
                Serial.println("Start_Z");
            }
        }
        else if (Z_Move_Status == Stop)
        {
            if (Z_LastStatus == Move)
            {
                // controllerZ.emergencyStop();
                controllerZ.stopAsync();
                while (controllerZ.isRunning())
                {
                }
                Z_LastStatus = Initiat_Stop;
                Serial.println("Initiat_Stop_Z");
            }
            if (Z_LastStatus == Initiat_Stop)
            {
                if (controllerZ.isRunning())
                {
                    //Do nothing
                }
                else
                {
                    Z_LastStatus = Stop;
                    Serial.println("Stop_Z");
                }
            }
        }
        else if (Z_Move_Status == MStop)
        {
            if (Z_LastStatus == Move)
            {
                controllerZ.emergencyStop();
                while (controllerZ.isRunning())
                {
                }
                Z_LastStatus = Initiat_Stop;
                Serial.println("Initiat_MStop_Z");
            }
            if (Z_LastStatus == Initiat_Stop)
            {
                if (controllerZ.isRunning())
                {
                    // Do nothing
                }
                else
                {
                    /**/
                    Zaxis.setPosition(0); // Zero Z motor
                    oldZ_MotorPuls = 0;   // sEt degree to zero
                    CurrentDegree = 0;    // curent degree to zero
                    /**/
                    Z_LastStatus = Stop;
                    Serial.println("MStop_Z");
                    Serial.println("Z-Degree Set 0");
                }
            }
        }
    }
};
needleMove NM;

#endif
