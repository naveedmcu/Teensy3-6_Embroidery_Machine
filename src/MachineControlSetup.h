#ifndef MACHINE_CONTROL_SETUP_H
#define MACHINE_CONTROL_SETUP_H
#include <Arduino.h>
#include <elapsedMillis.h>
#include "TM1638Setup.h"
#include "MotorsSetup.h"
#include "FrameSetup.h"
#include "LimitSW_Driver.h"
#include "mainProjectVar.h"
#include "mainProjectDefinition.h"
#include "DesignToMotor.h"
#include "BreakMotor.h"
#include "debugSerial.h"
#include "debugSerial.h"
#include "ColourSetup.h"
#include "ThreadBreakSetup.h"
#include "AmotorSetup.h"

/**/
//#define MainShaftRoundCompletePin 24 //24 pin used buy thread now
//#define MainShaftRoundCompletePin2 38
#define TestPin1 38
#define TestPin2 39
/**/

elapsedMillis WaitingExitFromCW_Limit;
elapsedMillis WaitingExitFromCCW_Limit;

elapsedMillis waitingXZERO;
elapsedMillis waitingYZERO;
elapsedMillis waitStart_F_R_Stiching;

elapsedMillis waitStopMachine;

ThreadPositionSetting NPS; //NeedlePositionSetting

designtostich DTS;
MachineCurrentStatus cMCS;
MachineCurrentStatus eMCS = mundefine; // Erroe type
MachineCurrentStatus shMCS;            // Shuttle Move
MachineCurrentStatus uMCS;
MachineCurrentStatus pausecauseMCS = mundefine; // PauseCause
MachineCurrentStatus sMCS = mundefine;          //Sub Machine Function
MachineCurrentStatus pMCS = mundefine;          //pREVIOUS ACTION
MachineCurrentStatus fMCS = mundefine;          // fILE OPNE AND COSE
StichLoadStatus SLS = StichLoadundefine;
allzero XZERO;
allzero ZZERO;
allzero YZERO;
allzero AZERO;

PositionStatus XST = PositionUndefine;
PositionStatus YST = PositionUndefine;

stichindex StichIndex = ZeorStich;
uint8_t mAction = 0;
uint8_t HomingStatus = 0;
uint8_t StichStaus;
/**/
uint8_t FirstStich_FW = No;
uint8_t Countinue_FW_Stiching = No;
uint8_t FirstStich_REV = No;
uint8_t Countinue_REV_Stiching = No;
/**/
// uint8_t FrameStaus;
// uint8_t NeedleStaus;
uint32_t StichNumber = 0;
// uint8_t nippleReached = 0;
uint8_t NippleJustUp = 0;
uint8_t needleJustTouch = 0;
uint8_t needleComingDown = 0;
uint8_t FrameDelayed = No;
uint8_t ShuttleRoundFlag = 0;
void Simple_MachineControl();
void SetSpeedXZERO();
void SetSpeedYZERO();
void ManualFrameAdjestMove();
uint8_t X_LeftZero();
uint8_t Y_LeftZero();

void f_XZERO();
void f_YZERO();
void f_ZZERO();
void f_Stiching();
void MainShaftPinInit();
volatile uint16_t MainShaftRoundCompleteCounter = 0;
volatile uint16_t MainShaftRoundCompleteCounter2 = 0;
volatile uint16_t MainShaftRoundCompleteCounter3 = 0;
volatile uint16_t needleTestpoint1 = 0;
// uint16_t MainShaftRoundCompleteCounter2 = 0;

struct NeedlePosition
{
    sint32 CurrentPosition = 0;
    sint32 LastPosition = 0;
    sint32 NextPostion = 0;
} NeedleMotor;
/**/
struct
{
    uint8_t Function;
    sint16_t Y_value;
    sint16_t X_value;
    uint8_t Hold;
    uint8_t First;
} jumpst;
/**/
void TestPinInit()
{
    pinMode(TestPin1, OUTPUT);
    pinMode(TestPin2, OUTPUT);
}
void MainShaftPinInit()
{
    //pinMode(MainShaftRoundCompletePin, INPUT_PULLUP);
    //pinMode(MainShaftRoundCompletePin2, INPUT_PULLUP);
}
// Shuttle move
void ShuttleRoundMove()
{
    if (MainShaftRoundCompleteCounter2 == 1)
    {
        MainShaftRoundCompleteCounter2 = 0;
        ShuttleRoundFlag = 1;
    }
    if (ShuttleRoundFlag == 1)
    {
        if (AvailableToMove == SM.A_StichStatus())
        {
            ShuttleRoundFlag = 0;
            Aaxis.setPosition(0);
            SM.A_StichRoundMove(0);
        }
    }
}
/**/

/**/
void f_NeedleThreadPosition(uint8_t _cMCS)
{
    switch (NPS)
    {
    case ThreadPosition:
        if (_cMCS == mPauseStich || _cMCS == mColourChange || _cMCS == mManual)
        {
            sint16 ThreadPostion = 90; // 160 Degree for Thread Position was past
            NeedleMotor.LastPosition = NM.CurrentDegree;
            NM.Z_Move(ThreadPostion);
            Zaxis.setTargetRel(Z_MotorPuls);
            controllerZ.moveAsync(Zaxis);
            SM.ShuttleThreadPosition(A_MotorPuls);
            while (controllerZ.isRunning())
            {
                ;
            }
            sMCS = mThreadPositon;
            // while (Nextion.availableForWrite() < 63)

            /**/
            // Nextion.print("baud=38400"); // Nextion Thread Reset Position
            // Nextion.print("\xFF\xFF\xFF");
            // Nextion.begin(38400);
            /**/

            Nextion.print("tm0.en=1"); // Nextion Thread Reset Position
            Nextion.print("\xFF\xFF\xFF");
            Serial.println(Nextion.availableForWrite());
            while (Nextion.availableForWrite() < 30)
            {
                ;
            }
            Nextion.print("tsw b9,1"); // Nextion Thread Reset Position Button Active
            Nextion.print("\xFF\xFF\xFF");
            while (Nextion.availableForWrite() < 30)
            {
                ;
            }
            Nextion.print("tsw b5,0"); // Nextion Pause Button Disable
            Nextion.print("\xFF\xFF\xFF");
            while (Nextion.availableForWrite() < 30)
            {
                ;
            }
            Nextion.print("tsw b6,0"); // Nextion Go Button Disable
            Nextion.print("\xFF\xFF\xFF");
            while (Nextion.availableForWrite() < 30)
            {
                ;
            }
            Nextion.print("tsw b7,0"); // Nextion F/R Button Disable
            Nextion.print("\xFF\xFF\xFF");
            while (Nextion.availableForWrite() < 30)
            {
                ;
            }
            // Nextion.print("tsw page1.b10,0"); // Nextion Stich-- Button Disable
            // Nextion.print("\xFF\xFF\xFF");
            // while (Nextion.availableForWrite() < 30)
            // {
            //     ;
            // }
            Nextion.print("tsw b8,0"); // Nextion Thread Button Disable
            Nextion.print("\xFF\xFF\xFF");
            while (Nextion.availableForWrite() < 30)
            {
                ;
            }
            /**/
            // Nextion.print(" baud=9600"); // Nextion Thread Reset Position
            // Nextion.print("\xFF\xFF\xFF");
            // Nextion.begin(9600);
            /**/
        }
        NPS = NPSundefine;
        /* code */
        break;
    case ReturnToPastPosition:
        Serial.println(cMCS);
        if (sMCS == mThreadPositon)
        {
            NM.Z_Move(NeedleMotor.LastPosition);
            Zaxis.setTargetRel(Z_MotorPuls);
            controllerZ.moveAsync(Zaxis);
            SM.ShuttleThreadPosition(A_MotorPuls);
            while (controllerZ.isRunning())
            {
                ;
            }
            sMCS = mundefine;
        }
        NPS = NPSundefine;
        break;
    default:

        break;
    }
}
/**/

