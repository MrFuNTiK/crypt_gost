#pragma once

#include <crypt_gost/hash/i_hash.hpp>

namespace crypt_gost
{

namespace hash
{

class GOST_34_11_2012_512 final : public I_Hash
{
public:
    static constexpr size_t HASH_SIZE = 64;
public:
    class Impl;

    GOST_34_11_2012_512();
    ~GOST_34_11_2012_512() noexcept;

    GOST_34_11_2012_512( const GOST_34_11_2012_512& ) = delete;
    GOST_34_11_2012_512& operator=( const GOST_34_11_2012_512& ) = delete;

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
