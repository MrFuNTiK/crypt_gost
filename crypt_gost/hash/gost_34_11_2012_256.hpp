#pragma once

#include <crypt_gost/crypto/hash/gost_34_11_2012_256.hpp>
#include "gost_34_11_2012.hpp"

namespace crypt_gost
{

namespace crypto
{

namespace hash
{

class GOST_34_11_2012_256__HASH_BLOCK final : public crypt_gost::hash::GOST_34_11_2012__HASH_BLOCK
{
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

} // namespace crypto

} // namespace crypt_gost
