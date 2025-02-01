#pragma once

#include <mutex>
#include <core/allocator/i_allocator.hpp>
#include <core/allocator/heap_manager/heap_manager.h>

namespace crypt_gost
{

namespace core
{

namespace allocator
{

constexpr size_t STACK_SIZE = 32 * 1024 * 1024; // 32kb

class StackAllocator : public crypt_gost::core::allocator::I_Allocator
{
public:
    ~StackAllocator();
    StackAllocator( const StackAllocator& ) = delete;
    StackAllocator operator=( const StackAllocator& ) = delete;

    /**
     * @brief Get thread-safe instance of allocator.
     *
     * @return Instance of allocator.
     */
    static StackAllocator& GetInstance();

    void* Allocate( size_t size, size_t alignment = 0 ) noexcept override;
    void Deallocate( void* ptr ) noexcept override;

private:
    StackAllocator();
    unsigned char stack[ STACK_SIZE ] = { 0 };
    HeapManager* manager = nullptr;
    std::mutex mt;
};

} // namespace allocator

} // namespace core

} // namespace crypt_gost