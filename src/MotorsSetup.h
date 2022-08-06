#ifndef TEST_XY_STEPPER_H
#define TEST_XY_STEPPER_H

/**/
#include <Arduino.h>
//#include <AccelStepper.h>
#include "TeensyStep.h"
#include "mainProjectDefinition.h"
#include "TM1638Setup.h"
#include "debugSerial.h"
/**/
#define Z_Motor_StepPin 2 // Zero to Increment
#define Z_Motor_DirPin 3  // Zero to Dir change
#define Z_Motor_EnPin 4   // Zero to Enable

#define Y_Motor_StepPin 5 // Zero to Increment
#define Y_Motor_DirPin 6  // Zero to Dir change
#define Y_Motor_EnPin 7   // Zero to Enable

#define X_Motor_StepPin 8 // Zero to Increment
#define X_Motor_DirPin 9  // Zero to Dir change
#define X_Motor_EnPin 10  // Zero to Enable

#define B_Motor_StepPin 23 // Zero to Increment
#define B_Motor_DirPin 22  // Zero to Dir change
#define B_Motor_EnPin 21   // Zero to Enable

#define A_Motor_StepPin 18 // Zero to Increment
#define A_Motor_DirPin 19  // Zero to Dir change
#define A_Motor_EnPin 20   // Zero to Enable
/**/

// Stepper Aaxis(Z_Motor_StepPin, Z_Motor_DirPin); //STEP pin =  2, DIR pin = 3
// Stepper Yaxis(Y_Motor_StepPin, Y_Motor_DirPin); //STEP pin =  5, DIR pin = 6
// Stepper Xaxis(X_Motor_StepPin, X_Motor_DirPin); //STEP pin =  8, DIR pin = 9
// Stepper Baxis(B_Motor_StepPin, B_Motor_DirPin); //STEP pin = 23, DIR pin = 22
// Stepper Zaxis(A_Motor_StepPin, A_Motor_DirPin); //STEP pin = 18, DIR pin = 19

Stepper Zaxis(Z_Motor_StepPin, Z_Motor_DirPin); //STEP pin =  2, DIR pin = 3
Stepper Yaxis(Y_Motor_StepPin, Y_Motor_DirPin); //STEP pin =  5, DIR pin = 6
Stepper Xaxis(X_Motor_StepPin, X_Motor_DirPin); //STEP pin =  8, DIR pin = 9
Stepper Baxis(B_Motor_StepPin, B_Motor_DirPin); //STEP pin = 23, DIR pin = 22
Stepper Aaxis(A_Motor_StepPin, A_Motor_DirPin); //STEP pin = 18, DIR pin = 19

StepControl controllerXY(10, 5000);
StepControl controllerY(10, 5000); // secondry use with A motor
StepControl controllerX(10, 5000); // Secondary use with B motor
StepControl controllerZ(10, 5000);

// AccelStepper X_axis(1, X_Motor_StepPin, X_Motor_DirPin); // pin 3 = step, pin 6 = direction
// AccelStepper Y_axis(1, Y_Motor_StepPin, Y_Motor_DirPin); // pin 4 = step, pin 7 = direction
// AccelStepper Z_axis(1, Z_Motor_StepPin, Z_Motor_DirPin); // pin 5 = step, pin 8 = direction

void ON_All_motor();
void OFF_All_motor();

void motorLoop()
{
    ON_All_motor();
    Zaxis.setTargetRel(1000); // Set target position to 1000 steps from current position
    controllerZ.move(Zaxis);  // Blocking Function
}

void X_Stepper_EN()
{
    digitalWrite(X_Motor_EnPin, 0);
}
void X_Stepper_DIS()
{
    digitalWrite(X_Motor_EnPin, 1);
}
void Y_Stepper_EN()
{
    digitalWrite(Y_Motor_EnPin, 0);
}
void Y_Stepper_DIS()
{
    digitalWrite(Y_Motor_EnPin, 1);
}
void Z_Stepper_EN()
{
    digitalWrite(Z_Motor_EnPin, 0);
}
void Z_Stepper_DIS()
{
    digitalWrite(Z_Motor_EnPin, 1);
}
void B_Stepper_EN()
{
    digitalWrite(B_Motor_EnPin, 0);
}
void B_Stepper_DIS()
{
    digitalWrite(B_Motor_EnPin, 1);
}

void A_Stepper_EN()
{
    digitalWrite(A_Motor_EnPin, 0);
}
void A_Stepper_DIS()
{
    digitalWrite(A_Motor_EnPin, 1);
}

