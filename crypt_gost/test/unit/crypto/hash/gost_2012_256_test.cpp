#include "gtest/gtest.h"
#include <crypt_gost/crypto/hash/i_hash.hpp>
#include <gtest/gtest.h>

#include <crypt_gost/crypto/hash/gost_34_11_2012_256.hpp>

#include "../../../util/gost_34_11_2012.hpp"

using namespace crypt_gost::crypto::hash;
using namespace crypt_gost::test::hash;

class HashTest : public ::testing::TestWithParam< I_Hash::HashAlg >
{
};

namespace
{

std::vector< uint8_t > RandomData( size_t size )
{
    std::vector< uint8_t > data( size, 0 );
    for( auto& byte: data )
    {
        byte = rand();
    }
    return data;
}

} // namespace

TEST_P( HashTest, basic )
{
    auto data = RandomData( 1000 );
    auto hash = Factory( GetParam() );
    hash->Update( data );
    hash->Final();
    std::vector< uint8_t > result;
    hash->GetHash( result );
}

TEST_P( HashTest, concatDataViaUpdate )
{
    const size_t TOTAL_DATA_SIZE = 1000;
    const size_t CHUNK_SIZE = 100;
    std::vector< uint8_t > result_single;
    std::vector< uint8_t > result_by_chunks;
    auto data = RandomData( TOTAL_DATA_SIZE );
    auto hash_single = Factory( GetParam() );
    auto hash_by_chunks = Factory( GetParam() );

    hash_single->Update( data );
    hash_single->Final();
    hash_single->GetHash( result_single );

    for( size_t i = 0; i < TOTAL_DATA_SIZE / CHUNK_SIZE; ++i )
    {
        auto start = std::next( data.begin(), CHUNK_SIZE * i );
        const std::vector< uint8_t > chunk( start, std::next( start, CHUNK_SIZE ) );
        hash_by_chunks->Update( chunk );
    }
    hash_by_chunks->Final();
    hash_by_chunks->GetHash( result_by_chunks );
    ASSERT_EQ( result_single, result_by_chunks );
}

static std::string TestNameGenerator( const ::testing::TestParamInfo< I_Hash::HashAlg >& info )
{
    switch( info.param )
    {
    case I_Hash::HashAlg::GOST_34_11_2012_256:
    {
        return "gost_34_11_2012_256";
    }
    case I_Hash::HashAlg::GOST_34_11_2012_512:
    {
        return "gost_34_11_2012_512";
    }
    }
}

INSTANTIATE_TEST_SUITE_P( UnitTest,
                          HashTest,
                          ::testing::Values( I_Hash::HashAlg::GOST_34_11_2012_256,
                                             I_Hash::HashAlg::GOST_34_11_2012_512 ),
                          TestNameGenerator );
