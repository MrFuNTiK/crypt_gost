#pragma once

#include <cstdint>
#include <cstring>
#include <core/allocator/heap_allocator.hpp>

using namespace crypt_gost::core::allocator;
using namespace crypt_gost::core;

namespace crypt_gost
{

namespace core
{

namespace math
{

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
    , bitSize_( bitSize )
    , bytesSize_( bitSize / 8 )
    , allocator_( alloc )
    {
        bytes_.u8 = (uint8_t*)allocator_.Allocate( bytesSize_ );
        if( !bytes_.u8 )
        {
            throw std::runtime_error( "Alloc failure" );
        }

        if( bytes )
        {
            std::memcpy( bytes_.u8, bytes, bytesSize_ );
        }
    };

    ~LongNumber()
    {
        allocator_.Deallocate( bytes_.u8 );
    };

    LongNumber( const LongNumber& other )
        : bitSize_( other.bitSize_ )
        , bytesSize_( other.bytesSize_ )
        , allocator_( other.allocator_ )
    {
        bytes_.u8 = (uint8_t*)allocator_.Allocate( bytesSize_ );
        if( !bytes_.u8 )
        {
            throw std::runtime_error( "Alloc failure" );
        }
        std::memcpy( bytes_.u8, other.bytes_.u8, bytesSize_ );
    }

    LongNumber& operator=( const LongNumber& other )
    {
        static_assert( bytesSize_ == other.bytesSize_ );

        allocator_.Deallocate( bytes_.u8 );
        allocator_ = other.allocator_;
        bytes_.u8 = allocator_.Allocate( bytesSize_ );
        if( !bytes_.u8 )
        {
            throw std::runtime_error( "Alloca failure" );
        }
        std::memcpy( bytes_.u8, other.bytes_.u8, bytesSize_ );
        return *this;
    }

    LongNumber( LongNumber&& other )
        : bytes_( other.bytes_ )
        , bitSize_( other.bitSize_ )
        , bytesSize_( other.bytesSize_ )
        , allocator_( other.allocator_ )
    {
        other.bytes_.u8 = nullptr;
    }

    LongNumber& operator+=( const LongNumber& other )
    {
        bool carry = false;
        for( size_t i = 0; i < bytesSize_ / 4; ++i )
        {
            bytes_.u32[i] += other.bytes_.u32[i] + carry;
            carry = bytes_.u32[i] < other.bytes_.u32[i];
        }
        return *this;
    }

    LongNumber operator+( const LongNumber& other )
    {
        LongNumber ret(*this);
        ret += other;
        return ret;
    }

private:
    union Bytes
    {
        Bytes() : u8( nullptr ){};
        uint8_t* u8;
        uint32_t* u32;
    };

    Bytes bytes_;
    const size_t bitSize_;
    const size_t bytesSize_;
    crypt_gost::core::allocator::I_Allocator& allocator_;
};


} // namespace math

} // namespace core

} // namespace crypt_gost
