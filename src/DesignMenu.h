#ifndef DESIGN_MENU_H
#define DESIGN_MENU_H

#include <Arduino.h>
#include <elapsedMillis.h>
#include "SD_Setup.h"
#include "MotorsSetup.h"
#include "MachineControlSetup.h"
#include "mainProjectVar.h"
#include "BreakMotor.h"

/**/
typedef enum
{
    _undefine,
    _newfile,
    _write,
    _read,
    _close,
    _load,
    _go,
    _manualforwardstich,
    _manualreversestich,
    _manualforwardstichstop,
    _manualreversestichstop,
    _manualreverseonestich,
    _manualforwardonestich,
    _homing,
    _leftup,
    _leftdown,
    _rightup,
    _rightdown,
    _yzero,
    _xzero,
    _zzero,
    _manual,
    _stop,
    _pause,
    _breakon,
    _breakoff,
    _powerup,
    _powerdown,
    _threadpos,
    _resetpos
} Commands;
/**/
elapsedMillis DataReceivedDelay;
typedef enum
{
    uFileCreat = 1,
    uFileOpen,
    uWrite,
    uRead,
    uLoad,
    uFileClose,
    uManual,
    uZPosition,
    uStart,
    uStichfManual,
    uStichrManual,
    uStichfManualStop,
    uStichrManualStop,
    uFirstStich,
    uZHomePositon,
    uYHomePositon,
    uXHomePositon,
    // uXYZHomePositon,
    uXYZLeftUPHomePositon,
    uXYZLeftDownHomePositon,
    uXYZRightUPHomePositon,
    uXYZRightDownHomePositon,
    uStop,
    uWait,
    uBreakON,
    uBreakOFF,
    uCancel,
    uPowerUP,
    uPowerDown,
    uUndefine
} userinputdata;

userinputdata UID;
Commands _cmd;
uint8_t computerByte = 0;
uint8_t analusisReturnValue = 0;
size_t DesignSaveProgressStatus = 0;

void GetfeedMenuData();
void ManualLevelOne();
void sendData();

