#include <type_traits>
#include <cstdlib>

namespace crypt_gost
{

namespace core
{

namespace util
{

namespace traits
{

template < typename T, std::enable_if_t< std::is_integral< T >::value, bool > = true >
constexpr size_t BitsNumberOf()
{
    return sizeof( T ) * 8;
}

template < typename T, std::enable_if_t< std::is_integral< T >::value, bool > = true >
constexpr size_t BitsNumberOf( T value )
{
    return sizeof( value ) * 8;
}

bool IsLittleEndian()
{
    int32_t a = 1;
    return *( int8_t* )( &a ) == 1;
}

template < typename T, std::enable_if_t< std::is_integral< T >::value, bool > = true >
T ChangeEndiannes( T number )
{
    T ret = 0;
    for( size_t i = 0; i < sizeof( T ); ++i )
    {
        size_t tmp = ( ( uint8_t* )( &number ) )[ i ];
        ret |= tmp << 8 * ( sizeof( T ) - i - 1 );
    }
    return ret;
}

} // namespace traits

} // namespace util

} // namespace core

} // namespace crypt_gost