//static void fMainShaftRoundComplete()
//{
//    MainShaftRoundCompleteCounter++;
//digitalWrite(TestPin1, !digitalRead(TestPin1));
//}
// void fMainShaftRoundComplete2()
// {
//     MainShaftRoundCompleteCounter2++;
// }

void f_Stiching()
{
    //---------------------------------------
    // Machine Stiching State Machine
    //---------------------------------------
    switch (DTS)
    {
    //------------------------------------------------------------------------
    // doPrepareStichingForward
    //------------------------------------------------------------------------
    case doPrepareStichingForward:
    {
        // MainShaftRoundCompleteCounter = 0;
        //attachInterrupt(digitalPinToInterrupt(MainShaftRoundCompletePin), fMainShaftRoundComplete, FALLING);
        // attachInterrupt(digitalPinToInterrupt(MainShaftRoundCompletePin2), fMainShaftRoundComplete2, RISING);
        if (StichIndex == ZeorStich || StichIndex == LastStich)
        {
            StichIndex = FirstStich;
            SLS = StichunLoaded;
            StichNumber = 0;
            NumberofStichPasswhileThreadBreak = 0;
            jumpst.First = No;
            cMCS = mFristStich;
            SetupinitVariables();
            NM.InitialPosition270Degree();     // 270 Degree
            MainShaftRoundCompleteCounter = 0; //dummeny
            // MainShaftRoundCompleteCounter2 = 0; //dummeny
        }
        // Check Thread Sensor;
        if (cMCS == mPauseStich)
        {
            DTS = doReadStichfunction;
            NumberofStichPasswhileThreadBreak = 0;
        }
        if (cMCS == mColourChange)
        {
            // BAction = BreakOFF;
            DTS = doReadStichfunction;
            NumberofStichPasswhileThreadBreak = 0;
        }
#ifdef StichByStich
        DTS = doDebugPauseStiching;
#endif
    }
    //------------------------------------------------------------------------
    // StichByStich
    //------------------------------------------------------------------------
#ifdef StichByStich
    case doDebugPauseStiching:
    {
        if (debugStichbyStich == debugStartStich)
        {
            Serial.println("StichByStich");
            debugStichbyStich = debugStopStich;
            DTS = doReadStichfunction;
            MainShaftRoundCompleteCounter = 0;
        }
        break;
    }
#endif
    //------------------------------------------------------------------------
    //Read Next Stich from SD
    // doReadStichfunction
    //------------------------------------------------------------------------
    case doReadStichfunction:
    {
        if (uMCS == mPauseStich)
        {
            // if (jumpst.Hold == jumprelease)
            // {
            //     uMCS = mundefine;
            //     DTS = doPauseStiching;
            //     detachInterrupt(digitalPinToInterrupt(MainShaftRoundCompletePin));
            //     // detachInterrupt(digitalPinToInterrupt(MainShaftRoundCompletePin2));
            // }
            // else if (jumpst.Hold == jumphold)
            // {
            //     //do nothing
            // }
            if (jumpst.First == No)
            {
                uMCS = mundefine;
                DTS = doPauseStiching;
                //detachInterrupt(digitalPinToInterrupt(MainShaftRoundCompletePin));
            }
            else
            {
                uMCS = mundefine;
            }
            // else
            // {
            //     uMCS = mundefine;
            //     DTS = doPauseStiching;
            //     detachInterrupt(digitalPinToInterrupt(MainShaftRoundCompletePin));
            //     // detachInterrupt(digitalPinToInterrupt(MainShaftRoundCompletePin2));
            // }
        }
        else
        {
            if (StichIndex != LastStich)
            {
                if (eMCS == mDelayFrameError)
                {
                    eMCS = mundefine;
                }
                else
                {
                    // if (SLS == StichunLoaded)
                    // {
                        mAction = GET_DATA('F');
                        SLS = StichLoaded;
                    // }
                }
            }
            else
            {
                mAction = EOD;
            }
            if (mAction == EOD) // End design 248 Last Stich Funciton
            {
                StichStaus = Finished;
                fMCS = mStichFileClose;
                CloseFile_SDCard();
                StichIndex = LastStich;
                if (jumpst.Hold == jumphold)
                {
                    jumpst.First = No;
                    // jumpst.Y_value += Y_distance;
                    // jumpst.X_value += X_distance;
                    Y_distance = jumpst.Y_value;
                    X_distance = jumpst.X_value;
                    /**/
                    FM.Y_Move(Y_distance);
                    FM.X_Move(X_distance);
                    NM.Z_Move(Z_degree);
                }
                waitStopMachine = 0;
                DTS = doEOD; // Stiching Close and Design End
            }
            //---------------------------------------------------------readyExecuteStich
            else if (mAction == readyExecuteStich) // Stiching
            {
                StichIndex = ContinueStich;
                if (cMCS == mJumping || cMCS == mColourChange || cMCS == mPauseStich)
                {
                    // attachInterrupt(digitalPinToInterrupt(MainShaftRoundCompletePin), fMainShaftRoundComplete, RISING);
                    if (MainShaftRoundCompleteCounter >= 1)
                    {
                        Serial.println("MainShaftCounter>=1");
                        MainShaftRoundCompleteCounter = 0;
                        // MainShaftRoundCompleteCounter2 = 0;
                    }
                    if (pMCS == mJumping)
                    {
                        cMCS = pMCS;
                    }
                }
                jumpst.First = No;
                jumpst.Hold = jumpHoldundefine;
                FM.Y_Move(Y_distance);
                FM.X_Move(X_distance);
                NM.Z_Move(Z_degree);
                // Serial.println(Z_degree);
                DTS = doStichingForward;
            }
            //---------------------------------------------------------readyExecuteJump
            else if (mAction == readyExecuteJump) // Jump First Stich
            {
                if (jumpst.First == No)
                {
                    // if (cMCS == mFristStich)
                    // {
                    //     MainShaftRoundCompleteCounter = 1;
                    // }
                    // if (cMCS == mPauseStich)
                    // {
                    //     MainShaftRoundCompleteCounter = 1;
                    // }
                    // detachInterrupt(digitalPinToInterrupt(MainShaftRoundCompletePin));
                    jumpst.First = Yes;
                    jumpst.Hold = jumphold;
                    jumpst.Y_value = 0;
                    jumpst.X_value = 0;
                    FM.Y_Move(Y_distance);
                    FM.X_Move(X_distance);
                    NM.Z_Move(Z_degree);
                    StichNumber = StichNumber + 1;
                    tm.displayIntNum(StichNumber, true);
                    DTS = doJumpForward;
                    waitStopMachine = 0;
                    Serial.println("Frist jumpst execuate");
#ifdef debugAddingJumpStich
                    Serial.println("jumpst.Hold jumphold");
                    Serial.println(Y_distance);
                    Serial.println(X_distance);
                    Serial.println(Z_degree);
#endif
                }
                else if (jumpst.Hold == jumphold)
                {

                    jumpst.Y_value += Y_distance;
                    jumpst.X_value += X_distance;

#ifdef debugAddingJumpStich
                    Serial.println("AddingJumpStich");
                    Serial.println(jumpst.Y_value);
                    Serial.println(jumpst.X_value);
#endif
                    StichNumber = StichNumber + 1;
                    SLS == StichunLoaded
                    DTS = doReadStichfunction;
                }
            }
            //---------------------------------------------------------readyExecuteVirtualJump
            else if (mAction == readyExecuteVirtualJump)
            {
#ifdef debugAddingJumpStich
                Serial.println("jumpst.Hold jump release");
#endif
                jumpst.First = No;
                jumpst.Hold = jumprelease;
                StichNumber = StichNumber + 1;
                tm.displayIntNum(StichNumber, true);
                jumpst.Y_value += Y_distance;
                jumpst.X_value += X_distance;
#ifdef debugAddingJumpStich
                Serial.println("AddingJumpStich");
                Serial.println(jumpst.Y_value);
                Serial.println(jumpst.X_value);
#endif
                Y_distance = jumpst.Y_value;
                X_distance = jumpst.X_value;
                /**/
                FM.Y_Move(Y_distance);
                FM.X_Move(X_distance);
                // NM.Z_Move(Z_degree);
                /**/
                DTS = doJumpForward;
                waitStopMachine = 0;
            }
            //---------------------------------------------------------Colour Stich
            else if (mAction == readyExecuteColour)
            {
                // if (jumpst.Hold == jumphold)
                // {
                //     jumpst.First = No;
                //     jumpst.Y_value += Y_distance;
                //     jumpst.X_value += X_distance;
                //     Y_distance = jumpst.Y_value;
                //     X_distance = jumpst.X_value;
                //     /**/
                //     FM.Y_Move(Y_distance);
                //     FM.X_Move(X_distance);
                //     // NM.Z_Move(Z_degree);
                // }
                DTS = doColourChange;
            }
        }
        break;
    }
    //---------------------------------------------------------------------------------
    // doStichingForward
    //---------------------------------------------------------------------------------
    case doStichingForward:
    {
        StichStaus = FM.XYZ_StichStatus();
        if (StichStaus == AvailableToMove)
        {
            if (cMCS == mStiching || cMCS == mJumping || cMCS == mColourChange || cMCS == mPauseStich || cMCS == mFristStich)
            {
                if (cMCS == mColourChange)
                {
                    /**/
                    cMCS = mStichDone;
                    BAction = BreakOFF;
                    f_BreakONOFF(cMCS);
                    /**/
                }
                if (cMCS == mPauseStich)
                {
                    MainShaftRoundCompleteCounter = 1;
                    pMCS = cMCS;
                }
                if (cMCS == mJumping)
                {
                    pMCS = cMCS;
                    FM.Setup_StichFrameSpeed();
                    NM.Z_StichMove();
                    SM.A_StichFollowMove(A_MotorPuls);
                    /**/
                    cMCS = mStichDone;
                    BAction = BreakOFF;
                    f_BreakONOFF(cMCS);
                    /**/
#ifdef debugSetup_JumpFrameSpeed
                    Serial.println("needle move after Jumping");
#endif
                }
                else if (cMCS == mFristStich)
                {
                    Serial.println("First Stich");
                    pMCS = cMCS;
                    FM.Setup_StichFrameSpeed();
                    NM.Z_StichMove();
                    SM.A_StichFollowMove(A_MotorPuls);
                    /**/
                    cMCS = mStichDone;
                    BAction = BreakOFF;
                    f_BreakONOFF(cMCS);
                    /**/
                }
            }
            if (MainShaftRoundCompleteCounter2 == 1)
            {
                MainShaftRoundCompleteCounter2 = 0;
                if (pMCS == mStiching)
                {
                    cMCS = mNeedlingMoving;
                    FM.Setup_StichFrameSpeed();
                    NM.Z_StichMove();
                    ShuttleRoundFlag = 1;
                    if (WaitA == SM.A_StichStatus())
                    {
                        Serial.println("SMD line 560 Main ControlSetup.h"); // Shuttle Motor Delay
                    }
                }
                else if (pMCS == mJumping)
                {
                    SM.A_StichRoundMove(0);
                }
                else if (pMCS == mFristStich)
                {
                    SM.A_StichRoundMove(0);
                }
            }
            if (ShuttleRoundFlag == 1)
            {
                if (AvailableToMove == SM.A_StichStatus())
                {
                    ShuttleRoundFlag = 0;
                    Aaxis.setPosition(0);
                    SM.A_StichRoundMove(A_MotorPuls);
                }
            }
            if (MainShaftRoundCompleteCounter >= 1)
            {
                needleTestpoint1 = 0;
                digitalWrite(TestPin2, 1);
                FM.Setup_StichFrameSpeed();
                if (pMCS == mJumping || pMCS == mFristStich)
                {
                    FM.XY_StichMove(); // this is not blocking function
                    SLS = StichunLoaded;
                    // pMCS = cMCS;
                }
                else if (cMCS == mNeedlingMoving)
                {
                    FM.XY_StichMove(); // this is not blocking function
                    SLS = StichunLoaded;
                    // SM.A_StichFollowMove(Z_MotorPuls);
                    // pMCS = cMCS;
                }
                else if (pMCS == mPauseStich)
                {
                    // FM.XY_StichMove(); // this is not blocking function
                    FM.XYZ_StichMove(); // this is not blocking function
                    SLS = StichunLoaded;
                    /**/
                    cMCS = mStichDone;
                    BAction = BreakOFF;
                    f_BreakONOFF(cMCS);
                    /**/
                    // pMCS = cMCS;
                }
                StichStaus = FM.XYZ_StichStatus();
                StichNumber = StichNumber + 1;
                tm.displayIntNum(StichNumber, true);
                cMCS = mStiching;
                shMCS = mStiching;
                pMCS = cMCS;
                // MainShaftRoundCompleteCounter2 = 0;
                DTS = doReadStichfunction;

                if (MainShaftRoundCompleteCounter == 2)
                {
                    if (cMCS == mNeedlingMoving)
                    {
                        FM.XY_StichMove(); // this is not blocking function
                    }
                    BAction = BreakON;
                    DTS = doPauseStiching;
                    Serial.println("2XOverSpeedDetected");
                }
                else if (MainShaftRoundCompleteCounter >= 3)
                {
                    if (cMCS == mNeedlingMoving)
                    {
                        FM.XY_StichMove(); // this is not blocking function
                        SLS = StichunLoaded;
                    }
                    BAction = BreakON;
                    DTS = doPauseStiching;
                    Serial.println("3XOverSpeedDetected");
                }
                if (digitalReadFast(threadpin) == 0)
                {
                    NumberofStichPasswhileThreadBreak++;
                    if (NumberofStichPasswhileThreadBreak >= 5)
                    {
                        BAction = BreakON;
                        DTS = doPauseStiching;
                        pausecauseMCS = mThreadBreak;
                        Serial.println("ThreadBreak");
                    }
                }
                else
                {
                    NumberofStichPasswhileThreadBreak = 0;
                }
                MainShaftRoundCompleteCounter = 0;
            }

#ifdef StichByStich
            DTS = doDebugPauseStiching;
#endif
        }
        // StichStaus = FM.XYZ_StichStatus();
        // if (StichStaus == WaitXY)
        // {
        //     // digitalWrite(TestPin1, 0);
        //     //while (StichStaus == WaitXY)
        //     //{
        //     if (needleTestpoint1 == 1)
        //     {
        //         needleTestpoint1 = 0;
        //         // int CUREENTSPEED;
        //         // Serial.println("FD"); //Frame Delay
        //         //CUREENTSPEED = controllerXY.getCurrentSpeed();
        //         //Serial.println(CUREENTSPEED);
        //         // DTS = doPauseStiching;
        //         break;
        //     }
        //     //}
        //     // digitalWrite(TestPin1, 1);
        // }
        // else
        // {
        //     digitalWrite(TestPin2, 0);
        // }
        // if (StichStaus == WaitZ)
        // {
        //     //  digitalWrite(TestPin1, 0);
        //     if (MainShaftRoundCompleteCounter == 1)
        //     {
        //         //MainShaftRoundCompleteCounter = 0;
        //         // size_t CUREENTSPEED;
        //         //BAction = BreakON;
        //         Serial.println("ND"); // nipple Z motor
        //         //CUREENTSPEED = controllerXY.getCurrentSpeed();
        //         //Serial.println(CUREENTSPEED);
        //         DTS = doPauseStiching;
        //     }
        //     else if (MainShaftRoundCompleteCounter > 1)
        //     {
        //         // size_t CUREENTSPEED;
        //         //BAction = BreakON;
        //         //DTS = doPauseStiching;
        //         Serial.println("2ND"); //nipple Z motor
        //         //CUREENTSPEED = controllerXY.getCurrentSpeed();
        //         //Serial.println(CUREENTSPEED);
        //     }
        // }
        else if (StichStaus == Wait)
        {
            if (FrameDelayed == Yes)
            {
                BAction = BreakON;
                pausecauseMCS = mDelayFrameError;
                DTS = doPauseStiching;
                Serial.println("Frame Delay Pause Exe");
            }
        }

        //-------------------------------------------------------------------
        break;
    }
    //------------------------------------------------------------------------
    //Main Motor Execuation
    // doJumpForward
    //------------------------------------------------------------------------
    case doJumpForward:
    {
        StichStaus = FM.XYZ_StichStatus();
        if (StichStaus == AvailableToMove)
        {
            if (cMCS == mStiching || cMCS == mJumping || cMCS == mFristStich || cMCS == mColourChange || cMCS == mPauseStich)
            {
                if (MainShaftRoundCompleteCounter > 1)
                {
                    if (cMCS == mColourChange)
                    {
                        MainShaftRoundCompleteCounter = 1;
                    }
                }
                if (cMCS == mPauseStich)
                {
                    MainShaftRoundCompleteCounter = 1;
                }
                if (cMCS == mFristStich)
                {
                    MainShaftRoundCompleteCounter = 1;
                }
                /**/
                pMCS = cMCS;
                cMCS = mStichDone;
                BAction = BreakON;
                waitStopMachine = 0;
            }
            /**/
            //Shuttle Move
            if (MainShaftRoundCompleteCounter2 == 1)
            {
                MainShaftRoundCompleteCounter2 = 0;
                SM.A_StichRoundMove(A_MotorPuls);
                //ShuttleRoundMove();
            }
            //END
            if (MainShaftRoundCompleteCounter == 1)
            {
                FM.Setup_JumpFrameSpeed();
                if (waitStopMachine >= 500 || cMCS == mColourChange) // wait Break mechanical Delay
                {
                    waitStopMachine = 2000;
                    // FM.Y_Move(Y_distance);
                    // FM.X_Move(X_distance);
                    // NM.Z_Move(Z_degree);
                    if (jumpst.First == Yes)
                    {
                        FM.XYZ_StichMove(); // this is not blocking function
                        SLS = StichunLoaded;
                        StichStaus = FM.XYZ_StichStatus();
                        while (StichStaus != AvailableToMove)
                        {
                            StichStaus = FM.XYZ_StichStatus();
                        }
                        if (pMCS == mFristStich || pMCS == mColourChange)
                        {
                            SM.A_StichFollowMove(A_MotorPuls);
                        }
                        shMCS = mJumping;
                    }
                    else
                    {
                        FM.XY_StichMove(); // this is not blocking function
                        SLS = StichunLoaded;
                        StichStaus = FM.XYZ_StichStatus();
                        while (StichStaus != AvailableToMove)
                        {
                            StichStaus = FM.XYZ_StichStatus();
                        }
                    }
                    cMCS = mJumping;
                    shMCS = mJumping;
                    DTS = doReadStichfunction;
#ifdef StichByStich
                    DTS = doDebugPauseStiching;
#endif
                }
            }
        }
        else if (StichStaus == Wait)
        {
            ;
        }
        else if (StichStaus == WaitZ)
        {
            ;
        }
        else if (StichStaus == WaitXY)
        {
            ;
        }
        break;
    }
    //------------------------------------------------------------------------
    // doEOD
    //------------------------------------------------------------------------
    case doEOD: // Stiching Close and Design End
    {
        if (jumpst.Hold == jumphold)
        {
            StichStaus = FM.XYZ_StichStatus();
            if (StichStaus == AvailableToMove)
            {
                if (cMCS == mStiching || cMCS == mJumping || cMCS == mFristStich)
                {
                    cMCS = mEndDesign;
                    //detachInterrupt(digitalPinToInterrupt(MainShaftRoundCompletePin)); // 2020
                    // detachInterrupt(digitalPinToInterrupt(MainShaftRoundCompletePin2));
                    BAction = BreakON;
                    f_BreakONOFF(cMCS);
                    waitStopMachine = 0;
                }
                FM.Setup_JumpFrameSpeed();
                if (waitStopMachine >= 500) // wait Break mechanical Delay
                {
                    jumpst.Hold = jumprelease;
                    tm.displayIntNum(StichNumber, true);
                    waitStopMachine = 2000;
                    // FM.Y_Move(Y_distance);
                    // FM.X_Move(X_distance);
                    // NM.Z_Move(Z_degree);
                    FM.XYZ_StichMove(); // this is not blocking function
                    SLS = StichunLoaded;
                    SM.A_StichFollowMove(A_MotorPuls);
                    cMCS = mEndDesign;
                    DTS = doStichingundefine;
                    FrameSpeed = undefineSpeed;
                    BAction = BreakON;
                    f_BreakONOFF(cMCS);
                    Nextion.print("page1.b6.bco=50712"); // Go Button DEFAULT
                    Nextion.print("\xFF\xFF\xFF");
#ifdef StichByStich
                    DTS = doDebugPauseStiching;
#endif
                }
            }
            else if (StichStaus == Wait)
            {
                ;
            }
            else if (StichStaus == WaitZ)
            {
                ;
            }
            else if (StichStaus == WaitXY)
            {
                ;
            }
        }
        else
        {
            // detachInterrupt(digitalPinToInterrupt(MainShaftRoundCompletePin));
            cMCS = mEndDesign;
            if (MainShaftRoundCompleteCounter2 == 1)
            {
                MainShaftRoundCompleteCounter2 = 0;
                ShuttleRoundFlag = 1;
            }
            if (ShuttleRoundFlag == 1)
            {
                if (AvailableToMove == SM.A_StichStatus())
                {
                    ShuttleRoundFlag = 0;
                    SM.A_StichRoundMove(0);
                    Nextion.print("page1.b6.bco=50712"); // Go Button Normal Colour
                    Nextion.print("\xFF\xFF\xFF");
                    while (Nextion.availableForWrite() <= 30)
                    {
                        ;
                    }
                    // detachInterrupt(digitalPinToInterrupt(MainShaftRoundCompletePin2));
                    DTS = doStichingundefine;
                    FrameSpeed = undefineSpeed;
                }
            }
            BAction = BreakON;
            f_BreakONOFF(cMCS);
        }
        break;
    }
    //------------------------------------------------------------------------
    // doColourChange
    //------------------------------------------------------------------------
    case doColourChange:
    {
        if (jumpst.Hold == jumphold)
        {
            //             Serial.println("Colour B/W Jump");
            //             StichStaus = FM.XYZ_StichStatus();
            //             if (StichStaus == AvailableToMove)
            //             {
            //                 if (cMCS == mStiching || cMCS == mJumping || cMCS == mFristStich)
            //                 {
            //                     cMCS = mStichDone;
            //                 }
            //                 FM.Setup_JumpFrameSpeed();
            //                 waitStopMachine = 2000;
            //                 jumpst.Hold = jumprelease;
            //                 StichNumber = StichNumber + 1;
            //                 tm.displayIntNum(StichNumber, true);
            //                 // FM.Y_Move(Y_distance);
            //                 // FM.X_Move(X_distance);
            //                 // NM.Z_Move(Z_degree);
            //                 FM.XYZ_StichMove(); // this is not blocking function
            //                 SM.A_StichFollowMove(A_MotorPuls);
            //                 cMCS = mColourChange;
            //                 DTS = doStichingundefine;
            //                 BAction = BreakON;
            //                 f_BreakONOFF(cMCS);
            //                 Serial.println("ColourChangeStiching");
            //                 Nextion.print("tm1.en=1"); // Nextion Pause Stich button  Blinking
            //                 Nextion.print("\xFF\xFF\xFF");
            //                 while (Nextion.availableForWrite() <= 30)
            //                 {
            //                     ;
            //                 }
            //                 /**/
            //                 Nextion.print("page 2"); // Go Button Green
            //                 Nextion.print("\xFF\xFF\xFF");
            //                 /**/
            //                 while (Nextion.availableForWrite() <= 30)
            //                 {
            //                     ;
            //                 }
            // #ifdef StichByStich
            //                 DTS = doDebugPauseStiching;
            // #endif
            //             }
            //             else if (StichStaus == Wait)
            //             {
            //                 ;
            //             }
            //             else if (StichStaus == WaitZ)
            //             {
            //                 ;
            //             }
            //             else if (StichStaus == WaitXY)
            //             {
            //                 ;
            //             }
        }
        else
        {
            cMCS = mColourChange;
            if (MainShaftRoundCompleteCounter2 == 1)
            {
                MainShaftRoundCompleteCounter2 = 0;
                ShuttleRoundFlag = 1;
                BAction = BreakON;
                f_BreakONOFF(cMCS);
            }
            if (ShuttleRoundFlag == 1)
            {
                if (AvailableToMove == SM.A_StichStatus())
                {
                    ShuttleRoundFlag = 0;
                    SM.A_StichRoundMove(0);
                    StichNumber = StichNumber + 1;
                    tm.displayIntNum(StichNumber, true);
                    DTS = doStichingundefine;
                    Serial.println("ColourChangeStiching");
                    Nextion.print("tm1.en=1"); // Nextion Pause Stich button  Blinking
                    Nextion.print("\xFF\xFF\xFF");
                    while (Nextion.availableForWrite() <= 30)
                    {
                        ;
                    }
                    /**/
                    Nextion.print("page 2"); // Go Button Green
                    Nextion.print("\xFF\xFF\xFF");
                    /**/
                    while (Nextion.availableForWrite() <= 30)
                    {
                        ;
                    }
                }
            }
        }
        break;
    }
    //------------------------------------------------------------------------
    // doPauseStiching
    //------------------------------------------------------------------------
    case doPauseStiching:
    {
        if (cMCS != mPauseStich)
        {
            pMCS = cMCS;
            cMCS = mPauseStich;
            BAction = BreakON;
            f_BreakONOFF(cMCS);
            Serial.println("PauseStiching");
            Nextion.print("tm1.en=1"); // Nextion Pause Stich button  Blinking
            Nextion.print("\xFF\xFF\xFF");
            while (Nextion.availableForWrite() <= 30)
            {
                ;
            }
            Nextion.print("page1.b6.bco=50712"); // Go Button Normal Colour
            Nextion.print("\xFF\xFF\xFF");
            while (Nextion.availableForWrite() <= 30)
            {
                ;
            }
        }
        if (pausecauseMCS == mDelayFrameError)
        {
            pausecauseMCS = mundefine;
            eMCS = mDelayFrameError;
        }
        if (pausecauseMCS == mThreadBreak)
        {
            Nextion.print("page 6"); // Go Button Green
            Nextion.print("\xFF\xFF\xFF");
            while (Nextion.availableForWrite() <= 30)
            {
                ;
            }
            Nextion.print("tm0.en=1"); // Go Button Green
            Nextion.print("\xFF\xFF\xFF");
            pausecauseMCS = mundefine;
        }
        /**/
        // Shuttle Move when Needle enter into Shuttle
        if (MainShaftRoundCompleteCounter2 == 1)
        {
            MainShaftRoundCompleteCounter2 = 0;
            ShuttleRoundFlag = 1;
        }
        if (ShuttleRoundFlag == 1)
        {
            if (AvailableToMove == SM.A_StichStatus())
            {
                ShuttleRoundFlag = 0;
                SM.A_StichRoundMove(0);
            }
        }
        //END
        /**/
        break;
    }
    //------------------------------------------------------------------------
    // doStopStiching
    //------------------------------------------------------------------------
    case doStopStiching:
    {
        if (cMCS == mStichDone || cMCS == mJumpDone)
        {
            cMCS = mStopStich;
            DTS = doStichingundefine;
            BAction = BreakON;
        }
        break;
    }
    //------------------------------------------------------------------------
    // doStichFManual
    //------------------------------------------------------------------------
    case doStichFManual:
    {
        if (StichIndex != LastStich)
        {
            if (FirstStich_FW == No || Countinue_FW_Stiching == Yes)
            {
                StichStaus = FM.XYZ_StichStatus();
                if (StichStaus == AvailableToMove)
                {
                    mAction = GET_DATA('F');
                    FM.Y_Move(Y_distance);
                    FM.X_Move(X_distance);
                    NM.Z_Move(Z_degree);
                    if (mAction == EOD) // End design 248 Last Stich Funciton
                    {
                        // StichStaus = Finished;
                        // cMCS = mEndDesign;
                        // fMCS = mStichFileClose;
                        // CloseFile_SDCard();
                        // DTS = doEOD; // Stiching Close and Design End
                        StichIndex = LastStich;
                        BAction = BreakON;
                        f_BreakONOFF(cMCS);
                    }
                    else if (mAction == readyExecuteStich ||
                             mAction == readyExecuteJump || mAction == readyExecuteVirtualJump)
                    {
                        FM.Setup_Stich_F_R_FrameSpeed();
                        FM.XYZ_StichMove(); // this is not blocking function
                        SM.A_StichFollowMove(A_MotorPuls);
                        StichNumber++;
                        tm.displayIntNum(StichNumber, true);
                    }
                    else if (mAction == readyExecuteColour)
                    {
                        StichNumber++;
                        tm.displayIntNum(StichNumber, true);
                    }
                    if (mAction == readyExecuteJump)
                    {
                        MainShaftRoundCompleteCounter = 1;
                    }
                    //DTS = doStichingundefine;
                    // FM.Y_Move(Y_distance);
                    // FM.X_Move(X_distance);
                    // NM.Z_Move(Z_degree);
                }
                else if (StichStaus == Wait)
                {
                    // cMCS = mStichDone;
                }
            }

            if (FirstStich_FW == No)
            {
                FirstStich_FW = Yes;
                waitStart_F_R_Stiching = 0;
            }
            if (waitStart_F_R_Stiching >= 300 && Countinue_FW_Stiching == No)
            {
                Countinue_FW_Stiching = Yes;
            }
            //-------------------------------------------------------------------
        }
        break;
    }
    case doStichFManualStop:
    {
        FirstStich_FW = No;
        Countinue_FW_Stiching = No;
        DTS = doStichingundefine;
        break;
    }
    //------------------------------------------------------------------------
    // doStichRManual
    //------------------------------------------------------------------------
    case doStichRManual:
    {
        if (StichNumber > 1)
        {
            if (FirstStich_REV == No || Countinue_REV_Stiching == Yes)
            {
                StichStaus = FM.XYZ_StichStatus();
                if (StichStaus == AvailableToMove)
                {
                    mAction = GET_DATA('R');
                    FM.Y_Move(Y_distance);
                    FM.X_Move(X_distance);
                    NM.Z_Move(Z_degree);
                    if (mAction == EOD) // End design 248 Last Stich Funciton
                    {
                        StichStaus = Finished;
                        cMCS = mEndDesign;
                        fMCS = mStichFileClose;
                        CloseFile_SDCard();
                        DTS = doEOD; // Stiching Close and Design End
                        StichIndex = LastStich;
                        BAction = BreakON;
                        f_BreakONOFF(cMCS);
                    }
                    else if (mAction == readyExecuteStich ||
                             mAction == readyExecuteJump || mAction == readyExecuteVirtualJump)
                    {
                        FM.Setup_Stich_F_R_FrameSpeed();
                        FM.XYZ_StichMove(); // this is not blocking function
                        SM.A_StichFollowMove(A_MotorPuls);
                        StichIndex = ContinueStich;
                        if (StichNumber == 1)
                        {
                            StichStaus = Finished;
                            cMCS = mEndDesign;
                            pMCS = cMCS;
                            fMCS = mStichFileClose;
                            CloseFile_SDCard();
                            DTS = doEOD; // Stiching Close and Design End
                            StichIndex = LastStich;
                            BAction = BreakON;
                            f_BreakONOFF(cMCS);
                        }
                        StichNumber--;
                        tm.displayIntNum(StichNumber, true);
                    }
                    else if (mAction == readyExecuteColour)
                    {
                        StichNumber--;
                        tm.displayIntNum(StichNumber, true);
                    }
                    // DTS = doStichingundefine;
                    // FM.Y_Move(Y_distance);
                    // FM.X_Move(X_distance);
                    // NM.Z_Move(Z_degree);
                }
                else if (StichStaus == Wait)
                {
                    // cMCS = mStichDone;
                }
            }
            if (FirstStich_REV == No)
            {
                FirstStich_REV = Yes;
                waitStart_F_R_Stiching = 0;
            }
            if (waitStart_F_R_Stiching >= 300 && Countinue_REV_Stiching == No)
            {
                Countinue_REV_Stiching = Yes;
            }
            //-------------------------------------------------------------------
        }
        break;
    }
    case doStichRManualStop:
    {
        FirstStich_REV = 0;
        Countinue_REV_Stiching = 0;
        DTS = doStichingundefine;
        break;
    }
    case doStichingundefine:
    {
        break;
    }
    default:
        break;
    }
}

