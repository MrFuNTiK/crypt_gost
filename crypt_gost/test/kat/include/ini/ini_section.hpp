#pragma once

#include <stdexcept>
#include <string>
#include <map>

#include "ini_property.hpp"

using IniProperties = std::map< std::string, std::string >;

namespace crypt_gost
{

namespace test
{

namespace kat
{

class IniSection final
{
public:
    IniSection( const std::string& name )
        : header_( name )
        , properties_()
    {
        if( header_.empty() )
        {
            throw std::logic_error( "Can not be unnamed" );
        }
    };

    IniSection( const IniSection& ) = default;
    IniSection& operator=( const IniSection& rhs ) = default;
    IniSection( IniSection&& ) noexcept = default;
    IniSection& operator=( IniSection&& ) noexcept = default;
    ~IniSection() noexcept = default;

    void AddProperty( const IniProperty& prop )
    {
        const auto item = properties_.emplace( prop.name, prop.value );
        if( !item.second )
        {
            throw std::logic_error( "Already exists" );
        }
    }

    const std::string& Name() const noexcept
    {
        return header_;
    }

    const std::string& Property( const char* name ) const
    {
        return properties_.at( name );
    }

    size_t Count() const noexcept
    {
        return properties_.size();
    }

private:
    std::string header_;
    IniProperties properties_;
};

} // namespace kat

} // namespace test

} // namespace crypt_gost
