#pragma once

#include <cassert>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cstring>
#include <core/allocator/heap_allocator.hpp>
#include <core/math/mem_buf.hpp>

#include <core/util/traits.hpp>

namespace crypt_gost
{

namespace core
{

namespace math
{

using namespace crypt_gost::core::allocator;
using namespace crypt_gost::core::util;
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
    explicit LongNumber( const uint8_t* bytes,
                         I_Allocator& alloc = HeapAllocator::GetInstance() )
    : bytes_()
    , buf_(bitSize/8, 4, alloc)
    {
        assert( bytes );
        bytes_.u8 = static_cast<uint8_t*>(buf_.GetBuf());

        if( bytes )
        {
            std::memcpy( bytes_.u8, bytes, bitSize / 8 );
        }
    };

    LongNumber( uint32_t value = 0,
                I_Allocator& alloc = HeapAllocator::GetInstance() )
    : bytes_()
    , buf_(bitSize/8, 4, alloc)
    {
        bytes_.u8 = static_cast<uint8_t*>(buf_.GetBuf());
        memset( buf_.GetBuf(), 0, BitSize() / 8 );
        bytes_.u32[ 0 ] = value;
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

    bool operator==( const LongNumber& other ) const
    {
        return 0 == std::memcmp( bytes_.u8, other.bytes_.u8, BitSize() / 8 );
    }

    LongNumber& operator+=( const LongNumber& other ) noexcept
    {
        bool carry = false;
        const bool isLittleEndian = traits::IsLittleEndian();
        for( size_t i = bitSize / traits::BitsNumberOf<uint32_t>() - 1; i != std::numeric_limits<size_t>::max(); --i )
        {
            uint32_t a = isLittleEndian ? traits::ChangeEndiannes( bytes_.u32[i] ) : bytes_.u32[i];
            uint32_t b = isLittleEndian ? traits::ChangeEndiannes( other.bytes_.u32[i] ) : other.bytes_.u32[i];
            uint32_t res = a + b + carry;
            carry = res < a;
            bytes_.u32[i] = isLittleEndian ? traits::ChangeEndiannes( res ) : res;
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

    LongNumber operator<<=( size_t shift )
    {
        if( shift > BitSize() )
        {
            memset(bytes_.u8, 0, BitSize()/8);
            return *this;
        }

        size_t wordsShift      = shift / traits::BitsNumberOf<uint32_t>();
        size_t perWordBitShift = shift % traits::BitsNumberOf<uint32_t>();
        const size_t TOTAL_WORDS_NUM = BitSize() / traits::BitsNumberOf<uint32_t>();

        // memcpy for overlapping buffers is undefined behabiour, so copy in cycle.
        size_t readPos = wordsShift;
        size_t writePos = 0;

        for( size_t i = 0; i < wordsShift; ++i)
        {
            assert( readPos < TOTAL_WORDS_NUM );
            assert( writePos < TOTAL_WORDS_NUM );
            bytes_.u32[ writePos ] = bytes_.u32[ readPos ];
            ++readPos;
            ++writePos;
        }

        size_t appendToRight = 0;
        const bool isLittleEndian = traits::IsLittleEndian();
        for( size_t i = wordsShift; i < TOTAL_WORDS_NUM; ++i )
        {
            size_t wordIdx = TOTAL_WORDS_NUM - i - 1;
            assert( wordIdx < TOTAL_WORDS_NUM );
            uint32_t word = isLittleEndian ? traits::ChangeEndiannes(bytes_.u32[ wordIdx ])
                                           : bytes_.u32[ wordIdx ];
            uint32_t buf = word >> ( traits::BitsNumberOf<uint32_t>() - perWordBitShift );
            uint32_t res = word << perWordBitShift;
            res |= appendToRight;
            bytes_.u32[wordIdx] = isLittleEndian ? traits::ChangeEndiannes(res)
                                                 : res;
            appendToRight = buf;
        }
        return *this;
    }

    LongNumber operator*=( const LongNumber& other )
    {
        LongNumber ret;
        LongNumber tmp(other);

        for( uint32_t i = 0; i < BitSize(); ++i )
        {
            if( CheckBit(i) )
            {
                tmp <<= 1;
                ret += tmp;
            }
        }
        return ret;
    }

    friend std::ostream& operator<<( std::ostream& os, const LongNumber& number )
    {
        for( size_t i = 0; i < number.BitSize() / 8 - 1; ++i )
        {
            os << std::setfill('0') << std::setw( 2 )
               << std::hex << static_cast<int>(number.bytes_.u8[i])
               << ":";
        }
        os << std::setfill('0') << std::setw( 2 )
           << std::hex << static_cast<int>(number.bytes_.u8[number.BitSize()/8-1])
           << std::flush;
        return os;
    }

private:
    constexpr inline size_t BitSize() const noexcept
    {
        return bitSize;
    }

    inline bool CheckBit( size_t bit ) const
    {
        if( bit > BitSize() )
        {
            std::stringstream ss;
            ss << "checked bit: " << bit << ", number bit size: " << BitSize();
            throw std::out_of_range( ss.str().c_str() );
        }

        return ( 1 << ( bit % 8 ) ) & bytes_.u8[ bit / 8 ];
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
