#include <gtest/gtest.h>

#include <crypt_gost/hash/gost_2012_256.hpp>

using namespace crypt_gost::hash;

class HashTest : public testing::Test
{};

namespace
{

void Randomize( std::vector< uint8_t>& data )
{
    for( auto& byte : data )
    {
        byte = rand();
    }
}

}

TEST_F( HashTest, basic )
{
    std::vector< uint8_t > data;
    data.resize( 2000 );
    ::Randomize( data );

    GOST_34_11_2012_256 hash;
    hash.Update( data );
    hash.Final();
    std::vector< uint8_t > result;
    hash.GetHash( result );
}