#pragma once

#include <mutex>
#include <allocator/i_allocator.hpp>
#include <allocator/heap_manager.h>

namespace crypt_gost
{

namespace allocator
{

constexpr size_t STACK_SIZE = 32 * 1024 * 1024; //32kb

class StackAllocator : public crypt_gost::allocator::I_Allocator
{
public:
    ~StackAllocator();
    StackAllocator(const StackAllocator&) = delete;
    StackAllocator operator =(const StackAllocator&) = delete;

    static StackAllocator& GetAllocator();

    void* Allocate( size_t size, size_t alignment = 0 ) noexcept override;
    void Deallocate( void* ptr ) noexcept override;

private:
    StackAllocator();
    unsigned char stack[ STACK_SIZE ] = { 0 };
    HeapManager* manager = nullptr;
    std::mutex mt;
};

} // namespace allocator

} // namespace crypt_gost