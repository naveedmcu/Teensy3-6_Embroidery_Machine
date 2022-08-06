#ifndef DESIGN_TO_MOTOR_H
#define DESIGN_TO_MOTOR_H
#include <Arduino.h>
#include "debugSerial.h"
#include "SD_Setup.h"
#include "mainProjectVar.h"
#include "ZmotorSetup.h"
enum
{
    DesignFunction,
    Y_Value,
    X_Value
};

struct
{
    uint8_t Function;
    uint8_t Y_value;
    uint8_t X_value;
    uint8_t rFunction = 0;
    uint8_t rY_value = 0;
    uint8_t rX_value = 0;
} st;

void SerialBLocking();
char ANALYSIS_START();
void chaking_stich_length();
void SD_FileOpen_Read();
void FatchStichData();
void FatchPreviousStichData();
/**/
void stichtoMachine()
{
}
/**/
// void DesignFileOpen(String _filename)
// {
// }
void OpenDesignFile()
{
    if (!file.open(filename2))
    {
        errorHalt("open failed");
    }
    Serial.println("\nDesing Open Success");
    Serial.println("File Size");
    Serial.println(file.size());
    file.rewind();
}

void FatchStichData()
{
    // uint32_t Cposition;
    // Cposition = file.position();
    // Serial.printf("fPosition:");
    // Serial.println(Cposition);

    if (file.available() > 0)
    {
        st.Function = file.read();
        // Serial.println(st.Function);
    }
    if (file.available() > 0)
    {
        st.Y_value = file.read();
        // Serial.println(st.Y_value);
    }
    if (file.available() > 0)
    {
        st.X_value = file.read();
        // Serial.println(st.X_value);
    }
#ifdef FetchForwardStichData
    Serial.println(">>>>>");
    Serial.println(st.Function);
    Serial.println(st.Y_value);
    Serial.println(st.X_value);
    Serial.println(">>>>>");
#endif
}
void FatchPreviousStichData()
{
    sint32_t Cposition = 0;
    if (file.available() > 0)
    {
        Cposition = file.position();
        // Serial.printf("cPosition:");
        // Serial.println(Cposition);
        if (Cposition >= 3)
        {
            Cposition = Cposition - 3;
            // Serial.printf("rPosition:");
            // Serial.println(Cposition);
            file.seek(Cposition);
            // Cposition = file.position();
            // Serial.printf("rSeekPosition:");
            // Serial.println(Cposition);
            if (file.available() > 0)
            {
                st.rFunction = file.read();
            }
            if (file.available() > 0)
            {
                st.rY_value = file.read();
            }
            if (file.available() > 0)
            {
                st.rX_value = file.read();
            }
            file.seek(Cposition);
        }
#ifdef FetchReverseStichData
        Serial.println("//////");
        Serial.println(st.rFunction);
        Serial.println(st.rY_value);
        Serial.println(st.rX_value);
        Serial.println("//////");
#endif
    }
}
/**/
void SetupinitVariables()
{
    end_of_jump = 'S';         // 'S'= START
    break_flag_internal = 'O'; // 'O'=ON
#ifdef debugSetupinitVariables
    Serial.println("end_of_jump='S':break_flag_internal='O'");
#endif
}
/**/
//------------------------------------------------------------------------------
uint8_t function_chk(uint8 function_id)
{
    uint8_t returnType = 0;
    // static char end_of_jump='S',break_flag_internal='O';  // 'S'= START 'F'OFF 'O'=ON
    //------------------------------------------------------------------------------
    if (function_id == 0 && end_of_jump == 'S') // stich data
    {
        if (break_flag_internal == 'O') // 'O'= ON
        {
            break_flag_internal = 'F'; //OFF
            // if (back_start == 'S')
            // {
            //     break_off();
            // }
        }
        end_of_jump = 'S';
        returnType = readyExecuteStich;
    }
    //------------------------------------------------------------------------------
    else if (function_id == 1) // jump data
    {
        if (break_flag_internal == 'F')
        {
            break_flag_internal = 'O'; //'O'=ON
            // if (back_start == 'S')
            // {
            //     break_on();
            // }
        }
        end_of_jump = 'J'; //'J' JUMP
        returnType = readyExecuteJump;
    }
    //------------------------------------------------------------------------------
    else if (function_id == 0 && end_of_jump == 'J')
    {
        end_of_jump = 'S'; //'S'= START
        Serial.println("VCjump");
        returnType = readyExecuteVirtualJump;
    }
    else if (function_id == 0X0B || function_id == 0X0C || function_id == 0X0D ||
             function_id == 0X0E || function_id == 0X0F || function_id == 0X00 ||
             function_id == 0X01 || function_id == 0X02 || function_id == 0X03)
    {
        // break_on();
        break_flag_internal = 'O';
        returnType = readyExecuteColour;
    }
    else
    {
        returnType = undefine;
    }
    return returnType;
}
/**/

