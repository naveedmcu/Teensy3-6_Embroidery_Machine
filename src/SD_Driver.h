#include <Arduino.h>
// #include <SD.h>
#include "SdFat.h"
#include "stddef.h"
#include "mainProjectDefinition.h"
#include "DesignFunctionDefinition.h"
#include "mainProjectVar.h"
//-----------------------------------
// #define DesignData_Show_via_Serial
//-----------------------------------

// 32 KiB buffer.
const size_t BUF_DIM = 32768;

// 8 MiB file.
const uint32_t FILE_SIZE = 256UL * BUF_DIM;

SdFatSdio sd;

SdFatSdioEX sdEx;

File file;

uint8_t DataRCVFlagFirstTime = No;

uint8_t buf[BUF_DIM];

// buffer as uint32_t
uint32_t *buf32 = (uint32_t *)buf;

size_t Count = 0;
char filename[] = "Design0001.TXT";
char filename1[] = "TESTRUN2.TXT";
char filename2[] = "Design0002.TXT";

String DFilename;

// Total usec in read/write calls.
uint32_t totalMicros = 0;
// Time in yield() function.
uint32_t yieldMicros = 0;
// Number of yield calls.
uint32_t yieldCalls = 0;
// Max busy time for single yield call.
uint32_t yieldMaxUsec = 0;
// Control access to the two versions of SdFat.
bool useEx = false;

bool sdBusy();
uint32_t kHzSdClk();
void yield();
void runTest2();
void runTest();


bool sdBusy()
{
    return useEx ? sdEx.card()->isBusy() : sd.card()->isBusy();
}
void errorHalt(const char *msg)
{
    if (useEx)
    {
        sdEx.errorHalt(msg);
    }
    else
    {
        sd.errorHalt(msg);
    }
}
uint32_t kHzSdClk()
{
    return useEx ? sdEx.card()->kHzSdClk() : sd.card()->kHzSdClk();
}
void yield()
{
    // Only count cardBusy time.
    if (!sdBusy())
    {
        return;
    }
    uint32_t m = micros();
    yieldCalls++;
    while (sdBusy())
    {
        // Do something here.
    }
    m = micros() - m;
    if (m > yieldMaxUsec)
    {
        yieldMaxUsec = m;
    }
    yieldMicros += m;
}
void runTest2()
{
    // Zero Stats
    totalMicros = 0;
    yieldMicros = 0;
    yieldCalls = 0;
    yieldMaxUsec = 0;
    if (!file.open("TeensyDemo.bin", O_RDWR | O_CREAT))
    {
        errorHalt("open failed");
    }
    Serial.println("\nsize,write,read");
    Serial.println("bytes,KB/sec,KB/sec");
    for (size_t nb = 512; nb <= BUF_DIM; nb *= 2)
    {
        file.truncate(0);
        uint32_t nRdWr = FILE_SIZE / nb;
        Serial.print(nb);
        Serial.print(',');
        uint32_t t = micros();
        for (uint32_t n = 0; n < nRdWr; n++)
        {
            // Set start and end of buffer.
            buf32[0] = n;
            buf32[nb / 4 - 1] = n;
            if (nb != file.write(buf, nb))
            {
                errorHalt("write failed");
            }
        }
        t = micros() - t;
        totalMicros += t;
        Serial.print(1000.0 * FILE_SIZE / t);
        Serial.print(',');
        file.rewind();
        t = micros();

        for (uint32_t n = 0; n < nRdWr; n++)
        {
            if ((int)nb != file.read(buf, nb))
            {
                errorHalt("read failed");
            }
            // crude check of data.
            if (buf32[0] != n || buf32[nb / 4 - 1] != n)
            {
                errorHalt("data check");
            }
        }
        t = micros() - t;
        totalMicros += t;
        Serial.println(1000.0 * FILE_SIZE / t);
    }
    file.close();
    Serial.print("\ntotalMicros  ");
    Serial.println(totalMicros);
    Serial.print("yieldMicros  ");
    Serial.println(yieldMicros);
    Serial.print("yieldCalls   ");
    Serial.println(yieldCalls);
    Serial.print("yieldMaxUsec ");
    Serial.println(yieldMaxUsec);
    Serial.print("kHzSdClk     ");
    Serial.println(kHzSdClk());
    Serial.println("Done");
}
void runTest()
{
    size_t nb = 0;
    nb=sdEx.exists(filename1);
    if (sdEx.exists(filename1))
    {
        sdEx.remove(filename1);
    }
    if (!file.open(filename1, O_RDWR | O_CREAT))
    {
        errorHalt("open failed");
    }
    Serial.println("\nFile Created");
    nb = 5;
    buf[0] = 'H';
    buf[1] = 'e';
    buf[2] = 'l';
    buf[3] = 'l';
    buf[4] = 'o';

    if (nb != file.write(buf, nb))
    {
        errorHalt("write failed");
    }
    file.rewind();
    file.close();
    if (!file.open(filename1))
    {
        errorHalt("open failed");
    }
    while (file.available())
    {
        Serial.write(file.read());
    }
    file.close();
    Serial.println("\nFile Closed");
}
void File_Info()
{
    sdEx.ls("/", LS_R);
}
void SD_FileOpen()
{
    // DFilename = String(filename2);
    if (sdEx.exists(filename2))
    {
        sdEx.remove(filename2);
    }
    if (!file.open(filename2, O_RDWR | O_CREAT))
    {
        errorHalt("open failed");
    }
    else
    {
        while (Nextion.availableForWrite() <= 30)
        {
            ;
        }
        Nextion.print("t0.txt=\"Creat New File\"");
        Nextion.print("\xFF\xFF\xFF");
        Serial.println("\nFile OPEN");
    }
    file.truncate(0);
    file.rewind();
    Serial.println("WaitingStich!");
    while (Nextion.availableForWrite() <= 30)
    {
        ;
    }
    Nextion.print("t1.txt=\"Waiting Stich!\"");
    Nextion.print("\xFF\xFF\xFF");
    Serial.clear();
    Serial1.flush();
    DataRCVFlagFirstTime = No;
}

