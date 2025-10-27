#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <type_traits>

namespace crypt_gost
{

namespace core
{

namespace crypto
{

template < class Processor >
class BlockProcessor
{
public:
    void Update( const uint8_t* data, size_t size )
    {
        // Process data that stayed unprocessed in last call
        size_t numToCopy = Processor::BLOCK_SIZE - unprocessed.size;
        numToCopy = numToCopy > size ? size : numToCopy;
        std::memcpy( unprocessed.data, data, numToCopy );
        unprocessed.size += numToCopy;
        size -= numToCopy;
        data += numToCopy;

        assert( numToCopy <= Processor::BLOCK_SIZE );

        // If still not enogh data
        if( unprocessed.size < Processor::BLOCK_SIZE )
        {
            return;
        }

        // Process [unprocessed + part of new data]
        processor.ProcessBlock( unprocessed.data );

        // Process rest full blocks
        unprocessed.size = size;
        for( size_t i = 0; i < size / Processor::BLOCK_SIZE; ++i )
        {
            std::memcpy( unprocessed.data, data, Processor::BLOCK_SIZE );
            processor.ProcessBlock( unprocessed.data );
            data += Processor::BLOCK_SIZE;
            unprocessed.size -= Processor::BLOCK_SIZE;
        }

        // Save unprocessed data
        std::memset( unprocessed.data, 0, Processor::BLOCK_SIZE );
        std::memcpy( unprocessed.data, data, unprocessed.size );
    }

    void Final()
    {
        Processor::DoPadding( unprocessed.data, unprocessed.size );
        processor.Final( unprocessed.data );
    }

    void GetResult( uint8_t* result )
    {
        processor.GetResult( result );
    }

private:
    struct DataBuffer
    {
        uint8_t data[ Processor::BLOCK_SIZE ] = { 0 };
        size_t size = 0;
    };
    Processor processor;
    DataBuffer unprocessed;
};

} // namespace crypto

} // namespace core

} // namespace crypt_gost
