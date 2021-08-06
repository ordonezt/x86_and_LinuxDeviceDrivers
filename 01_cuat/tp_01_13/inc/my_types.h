#ifndef __MY_TYPES_H
#define __MY_TYPES_H

typedef signed char             int8_t;
typedef signed short int        int16_t;
typedef signed int              int32_t;
typedef signed long long int    int64_t;

typedef unsigned char           uint8_t;
typedef unsigned short int      uint16_t;
typedef unsigned int            uint32_t;
typedef unsigned long long int  uint64_t;

typedef enum{
    false,
    true
}bool;

#define NULL    0
#define MAGIC_BREAKPOINT    asm("xchg %bx, %bx");
#endif /*__MY_TYPES_H*/