uint8_t GET_DATA(int8 f_r)
{
    char function_name = 0;
    uint8_t function = 0;
    uint8_t returnfunction = 0;
    sint8 Y_Val = 0;
    sint8 X_Val = 0;

    if (f_r == 'F') //------------------------------ FORWORD FUNCTION
    {
        FatchStichData();
        if (st.Function != 248)
        {
            function = st.Function;
            function_name = function & 0xF0;
            //    UART2_Write(STOR[BLOCK]);
            if (function_name == 0x80 || function_name == 0xA0 || function_name == 0xC0 || function_name == 0xE0)
            {
                if (function_name == 0x80)
                {
                    // X_Val_N();
                    // Y_Val_N();
                    X_Val = st.X_value * -1;
                    Y_Val = st.Y_value * -1;
                }
                else if (function_name == 0xA0)
                {
                    // X_Val_P();
                    // Y_Val_N();
                    X_Val = st.X_value;
                    Y_Val = st.Y_value * -1;
                }
                else if (function_name == 0xC0)
                {
                    // X_Val_N();
                    // Y_Val_P();
                    X_Val = st.X_value * -1;
                    Y_Val = st.Y_value;
                }
                else if (function_name == 0xE0)
                {
                    // X_Val_P();
                    // Y_Val_P();
                    X_Val = st.X_value;
                    Y_Val = st.Y_value;
                }
                Y_distance = Y_Val;
                X_distance = X_Val;
                if (Y_distance == 0 && X_distance == 0)
                {
                    // Do nothing
                }
                else
                {
                    Z_degree = NM.GET_DEGREE(st.Function, st.Y_value, st.X_value);
                }
#ifdef StichDataDisplay
                Serial.println("*StichData*");
                Serial.println(function, DEC);
                Serial.println(st.Y_value, DEC);
                Serial.println(st.X_value, DEC);
                Serial.println(Z_degree, 2);
                Serial.println("*****");
#endif
                function_name = function & 0x0F;
                returnfunction = function_chk(function_name);
                return returnfunction;
            }
        }
        else
        {
#ifdef StichDataDisplay
            Serial.println("*****");
            Serial.println(st.Function, DEC);
            Serial.println(st.Y_value, DEC);
            Serial.println(st.X_value, DEC);
#endif
            return EOD;
        }
    }
    ///------------------------------ REVERSE FUNCTION
    else if (f_r == 'R')
    {
        FatchPreviousStichData();
        if (st.rFunction != 248)
        {
            function = st.rFunction;
            function_name = function & 0xF0;
            //    UART2_Write(STOR[BLOCK]);
            if (function_name == 0x80 || function_name == 0xA0 || function_name == 0xC0 || function_name == 0xE0)
            {
                if (function_name == 0x80)
                {
                    // X_Val_N();
                    // Y_Val_N();
                    X_Val = st.rX_value * -1;
                    Y_Val = st.rY_value * -1;
                }
                else if (function_name == 0xA0)
                {
                    // X_Val_P();
                    // Y_Val_N();
                    X_Val = st.rX_value;
                    Y_Val = st.rY_value * -1;
                }
                else if (function_name == 0xC0)
                {
                    // X_Val_N();
                    // Y_Val_P();
                    X_Val = st.rX_value * -1;
                    Y_Val = st.rY_value;
                }
                else if (function_name == 0xE0)
                {
                    // X_Val_P();
                    // Y_Val_P();
                    X_Val = st.rX_value;
                    Y_Val = st.rY_value;
                }
                Y_distance = Y_Val * -1;
                X_distance = X_Val * -1;
                if (Y_distance == 0 && X_distance == 0)
                {
                    // Do nothing
                }
                else
                {
                    Z_degree = NM.GET_DEGREE(st.rFunction, st.rY_value, st.rX_value);
                }
#ifdef StichDataDisplay
                Serial.println("*StichData*");
                Serial.println(function, DEC);
                Serial.println(st.rY_value, DEC);
                Serial.println(st.rX_value, DEC);
                Serial.println(Z_degree, 2);// leading zero 2 after '.'
                Serial.println("*****");
#endif
                function_name = function & 0x0F;
                returnfunction = function_chk(function_name);
                return returnfunction;
            }
        }
        else
        {
            // function = st.rFunction;
            return EOD;
        }
    }

    return undefine;
}
//------------------------------------------------------------------------------
//   chaking_stich_length();
//------------------------------------------------------------------------------
void chaking_stich_length()
{
    char Lbyte = 0;
    Lbyte = file.read();
    if (Lbyte > 75)
    {
        Serial.println("StichLongy");
    }
    Lbyte = file.read();
    if (Lbyte > 75)
    {
        Serial.println("StichLongy");
    }
}
//------------------------------------------------------------------------------
//   ANALYSIS_START();
//------------------------------------------------------------------------------
char ANALYSIS_START()
{
    unsigned int stop = 0, trim = 0, clour = 0;
    size_t Fbyte;
    long int stich = 0;
    uint8_t colour_function = 0;
    char Buff[100];

    Serial.println("\nFile ANALYSIS Start");
    if (!file.open(filename2))
    {
        errorHalt("open failed");
    }
    else
    {
        Serial.println("File Open Success");
        Nextion.print("t1.txt=\"Read!\"");
        Nextion.write(0xff);
        Nextion.write(0xff);
        Nextion.write(0xff);
    }
    file.rewind();
    Serial.println("Available Write Byte:");
    Serial.println(Serial.availableForWrite());
    Serial.println("File Size:");
    Serial.println(file.size());
    while (file.available() > 0)
    {
        //if (Serial.availableForWrite() >= 10)
        {
            Fbyte = file.read();
            //______________________________________________________________________CHAIN
            if (Fbyte == 128 || Fbyte == 160 || Fbyte == 224 || Fbyte == 192)
            {
                stich += 1;
                chaking_stich_length();
            }
            //______________________________________________________________________JUMP
            else if (Fbyte == 129 || Fbyte == 161 || Fbyte == 225 || Fbyte == 193)
            {
                stich += 1;
                chaking_stich_length();
            }
            //______________________________________________________________________MOSS
            else if (Fbyte == 138 || Fbyte == 170 || Fbyte == 202 || Fbyte == 234)
            {
                stich += 1;
                chaking_stich_length();
            }
            //______________________________________________________________________CHAN
            else if (Fbyte == 137 || Fbyte == 169 || Fbyte == 201 || Fbyte == 233)
            {
                stich += 1;
                chaking_stich_length();
            }
            //______________________________________________________________________STOP
            else if (Fbyte == 136 || Fbyte == 168 || Fbyte == 200 || Fbyte == 232)
            {
                stop += 1;
                stich += 1;
                chaking_stich_length();
            }
            //______________________________________________________________________TRIM
            else if (Fbyte == 231 || Fbyte == 230)
            {
                trim += 1;
                stich += 1;
                chaking_stich_length();
            }
            //______________________________________________________________________COLOUR
            else if (Fbyte == 139 || Fbyte == 171 || Fbyte == 203 || Fbyte == 235 ||
                     Fbyte == 140 || Fbyte == 172 || Fbyte == 204 || Fbyte == 236 ||
                     Fbyte == 141 || Fbyte == 173 || Fbyte == 205 || Fbyte == 237 ||
                     Fbyte == 142 || Fbyte == 174 || Fbyte == 206 || Fbyte == 238 ||
                     Fbyte == 143 || Fbyte == 175 || Fbyte == 207 || Fbyte == 239 ||
                     Fbyte == 144 || Fbyte == 176 || Fbyte == 208 || Fbyte == 240 ||
                     Fbyte == 145 || Fbyte == 177 || Fbyte == 209 || Fbyte == 241 ||
                     Fbyte == 146 || Fbyte == 178 || Fbyte == 210 || Fbyte == 242 ||
                     Fbyte == 147 || Fbyte == 179 || Fbyte == 211 || Fbyte == 243 ||
                     Fbyte == 148 || Fbyte == 180 || Fbyte == 212 || Fbyte == 244 ||
                     Fbyte == 149 || Fbyte == 181 || Fbyte == 213 || Fbyte == 245 ||
                     Fbyte == 150 || Fbyte == 182 || Fbyte == 214 || Fbyte == 246 ||
                     Fbyte == 151 || Fbyte == 183 || Fbyte == 215 || Fbyte == 247)
            {
                colour_function = Fbyte & 0xf0;
                if (colour_function == 0x90 || colour_function == 0xB0 || colour_function == 0xD0 ||
                    colour_function == 0xF0)
                {
                    colour_function = Fbyte;
                    sprintf(Buff, "Colour Function Over Loaded:%u", colour_function);
                    Serial.println(Buff);
                }
                clour += 1;
                stich += 1;
                chaking_stich_length();
            }
            //______________________________________________________________________SLOW FAST
            else if (Fbyte == 132 || Fbyte == 164 || Fbyte == 196 || Fbyte == 228 ||
                     Fbyte == 130 || Fbyte == 162 || Fbyte == 194 || Fbyte == 226)
            {
                Serial.println("Slow OR Fast");
                Serial.println(Fbyte, DEC);
                stich += 1;
                chaking_stich_length();
            }
            //______________________________________________________________________END_DESIGNED
            else if (Fbyte == EOD)
            {
                sprintf(Buff, "Stich:%lu", stich);
                Serial.println(Buff);
                sprintf(Buff, "Stop:%u", stop);
                Serial.println(Buff);
                sprintf(Buff, "Trim:%u", trim);
                Serial.println(Buff);
                clour++;
                sprintf(Buff, "Clours:%u", clour);
                Serial.println(Buff);
                file.close();
                Serial.println("\nFile Closed");
                /**/
                Nextion.printf("n3.val=%lu", stop);
                Nextion.print("\xFF\xFF\xFF");
                Nextion.printf("n2.val=%lu", clour);
                Nextion.print("\xFF\xFF\xFF");
                Nextion.printf("n1.val=%lu", trim);
                Nextion.print("\xFF\xFF\xFF");
                Nextion.printf("n0.val=%lu", stich);
                Nextion.print("\xFF\xFF\xFF");
                /**/
                return 'K';
            }
            else
            {
                Serial.println("FUNCTION");
                Serial.println(Buff);
                sprintf(Buff, "STICH#:%lu", stich);
                Serial.println(Buff);
                file.close();
                Serial.println("\nFile Closed");
                return 'B';
            }
        }
    }
    file.close();
    Serial.println("\nFile Closed");
    return 'B';
}
//----------------------------------------------
void SerialBLocking()
{
    while (1)
    {
        if (Serial.available())
        {
            break;
        }
    }
}
#endif