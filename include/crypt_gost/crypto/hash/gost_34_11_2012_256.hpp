#pragma once

#include <crypt_gost/crypto/hash/i_hash.hpp>

namespace crypt_gost
{

namespace crypto
{

namespace hash
{

class GOST_34_11_2012_256 final : public crypt_gost::crypto::hash::I_Hash
{
public:
    static constexpr size_t HASH_SIZE = 32;

public:
    GOST_34_11_2012_256();
    ~GOST_34_11_2012_256() noexcept;

    GOST_34_11_2012_256( const GOST_34_11_2012_256& ) = delete;
    GOST_34_11_2012_256& operator=( const GOST_34_11_2012_256& ) = delete;

    /**
     * @brief Process more data to calculate hash. Can be called several times.
     *
     * @param[in] ptr Pointer to data-to-hash.
     * @param[in] size Size of data @p ptr.
     */
    void Update( const uint8_t* data, size_t size ) override;

    /**
     * @brief Process more data to calculate hash. Can be called several times.
     *
     * @param[in] data Data-to-hash.
     */
    void Update( const std::vector< uint8_t >& data ) override;

    /**
     * @brief Finalize hash calculation. No @ref Update() call is possible after this call.
     *
     */
    void Final() override;

    /**
     * @brief Get result hash value. Can be called only after @ref Final() call.
     * 
     * @param[out] hash Hash value.
     */
    void GetHash( std::vector< uint8_t >& hash ) override;

private:
    class Impl;
    std::unique_ptr< Impl > impl_;
};

} // namespace hash

} // namespace crypto

} // namespace crypt_gost
