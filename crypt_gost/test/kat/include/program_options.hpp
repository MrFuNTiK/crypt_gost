#pragma once

#include <cstring>
#include <string>
#include <vector>
#include <map>

struct program_arg
{
    const char* name;
    const char* description;
    bool hasValue;
};

using program_options = std::map< std::string, std::string >;

class OptionsParser final
{
public:
    OptionsParser( const std::vector< program_arg >& args )
        : args_( args ) {};
    OptionsParser( const OptionsParser& ) = delete;
    OptionsParser( OptionsParser&& ) = delete;

public:
    program_options ParseArgs( int argc, const char** argv ) const noexcept
    {
        program_options parsed_options;

        for( int i = 1; i < argc; )
        {
            const char* optName = argv[ i ] + 2;
            const char* optValue = argv[ i + 1 ];
            const auto arg = FindArg( optName );
            if( !arg )
            {
                ++i;
                continue;
            }

            parsed_options.emplace( arg->name, arg->hasValue ? optValue : "" );
            i += 1 + arg->hasValue;
        }

        return parsed_options;
    }

private:
    const program_arg* FindArg( const char* check ) const noexcept
    {
        for( const auto& arg: args_ )
        {
            if( 0 == std::strncmp( arg.name, check, std::strlen( arg.name ) ) )
            {
                return &arg;
            }
        }
        return nullptr;
    }

private:
    const std::vector< program_arg > args_;
};