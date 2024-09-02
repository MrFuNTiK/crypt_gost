#pragma once

#include <cstdlib>

namespace crypt_gost {

namespace core {

namespace allocator {

/**
 * @brief Allocator interface.
 * 
 */
class I_Allocator
{
public:
    /**
     * @brief Allocate memory.
     * 
     * @param[in] size Minimal size of memory to allocate (bytes).
     * @param[in] alignment Alignment of allocated memory.
     * 
     * @return void* Allocated memory.
     * @retval !nullptr - In case of success.
     * @retval nullptr - In case of error.
     */
    virtual void* Allocate( size_t size, size_t alignment = 0 ) noexcept = 0;

    /**
     * @brief Deallocate memory.
     * 
     * @param[in] ptr Pointer to allocated memory. 
     */
    virtual void Deallocate( void* ptr ) noexcept = 0;
};

} // namespace core

} // namespace allocator

} // namespace crypt_gost