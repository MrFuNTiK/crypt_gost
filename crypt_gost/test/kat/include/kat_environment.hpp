#pragma once

#include <array>
#include <map>
#include <stdexcept>
#include "ini/ini_config.hpp"

enum test_algo : size_t
{
    BEGIN,
    GOST_34_11_2012 = BEGIN,
    COUNT,
};

using IniConfigs = std::map<std::string, IniConfig>;

static inline test_algo& operator++( test_algo& algo )
{
    switch( algo )
    {
    case( COUNT ):
    {
        return algo;
    }
    default:
    {
        size_t integer = static_cast< size_t >( algo );
        ++integer;
        algo = static_cast< test_algo >( integer );
        return algo;
    }
    }
}

#define TEST_ALGO_SERIALIZE( ALG ) \
    case( ALG ):                   \
    {                              \
        return #ALG;               \
    }

// clang-format off

static inline const char* ToString( test_algo algo )
{
    switch( algo )
    {
        TEST_ALGO_SERIALIZE( GOST_34_11_2012 );
        case( COUNT ):
        {
            throw std::logic_error( "not serializable" );
        }
    }
}

// clang-format on

class KAT_environment
{
public:
    static KAT_environment& Instance();
    void SetUpByArgs( int argc, char** argv );
    void RegisterConfig( test_algo alg, IniConfig&& config );
    void LoadConfigs( const std::string& data_config_path );
    const IniConfig& GetConfig( test_algo alg );

private:
    IniConfigs configs_;
};