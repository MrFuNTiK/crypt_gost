#include <gtest/gtest.h>

#include <crypt_gost/hash/gost_2012_256.hpp>

using namespace crypt_gost::hash;

class HashTest : public testing::Test
{};

namespace
{

std::vector<uint8_t> RandomData(size_t size) {
    std::vector<uint8_t> data(size, 0);
    for( auto& byte : data )
    {
        byte = rand();
    }
    return data;
} 


}

TEST_F( HashTest, basic )
{
    auto data = RandomData(1000);
    GOST_34_11_2012_256 hash;
    hash.Update( data );
    hash.Final();
    std::vector< uint8_t > result;
    hash.GetHash( result );
}

TEST_F(HashTest, concatDataViaUpdate) {
    const size_t TOTAL_DATA_SIZE = 1000;
    const size_t CHUNK_SIZE = 100;
    std::vector< uint8_t > result_single;
    std::vector< uint8_t > result_by_chunks;
    auto data = RandomData(TOTAL_DATA_SIZE);
    GOST_34_11_2012_256 hash_single;
    GOST_34_11_2012_256 hash_by_chunks;

    hash_single.Update( data );
    hash_single.Final();
    hash_single.GetHash( result_single );

    for(size_t i = 0; i < TOTAL_DATA_SIZE / CHUNK_SIZE; ++i) {
        auto start = std::next(data.begin(), CHUNK_SIZE * i);
        const std::vector<uint8_t> chunk(start, std::next(start, CHUNK_SIZE));
        hash_by_chunks.Update(chunk);
    }
    hash_by_chunks.Final();
    hash_by_chunks.GetHash(result_by_chunks);
    ASSERT_EQ(result_single, result_by_chunks);
}