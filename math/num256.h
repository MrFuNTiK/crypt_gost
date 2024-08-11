#pragma once

#include <stdint.h>

#define NUMBER_256_SIZE_BYTES   256/8
#define NUMBER_256_SIZE_UI64    256/64

typedef struct 
{
    union 
    {
        uint8_t ui8[ NUMBER_256_SIZE_BYTES ];
        uint64_t ui64[ NUMBER_256_SIZE_UI64 ];
    } d;
    
} CrptMath_256Number;
