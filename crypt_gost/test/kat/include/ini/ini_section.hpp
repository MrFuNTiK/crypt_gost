#pragma once

#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>

#include "ini_property.hpp"

using IniProperties = std::vector< IniProperty >;

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
        const std::string& name = prop.name;
        if( std::find_if(
                properties_.begin(),
                properties_.end(),
                [ name ]( const IniProperty& prop ) -> bool { return name == prop.name; } )
            != properties_.end() )
        {
            throw std::logic_error( "already exists" );
        }
        properties_.push_back( prop );
    }

    const std::string& Name() const noexcept
    {
        return header_;
    }

    const std::string& Property( const char* name ) const
    {
        auto found = std::find_if(
            properties_.begin(),
            properties_.end(),
            [ name ]( const IniProperty& prop ) -> bool { return name == prop.name; } );
        if( found == properties_.end() )
        {
            throw std::runtime_error( "not found" );
        }
        return found->value;
    }

    size_t Count() const noexcept
    {
        return properties_.size();
    }

private:
    std::string header_;
    IniProperties properties_;
};
