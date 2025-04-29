#pragma once

#include <cassert>
#include <vector>
#include <iostream>
#include <functional>
#include <iomanip>
#include <cstdint>
#include <cstring>
#include <core/allocator/heap_allocator.hpp>
#include <core/util/mem_buf.hpp>

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

#ifdef CRYPT_GOST_HAS_BYTE_ORDERING
#    ifdef CRYPT_GOST_LITTLE_ENDIAN
#        define BYTE_SWAP( x ) traits::ChangeByteOrdering( ( x ) )
#    elif defined( CRYPT_GOST_BIG_ENDIAN )
#        define BYTE_SWAP( x ) ( x )
#    endif
#else
static const bool IS_LITTLE_ENDIAN = traits::IsLittleEndian();
#    define BYTE_SWAP( x ) ( IS_LITTLE_ENDIAN ? traits::ChangeByteOrdering( ( x ) ) : ( x ) )
#endif

namespace sfinae
{

constexpr bool IsPowerOfTwo( const size_t value )
{
    return ( value & ( value - 1 ) ) == 0;
}

template < size_t number >
struct is_power_of_two
{
    static const bool value = IsPowerOfTwo( number );
};

} // namespace sfinae

template < size_t bitSize,
           typename T = uint64_t,
           std::enable_if_t< sfinae::is_power_of_two< bitSize >::value, bool > = true,
           std::enable_if_t< std::is_integral< T >::value, bool > = true,
           std::enable_if_t< std::is_unsigned< T >::value, bool > = true >
class LongNumber final
{
public:
    explicit LongNumber( const std::initializer_list< uint8_t > bytes,
                         I_Allocator& alloc = HeapAllocator::GetInstance() )
        : bytes_()
        , buf_( bitSize / 8, 8, alloc )
        , isZero_( true )
    {
        bytes_.byte = static_cast< uint8_t* >( buf_.GetBuf() );

        if( bytes.size() != bitSize / 8 )
        {
            throw std::runtime_error( "Invalid byte sequence size" );
        }

        std::memcpy( bytes_.byte, bytes.begin(), bitSize / 8 );
        if( !CheckIsZero() )
        {
            ByteSwap();
        }
    };

    LongNumber( T value = 0, I_Allocator& alloc = HeapAllocator::GetInstance() )
        : bytes_()
        , buf_( bitSize / 8, 4, alloc )
        , isZero_( value == 0 )
    {
        bytes_.byte = static_cast< uint8_t* >( buf_.GetBuf() );
        memset( buf_.GetBuf(), 0, bitSize / 8 );
        bytes_.word[ traits_.COUNT_OF_WORDS - 1 ] = value;
    };

    ~LongNumber() = default;

    LongNumber( const LongNumber& other )
        : bytes_()
        , buf_( other.buf_ )
        , isZero_( other.isZero_ )
    {
        bytes_.byte = static_cast< uint8_t* >( buf_.GetBuf() );
    }

    LongNumber& operator=( const LongNumber& other )
    {
        buf_ = other.buf_;
        bytes_.byte = static_cast< uint8_t* >( buf_.GetBuf() );
        isZero_ = other.isZero_;
        return *this;
    }

    LongNumber( LongNumber&& other )
        : buf_( std::move( other.buf_ ) )
        , isZero_( other.isZero_ )
    {
        bytes_.byte = static_cast< uint8_t* >( buf_.GetBuf() );
    }

    LongNumber& operator=( LongNumber&& other )
    {
        buf_ = std::move( other.buf_ );
        bytes_.byte = static_cast< uint8_t* >( buf_.GetBuf() );
        isZero_ = other.isZero_;
        return *this;
    }

    bool operator==( const LongNumber& other ) const
    {
        return 0 == std::memcmp( bytes_.byte, other.bytes_.byte, bitSize / 8 );
    }

    LongNumber& operator+=( const LongNumber& other ) noexcept
    {
        if( other.isZero_ )
        {
            return *this;
        }

        bool carry = false;
        for( size_t i = traits_.COUNT_OF_WORDS - 1; i != std::numeric_limits< size_t >::max(); --i )
        {
            T a = bytes_.word[ i ];
            T b = other.bytes_.word[ i ];
            T res = a + b;
            bool carryNext = res < a;
            res += carry;
            if( res == 0 && carry )
            {
                carryNext = true;
            }
            bytes_.word[ i ] = res;
            carry = carryNext;
        }
        CheckIsZero();
        return *this;
    }

    LongNumber operator+( const LongNumber& other ) const
    {
        LongNumber ret( *this );
        ret += other;
        return ret;
    }

    LongNumber& operator^=( const LongNumber& other ) noexcept
    {
        for( size_t i = 0; i < traits_.COUNT_OF_WORDS; ++i )
        {
            bytes_.word[ i ] ^= other.bytes_.word[ i ];
        }
        return *this;
    }

    LongNumber operator^( const LongNumber& other ) const
    {
        LongNumber ret( *this );
        ret ^= other;
        return ret;
    }

