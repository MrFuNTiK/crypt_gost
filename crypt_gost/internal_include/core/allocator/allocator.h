#pragma once

#include <stdlib.h>

typedef void* ( *AllocateMemory_fn )( size_t size, size_t alignment );
typedef void ( *DeallocateMemory_fn )( void* ptr );

struct core_allocator_st
{
    AllocateMemory_fn allocate;
    DeallocateMemory_fn deallocate;
};

typedef struct core_allocator_st CoreAllocator;

extern const CoreAllocator CoreHeapAlloc;
extern const CoreAllocator CoreStackAlloc;
