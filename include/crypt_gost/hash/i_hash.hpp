#pragma once

#include <cstdint>
#include <memory>
#include <vector>

namespace crypt_gost
{

namespace hash
{

enum class HashAlg
{
    GOST_34_11_2012_256,
    GOST_34_11_2012_512
};

class I_Hash
{

public:
    I_Hash(HashAlg alg);
    virtual ~I_Hash() = default;

    virtual void Update(const uint8_t* ptr, size_t size) = 0;
    virtual void Update(const std::vector<uint8_t>& data) = 0;
    virtual void Final() = 0;
    virtual void Final(std::vector<uint8_t>& hash) = 0;
    virtual void GetHash(std::vector<uint8_t>& hash) = 0;
    inline HashAlg GetHashAlg() const noexcept;
    inline size_t GetHashBitSize() const noexcept;

private:
    HashAlg alg_;
};

std::unique_ptr<I_Hash> HashByAlg(HashAlg alg);

} // namespace hash


} // namesapce crypt_gost