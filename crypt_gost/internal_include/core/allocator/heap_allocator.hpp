#pragma once

#include <mutex>
#include <core/allocator/i_allocator.hpp>

namespace crypt_gost
{

namespace core
{

namespace allocator
{

/**
 * @brief Heap allocator.
 *
 * Wrapper around malloc(), aligned_malloc(), free.
 *
 */
class HeapAllocator : public crypt_gost::core::allocator::I_Allocator
{
public:
    ~HeapAllocator() = default;
    HeapAllocator( const HeapAllocator& ) = delete;
    HeapAllocator operator=( const HeapAllocator& ) = delete;

    static HeapAllocator& GetInstance();

    void* Allocate( size_t size, size_t alignment = 0 ) noexcept override;
    void Deallocate( void* ptr ) noexcept override;

private:
    HeapAllocator() = default;
};

} // namespace allocator

} // namespace core

} // namespace crypt_gost
