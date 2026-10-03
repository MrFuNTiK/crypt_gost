#include <core/crypto/block_processor.h>
#include <core/math/math.hpp>
#include <core/util/traits.hpp>
#include <crypt_gost/hash/gost_2012_256.hpp>
#include "gost_34_11_2012_256.hpp"

using namespace crypt_gost::hash;
using namespace crypt_gost::core;

// clang-format off

class GOST_34_11_2012_256::Impl
{
public:
    void Update( const uint8_t* data, size_t size )
    {
        hash.Update( data, size );
    }

    void Update( const std::vector< uint8_t > data )
    {
        hash.Update( data.data(), data.size() );
    }

    void Final()
    {
        hash.Final();
    }

    void Final( std::vector< uint8_t >& result )
    {
        result.resize( GOST_34_11_2012_256::HASH_SIZE );
        hash.Final();
        hash.GetResult( result.data() );
    }

    void GetHash( std::vector< uint8_t >& result )
    {
        result.resize( GOST_34_11_2012_256::HASH_SIZE );
        hash.GetResult( result.data() );
    }

private:
    core::crypto::BlockProcessor< GOST_34_11_2012_256__HASH_BLOCK > hash;
};

GOST_34_11_2012_256::GOST_34_11_2012_256()
    : I_Hash( HashAlg::GOST_34_11_2012_256 )
    , impl_( std::make_unique< GOST_34_11_2012_256::Impl >() ) {};

GOST_34_11_2012_256::~GOST_34_11_2012_256() noexcept {};

void GOST_34_11_2012_256::Update( const std::vector< uint8_t >& data )
{
    impl_->Update( data );
}

void GOST_34_11_2012_256::Update( const uint8_t* data, size_t size )
{
    impl_->Update( data, size );
}

void GOST_34_11_2012_256::Final()
{
    impl_->Final();
}

void GOST_34_11_2012_256::Final( std::vector< uint8_t >& hash )
{
    impl_->Final();
    GetHash( hash );
}

void GOST_34_11_2012_256::GetHash( std::vector< uint8_t >& hash )
{
    impl_->GetHash( hash );
}
