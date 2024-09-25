#pragma once

#include <vector>
#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cstring>
#include <core/allocator/heap_allocator.hpp>
#include <core/math/mem_buf.hpp>

namespace crypt_gost
{

namespace core
{

namespace math
{

using namespace crypt_gost::core::allocator;
using namespace crypt_gost::core;

namespace sfinae
{

constexpr bool IsPowerOfTwo( const size_t value )
{
    return ( value & ( value - 1 ) ) == 0;
}

template <size_t number>
struct is_power_of_two
{
    static const bool value = IsPowerOfTwo( number );
};

} // namespace sfinae

template<size_t bitSize,
         std::enable_if_t< sfinae::is_power_of_two< bitSize >::value,
                           bool> = true >
class LongNumber final
{
public:
    LongNumber( uint8_t* bytes = nullptr,
                I_Allocator& alloc = HeapAllocator::GetInstance() )
    : bytes_()
    , buf_(bitSize/8, 4, alloc)
    {
        bytes_.u8 = static_cast<uint8_t*>(buf_.GetBuf());

        if( bytes )
        {
            std::memcpy( bytes_.u8, bytes, bitSize / 8 );
        }
    };

    ~LongNumber() = default;

    LongNumber( const LongNumber& other )
        : bytes_()
        , buf_(other.buf_)
    {
        bytes_.u8 = static_cast<uint8_t*>(buf_.GetBuf());
    }

    LongNumber& operator=( const LongNumber& other )
    {
        buf_ = other.buf_;
        bytes_.u8 = static_cast<uint8_t*>(buf_.GetBuf());
        return *this;
    }

    LongNumber( LongNumber&& other )
        : buf_(std::move(other.buf_))
    {
        bytes_.u8 = static_cast<uint8_t*>(buf_.GetBuf());
    }

    LongNumber& operator=( LongNumber&& other )
    {
        buf_ = std::move(other.buf_);
        bytes_.u8 = static_cast<uint8_t*>(buf_.GetBuf());
        return *this;
    }

    LongNumber& operator+=( const LongNumber& other ) noexcept
    {
        bool carry = false;
        for( size_t i = 0; i < bitSize / 8 / 4; ++i )
        {
            bytes_.u32[i] += other.bytes_.u32[i] + carry;
            carry = bytes_.u32[i] < other.bytes_.u32[i];
        }
        return *this;
    }

    LongNumber operator+( const LongNumber& other ) const
    {
        LongNumber ret(*this);
        ret += other;
        return ret;
    }

    LongNumber& operator^=( const LongNumber& other ) noexcept
    {
        for( size_t i = 0; i < bitSize / 8 / 4; ++i )
        {
            bytes_.u32[i] ^= other.bytes_.u32[i];
        }
        return *this;
    }

    LongNumber operator^( const LongNumber& other ) const
    {
        LongNumber ret(*this);
        ret ^= other;
        return ret;
    }

    friend std::ostream& operator<<( std::ostream& os, const LongNumber& number )
    {
        for( size_t i = 0; i < number.BitSize() / 8 - 1; ++i )
        {
            os << "0x" << std::setfill('0') << std::setw( 2 )
               << std::hex << static_cast<int>(number.bytes_.u8[i])
               << ", ";
        }
        os << "0x" << std::setfill('0') << std::setw( 2 )
           << std::hex << static_cast<int>(number.bytes_.u8[number.BitSize()/8-1])
           << std::flush;
        return os;
    }

private:
    inline size_t BitSize() const noexcept
    {
        return bitSize;
    }

private:
    union Bytes
    {
        Bytes() : u8( nullptr ){};
        uint8_t* u8;
        uint32_t* u32;
    };

    Bytes bytes_;
    util::MemBuf buf_;
};


} // namespace math

} // namespace core

} // namespace crypt_gost
