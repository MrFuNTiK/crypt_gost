#pragma once

#include <cstdlib>
#include "i_allocator.hpp"

namespace crypt_gost
{

namespace core
{

namespace allocator
{

struct DefaultAllocator : I_Allocator
{
    inline void* Allocate(size_t numBytes ) override
    {
        return std::malloc(numBytes);
    };

    inline void Deallocate(void* bytes, size_t numBytes ) override
    {
        std::free(bytes);
    };
};

} // namespace allocator

} // namespace core

} // namespace crypt_gost