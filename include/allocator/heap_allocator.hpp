#pragma once

#include <mutex>
#include <allocator/i_allocator.hpp>
#include <allocator/heap_manager.h>

namespace crypt_gost
{

namespace allocator
{

/**
 * @brief Heap allocator.
 * 
 * Wrapper around malloc(), aligned_malloc(), free.
 * 
 */
class HeapAllocator : public crypt_gost::allocator::I_Allocator
{
public:
    ~HeapAllocator() = default;
    HeapAllocator(const HeapAllocator&) = delete;
    HeapAllocator operator =(const HeapAllocator&) = delete;

    static HeapAllocator& GetAllocator();

    void* Allocate( size_t size, size_t alignment = 0 ) noexcept override;
    void Deallocate( void* ptr ) noexcept override;

private:
    HeapAllocator() = default;
};

} // namespace allocator

} // namespace crypt_gost