void f_AZERO()
{
    //---------------------------------------
    // DO Z MOTOR Position State Machine
    //---------------------------------------
    switch (AZERO)
    {
    case doHoming:
        HomingStatus = SM.A_Axis_Homing();

        if (HomingStatus == SM.HomingDone)
        {
            AZERO = doZHomingDone;
        }
        if (HomingStatus == SM.HomingError)
        {
            AZERO = doZHomingError;
        }
        break;
    case doStop:
        SM.StopAmotorEmergency();
        break;

    default:
        break;
    }
}

void f_ZZERO()
{
    //---------------------------------------
    // DO Z MOTOR Position State Machine
    //---------------------------------------
    switch (ZZERO)
    {
    case doHoming:
        HomingStatus = NM.Z_Axis_Homing();
        if (HomingStatus == NM.HomingDone)
        {
            ZZERO = doZHomingDone;
        }
        if (HomingStatus == NM.HomingError)
        {
            ZZERO = doZHomingError;
        }
        break;
    case doStop:
        NM.StopZmotorEmergency();
        break;

    default:
        break;
    }
#ifdef debugZstepPlus
    if (debugZmotorPuls == plusNegative)
    {
        digitalWrite(Z_Motor_StepPin, 0);
        debugZmotorPuls = plusundefine;
    }
    else if (debugZmotorPuls == plusPositive)
    {
        digitalWrite(Z_Motor_StepPin, 1);
        debugZmotorPuls = plusundefine;
    }
    else
    {
    }
#endif
}
//---------------------------------------
// DO X MOTOR Position State Machine
//---------------------------------------
void XStopEmergencyStop()
{
    controllerX.emergencyStop();
    XST = MotorStopInit;
}
void f_XZERO()
{
    sint32 FreeMove;
    switch (XZERO)
    {
    case goLeftZero:
    {
        if (XST == ZeroDone || XST == ZeroError)
        {
            XST = PositionUndefine;
        }
        if (XST == PositionUndefine || XST == ForceStop)
        {
            XST = Positiondefine;
            waitingXZERO = 0;
            FM.SetSpeedXZERO();
            attachInterrupt(digitalPinToInterrupt(Right_SW), XStopEmergencyStop, RISING);
        }
        if (digitalRead(Right_SW) == Close)
        {
            detachInterrupt(digitalPinToInterrupt(Right_SW));
            XST = MotorStopInit;
        }
        else if (XST == Positiondefine || XST == MotorMovingInitiat)
        {
            if (XST != MotorMovingInitiat)
            {
                XST = MotorMovingInitiat;
                FreeMove = 2500 * microStep;
                Xaxis.setTargetRel(FreeMove);
                controllerX.moveAsync(Xaxis);
                Serial.println("X Running!!");
            }
            if (XST == MotorMovingInitiat)
            {
                if (waitingXZERO > 10000)
                {
                    XST = ZeroError;
                    XZERO = XZeroError;
                    waitingXZERO = 0;
                    controllerX.emergencyStop();
                    Serial.println("X Homing Error");
                }
            }
        }
        if (XST == MotorStopInit)
        {
            detachInterrupt(digitalPinToInterrupt(Right_SW));
            Xaxis.setPosition(0); // set start position of counter use after homming position
            XST = ZeroDone;
            XZERO = doXZeroDone;
            Serial.println("X Zero Done");
        }
        break;
    }
    case goRightZero:
    {
        if (XST == ZeroDone || XST == ZeroError)
        {
            XST = PositionUndefine;
        }
        if (XST == PositionUndefine || XST == ForceStop)
        {
            XST = Positiondefine;
            waitingXZERO = 0;
            FM.SetSpeedXZERO();
            attachInterrupt(digitalPinToInterrupt(Left_SW), XStopEmergencyStop, RISING);
        }
        if (digitalRead(Left_SW) == Close)
        {
            detachInterrupt(digitalPinToInterrupt(Left_SW));
            XST = MotorStopInit;
        }
        else if (XST == Positiondefine || XST == MotorMovingInitiat)
        {
            if (XST != MotorMovingInitiat)
            {
                XST = MotorMovingInitiat;
                FreeMove = -2500 * microStep;
                Xaxis.setTargetRel(FreeMove);
                controllerX.moveAsync(Xaxis);
                Serial.println("X Running!!");
            }
            if (XST == MotorMovingInitiat)
            {
                if (waitingXZERO > 10000)
                {
                    XST = ZeroError;
                    XZERO = XZeroError;
                    waitingXZERO = 0;
                    controllerX.emergencyStop();
                    Serial.println("X Homing Error");
                }
            }
        }
        if (XST == MotorStopInit)
        {
            detachInterrupt(digitalPinToInterrupt(Left_SW));
            Xaxis.setPosition(0); // set start position of counter use after homming position
            XST = ZeroDone;
            XZERO = doXZeroDone;
            Serial.println("X Zero Done");
        }
        break;
    }
    case doStop:
    {
        if (XST != ForceStop)
        {
            Serial.println("X Force Stop");
            controllerX.emergencyStop();
            XST = ForceStop;
        }
        break;
    }
    default:
        break;
    }
}
//---------------------------------------
// DO Y MOTOR Position State Machine
//---------------------------------------
void YStopEmergencyStop()
{
    controllerY.emergencyStop();
    YST = MotorStopInit;
}
void f_YZERO()
{
    sint32 FreeMove;
    switch (YZERO)
    {
    case goUpZero:
    {
        if (YST == ZeroDone || YST == ZeroError)
        {
            YST = PositionUndefine;
        }
        if (YST == PositionUndefine || YST == ForceStop)
        {
            YST = Positiondefine;
            waitingYZERO = 0;
            FM.SetSpeedYZERO();
            attachInterrupt(digitalPinToInterrupt(Forward_SW), YStopEmergencyStop, RISING);
        }
        if (digitalRead(Forward_SW) == Close)
        {
            detachInterrupt(digitalPinToInterrupt(Forward_SW));
            YST = MotorStopInit;
        }
        else if (YST == Positiondefine || YST == MotorMovingInitiat)
        {
            if (YST != MotorMovingInitiat)
            {
                YST = MotorMovingInitiat;
                FreeMove = 7620 * microStep;
                Yaxis.setTargetRel(FreeMove);
                controllerY.moveAsync(Yaxis);
                Serial.println("Y Running!!");
            }
            if (YST == MotorMovingInitiat)
            {
                if (waitingYZERO > 30000)
                {
                    YST = ZeroError;
                    XZERO = YZeroError;
                    waitingYZERO = 0;
                    controllerY.emergencyStop();
                    Serial.println("Y Homing Error");
                }
            }
        }
        if (YST == MotorStopInit)
        {
            detachInterrupt(digitalPinToInterrupt(Forward_SW));
            Yaxis.setPosition(0); // set start position of counter use after homming position
            YST = ZeroDone;
            YZERO = doYZeroDone;
            Serial.println("Y Zero Done");
        }
        break;
    }
    case goDownZero:
    {
        if (YST == ZeroDone)
        {
            YST = PositionUndefine;
        }
        if (YST == PositionUndefine || YST == ForceStop)
        {
            YST = Positiondefine;
            FM.SetSpeedYZERO();
            attachInterrupt(digitalPinToInterrupt(BackWard_SW), YStopEmergencyStop, RISING);
        }
        if (digitalRead(BackWard_SW) == Close)
        {
            detachInterrupt(digitalPinToInterrupt(BackWard_SW));
            YST = MotorStopInit;
        }
        else if (YST == Positiondefine || YST == MotorMovingInitiat)
        {
            if (YST != MotorMovingInitiat)
            {
                YST = MotorMovingInitiat;
                FreeMove = -7620 * microStep;
                Yaxis.setTargetRel(FreeMove);
                controllerY.moveAsync(Yaxis);
                Serial.println("Y Running!!");
            }
            if (YST == MotorMovingInitiat)
            {
                if (waitingYZERO > 30000)
                {
                    YST = ZeroError;
                    XZERO = YZeroError;
                    waitingYZERO = 0;
                    controllerY.emergencyStop();
                    Serial.println("Y Homing Error");
                }
            }
        }
        if (YST == MotorStopInit)
        {
            detachInterrupt(digitalPinToInterrupt(BackWard_SW));
            Yaxis.setPosition(0); // set start position of counter use after homming position
            YST = ZeroDone;
            YZERO = doYZeroDone;
            Serial.println("Y Zero Done");
        }
        break;
    }
    case doStop:
    {
        if (YST != ForceStop)
        {
            Serial.println("Y Force Stop");
            controllerY.emergencyStop();
            YST = ForceStop;
        }
        break;
    }
    default:
        break;
    }
}
//----------------------------------------------------------------------------------
void ManualFrameAdjestMove()
{
    if (cMCS == mPauseStich || cMCS == mColourChange)
    {
        uint8_t val = 0;
        val = tm.readButtons();
        Button_7 = ((val >> 7) & 1);
        Button_6 = ((val >> 6) & 1);
        Button_5 = ((val >> 5) & 1);
        Button_4 = ((val >> 4) & 1);
        Button_3 = ((val >> 3) & 1);
        Button_2 = ((val >> 2) & 1);
        Button_1 = ((val >> 1) & 1);
        Button_0 = ((val)&1);
        //--------------------
        if (Button_7 == 1 || Button_6 == 1)
        {
            if (Button_7)
            {
                if (digitalRead(Right_SW) == Open)
                {
                    tm.setLED(7, 1);
                    //FM.Y_FreeMove_Positive();
                    FM.X_Move_One_MM(250);
                    FM.X_FrameFreeMove(Move);
                }
                else if (digitalRead(Right_SW) == Close)
                {
                    FM.X_FrameFreeMove(MStop);
                }
            }
            else if (Button_6)
            {
                if (digitalRead(Left_SW) == Open)
                {
                    tm.setLED(6, 1);
                    FM.X_Move_One_MM(-250);
                    FM.X_FrameFreeMove(Move);
                }
                else if (digitalRead(Left_SW) == Close)
                {
                    FM.X_FrameFreeMove(MStop);
                }
            }
        }
        else
        {
            FM.X_FrameFreeMove(Stop);
            tm.setLED(7, 0);
            tm.setLED(6, 0);
        }
        //--------------------
        if (Button_5 == 1 || Button_4 == 1)
        {
            if (Button_5)
            {
                // if (digitalRead(BackWard_SW) == Close || digitalRead(Forward_SW) == Close)
                if (digitalRead(Forward_SW) == Open)
                {
                    tm.setLED(5, 1);
                    //FM.Y_FreeMove_Positive();
                    FM.Y_Move_One_MM(762);
                    FM.Y_FrameFreeMove(Move);
                }
                else if (digitalRead(Forward_SW) == Close)
                {
                    FM.Y_FrameFreeMove(MStop);
                }
            }
            else if (Button_4)
            {
                if (digitalRead(BackWard_SW) == Open)
                {
                    tm.setLED(4, 1);
                    //FM.Y_FreeMove_Negative();
                    FM.Y_Move_One_MM(-762);
                    FM.Y_FrameFreeMove(Move);
                }
                else if (digitalRead(BackWard_SW) == Close)
                {
                    FM.Y_FrameFreeMove(MStop);
                }
            }
        }
        else
        {
            FM.Y_FrameFreeMove(Stop);
            tm.setLED(5, 0);
            tm.setLED(4, 0);
        }
        //--------------------------------------------------------------------------------
        if (Button_1 == 1 || Button_0 == 1)
        {
            if (Button_1)
            {
                if (Z_MotorHomePosition == udefine || Z_MotorHomePosition == CW_Limit)
                {
                    // tm.setLED(1, 1);
                    NM.Z_Move_One_MM(4000);
                    NM.Z_FrameFreeMove(Move);
                }
                if (digitalRead(HomePositoin_SW) == HomePosition)
                {
                    if (Z_MotorHomePosition == CW_Limit)
                    {
                        WaitingExitFromCCW_Limit = 0;
                        //do nothing..
                    }
                    else
                    {
                        Z_MotorHomePosition = CCW_Limit;
                        NM.Z_FrameFreeMove(MStop);
                    }
                }
                else
                {
                    if (Z_MotorHomePosition == CW_Limit)
                    {
                        if (WaitingExitFromCCW_Limit >= 500)
                        {
                            WaitingExitFromCW_Limit = 0;
                            Z_MotorHomePosition = udefine;
                        }
                    }
                }
            }
            else if (Button_0)
            {
                if (Z_MotorHomePosition == udefine || Z_MotorHomePosition == CCW_Limit)
                {
                    NM.Z_Move_One_MM(-4000);
                    NM.Z_FrameFreeMove(Move);
                }
                if (digitalRead(HomePositoin_SW) == HomePosition)
                {
                    if (Z_MotorHomePosition == CCW_Limit)
                    {
                        WaitingExitFromCW_Limit = 0;
                        //do nothing..
                    }
                    else
                    {
                        Z_MotorHomePosition = CW_Limit;
                        NM.Z_FrameFreeMove(MStop);
                    }
                }
                else
                {
                    if (Z_MotorHomePosition == CCW_Limit)
                    {
                        if (WaitingExitFromCW_Limit >= 500)
                        {
                            WaitingExitFromCCW_Limit = 0;
                            Z_MotorHomePosition = udefine;
                        }
                    }
                }
            }
        }
        else
        {
            NM.Z_FrameFreeMove(Stop);
            if (Z_MotorHomePosition == CCW_Limit || Z_MotorHomePosition == CW_Limit)
            {
                //do nothing
                //Serial.println(Z_MotorHomePosition, DEC);
            }
            else
            {
                //Serial.println(Z_MotorHomePosition, DEC);
                Z_MotorHomePosition = udefine;
            }
        }
    }
}
//----------------------------------------------------------------------------------
void Simple_MachineControl()
{
    if (cMCS == mManual)
    {
        uint8_t val = 0;
        val = tm.readButtons();
        Button_7 = ((val >> 7) & 1);
        Button_6 = ((val >> 6) & 1);
        Button_5 = ((val >> 5) & 1);
        Button_4 = ((val >> 4) & 1);
        Button_3 = ((val >> 3) & 1);
        Button_2 = ((val >> 2) & 1);
        Button_1 = ((val >> 1) & 1);
        Button_0 = ((val)&1);
        //--------------------
        if (Button_7 == 1 || Button_6 == 1)
        {
            if (Button_7)
            {
                if (digitalRead(Right_SW) == Open)
                {
                    tm.setLED(7, 1);
                    //FM.Y_FreeMove_Positive();
                    FM.X_Move_One_MM(250);
                    FM.X_FrameFreeMove(Move);
                }
                else if (digitalRead(Right_SW) == Close)
                {
                    FM.X_FrameFreeMove(MStop);
                }
            }
            else if (Button_6)
            {
                if (digitalRead(Left_SW) == Open)
                {
                    tm.setLED(6, 1);
                    FM.X_Move_One_MM(-250);
                    FM.X_FrameFreeMove(Move);
                }
                else if (digitalRead(Left_SW) == Close)
                {
                    FM.X_FrameFreeMove(MStop);
                }
            }
        }
        else
        {
            FM.X_FrameFreeMove(Stop);
            tm.setLED(7, 0);
            tm.setLED(6, 0);
        }
        //--------------------
        if (Button_5 == 1 || Button_4 == 1)
        {
            if (Button_5)
            {
                // if (digitalRead(BackWard_SW) == Close || digitalRead(Forward_SW) == Close)
                if (digitalRead(Forward_SW) == Open)
                {
                    tm.setLED(5, 1);
                    //FM.Y_FreeMove_Positive();
                    FM.Y_Move_One_MM(762); // 762
                    FM.Y_FrameFreeMove(Move);
                }
                else if (digitalRead(Forward_SW) == Close)
                {
                    FM.Y_FrameFreeMove(MStop);
                }
            }
            else if (Button_4)
            {
                if (digitalRead(BackWard_SW) == Open)
                {
                    tm.setLED(4, 1);
                    //FM.Y_FreeMove_Negative();
                    FM.Y_Move_One_MM(-762); //762
                    FM.Y_FrameFreeMove(Move);
                }
                else if (digitalRead(BackWard_SW) == Close)
                {
                    FM.Y_FrameFreeMove(MStop);
                }
            }
        }
        else
        {
            FM.Y_FrameFreeMove(Stop);
            tm.setLED(5, 0);
            tm.setLED(4, 0);
        }
        //--------------------------------------------------------------------------------
        if (Button_1 == 1 || Button_0 == 1)
        {
            if (Button_1)
            {
                if (Z_MotorHomePosition == udefine || Z_MotorHomePosition == CW_Limit)
                {
                    // tm.setLED(1, 1);
                    NM.Z_Move_One_MM(1000);
                    NM.Z_FrameFreeMove(Move);
                }
                if (digitalRead(HomePositoin_SW) == HomePosition)
                {
                    if (Z_MotorHomePosition == CW_Limit)
                    {
                        WaitingExitFromCCW_Limit = 0;
                        //do nothing..
                    }
                    else
                    {
                        Z_MotorHomePosition = CCW_Limit;
                        NM.Z_FrameFreeMove(MStop);
                    }
                }
                else
                {
                    if (Z_MotorHomePosition == CW_Limit)
                    {
                        if (WaitingExitFromCCW_Limit >= 500)
                        {
                            WaitingExitFromCW_Limit = 0;
                            Z_MotorHomePosition = udefine;
                        }
                    }
                }
            }
            else if (Button_0)
            {
                if (Z_MotorHomePosition == udefine || Z_MotorHomePosition == CCW_Limit)
                {
                    NM.Z_Move_One_MM(-1000);
                    NM.Z_FrameFreeMove(Move);
                }
                if (digitalRead(HomePositoin_SW) == HomePosition)
                {
                    if (Z_MotorHomePosition == CCW_Limit)
                    {
                        WaitingExitFromCW_Limit = 0;
                        //do nothing..
                    }
                    else
                    {
                        Z_MotorHomePosition = CW_Limit;
                        NM.Z_FrameFreeMove(MStop);
                    }
                }
                else
                {
                    if (Z_MotorHomePosition == CCW_Limit)
                    {
                        if (WaitingExitFromCW_Limit >= 500)
                        {
                            WaitingExitFromCCW_Limit = 0;
                            Z_MotorHomePosition = udefine;
                        }
                    }
                }
            }
        }
        else
        {
            NM.Z_FrameFreeMove(Stop);
            if (Z_MotorHomePosition == CCW_Limit || Z_MotorHomePosition == CW_Limit)
            {
                //do nothing
                //Serial.println(Z_MotorHomePosition, DEC);
            }
            else
            {
                //Serial.println(Z_MotorHomePosition, DEC);
                Z_MotorHomePosition = udefine;
            }
        }
        // if (Serial.available())
        // {
        //     Var = Serial.read();
        //     if (Var == 'A' || Var == 'a')
        //     {
        //         Serial.println(Z_MotorHomePosition, DEC);
        //     }
        //     Var = 0;
        // }
        //--------------------

        if (Button_3)
        {
            // OFF_All_motor();
            //tm.setLED(3, 1);
        }
        else
        {
            //tm.setLED(3, 0);
        }
        //--------------------
        if (Button_2)
        {
            if (buttonPress == No)
            {
                buttonPress = Yes;
                Enable_Disable = !Enable_Disable;
                if (Enable_Disable)
                {
                    ON_All_motor();
                }
                else
                {
                    OFF_All_motor();
                }
            }
        }
        else
        {
            if (buttonPress == Yes)
            {
                buttonPress = No;
            }
        }
    }
}
#endif