#pragma once

#include "ini_section.hpp"
#include <vector>
#include <stdexcept>

using IniSections = std::map< std::string, IniSection >;

class IniConfig final
{
public:
    IniConfig() = default;
    IniConfig( const IniConfig& ) = default;
    IniConfig& operator=( const IniConfig& ) = default;
    IniConfig( IniConfig&& ) noexcept = default;
    IniConfig& operator=( IniConfig&& ) noexcept = default;
    ~IniConfig() noexcept = default;

    void AddSection( const IniSection& section )
    {
        const auto item = sections_.emplace( section.Name(), section );
        if( !item.second )
        {
            throw std::logic_error( "already exists" );
        }
    }

    const IniSection& Section( const char* name ) const
    {
        return sections_.at( name );
    }

    const IniSections& Sections() const noexcept
    {
        return sections_;
    }

    const std::vector<IniSection> SectionsList() const {
        std::vector<IniSection> sections;
        for(const auto& section : sections_) {
            sections.push_back(section.second);
        }
        return sections;
    }

private:
    IniSections sections_;
};