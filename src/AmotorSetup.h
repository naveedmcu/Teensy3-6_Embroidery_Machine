#ifndef A_MOTOR_SETUP_H
#define A_MOTOR_SETUP_H
#include <Arduino.h>
#include <math.h>
#include <elapsedMillis.h>
#include "debugSerial.h"
#include "mainProjectDefinition.h"
#include "MotorsSetup.h"
#include "LimitSW_Driver.h"
#include "mainProjectVar.h"
#include "mainProjectVar.h"
/**/
#define AmotorHomePosition 1
/**/
#define AMotorHomePositoin_SW 37
/**/
#define AmotorHomeSPEED 5000
#define AmotorHomeAcceleration 200000
/**/
#define ShuttleFreeSpeed 20000
#define ShuttleFreeAccel 200000
/**/
#define StichShuttleSpeed 65000  // servo motor 60000  perfect 1000 plus perR/evolution
#define StichShuttleAccel 330000 // servo motor 490000
// #define StichShuttleSpeed 5000   // servo motor
// #define StichShuttleAccel 100000 // servo motor

volatile sint32 A_MotorPuls = 0;
volatile sint8 vAstatus = 0;

ThreadPositionSetting SPS; //ShuttlePositionSetting

enum
{
    vAundefine = 1,
    vAinit_Stop,
    vAinit_Start
};
void AStopEmergencyStop()
{
    controllerY.emergencyStop();
    vAstatus = vAinit_Stop;
}

class shuttleMove
{
private:
    /**/
    elapsedMillis waitingforHomePosition = 0;
    float oldA_MotorPuls = 0;
    uint8_t Astatus = nodefine;
    /**/
    float_t AmotorMicroStepping = 1;
    float_t GearRatio = 0.5;
    float_t fullRoundPulsesAmotor = 1000.0 * GearRatio;
    float_t halfRoundPulsesAmotor = fullRoundPulsesAmotor / 2.0;
    float plusesperDegree = fullRoundPulsesAmotor / 360.0;
    float plusesper180Degree = 180.0 * plusesperDegree;
    /**/
    typedef enum
    {
        ShuttleSpeedundefine = 1,
        JumpingShuttleSpeed,
        StichingShuttleSpeed,
        /**/
        ManualShuttleMoveSpeed,
        HomingShuttleMoveSpeed
        /**/
    } FS;
    FS ShuttleSpeed = ShuttleSpeedundefine;

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
    void AmotorInit()
    {
        pinMode(AMotorHomePositoin_SW, INPUT_PULLDOWN);
    }
    uint8_t A_StichStatus()
    {
        if (controllerY.isRunning())
        {
            return WaitA;
        }
        else
        {
            if (controllerY.isOk())
            {
                return AvailableToMove;
            }
            else
            {
                return Wait;
            }
        }
    }
    void AmotorHomingSpeedSet()
    {
        if (ShuttleSpeed != HomingShuttleMoveSpeed)
        {
#ifdef debugSetup_StichFrameSpeed
            Serial.println("Set Homing Speed AZERO");
#endif
            Aaxis
                .setMaxSpeed(AmotorHomeSPEED)             // steps/s
                .setAcceleration(AmotorHomeAcceleration); // steps/s
            ShuttleSpeed = HomingShuttleMoveSpeed;
        }
    }
    void Setup_StichShuttleSpeed()
    {
        if (ShuttleSpeed != StichingShuttleSpeed)
        {
#ifdef debugSetup_StichFrameSpeed
            Serial.println("Shuttle Speed Stiching");
#endif
            Aaxis
                .setMaxSpeed(StichShuttleSpeed)      // steps/s
                .setAcceleration(StichShuttleAccel); // steps/s
            ShuttleSpeed = StichingShuttleSpeed;
        }
    }
    /**/
    // Shuttle-Thread-Position
    /**/
    void ShuttleThreadPosition(sint32 _ShuttleThreadPosition_)
    {
        A_StichFollowMove(_ShuttleThreadPosition_);
    }
    /**/

