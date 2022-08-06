#ifndef DEBUG_SERIAL_H
#define DEBUG_SERIAL_H
//-----------------------------------------------------
// #define StichDataDisplay
//-----------------------------------------------------
// #define StichPulsDisplay
//-----------------------------------------------------
//  #define debugAddingJumpStich
//-----------------------------------------------------
// #define debugSetup_JumpFrameSpeed
//-----------------------------------------------------
// #define debugSetup_StichFrameSpeed
//-----------------------------------------------------
// #define debugSetupinitVariables
//-----------------------------------------------------
// #define debugf_BreakONOFF
//-----------------------------------------------------
// #define Debug_Z_Move
//-----------------------------------------------------
// #define Debug_A_Move
//-----------------------------------------------------
// #define StichByStich
//-----------------------------------------------------
// #define FetchForwardStichData
//-----------------------------------------------------
// #define FetchReverseStichData
//-----------------------------------------------------
// #define debugZstepPlus  // P for positive plus and N for Negative Plus
//-----------------------------------------------------
//-----------------------------------------------------
//#define debugStichManual // DesignMenu.h
//-----------------------------------------------------
//  #define debugEncoderfunction // Encoder.h
// #define debugEncoderreading // Encoder.h
//-----------------------------------------------------
  // #define debugMotorPlusPolarity //MotorSetup.h
//-----------------------------------------------------
#define debufSerialdebugInput //debugSerial.h
//-----------------------------------------------------

//  #define ON_OFF_withfreeMove
#define debugStartStich 1
#define debugStopStich 0
#define plusNegative 1
#define plusPositive 2
#define plusundefine 3
uint8_t debugStichbyStich = debugStopStich;
uint8_t debugZmotorPuls = plusNegative;

uint8_t ByteRcv = 0;
#ifdef debufSerialdebugInput
void SerialdebugInput()
{
  if (Serial.available())
  {
    ByteRcv = Serial.read();
  }
}
#endif
#endif
