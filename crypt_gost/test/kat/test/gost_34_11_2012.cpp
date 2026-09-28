#include "crypt_gost/hash/i_hash.hpp"
#include <ini/ini_section.hpp>
#include "gtest/gtest.h"
#include <algorithm>
#include <crypt_gost/hash/gost_2012_256.hpp>
#include <gtest/gtest.h>
#include <memory>
#include <stdexcept>

#include "kat_utils.hpp"
#include <kat_environment.hpp>

using namespace crypt_gost::hash;

HashAlg FromString( const std::string& str )
{
    if( str == "256" )
    {
        return HashAlg::GOST_34_11_2012_256;
    }
    else if( str == "512" )
    {
        return HashAlg::GOST_34_11_2012_512;
    }
    throw std::runtime_error( "undefined hash size" );
}

std::unique_ptr< I_Hash > Fabric( HashAlg alg )
{
    switch( alg )
    {
    case crypt_gost::hash::HashAlg::GOST_34_11_2012_256:
        return std::make_unique< GOST_34_11_2012_256 >();
    default:
        throw std::logic_error( "not supported" );
    }
}

std::string TestName( const IniSection& section )
{
    return section.Name();
}

class GOST_34_11_2012_test : public ::testing::TestWithParam< IniSection >
{
    GOST_34_11_2012_test() = default;

    void SetUp() override
    {
        auto section = GetParam();
        data = ParseHex( section.Property( "data" ) );
        result = ParseHex( section.Property( "hash" ) );
        hash = Fabric( FromString( section.Property( "hash_size" ) ) );
    }

public:
    std::vector< uint8_t > data;
    std::vector< uint8_t > result;
    std::unique_ptr< I_Hash > hash;
};

INSTANTIATE_TEST_SUITE_P( KAT,
                          GOST_34_11_2012_test,
                          ::testing::ValuesIn( KAT_environment::Instance()
                                                   .GetConfig( test_algo::GOST_34_11_2012 )
                                                   .Sections() ) );
