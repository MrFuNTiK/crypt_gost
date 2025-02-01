#include <stdexcept>
#include <core/allocator/stack_allocator.hpp>

using namespace crypt_gost::core::allocator;

StackAllocator::StackAllocator()
{
    manager = HeapManager_Initialize( stack, sizeof( stack ), nullptr );
    if( !manager )
    {
        throw std::runtime_error( "Failed to create heap manager" );
    }
}

StackAllocator::~StackAllocator()
{
    HeapManager_Finalize( manager );
}

StackAllocator& StackAllocator::GetInstance()
{
    static StackAllocator allocator;
    return allocator;
}

void* StackAllocator::Allocate( size_t size, size_t alignment ) noexcept
{
    std::lock_guard guard( mt );
    return HeapManager_Allocate( manager, size, alignment );
}

void StackAllocator::Deallocate( void* ptr ) noexcept
{
    std::lock_guard guard( mt );
    HeapManager_Deallocate( manager, ptr );
}