    LongNumber operator<<=( size_t shift )
    {
        if( isZero_ )
        {
            return *this;
        }

        if( shift == 0 )
        {
            return *this;
        }

        if( shift > bitSize )
        {
            memset( bytes_.byte, 0, bitSize / 8 );
            return *this;
        }

        size_t wordsShift = shift / traits::BitsNumberOf( bytes_.word[ 0 ] );
        size_t perWordBitShift = shift % traits::BitsNumberOf( bytes_.word[ 0 ] );

        // memcpy for overlapping buffers is undefined behabiour, so copy in
        // cycle.
        size_t readPos = wordsShift;
        size_t writePos = 0;

        for( size_t i = 0; i < traits_.COUNT_OF_WORDS - wordsShift; ++i )
        {
            assert( readPos < traits_.COUNT_OF_WORDS );
            assert( writePos < traits_.COUNT_OF_WORDS );
            bytes_.word[ writePos ] = bytes_.word[ readPos ];
            ++readPos;
            ++writePos;
        }
        for( size_t i = traits_.COUNT_OF_WORDS - wordsShift; i < traits_.COUNT_OF_WORDS; ++i )
        {
            bytes_.word[ i ] = 0;
        }

        size_t appendToRight = 0;
        for( size_t i = wordsShift; i < traits_.COUNT_OF_WORDS; ++i )
        {
            size_t wordIdx = traits_.COUNT_OF_WORDS - i - 1;
            assert( wordIdx < traits_.COUNT_OF_WORDS );
            T word = bytes_.word[ wordIdx ];
            T buf = word >> ( traits_.WORD_BIT_SIZE - perWordBitShift );
            T res = word << perWordBitShift;
            res |= appendToRight;
            bytes_.word[ wordIdx ] = res;
            appendToRight = buf;
        }
        CheckIsZero();
        return *this;
    }

    LongNumber operator*=( const LongNumber& other )
    {
        if( isZero_ || other.isZero_ )
        {
            memset( buf_.GetBuf(), 0, traits_.COUNT_OF_BYTES );
            return *this;
        }

        size_t lastCheckedBit = 0;
        LongNumber tmp( *this );
        memset( buf_.GetBuf(), 0, traits_.COUNT_OF_BYTES );
        for( size_t i = 0; i < traits_.NUMBER_BIT_SIZE; ++i )
        {
            if( other.CheckBit( i ) )
            {
                size_t shift = i - lastCheckedBit;
                tmp <<= ( shift );
                *this += tmp;
                lastCheckedBit = i;
            }
        }
        return *this;
    }

    LongNumber operator*( const LongNumber& other ) const
    {
        LongNumber ret;
        size_t firstBitsCount = 0;
        size_t secondBitsCount = 0;
        std::reference_wrapper< const LongNumber > left = *this;
        std::reference_wrapper< const LongNumber > right = other;

        if( isZero_ || other.isZero_ )
        {
            return ret;
        }

        for( size_t i = 0; i < traits_.NUMBER_BIT_SIZE; ++i )
        {
            firstBitsCount += CheckBit( i );
            secondBitsCount += other.CheckBit( i );
        }
        if( firstBitsCount > secondBitsCount )
        {
            std::swap( left, right );
        }

        ret = left;
        ret *= right;
        return ret;
    }

    friend std::ostream& operator<<( std::ostream& os, const LongNumber& number )
    {
        number.ByteSwap();
        for( size_t i = 0; i < number.traits_.COUNT_OF_BYTES - 1; ++i )
        {
            os << std::setfill( '0' ) << std::setw( 2 ) << std::hex
               << static_cast< int >( number.bytes_.byte[ i ] ) << ":";
        }
        os << std::setfill( '0' ) << std::setw( 2 ) << std::hex
           << static_cast< int >( number.bytes_.byte[ number.BitSize() / 8 - 1 ] ) << std::flush;
        number.ByteSwap();
        return os;
    }

private:
    constexpr inline size_t BitSize() const noexcept
    {
        return bitSize;
    }

    inline bool CheckBit( size_t bit ) const
    {
        if( bit > bitSize )
        {
            std::stringstream ss;
            ss << "checked bit: " << bit << ", number bit size: " << bitSize;
            throw std::out_of_range( ss.str().c_str() );
        }
        size_t wordIdx = traits_.COUNT_OF_WORDS - 1 - bit / traits_.WORD_BIT_SIZE;
        size_t bitIdx = bit % traits_.WORD_BIT_SIZE;
        T word = bytes_.word[ wordIdx ];
        return ( ( T )1 << ( bitIdx ) ) & word;
    }

    bool CheckIsZero()
    {
        for( size_t i = 0; i < traits_.COUNT_OF_WORDS; ++i )
        {
            if( bytes_.word[ i ] != 0 )
            {
                isZero_ = false;
                return isZero_;
            }
        }
        isZero_ = true;
        return isZero_;
    }

    void ByteSwap() const
    {
        for( size_t i = 0; i < traits_.COUNT_OF_WORDS; ++i )
        {
            bytes_.word[ i ] = BYTE_SWAP( bytes_.word[ i ] );
        }
    }

private:
    union Bytes
    {
        Bytes()
            : byte( nullptr ){};
        uint8_t* byte;
        T* word;
    };

    struct Traits
    {
        size_t NUMBER_BIT_SIZE;
        size_t COUNT_OF_BYTES;
        size_t COUNT_OF_WORDS;
        size_t WORD_BIT_SIZE;
    };

    static constexpr Traits traits_{ bitSize,
                                     bitSize / 8,
                                     bitSize / traits::BitsNumberOf< T >(),
                                     traits::BitsNumberOf< T >() };

    Bytes bytes_;
    util::MemBuf buf_;
    bool isZero_;
};

} // namespace math

} // namespace core

} // namespace crypt_gost
