#pragma once

#include <stdlib.h>
#include <stdalign.h>

// clang-format off

static inline size_t DIFF_UPTO_ALIGNMENT( void* ptr, size_t algn )
{
    if( algn )
    {
        if( ( size_t )ptr % algn )
        {
            return algn - ( (size_t)ptr % algn );
        }
    }
    return 0;
}

static inline size_t PTR_DIFF( void* left, void* right )
{
    return (unsigned char*)right - (unsigned char*)left;
}

#define INTERP_AS( type, var )                  ( *( type* )( &var ) )
#define INTERP_PTR_AS( type, ptr )              ( *( type* )( ptr ) )
#define SHIFT_PTR_RIGHT( ptr, num )             ( ( unsigned char* )( ptr ) + ( num ) )
#define SHIFT_PTR_LEFT( ptr, num )              ( ( unsigned char* )( ptr ) - ( num ) )
#define SHIFT_PTR_UPTO_ALIGNMENT( ptr, algn )   ( ( void* )( size_t )( ptr ) + DIFF_UPTO_ALIGNMENT( ptr, algn ) )

// clang-format on