int8 SerialDataSave_into_SD()
{
    size_t nb = 0;
    size_t RCVdata = Serial1.available();
    size_t rcvByte;
    static uint8_t FiletoCloseCounter = 0;
    static uint8_t SartFileClose = No;
    if (RCVdata > 0)
    {
        if (DataRCVFlagFirstTime == No)
        {
            DataRCVFlagFirstTime = Yes;
            // Serial.println("Data Receiving!!");
            Nextion.print("t1.txt=\"Data Coming\""); // ON print when off button press
            Nextion.print("\xFF\xFF\xFF");
        }
        //Serial.println(RCVdata);
        if (RCVdata > 60)
        {
            Serial.println(RCVdata);
        }
        nb = 1;
        buf[0] = Serial1.read();
        rcvByte = buf[0];
        //Serial.println(buf[0]);
        if (nb != file.write(buf, nb))
        {
            errorHalt("write failed");
        }
        else
        {
            if (rcvByte == EOD)
            {
                // Serial.println("\nEnd of Design Start");
                SartFileClose = Yes;
                FiletoCloseCounter = 0;
            }
            else if (SartFileClose == Yes)
            {
                FiletoCloseCounter++;
                if (FiletoCloseCounter >= 2)
                {
                    SartFileClose = No;
                    FiletoCloseCounter = 0;
                    // Serial.println("Design Saved");
                    Nextion.print("page3.t1.txt=\"Design Saved!\""); // ON print when off button press
                    Nextion.print("\xFF\xFF\xFF");
                    return SavedSuccess;
                }
            }
        }
    }
    return ReceivingMode;
    // size_t RETURN_DATA = nb;
    // nb = 5;
    // buf[0] = 'F';
    // buf[1] = 'I';
    // buf[2] = 'L';
    // buf[3] = 'E';
    // buf[4] = 'R';
    // if (nb == 5)
    // {
    //     RETURN_DATA = file.write(buf, nb);
    //     if (nb != RETURN_DATA)
    //     {
    //         errorHalt("write failed");
    //     }
    //     Serial.println(RETURN_DATA);
    // }
}
void CloseFile_SDCard()
{
    file.rewind();
    file.close();
    Serial.println("\nFile Close");
}
void SD_FileOpen_Read()
{
    // size_t SavedByte_SD = 0;
    // uint32_t TotalBytes = 0;
    // Serial.println("\nFile READ Start");
    // if (!file.open(filename2))
    // {
    //     errorHalt("open failed");
    // }
    // file.rewind();
    // Serial.println("Available Write Byte:");
    // Serial.println(Serial.availableForWrite());
    // Serial.println("File Size");
    // Serial.println(file.size());

    //     while (file.available() > 0)
    //     {
    //         if (Serial.availableForWrite() >= 63)
    //         {
    //             SavedByte_SD = file.read();
    // #ifdef DesignData_Show_via_Serial
    //             Serial.println(SavedByte_SD);
    // #endif
    //             TotalBytes++;
    //             if (SavedByte_SD == EOD)
    //             {
    //                 TotalBytes--;

    //                 SavedByte_SD = file.read();
    // #ifdef DesignData_Show_via_Serial
    //                 Serial.println(SavedByte_SD);
    // #endif
    //                 SavedByte_SD = file.read();

    // #ifdef DesignData_Show_via_Serial
    //                 Serial.println(SavedByte_SD);
    // #endif

    //                 TotalBytes = TotalBytes / 3;
    //                 Serial.printf("TotalStich:%lu", TotalBytes);
    //                 //Nextion.printf("n0.val=%lu", TotalBytes);
    //                 //Nextion.print(0xff);
    //                 //Nextion.print(0xff);
    //                 //Nextion.print(0xff);

    //                 // Nextion.write(0xff);
    //                 // Nextion.write(0xff);
    //                 // Nextion.write(0xff);
    //                 Serial.println("\r\ndesign Ended");
    //             }
    //             //Serial.write('1');
    //             //Serial.flush();
    //         }
    //     }
    // file.close();
    // Serial.println("\nFile Closed");
}
