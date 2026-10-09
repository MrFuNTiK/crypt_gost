#pragma once

#include <string>
#include <stdexcept>

#include <crypt_gost/crypto/hash/i_hash.hpp>
#include <crypt_gost/crypto/hash/gost_34_11_2012_256.hpp>
#include <crypt_gost/crypto/hash/gost_34_11_2012_512.hpp>

using namespace crypt_gost;

namespace crypt_gost
{

namespace test
{

namespace hash
{

static inline crypto::hash::I_Hash::HashAlg FromString( const std::string& str )
{
    if( str == "256" )
    {
        return crypto::hash::I_Hash::HashAlg::GOST_34_11_2012_256;
    }
    else if( str == "512" )
    {
        return crypto::hash::I_Hash::HashAlg::GOST_34_11_2012_512;
    }
    throw std::runtime_error( "undefined hash size" );
}

static inline std::unique_ptr< crypto::hash::I_Hash > Factory( crypto::hash::I_Hash::HashAlg alg )
{
    switch( alg )
    {
    case crypto::hash::I_Hash::HashAlg::GOST_34_11_2012_256:
        return std::make_unique< crypto::hash::GOST_34_11_2012_256 >();
    case crypto::hash::I_Hash::HashAlg::GOST_34_11_2012_512:
        return std::make_unique< crypto::hash::GOST_34_11_2012_512 >();
    default:
        throw std::logic_error( "not supported" );
    }
}

} // namespace hash

} // namespace test

} // namespace crypt_gost
