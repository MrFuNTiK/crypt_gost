#include <cassert>
#include <crypt_gost/hash/i_hash.hpp>

using namespace crypt_gost::hash;

namespace
{

size_t GetHashBitSize(HashAlg alg) noexcept
{
    switch(alg)
    {
    case HashAlg::GOST_34_11_2012_256:
        return 256;
    case HashAlg::GOST_34_11_2012_512:
        return 512;
    default:
        assert(0 && "undefined algorythm");
        return -1;
    }
}

} // namespace

I_Hash::I_Hash(HashAlg alg) : alg_( alg ){};

HashAlg I_Hash::GetHashAlg() const noexcept
{
    return alg_;
}

size_t I_Hash::GetHashBitSize() const noexcept
{
    return ::GetHashBitSize(alg_);
}
