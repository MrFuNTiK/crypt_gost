#pragma once

#include <crypt_gost/hash/i_hash.hpp>

namespace crypt_gost
{

namespace hash
{

class GOST_34_11_2012_256 final : public I_Hash
{
public:
    class Impl;

    GOST_34_11_2012_256();
    ~GOST_34_11_2012_256() noexcept;

    GOST_34_11_2012_256( const GOST_34_11_2012_256& ) = delete;
    GOST_34_11_2012_256& operator=( const GOST_34_11_2012_256& ) = delete;

    void Update( const uint8_t* data, size_t size ) override;
    void Update( const std::vector< uint8_t >& data ) override;
    void Final() override;
    void Final( std::vector< uint8_t >& hash ) override;
    void GetHash( std::vector< uint8_t >& hash ) override;

private:
    std::unique_ptr< Impl > impl_;
};

} // namespace hash

} // namespace crypt_gost
