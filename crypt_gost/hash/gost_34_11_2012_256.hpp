#pragma once

#include "crypt_gost/hash/gost_2012_256.hpp"
#include "crypt_gost/hash/gost_2012_512.hpp"
#include "gost_34_11_2012.hpp"

namespace crypt_gost
{

namespace hash
{

class GOST_34_11_2012_256__HASH_BLOCK final : public GOST_34_11_2012__HASH_BLOCK
{
private:
    static constexpr size_t HASH_SIZE = 32;

public:
    GOST_34_11_2012_256__HASH_BLOCK()
    {
        memset( h_, 0x01, sizeof( h_ ) );
    };

    size_t GetHashSize() noexcept override
    {
        return GOST_34_11_2012_256::HASH_SIZE;
    }
};

} // namespace hash

} // namespace crypt_gost
