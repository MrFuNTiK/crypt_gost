#pragma once

#include <list>
#include <cstdlib>
#include "i_allocator.hpp"

namespace crypt_gost
{

namespace core
{

namespace allocator
{

class SecureAllocator : I_Allocator
{
public:
    struct Chunk
    {
        u_int8_t* chunk;
        size_t size;
    };
    SecureAllocator( const SecureAllocator& ) = delete;
    SecureAllocator operator=( const SecureAllocator& ) = delete;
    friend void InitAllocator( size_t heapSize );

    static SecureAllocator& GetAllocator();
    void* Allocate(size_t numBytes ) override;
    void Deallocate(void* bytes, size_t numBytes ) override;

private:
    SecureAllocator();

private:
    std::list<Chunk> freeChunk;
    std::list<Chunk> inuseChunks;
};

} // namespace allocator

} // namespace core

} // namespace crypt_gost