void ON_All_motor()
{
    X_Stepper_EN();
    Y_Stepper_EN();
    Z_Stepper_EN();
    B_Stepper_EN();
    A_Stepper_EN();
    tm.setLED(2, 1);
    All_Motor_OFF = No;
    Nextion.print("page0.t1.txt=\"PowerUP\""); // ON print when on button press
    Nextion.write(0xff);
    Nextion.write(0xff);
    Nextion.write(0xff);
    while (Nextion.availableForWrite() <= 30)
    {
        ;
    }
    Nextion.print("page0.b0.bco=63488");
    Nextion.write(0xff);
    Nextion.write(0xff);
    Nextion.write(0xff);
    while (Nextion.availableForWrite() <= 30)
    {
        ;
    }
}

void OFF_All_motor()
{
    X_Stepper_DIS();
    Y_Stepper_DIS();
    Z_Stepper_DIS();
    B_Stepper_DIS();
    A_Stepper_DIS();
    tm.setLED(2, 0);
    All_Motor_OFF = Yes;
    Nextion.print("page0.t1.txt=\"PowerDown\""); // ON print when off button press
    Nextion.write(0xff);
    Nextion.write(0xff);
    Nextion.write(0xff);
    while (Nextion.availableForWrite() <= 30)
    {
        ;
    }
    Nextion.print("page0.b0.bco=1024");
    Nextion.write(0xff);
    Nextion.write(0xff);
    Nextion.write(0xff);
    while (Nextion.availableForWrite() <= 30)
    {
        ;
    }
}
void SteppersMotorsInit()
{
    pinMode(X_Motor_StepPin, OUTPUT);
    pinMode(Y_Motor_StepPin, OUTPUT);
    pinMode(Z_Motor_StepPin, OUTPUT);
    pinMode(B_Motor_StepPin, OUTPUT);
    pinMode(A_Motor_StepPin, OUTPUT);

    pinMode(X_Motor_EnPin, OUTPUT);
    pinMode(Y_Motor_EnPin, OUTPUT);
    pinMode(Z_Motor_EnPin, OUTPUT);
    pinMode(B_Motor_EnPin, OUTPUT);
    pinMode(A_Motor_EnPin, OUTPUT);
    /**/
    digitalWrite(X_Motor_StepPin, 1);
    digitalWrite(Y_Motor_StepPin, 1);
    digitalWrite(Z_Motor_StepPin, 1);
    digitalWrite(B_Motor_StepPin, 1);
    digitalWrite(A_Motor_StepPin, 1);
    /**/
    digitalWrite(X_Motor_EnPin, 1);
    digitalWrite(Y_Motor_EnPin, 1);
    digitalWrite(Z_Motor_EnPin, 1);
    digitalWrite(B_Motor_EnPin, 1);
    digitalWrite(A_Motor_EnPin, 1);
    /**/
    OFF_All_motor();
    // setup the motors

    Aaxis                        // AMOTOR IS SHUTTLE MOTOR
        .setMaxSpeed(50000)      // steps/s
        .setAcceleration(200000) // steps/s^2
        .setStepPinPolarity(LOW) // driver accepts active LOW pulses ((driver name:HBS507))
        // .setStepPinPolarity(HIGH) // driver accepts active active HIGH pulses (driver name:2H-865)
        .setInverseRotation(true) // Rotation Inverse
        .setPosition(0);          // set start position of counter use after homming position

    Zaxis                         // ZMOTOR IS NEEDLE MOTOR
        .setMaxSpeed(13500)       // steps/s
        .setAcceleration(375000)  // steps/s^2
                                  // .setStepPinPolarity(LOW)  // driver accepts active LOW pulses ((driver name:HBS507))
                                  // .setStepPinPolarity(LOW)  // driver accepts active LOW pulses (driver name:2H-865)
        .setStepPinPolarity(HIGH) // driver accepts active HIGH pulses (driver name:JK2060)
        .setInverseRotation(true) // Rotation Inverse
        .setPosition(0);          // set start position of counter use after homming position

    Xaxis
        .setMaxSpeed(20000)       // steps/s
        .setAcceleration(100000)  // steps/s^2
        .setStepPinPolarity(HIGH) // driver accepts active HIGH pulses (driver name:JK1545)
        //.setInverseRotation(false) // Rotation Inverse
        .setInverseRotation(true) // Rotation Inverse
        .setPosition(0);          // set start position of counter use after homming position

    Yaxis
        .setMaxSpeed(20000)       // steps/s
        .setAcceleration(100000)  // steps/s^2
        .setStepPinPolarity(HIGH) // driver accepts active HIGH pulses (driver name:JK1545)
        // .setInverseRotation(false) // Rotation Inverse
        .setInverseRotation(true) // Rotation Inverse
        .setPosition(0);          // set start position of counter use after homming position
                                  //-----------------------------------------------------------------------------------
    // Zaxis.setTargetRel(1000);
    // controllerZ.move(Zaxis);
    //-----------------------------------------------------------------------------------
}
void AccelStepper_init()
{
    // pinMode(X_Motor_StepPin, OUTPUT);
    // pinMode(Y_Motor_StepPin, OUTPUT);
    // pinMode(Z_Motor_StepPin, OUTPUT);
    // pinMode(X_Motor_DirPin, OUTPUT);
    // pinMode(Y_Motor_DirPin, OUTPUT);
    // pinMode(Z_Motor_DirPin, OUTPUT);
    // pinMode(X_Motor_EnPin, OUTPUT);
    // pinMode(Y_Motor_EnPin, OUTPUT);
    // pinMode(Z_Motor_EnPin, OUTPUT);
    // /**/
    // digitalWrite(X_Motor_StepPin, 1);
    // digitalWrite(Y_Motor_StepPin, 1);
    // digitalWrite(Z_Motor_StepPin, 1);
    // /**/
    // digitalWrite(X_Motor_EnPin, 1);
    // digitalWrite(Y_Motor_EnPin, 1);
    // digitalWrite(Z_Motor_EnPin, 1);
    // /**/
    // OFF_All_motor();
    // // setup the motors
    // X_axis.setMaxSpeed(100000);
    // X_axis.setSpeed(100000);
    // // X_axis.setAcceleration(300000);
    // X_axis.setPinsInverted(true, true, false);
    // /**/
    // Y_axis.setMaxSpeed(100000);
    // Y_axis.setSpeed(100000);
    // // Y_axis.setAcceleration(300000);
    // Y_axis.setPinsInverted(true, true, false);
    // /**/
    // Z_axis.setMaxSpeed(100000);
    // Z_axis.setSpeed(100000);
    // // Z_axis.setAcceleration(120000);
    // Z_axis.setPinsInverted(true, true, false);
}
//----------------------------------------
void Y_OneStepMove_Positiove()
{
    if (!controllerY.isRunning())
    {
        Yaxis.setTargetRel(6400);
        controllerY.moveAsync(Yaxis);
    }
}
void Y_OneStepMove_Negative()
{
    if (!controllerY.isRunning())
    {
        Yaxis.setTargetRel(-6400);
        controllerY.moveAsync(Yaxis);
    }
}
//----------------------------------------

