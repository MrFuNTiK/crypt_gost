#include <core/allocator/allocator.h>
#include <core/allocator/heap_allocator.hpp>
#include <core/allocator/stack_allocator.hpp>

using namespace crypt_gost::core::allocator;

void* HeapAllocate( size_t size, size_t alignment )
{
    return HeapAllocator::GetInstance().Allocate( size, alignment );
}

void HeapDeallocate( void* ptr )
{
    HeapAllocator::GetInstance().Deallocate( ptr );
}

void* StackAllocate( size_t size, size_t alignment )
{
    return StackAllocator::GetInstance().Allocate( size, alignment );
}

void StackDeallocate( void* ptr )
{
    StackAllocator::GetInstance().Deallocate( ptr );
}
