#include "crypt_gost/hash/i_hash.hpp"
#include <ini/ini_section.hpp>
#include "gtest/gtest.h"
#include <algorithm>
#include <crypt_gost/hash/gost_2012_256.hpp>
#include <crypt_gost/hash/gost_2012_512.hpp>
#include <gtest/gtest.h>
#include <iomanip>
#include <memory>
#include <stdexcept>

#include "kat_utils.hpp"
#include <kat_environment.hpp>

using namespace crypt_gost::hash;
using namespace crypt_gost::test::kat;

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
    case crypt_gost::hash::HashAlg::GOST_34_11_2012_512:
        return std::make_unique< GOST_34_11_2012_512 >();
    default:
        throw std::logic_error( "not supported" );
    }
}

std::string TestName( const IniSection& section )
{
    return section.Name();
}

namespace std
{

void PrintTo( const std::vector< uint8_t >& vec, std::ostream* os )
{
    *os << vec.size() / 2 << ":    ";
    for( size_t i = 0; i < vec.size() - 1; ++i )
    {
        *os << std::setfill( '0' ) << std::setw( 2 ) << std::hex << static_cast< int >( vec[ i ] )
            << ":";
    }
    *os << std::setfill( '0' ) << std::setw( 2 ) << std::hex
        << static_cast< int >( vec[ vec.size() - 1 ] ) << std::flush;
}

} // namespace std

class GOST_34_11_2012_test : public ::testing::TestWithParam< IniSection >
{
public:
    GOST_34_11_2012_test() = default;

    void SetUp() override
    {
        auto section = GetParam();
        data = ParseHex( section.Property( "data" ) );
        result = ParseHex( section.Property( "result" ) );
        hash = Fabric( FromString( section.Property( "hash_size" ) ) );

        std::reverse( data.begin(), data.end() );
        std::reverse( result.begin(), result.end() );
    }

public:
    std::vector< uint8_t > data;
    std::vector< uint8_t > result;
    std::unique_ptr< I_Hash > hash;
};

TEST_P( GOST_34_11_2012_test, ethalon )
{
    std::vector< uint8_t > result;
    hash->Update( data );
    hash->Final();
    hash->GetHash( result );
    ASSERT_EQ( result, this->result );
}

std::string TestNameGenerator( const ::testing::TestParamInfo< IniSection >& section )
{
    return section.param.Name();
}

INSTANTIATE_TEST_SUITE_P( KAT,
                          GOST_34_11_2012_test,
                          ::testing::ValuesIn( TestEnvironment::Instance()
                                                   .GetConfig( test_algo::GOST_34_11_2012 )
                                                   .SectionsList() ),
                          TestNameGenerator );
