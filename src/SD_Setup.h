#ifndef SD_SETUP_H
#define SD_SETUP_H
#include "SD_Driver.h"
#include "debugSerial.h"

void SD_Setup() // SD begin funtion Called
{
    if (!sdEx.begin())
    {
        sd.initErrorHalt("SdFatSdioEX begin() failed");
    }
    sdEx.chvol();
    sdEx.cacheClear();
    // CloseFile_SDCard();
    Serial.printf("\r\nSDex Init");
    Serial.printf("\r\nSerial Test");
}
void SD_runtime()
{
    // if (cMCS == mManual)
    // {
    if (ByteRcv == 'L')
    {
        ByteRcv = 0;
        File_Info();
    }
    // }
}
#endif