void ManualLevelOne()
{
    switch (UID)
    {
    case uFileCreat:
    {
        if (cMCS == mManual)
        {
            Nextion.print("page 3"); // Go Button Green
            Nextion.print("\xFF\xFF\xFF");
            while (Nextion.availableForWrite() <= 30)
            {
                ;
            }
            SD_FileOpen();
            UID = uWrite;
        }
        break;
    }
    case uWrite:
    {
        if (cMCS == mManual)
        {
            DesignSaveProgressStatus = SerialDataSave_into_SD();
            if (DesignSaveProgressStatus == SavedSuccess)
            {
                fMCS = mStichFileClose;
                CloseFile_SDCard();
                ANALYSIS_START();
                UID = uFileClose;
            }
            else if (DesignSaveProgressStatus == ReceivingMode)
            {
                break;
            }
        }
        else
        {
            /* code */
        }
        break;
    }
    case uFileClose:
    {
        if (cMCS == mManual)
        {
            fMCS = mStichFileClose;
            CloseFile_SDCard();
            Nextion.print("page0.t1.txt=\"Close!\"");
            Nextion.write(0xff);
            Nextion.write(0xff);
            Nextion.write(0xff);
            tm.displayIntNum(0x00, true);
        }
        UID = uUndefine;
        break;
    }
    case uRead:
    {
        if (cMCS == mManual && fMCS == mStichFileClose)
        {
            SD_FileOpen_Read();
            analusisReturnValue = ANALYSIS_START();
        }
        UID = uUndefine;
        break;
    }
    case uLoad:
    {
        if (cMCS == mEndDesign)
        {
            fMCS = mStichFileClose;
            CloseFile_SDCard();
        }
        if (fMCS == mStichFileClose)
        {
            fMCS = mStichFileOpen;
            OpenDesignFile();
            tm.displayIntNum(0, true);
            Serial.println("mStichFileOpen");
            ZZERO = doHoming;
            AZERO = doHoming;
            BAction = BreakON;
            StichNumber = 0;
            StichIndex = ZeorStich;
            Nextion.print("page0.t1.txt=\"Load!\"");
            Nextion.write(0xff);
            Nextion.write(0xff);
            Nextion.write(0xff);
        }
        UID = uUndefine;
        break;
    }
    case uStart:
    {
        if (fMCS == mStichFileOpen && cMCS == mManual)
        {
            FM.Setup_StichFrameSpeed();
            fMCS = mStichLoad;
            StichStaus = Ready_Stich;
            DTS = doPrepareStichingForward;
            Serial.println("mStichingStart");
            Nextion.print("page1.b6.bco=1024"); // Go Button Green
            Nextion.print("\xFF\xFF\xFF");
            while (Nextion.availableForWrite() <= 30)
            {
                ;
            }
        }
        if (cMCS == mPauseStich || cMCS == mColourChange)
        {
            FM.Setup_StichFrameSpeed();
            DTS = doPrepareStichingForward;
            Serial.println("mResumeStiching");
            Nextion.print("page1.b6.bco=1024"); // Go Button Green
            Nextion.print("\xFF\xFF\xFF");
            while (Nextion.availableForWrite() <= 30)
            {
                ;
            }
        }
        UID = uUndefine;
        break;
    }
    //------------------------------------------------------
    case uWait:
    {
        uMCS = mPauseStich;
        UID = uUndefine;
        break;
    }
    //------------------------------------------------------
    // uStichfManual
    //------------------------------------------------------
    case uStichfManual:
    {
        if (fMCS == mStichFileOpen && cMCS == mManual)
        {
#ifdef debugStichManual
            Serial.println("mStichManual");
#endif
            fMCS = mStichLoad;
            DTS = doStichFManual;
        }
        if (cMCS == mPauseStich)
        {
            // FM.Setup_FrameSpeed();
            DTS = doStichFManual;
#ifdef debugStichManual
            Serial.println("Forward++");
#endif
        }
        UID = uUndefine;
        break;
    }
    //----------------
    //uStichfManualStop
    //----------------
    case uStichfManualStop:
    {
        if (cMCS == mPauseStich)
        {
            // FM.Setup_FrameSpeed();
            DTS = doStichFManualStop;
#ifdef debugStichManual
            Serial.println("Forward++Stop");
#endif
        }
        UID = uUndefine;
        break;
    }
    //------------------------------------------------------
    // uStichrManual
    //------------------------------------------------------
    case uStichrManual:
    {
        if (fMCS == mStichFileOpen && cMCS == mManual)
        {
#ifdef debugStichManual
            Serial.println("mStichRManual");
#endif
            fMCS = mStichLoad;
            DTS = doStichRManual;
        }
        if (cMCS == mPauseStich)
        {
            // FM.Setup_FrameSpeed();
            DTS = doStichRManual;
#ifdef debugStichManual
            Serial.println("Reverse--");
#endif
        }
        UID = uUndefine;
        break;
    }
    //-----------------
    //uStichrManualStop
    //------------------
    case uStichrManualStop:
    {
        if (cMCS == mPauseStich)
        {
            // FM.Setup_FrameSpeed();
            DTS = doStichRManualStop;
#ifdef debugStichManual
            Serial.println("Reverse--Stop");
#endif
        }
        UID = uUndefine;
        break;
    }
    //------------------------------------------------------
    case uYHomePositon:
    {
        if (cMCS == mManual || cMCS == mXHoming || cMCS == mZHoming)
        {
            cMCS = mYHoming;
            YZERO = goDownZero;
            // YZERO = goUpZero;
            Nextion.print("page0.t1.txt=\"Y Zero!\"");
            Nextion.write(0xff);
            Nextion.write(0xff);
            Nextion.write(0xff);
        }
        UID = uUndefine;
        break;
    }
    case uXHomePositon:
    {
        if (cMCS == mManual || cMCS == mZHoming || cMCS == mYHoming)
        {
            cMCS = mXHoming;
            XZERO = goRightZero;
            // XZERO = goLeftZero;
            Nextion.print("page0.t1.txt=\"X Zero!\"");
            Nextion.write(0xff);
            Nextion.write(0xff);
            Nextion.write(0xff);
        }
        UID = uUndefine;
        break;
    }
    case uXYZRightUPHomePositon:
    {
        cMCS = mXYZHoming;
        // ZZERO = doHoming;
        YZERO = goDownZero;
        XZERO = goRightZero;
        Nextion.print("page0.t1.txt=\"All Zero!\"");
        Nextion.write(0xff);
        Nextion.write(0xff);
        Nextion.write(0xff);
        // XZERO = goRightZero;
        // YZERO = goDownZero;

        UID = uUndefine;
        break;
    }
    case uXYZRightDownHomePositon:
    {
        cMCS = mXYZHoming;
        // ZZERO = doHoming;
        YZERO = goUpZero;
        XZERO = goRightZero;
        Nextion.print("page0.t1.txt=\"All Zero!\"");
        Nextion.write(0xff);
        Nextion.write(0xff);
        Nextion.write(0xff);
        // XZERO = goRightZero;
        // YZERO = goDownZero;

        UID = uUndefine;
        break;
    }
    case uXYZLeftUPHomePositon:
    {

        cMCS = mXYZHoming;
        // ZZERO = doHoming;
        YZERO = goDownZero;
        XZERO = goLeftZero;
        Nextion.print("page0.t1.txt=\"All Zero!\"");
        Nextion.write(0xff);
        Nextion.write(0xff);
        Nextion.write(0xff);
        // XZERO = goRightZero;
        // YZERO = goDownZero;

        UID = uUndefine;
        break;
    }
    case uXYZLeftDownHomePositon:
    {

        cMCS = mXYZHoming;
        // ZZERO = doHoming;
        YZERO = goUpZero;
        XZERO = goLeftZero;
        Nextion.print("page0.t1.txt=\"All Zero!\"");
        Nextion.write(0xff);
        Nextion.write(0xff);
        Nextion.write(0xff);
        // XZERO = goRightZero;
        // YZERO = goDownZero;

        UID = uUndefine;
        break;
    }
    case uZPosition:
    {
        if (cMCS == mManual || cMCS == mXHoming || cMCS == mYHoming)
        {
            cMCS = mZHoming;
            ZZERO = doHoming;
            Serial.println("dZPosition");
        }
        UID = uUndefine;
        break;
    }
    case uManual:
    {
        if (cMCS == mEndDesign)
        {
            fMCS = mStichFileClose;
            CloseFile_SDCard();
            Serial.println("mManual");
            cMCS = mManual;
        }
        if (cMCS == mStop)
        {
            Serial.println("mManual");
            cMCS = mManual;
        }
        if (cMCS == mManual)
        {
            Nextion.print("page0.t1.txt=\"Manual\""); // ON print when off button press
            Nextion.write(0xff);
            Nextion.write(0xff);
            Nextion.write(0xff);
        }
        UID = uUndefine;
        break;
    }
    case uStop:
    {
        Serial.println("mStop");
        cMCS = mStop;
        ZZERO = doStop;
        YZERO = doStop;
        XZERO = doStop;
        DTS = doStopStiching;
        FrameSpeed = undefineSpeed;
        Nextion.print("page0.t1.txt=\"Stop!\"");
        Nextion.write(0xff);
        Nextion.write(0xff);
        Nextion.write(0xff);
        // breakON;
        // ;
        UID = uUndefine;
        break;
    }
    case uBreakOFF:
    {
        Serial.println("mBreak");
        BAction = BreakOFF;
        UID = uUndefine;
        break;
    }
    case uBreakON:
    {
        Serial.println("mBreak");
        BAction = BreakON;
        UID = uUndefine;
        break;
    }
    case uPowerUP:
    {
        Serial.println("dPowerUP");
        ON_All_motor();
        UID = uUndefine;
        break;
    }
    case uPowerDown:
    {
        Serial.println("dPowerDown");
        OFF_All_motor();
        UID = uUndefine;
        break;
    }
    default:
        UID = uUndefine;
        break;
    }
}
void debugSerialDatafeed()
{
    uint8_t sDATA = 0;
    if (Serial.available())
    {
        sDATA = Serial.read();

        if (sDATA == 'N' || sDATA == 'n')
        {
            debugStichbyStich = debugStartStich;
        }

        if (cMCS == mManual)
        {
            if (sDATA == 'N' || sDATA == 'n')
            {
                debugZmotorPuls = plusNegative;
            }
            else if (sDATA == 'P' || sDATA == 'p')
            {
                debugZmotorPuls = plusPositive;
            }
        }
    }
}
void GetfeedMenuData()
{
    static uint8_t firstCharReceived = 0;
    if (firstCharReceived == No)
    {
        DataReceivedDelay = 0;
    }
    if (Nextion.available())
    {
        firstCharReceived = Yes;

        String data_received = "";
        if (DataReceivedDelay >= 100)
        {
            DataReceivedDelay = 0;
            while (Nextion.available())
            {
                data_received += char(Nextion.read());
                // computerByte = +char(Seria//l.read());
            }
            firstCharReceived = No;
            Serial.println(data_received);
        }
        if (data_received == "1") //1 new
        {
            _cmd = _newfile;
        }
        else if (data_received == "2") // read
        {
            _cmd = _read;
        }
        else if (data_received == "3") //close
        {
            _cmd = _close;
        }
        else if (data_received == "4") //load
        {
            _cmd = _load;
        }
        else if (data_received == "5") //5 go
        {
            _cmd = _go;
        }
        else if (data_received == "6") //homing
        {
            _cmd = _homing;
        }
        else if (data_received == "7") // xzero
        {
            _cmd = _xzero;
        }
        else if (data_received == "8") //yzero
        {
            _cmd = _yzero;
        }
        else if (data_received == "9") //zzero
        {
            _cmd = _zzero;
        }
        else if (data_received == "10") //manual
        {
            _cmd = _manual;
        }
        else if (data_received == "11") //stop
        {
            _cmd = _stop;
        }
        else if (data_received == "12") //pause
        {
            _cmd = _pause;
        }
        else if (data_received == "13") //breakon
        {
            _cmd = _breakon;
        }
        else if (data_received == "14") //breakoff
        {
            _cmd = _breakoff;
        }
        else if (data_received == "15") //powerup
        {
            _cmd = _powerup;
        }
        else if (data_received == "16") //powerdown
        {
            _cmd = _powerdown;
        }
        else if (data_received == "17") // manualfstich manualForwardStich
        {
            _cmd = _manualforwardstich;
        }
        else if (data_received == "18") // manualrstich manualReverseStich
        {
            _cmd = _manualreversestich;
        }
        else if (data_received == "19") //threadpos Thread position
        {
            _cmd = _threadpos;
        }
        else if (data_received == "20") //resetpos Retrun after Thread
        {
            _cmd = _resetpos;
        }
        else if (data_received == "21") //leftup
        {
            _cmd = _leftup;
        }
        else if (data_received == "22") //leftdown
        {
            _cmd = _leftdown;
        }
        else if (data_received == "23") //rightup
        {
            _cmd = _rightup;
        }
        else if (data_received == "24") //rightdown
        {
            _cmd = _rightdown;
        }
        else if (data_received == "25") //_manualforwardstichstop manualForwardStichStop
        {
            _cmd = _manualforwardstichstop;
        }
        else if (data_received == "26") //_manualreversestichstop manualReverseStichStop
        {
            _cmd = _manualreversestichstop;
        }
        else if (data_received == "27") //_manualreverseonestich manualReverseOneStich
        {
            _cmd = _manualreverseonestich;
        }
        else if (data_received == "28") //_manualforwardonestich manualForwardOneStich
        {
            _cmd = _manualforwardonestich;
        }
        data_received = "";
        sendData();
    }
}
void sendData()
{
    //---------------------------------------------------------------------------SWITCH
    switch (_cmd)
    {
        //-------------------------------------------------------------------------N -New File
    case _newfile:
    {
        UID = uFileCreat;
        _cmd = _undefine;
        break;
    }
        //-------------------------------------------------------------------------W -Write
    case _write:
    {
        UID = uWrite;
        _cmd = _undefine;
        break;
    }
        //-------------------------------------------------------------------------R -Read
    case _read:
    {
        UID = uRead;
        _cmd = _undefine;
        break;
    }
        //-------------------------------------------------------------------------C -Close
    case _close:
    {
        UID = uFileClose;
        _cmd = _undefine;
        break;
    }
        //-------------------------------------------------------------------------L -Load
    case _load:
    {
        UID = uLoad;
        _cmd = _undefine;
        break;
    }
        //-------------------------------------------------------------------------G -GO
    case _go:
    {
        UID = uStart;
        _cmd = _undefine;
        break;
    }
        //-------------------------------------------------------------------------G -ManualForwardStich
    case _manualforwardstich:
    {
        UID = uStichfManual;
        _cmd = _undefine;
        break;
    }
    case _manualforwardstichstop:
    {
        UID = uStichfManualStop;
        _cmd = _undefine;
        break;
    }
    case _manualreversestich:
    {
        UID = uStichrManual;
        _cmd = _undefine;
        break;
    }
    case _manualreversestichstop:
    {
        UID = uStichrManualStop;
        _cmd = _undefine;
        break;
    }

    // case _manualforwardonestich:
    // {
    //     //UID = ;
    //     //_cmd = _undefine;
    //     break;
    // }
    // case _manualreverseonestich:
    // {
    //     //UID = ;
    //     //_cmd = _undefine;
    //     break;
    // }
    //-------------------------------------------------------------------------Z _Homing
    case _zzero:
    {
        UID = uZPosition;
        _cmd = _undefine;
        break;
    }
        //-------------------------------------------------------------------------Y _Y Zero
    case _yzero:
    {
        UID = uYHomePositon;
        _cmd = _undefine;
        break;
    }
        //-------------------------------------------------------------------------X _X Zero
    case _xzero:
    {
        UID = uXHomePositon;
        _cmd = _undefine;
        break;
    }
        //-------------------------------------------------------------------------H _XYZZero
    case _homing:
    {
        if (cMCS == mManual)
        {
            Nextion.print("page 4");
            Nextion.print("\xFF\xFF\xFF");
        }
        _cmd = _undefine;
        break;
    }
    case _leftup:
    {
        UID = uXYZLeftUPHomePositon;
        _cmd = _undefine;
        break;
    }
    case _leftdown:
    {
        UID = uXYZLeftDownHomePositon;
        _cmd = _undefine;
        break;
    }
    case _rightup:
    {
        UID = uXYZRightUPHomePositon;
        _cmd = _undefine;
        break;
    }
    case _rightdown:
    {
        UID = uXYZRightDownHomePositon;
        _cmd = _undefine;
        break;
    }
        //-------------------------------------------------------------------------M _Manual
    case _manual:
    {
        UID = uManual;
        _cmd = _undefine;
        break;
    }
        //-------------------------------------------------------------------------S -Stop
    case _stop:
    {
        UID = uStop;
        _cmd = _undefine;
        break;
    }
        //-------------------------------------------------------------------------Q -dWait
    case _pause:
    {
        UID = uWait;
        _cmd = _undefine;
        break;
    }
        //-------------------------------------------------------------------------A -Break
    case _breakon:
    {
        UID = uBreakON;
        _cmd = _undefine;
        break;
    }
        //-------------------------------------------------------------------------D -Break
    case _breakoff:
    {
        UID = uBreakOFF;
        _cmd = _undefine;
        break;
    }
        //-------------------------------------------------------------------------E -PowerON
    case _powerup:
    {
        UID = uPowerUP;
        _cmd = _undefine;
        break;
    }
        //-------------------------------------------------------------------------F -PowerOFF
    case _powerdown:
    {
        UID = uPowerDown;
        _cmd = _undefine;
        break;
    }
    case _threadpos:
    {
        NPS = ThreadPosition;
        _cmd = _undefine;
        break;
    }
    case _resetpos:
    {
        NPS = ReturnToPastPosition;
        _cmd = _undefine;
        break;
    }
    default:
        break;
    }
}
//-------------------------------------------------------------------------END SWITCH
#endif