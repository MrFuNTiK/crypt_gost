#pragma once

#include <stdlib.h>

#define INTERP_AS( type, var )                  ( *( type* )( &var ) )
#define INTERP_PTR_AS( type, ptr )              ( *( type* )( ptr ) )
#define SHIFT_PTR_RIGHT( ptr, num )             ( ( unsigned char* )( ptr ) + ( num ) )
#define SHIFT_PTR_LEFT( ptr, num )              ( ( unsigned char* )( ptr ) - ( num ) )
#define SHIFT_PTR_UPTO_ALIGNMENT( ptr, algn )   ( ( size_t )( ptr ) + ( algn ) - ( size_t )( ptr ) % ( algn ) )

static inline size_t PTR_DIFF( void* left, void* right )
{
    return (unsigned char*)right - (unsigned char*)left;
}