    void A_StichRoundMove(sint32 _PlusesData_)
    {
        sint32 tempPlusData = 0;
        tempPlusData = abs(_PlusesData_);
        A_MotorPuls = tempPlusData + fullRoundPulsesAmotor;
        if (_PlusesData_ > 0)
        {
            A_MotorPuls = A_MotorPuls;
        }
        else if (_PlusesData_ < 0)
        {
            A_MotorPuls = -A_MotorPuls;
        }
        // Serial.println(A_MotorPuls, DEC);
        Aaxis.setTargetRel(A_MotorPuls);
        controllerY.moveAsync(Aaxis);
    }
    void A_StichFollowMove(sint32 _PlusesData_)
    {
        A_MotorPuls = _PlusesData_;
        // Serial.println(Z_MotorPuls, DEC);
        if (A_MotorPuls != 0)
        {
            // if (A_MotorPuls >= 0)
            // {
            //     A_MotorPuls = A_MotorPuls;
            // }
            // else if (A_MotorPuls <= 0)
            // {
            //     A_MotorPuls = A_MotorPuls;
            // }
            A_MotorPuls = A_MotorPuls;
            Aaxis.setTargetRel(A_MotorPuls);
            controllerY.moveAsync(Aaxis);
        }
    }
    /**/
    void A_Move(float_t _Degree)
    {
        float newA_MotorPuls;
        sint32 differentA_MotorPuls = 0;
        sint32 resultA_MotorPuls = 0;
        sint32 absresultA_MotorPuls = 0;
        CurrentDegree = _Degree;
        newA_MotorPuls = _Degree * plusesperDegree;
        newA_MotorPuls = round(newA_MotorPuls);
        differentA_MotorPuls = newA_MotorPuls - oldA_MotorPuls;

        absresultA_MotorPuls = abs(differentA_MotorPuls);
        if (absresultA_MotorPuls > halfRoundPulsesAmotor)
        {
            if (newA_MotorPuls > oldA_MotorPuls)
            {
                resultA_MotorPuls = absresultA_MotorPuls - fullRoundPulsesAmotor;
            }
            else
            {
                resultA_MotorPuls = fullRoundPulsesAmotor - absresultA_MotorPuls;
            }
        }
        else if (absresultA_MotorPuls < halfRoundPulsesAmotor)
        {
            resultA_MotorPuls = differentA_MotorPuls;
        }
        else if (absresultA_MotorPuls == halfRoundPulsesAmotor)
        {
            resultA_MotorPuls = differentA_MotorPuls;
        }
#ifdef Debug_A_Move
        Serial.print("differentA_MotorPuls:");
        Serial.println(differentA_MotorPuls, DEC);

        Serial.print("oldA_MotorPuls:");
        Serial.println(oldA_MotorPuls, DEC);

        Serial.print("newA_MotorPuls:");
        Serial.println(newA_MotorPuls, DEC);

        Serial.print("resultA_MotorPuls:");
        Serial.println(resultA_MotorPuls, DEC);
#endif
        oldA_MotorPuls = newA_MotorPuls;
        A_MotorPuls = resultA_MotorPuls;
        A_MotorPuls = A_MotorPuls * AmotorMicroStepping;
#ifdef StichPulsDisplay
        Serial.println("*APlusData*");
        Serial.println(A_MotorPuls, DEC);
        Serial.println("******");
#endif
    }
    /**/
    uint8_t A_Axis_Homing()
    {
        if (Astatus == HomingError)
        {
            Astatus = nodefine;
            Serial.println("A was ERROR!!");
        }
        if (Astatus == ForceStop)
        {
            Astatus = nodefine;
            Serial.println("A was ForceStop!!");
        }
        if (Astatus == HomingDone)
        {
            Astatus = nodefine;
            Serial.println("A WAS homming!!");
        }
        if (Astatus == nodefine)
        {
            vAstatus = vAundefine;
            AmotorHomingSpeedSet();
            attachInterrupt(digitalPinToInterrupt(AMotorHomePositoin_SW), AStopEmergencyStop, RISING);
            waitingforHomePosition = 0;
            Serial.println("A HomingStart");
        }
        if (digitalRead(AMotorHomePositoin_SW) == AmotorHomePosition || vAstatus == vAinit_Stop)
        {
            if (Astatus != Init_Stop)
            {
                detachInterrupt(digitalPinToInterrupt(AMotorHomePositoin_SW));
                controllerY.emergencyStop();
                Serial.println("A Init_Stop");
                Aaxis.setPosition(0); // set start position of counter use after homming position
                Astatus = HomingDone;
                oldA_MotorPuls = 0;
                CurrentDegree = 0;
                Serial.println("A HomingDone");
            }
        }
        else if (Astatus == nodefine || Astatus == Init_Move)
        {
            if (Astatus != Init_Move)
            {
                Aaxis.setTargetRel(16000);
                controllerY.moveAsync(Aaxis);
                Astatus = Init_Move;
                waitingforHomePosition = 0;
                Serial.println("A Running!!");
            }
            if (Astatus == Init_Move)
            {
                if (waitingforHomePosition > 5000)
                {
                    controllerY.emergencyStop();
                    waitingforHomePosition = 0;
                    Serial.println("A Homing Error");
                    Astatus = HomingError;
                }
            }
        }

        if (Astatus == Init_Stop)
        {
            Astatus = nodefine;
            controllerZ.emergencyStop();
            Serial.println("A Force Stopped");
        }

        return Astatus;
    }
    void StopAmotorEmergency()
    {
        if (Astatus != ForceStop)
        {
            Astatus = ForceStop;
            controllerY.emergencyStop();
            Serial.println("A Force Stopped");
        }
    }
};
shuttleMove SM;
#endif
