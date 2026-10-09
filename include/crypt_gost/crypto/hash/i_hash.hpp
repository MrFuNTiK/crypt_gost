#pragma once

#include <cstdint>
#include <memory>
#include <vector>

namespace crypt_gost
{

namespace crypto
{

namespace hash
{

/**
 * @brief Abstract interface for hashing algorithm.
 *
 */
class I_Hash
{
public:
    /// @brief Hashing algorithm id.
    enum class HashAlg
    {
        GOST_34_11_2012_256, ///< GOST R 34.12-2012 with 256bit output.
        GOST_34_11_2012_512, ///< GOST R 34.12-2012 with 512bit output.
    };

    static constexpr size_t GOST_34_11_2012_256_HASH_SIZE = 32;

    static constexpr size_t GOST_34_11_2012_512_HASH_SIZE = 64;

public:
    I_Hash( HashAlg alg )
        : alg_( alg ) {};
    virtual ~I_Hash() = default;

    /**
     * @brief Process more data to calculate hash. Can be called several times.
     *
     * @param[in] ptr Pointer to data-to-hash.
     * @param[in] size Size of data @p ptr.
     */
    virtual void Update( const uint8_t* ptr, size_t size ) = 0;

    /**
     * @brief Process more data to calculate hash. Can be called several times.
     *
     * @param[in] data Data-to-hash.
     */
    virtual void Update( const std::vector< uint8_t >& data ) = 0;

    /**
     * @brief Finalize hash calculation. No @ref Update() call is possible after this call.
     *
     */
    virtual void Final() = 0;

    /**
     * @brief Get result hash value. Can be called only after @ref Final() call.
     * 
     * @param[out] hash Hash value.
     */
    virtual void GetHash( std::vector< uint8_t >& hash ) = 0;

    /**
     * @brief Get algorithm identificator.
     * 
     * @return HashAlg 
     */
    HashAlg GetHashAlg() const noexcept
    {
        return alg_;
    }

    /**
     * @brief Get result hash value size.
     * 
     * @return Hash value size.
     */
    size_t GetHashSize() const noexcept
    {
        switch( alg_ )
        {
        case I_Hash::HashAlg::GOST_34_11_2012_256:
            return I_Hash::GOST_34_11_2012_256_HASH_SIZE;
        case I_Hash::HashAlg::GOST_34_11_2012_512:
            return I_Hash::GOST_34_11_2012_512_HASH_SIZE;
        }
    }

private:
    HashAlg alg_;
};

} // namespace hash

} // namespace crypto

} // namespace crypt_gost
