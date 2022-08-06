#ifndef MAIN_PROJECTVAR_H
#define MAIN_PROJECTVAR_H
int count = 0;

/*XYZ motor manual Control Variable*/
enum
{
    Start_Cycl = 1,
    Move,
    Stop,
    Initiat_Stop,
    LimitSwitch,
    MStop
};

/*END*/
volatile boolean Enable_Disable = 0;
volatile boolean buttonPress = No;

/*Z motor manual Control Variable*/
enum
{
    udefine = 1,
    CCW_Limit,
    CW_Limit
};
enum
{
    Ready_Stich = 1,
    AvailableToMove,
    Wait,
    WaitXY,
    WaitZ,
    WaitA,
    Finished
};

typedef enum
{
    doStichingundefine = 1,
    doReadStichfunction,
    doStichingForward,
    doPrepareStichingForward,
    doStichingReverse,
    doPrepareStichingReverse,
    doJumpForward,
    doColourChange,
    doStichFManual,
    doStichFManualStop,
    doStichRManual,
    doStichRManualStop,
    doResumeStiching,
    doPauseStiching,
    doDebugPauseStiching,
    doEOD,
    doStopStiching
} designtostich;

typedef enum
{
    NPSundefine = 1,
    ThreadPosition,
    ReturnToPastPosition,
} ThreadPositionSetting;

typedef enum
{
    doundefine = 1,
    doHoming,
    doZHomingDone,
    doXZeroDone,
    doYZeroDone,
    doZHomingError,
    YZeroError,
    XZeroError,
    goThreadPosition,
    goLeftZero,
    goRightZero,
    goUpZero,
    goDownZero,
    doStop
} allzero;

typedef enum
{
    BreakUndefine = 1,
    BreakMotorSpeedSet,
    BreakON,
    BreakOFF,
    BreakStop,
    BreakDisable,
    BreakEnable,
    BreakError,
    BreakMovingStart,
    BreakDone
} BreakMotor;

typedef enum
{
    mundefine = 1,   //1
    mManual,         //2
    mFristStich,     //3
    mStich,          //4
    mStiching,       //5
    mNeedlingMoving, //6
    mStichDone,      //7
    mStichLoad,      //8
    mPauseStich,     //9
    mStopStich,      //10
    mStichFileOpen,  //11
    mStichFileClose, //12
    mFreeMove,       //13
    mTrim,           //14
    mColourChange,   //15
    mEndDesign,      //16
    mXYZHoming,      //17
    mZHoming,        //18
    mYHoming,        //19
    mXHoming,        //20
    mBreak,          //21
    mJump,           //22
    mJumpDone,       //23
    mJumping,        //24
    mThreadPositon,  //25
    mStop,           //26
    mThreadBreak,    //27
    mShuttleMoving,  //28
    mDelayFrameError
} MachineCurrentStatus;
typedef enum
{
    StichLoadundefine,
    StichLoaded = 1,
    StichunLoaded
} StichLoadStatus;
typedef enum
{
    FirstStich = 1,
    ContinueStich,
    LastStich,
    ZeorStich
} stichindex;
typedef enum
{
    PositionUndefine = 1,
    Positiondefine,
    MotorStopInit,
    MotorMovingInitiat,
    ZeroDone,
    ONorOFFDone,
    ZeroError,
    ForceStop
} PositionStatus;

uint8_t Z_MotorHomePosition = udefine;
/*Machine Control Variables*/
boolean Button_7 = 0;
boolean Button_6 = 0;
boolean Button_5 = 0;
boolean Button_4 = 0;
boolean Button_3 = 0;
boolean Button_2 = 0;
boolean Button_1 = 0;
boolean Button_0 = 0;

uint8_t Var = 0;
/*Design Save via Serial*/
enum
{
    SavedSuccess = 1,
    ReceivingMode
};
/*End*/
enum
{
    undefine = 1,
    noreadyExecute,
    readyExecuteStich,
    readyExecuteJump,
    readyExecuteVirtualJump,
    readyExecuteColour,
    StichLoad
};

/*Design to motor*/
sint16_t Y_distance;
sint16_t X_distance;
float_t Z_degree;
uint8_t F_machine;
uint8_t end_of_jump = 'S';         // 'S'= START
uint8_t break_flag_internal = 'O'; // 'O'=ON
/**/
typedef enum
{
    bUndefine = 1,
    bEngaged,
    bDisengaged,
    bEnable,
    bDisable,
    bStop
} BreakMotorAction;

// union {
//     char FlagsData;

//     struct
//     {
//         unsigned int FlagDual : 1;
//         unsigned int FlagAutoBritt : 1;
//         unsigned int FlagsDualP1 : 1;
//         unsigned int FlagsDualP2 : 1;
//         unsigned int FlagsDualP3 : 1;
//         unsigned int FlagsDualP4 : 1;
//         unsigned int FlagsDualP5 : 1;
//         unsigned int FlagsDualP6 : 1;
//     } Bits;
// } Flags;
//-------------------------------
#endif
