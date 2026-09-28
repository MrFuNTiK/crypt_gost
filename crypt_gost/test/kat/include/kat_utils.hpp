#pragma once

#include <sstream>
#include <stdexcept>
#include <string_view>
#include <vector>
#include <string>
#include <cstdint>

namespace
{
uint8_t CharToByte( const char ch )
{
    if( ch >= '0' && ch <= '9' )
        return ch - '0';
    if( ch >= 'a' && ch <= 'f' )
        return ch - 'a' + 10;
    if( ch >= 'A' && ch <= 'F' )
        return ch - 'A' + 10;
    throw std::invalid_argument( "Invalid symbol" );
}

} // namespace

std::vector< uint8_t > ParseHex( const std::string& str )
{
    if( str.size() % 2 )
    {
        throw std::runtime_error( "invalid length" );
    }

    std::vector< uint8_t > res( str.size() / 2 );
    for( size_t i = 0; i < str.size() / 2; i++ )
    {
        res[ i ] = ( ::CharToByte(str[ i ]) << 4 ) | ::CharToByte(str[ i + 1 ]);
    }
    return res;
}