//-----------------------------------------
void X_OneStepMove_Positiove()
{
    if (!controllerX.isRunning())
    {
        Xaxis.setTargetRel(6400);
        controllerX.moveAsync(Xaxis);
    }
}

void X_OneStepMove_Negative()
{
    if (!controllerX.isRunning())
    {
        Xaxis.setTargetRel(-6400);
        controllerX.moveAsync(Xaxis);
    }
}
void Y_OneStepMove_Home()
{
    if (!controllerY.isRunning())
    {
        Yaxis.setTargetAbs(0);
        controllerY.moveAsync(Yaxis);
    }
}
/**/
void Stepper_Motor_Moves()
{
    Xaxis.setTargetRel(500);
    Yaxis.setTargetRel(0);
    Zaxis.setTargetRel(0);
    controllerXY.moveAsync(Xaxis, Yaxis);
    controllerZ.moveAsync(Zaxis);
    while (controllerZ.isRunning() || controllerXY.isRunning())
    {
        delay(10);
    }
    Xaxis.setTargetRel(0);
    Yaxis.setTargetRel(1500);
    Zaxis.setTargetRel(4000);
    controllerXY.moveAsync(Xaxis, Yaxis);
    controllerZ.moveAsync(Zaxis);
    while (controllerZ.isRunning() || controllerXY.isRunning())
    {
        delay(10);
    }
    Xaxis.setTargetRel(-500);
    Yaxis.setTargetRel(0);
    Zaxis.setTargetRel(4000);
    controllerXY.moveAsync(Xaxis, Yaxis);
    controllerZ.moveAsync(Zaxis);
    while (controllerZ.isRunning() || controllerXY.isRunning())
    {
        delay(10);
    }
    Xaxis.setTargetRel(0);
    Yaxis.setTargetRel(-1500);
    Zaxis.setTargetRel(4000);
    controllerXY.moveAsync(Xaxis, Yaxis);
    controllerZ.moveAsync(Zaxis);
    while (controllerZ.isRunning() || controllerXY.isRunning())
    {
        delay(10);
    }
    /**/
    Zaxis.setTargetRel(4000);
    Xaxis.setTargetAbs(0);
    Yaxis.setTargetAbs(0);
    controllerXY.moveAsync(Xaxis, Yaxis);
    controllerZ.moveAsync(Zaxis);
    while (controllerZ.isRunning() || controllerXY.isRunning())
    {
        delay(10);
    }
    /**/
}

void MotorPluspolarity()
{
#ifdef debugMotorPlusPolarity
    SerialdebugInput();
    if (ByteRcv == 'P')
    {
        ByteRcv = 0;
        digitalWrite(X_Motor_StepPin, 1);
    }
    if (ByteRcv == 'N')
    {
        ByteRcv = 0;
        digitalWrite(X_Motor_StepPin, 0);
    }
#endif
}
#endif