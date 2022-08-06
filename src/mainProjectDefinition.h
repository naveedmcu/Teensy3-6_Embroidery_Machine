#ifndef MAIN_PROJECT_DEFINITION_H
#define MAIN_PROJECT_DEFINITION_H

#define Yes 1
#define No 0
#define OFF 1
#define ON 0
#define Open 0
#define Close 1
enum
{
    jumpHoldundefine = 1,
    jumphold,
    jumprelease
};
#define HomePosition 0
#define Non_HomePosition 1

#define Nextion Serial4
#define NextionBaudrate 9600

typedef signed char sint8_t;
typedef signed short sint16_t;
typedef signed long sint32_t;
typedef signed long long sint64_t;
typedef unsigned long long u_int64_t;
typedef float real32_t;
typedef double real64_t;

typedef unsigned char uint8;
typedef unsigned char u8;
typedef signed char sint8;
typedef bool sint1;
typedef signed char int8;
typedef signed char s8;
typedef unsigned short uint16;
typedef unsigned short u16;
typedef signed short sint16;
typedef signed short s16;
typedef unsigned int uint32;
typedef unsigned int u_int;
typedef unsigned int u32;
typedef signed int sint32;
typedef signed int s32;
typedef int int32;
typedef signed long long sint64;
typedef unsigned long long uint64;
typedef unsigned long long u64;
typedef float real32;
typedef double real64;

#endif

//  * =============================================================================  */
//  * %d = signed integer               %f = floating point number
//  * %s = string                     %.1f = float to 1 decimal place
//  * %c = character                  %.3f = float to 3 decimal places
//  * %e = scientific notation          %g = shortest representation of %e or %f
//  * %u = unsigned integer             %o = unsigned octal
//  * %x = unsigned hex (lowercase)     %X = unsigned hex (uppercase)
//  * %hd = short int                  %ld = long int
//  * %lld = long long int
//  * =============================================================================  */