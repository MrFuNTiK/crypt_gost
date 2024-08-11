#pragma once

#include <stdlib.h>

namespace crypt_gost
{

namespace core
{

namespace allocator
{

class I_Allocator
{
public:
    virtual inline void* Allocate(size_t numBytes ) = 0;
    virtual inline void Deallocate(void* bytes, size_t numBytes = 0 ) = 0;
};

} // namespace allocator

} // namespace core

} // namespace crypt_gost