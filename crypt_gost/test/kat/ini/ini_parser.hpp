#pragma once

#include "ini_config.hpp"
#include "ini_section.hpp"
#include "ini_property.hpp"
#include <iostream>
#include <istream>
#include <stdexcept>
#include <string>

namespace trim
{

namespace
{

static const std::string WHITESPACE = " \n\r\t\f\v";

static std::string ltrim( const std::string& s )
{
    size_t start = s.find_first_not_of( WHITESPACE );
    return ( start == std::string::npos ) ? "" : s.substr( start );
}

static std::string rtrim( const std::string& s )
{
    size_t end = s.find_last_not_of( WHITESPACE );
    return ( end == std::string::npos ) ? "" : s.substr( 0, end + 1 );
}

} // namespace

static std::string trim( const std::string& s )
{
    return rtrim( ltrim( s ) );
}

} // namespace trim

namespace property
{

static IniProperty FromString( const std::string& line )
{

    auto sep = std::find_if( line.begin(), line.end(), []( const char ch ) { return ch == '='; } );
    if( sep == line.end() )
    {
        throw std::runtime_error( "no property value" );
    }

    std::string name( line.begin(), sep );
    std::string value( ++sep, line.end() );

    return IniProperty{ .name = trim::trim( name ), .value = trim::trim( value ) };
}

} // namespace property

namespace section
{

namespace header
{

static bool IsSectionHeader( const std::string& str )
{
    return str.front() == '[' && str.back() == ']';
}

static std::string GetSectionHeader( const std::string& str )
{
    return trim::trim( std::string( ++str.begin(), std::next( str.begin(), str.size() - 1 ) ) );
}

} // namespace header

static IniSection ParseEntity( std::istream& is, const std::string& header )
{
    std::string line;
    IniSection entity( header );

    if( is.eof() )
    {
        throw std::runtime_error( "no section body after header" );
    }

    while( 1 )
    {
        std::getline( is, line );
        if( line == "\n" || line == "" )
        {
            break;
        }

        entity.AddProperty( property::FromString( line ) );
    }

    return entity;
}

} // namespace section

static inline IniConfig ParseIni( std::istream& is )
{
    IniConfig config;
    while( !is.eof() )
    {
        std::string line;
        std::getline( is, line );

        if( line == "\n" || line == "" )
        {
            continue;
        }

        if( !section::header::IsSectionHeader( line ) )
        {
            throw std::runtime_error( "no section header found" );
        }

        config.AddSection( section::ParseEntity( is, section::header::GetSectionHeader( line ) ) );
    }

    return config;
}
