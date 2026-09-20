#pragma once

#include "ini_section.hpp"
#include <algorithm>
#include <stdexcept>

using IniEntities = std::vector< IniSection >;

class IniConfig final
{
public:
    IniConfig() = default;
    IniConfig( const IniConfig& ) = default;
    IniConfig& operator=(const IniConfig&) = default;
    IniConfig( IniConfig&& ) noexcept = default;
    IniConfig& operator=(IniConfig&&) noexcept = default;
    ~IniConfig() noexcept = default;

    void AddSection( const IniSection& section )
    {
        if( std::find_if(
                entities_.begin(),
                entities_.end(),
                [ section ]( const IniSection& it ) -> bool { return section.Name() == it.Name(); } )
            != entities_.end() )
        {
            throw std::logic_error( "already exists" );
        }
        entities_.push_back( section );
    }

    const IniSection& Section( const char* name ) const
    {
        auto found = std::find_if(
            entities_.begin(),
            entities_.end(),
            [ name ]( const IniSection& enitity ) -> bool { return name == enitity.Name(); } );

        if( found == entities_.end() )
        {
            throw std::runtime_error( "not found" );
        }
        return *found;
    }

private:
    IniEntities entities_;
};