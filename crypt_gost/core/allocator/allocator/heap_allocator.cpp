#include <stdlib.h>
#include <core/allocator/heap_allocator.hpp>

using namespace crypt_gost::core::allocator;

HeapAllocator& HeapAllocator::GetInstance()
{
    static HeapAllocator allocator;
    return allocator;
}

void* HeapAllocator::Allocate( size_t size, size_t alignment ) noexcept
{
    return alignment ? aligned_alloc( alignment, size ) : malloc( size );
}

void HeapAllocator::Deallocate( void* ptr ) noexcept
{
    free( ptr );
}
