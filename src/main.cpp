/**/
// 4-24-2020 // 3.0mm Stich at 250 RPM
// 5-05-2020 // Shuttle Test pass at 290RPM
// 5-08-2020 // Zmotor Stepper Rotoration Error Fix with 200 plus full round and 1/5 microstepping Setting
// Amotor setting 1000 plus per Round Servo Motor //
// All Angular motion Fixed and Tested with severial Design
//wORKING OF sHUTTLE MOTOR WITH 1:2 gEAR rATION
/**/
#include <Arduino.h>
#include "ProjectHeader.h"
//#define ENCODER_DO_NOT_USE_INTERRUPTS

void setup()
{
  delay(1000);
  Nextion.begin(NextionBaudrate, SERIAL_8N1);
  Serial1.begin(38400, SERIAL_8N1);
  Serial.begin(115200);
  TestPinInit();   // tEST pIN
  initMainShaft(); // Encoder Setup
  BlinkLEDSetup(); // ON board LED output Mode
  TM1638Setup();
  SM.AmotorInit();
  Limit_SW.init_LimitsSW();
  ThreadBreakIOinit();
  //ColourIOinit();
  //  SD_Setup(); // SD begin funtion Called
  //  AddBlinkTask();       // Threading Task Blinking ON Board LED
  //  AddTM1638BlinkTask(); // Threading Task Blinking TM1638- MSB LED
  SteppersMotorsInit(); // All Stepper motros DR and STEP Pin Define and OutPut mode set,All Motor Off
  //AccelStepper_init();
  // pinMode(14, INPUT);
  // pinMode(15, INPUT);
}

boolean firsttimeSDinit = No;

void loop()
{
  _cmd = _undefine;
  cMCS = mManual;
  StichStaus = undefine;
  fMCS = mStichFileClose;
  DTS = doStichingundefine;
  ZZERO = doundefine;
  YZERO = doundefine;
  XZERO = doundefine;
  /*BREAK INIT*/
  BMA = bUndefine;
  /**/
  if (firsttimeSDinit == No)
  {
    firsttimeSDinit = Yes;
    SD_Setup(); // SD begin funtion Called
    pinMode(14, INPUT);
    pinMode(15, INPUT);
  }
  while (1)
  {
    SD_runtime();
    MotorPluspolarity();
    debugSerialDatafeed();
    GetfeedMenuData();
    ManualLevelOne();
    f_Stiching();
    ManualFrameAdjestMove();
    f_ZZERO();
    f_AZERO();
    f_YZERO();
    f_XZERO();
    f_BreakONOFF(cMCS);
    f_NeedleThreadPosition(cMCS);
    /**/
    RunTimeEncoderRead();
    //---------------------------------------
    // END
    //---------------------------------------
    Simple_MachineControl();
    Limit_SW.LimitSWLoop(cMCS); // Only Indication on Serial as well as Led 0 and 1 on tm138 board
    //--------------------
    //--------------------
    SuperLoop_BlinkTask(); // on board RED LED blinking
    //--------------------
    TM1635_LED_Blink(); // Led Blinking on Tm1638 3 number 0 to 7 total LED
                        //---------------------------------------
  }
}