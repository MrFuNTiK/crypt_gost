#pragma once

namespace crypt_gost
{

namespace core
{

namespace math
{

#include <cstdint>
#include <array>
#include <internal_include/core/math/math.hpp>
#include <internal_include/core/allocator/i_allocator.hpp>
#include <internal_include/core/allocator/default_allocator.hpp

template<size_t bitSize>
class LongNumber
{
public:
    LongNumber() : bitSize_( bitSize ), bytesSize_( bitSize / 8 )
    {
        static_assert( bitSize % 8 == 0 )

    }
    LongNumber( uint8_t* bytes = nullptr, size_t bitSize = 0, crypt_gost::core::allocator::IAllocator alloc = crypt_gost::core::allocator::DefaultAllocator() )
    {
        static_assert( bitSize % 8 == 0 );
    }

private:
    uint8_t* bytes;
    size_t bitSize_;
    size_t bytesSize_;
    crypt_gost::core::allocator::IAllocator allocator_;
};


} // namespace math

} // namespace core

} // namespace crypt_gost
