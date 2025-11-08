#pragma once

#include <type_traits>

namespace crypt_gost
{

namespace core
{

namespace util
{

namespace traits
{
namespace byte_order
{

enum class Endian
{
    LITTLE,
    BIG
};

static inline bool IsLittleEndian() noexcept
{
    int32_t a = 1;
    return *( int8_t* )( &a ) == 1;
}

static inline Endian HostByteOrder() noexcept
{
#ifdef CRYPT_GOST_HAS_BYTE_ORDERING
#    ifdef CRYPT_GOST_LITTLE_ENDIAN
    return Endian::LITTLE;
#    elif defined( CRYPT_GOST_BIG_ENDIAN )
    return Endian::BIG;
#    endif
#else
    static const bool IS_LITTLE_ENDIAN = byte_order::IsLittleEndian();
    return IS_LITTLE_ENDIAN ? Endian::LITTLE : Endian::BIG;
#endif
}

template < typename T, std::enable_if_t< std::is_integral< T >::value, bool > = true >
T ChangeByteOrdering( T number ) noexcept
{
    T ret = 0;
    for( size_t i = 0; i < sizeof( T ); ++i )
    {
        size_t tmp = ( ( uint8_t* )( &number ) )[ i ];
        ret |= tmp << 8 * ( sizeof( T ) - i - 1 );
    }
    return ret;
}


static inline void ChangeByteOrdering( uint8_t* bytes, size_t size ) noexcept
{
    assert( size % 2 == 0);
    for( size_t i = 0; i < size / 2; ++i )
    {
        std::swap( bytes[ i ], bytes[ size - i - 1 ] );
    }
}

} // namespace byte_order

namespace bit_length
{

template < typename T, std::enable_if_t< std::is_integral_v< T >, bool > = true >
constexpr size_t BitsNumberOf() noexcept
{
    return sizeof( T ) * 8;
}

template < typename T, std::enable_if_t< std::is_integral_v< T >, bool > = true >
constexpr size_t BitsNumberOf( T value ) noexcept
{
    return sizeof( value ) * 8;
}

} // namespace bit_length

} // namespace traits

} // namespace util

} // namespace core

} // namespace crypt_gost
