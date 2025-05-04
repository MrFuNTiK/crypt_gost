#pragma once

#include <cstring>
#include <core/allocator/heap_allocator.hpp>

namespace crypt_gost
{

namespace core
{

namespace util
{

using namespace crypt_gost::core::allocator;

class MemBuf final
{
public:
    MemBuf( size_t size, size_t alignment = 0, I_Allocator& alloc = HeapAllocator::GetInstance() )
        : size_( size )
        , capacity_( size )
        , alignment_( alignment )
        , buf_( alloc.Allocate( size, alignment ) )
        , alloc_( alloc )
    {
        if( !buf_ )
        {
            throw std::runtime_error( "Malloc failure" );
        }
    };

    MemBuf( const MemBuf& other )
        : MemBuf( other.size_, other.alignment_, other.alloc_ )
    {
        std::memcpy( buf_, other.buf_, size_ );
    }

    MemBuf& operator=( const MemBuf& other )
    {
        if( capacity_ < other.size_ || alignment_ != other.alignment_
            || !std::is_same< decltype( alloc_ ), decltype( other.alloc_ ) >::value )
        {
            alloc_.Deallocate( buf_ );
            buf_ = other.alloc_.Allocate( other.size_, other.alignment_ );
            if( !buf_ )
            {
                throw std::runtime_error( "Allocation failure" );
            }
            capacity_ = other.size_;
        }
        std::memset( buf_, 0, size_ );
        std::memcpy( buf_, other.buf_, other.size_ );
        size_ = other.size_;
        alignment_ = other.alignment_;
        return *this;
    }

    MemBuf( MemBuf&& other ) noexcept
        : size_( other.size_ )
        , capacity_( other.capacity_ )
        , alignment_( other.alignment_ )
        , buf_( other.buf_ )
        , alloc_( other.alloc_ )
    {
        other.buf_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
        other.alignment_ = 0;
    }

    MemBuf& operator=( MemBuf&& other ) noexcept
    {
        size_ = other.size_;
        capacity_ = other.capacity_;
        alignment_ = other.alignment_;
        alloc_.Deallocate( buf_ );
        buf_ = other.buf_;
        alloc_ = other.alloc_;

        other.size_ = 0;
        other.capacity_ = 0;
        other.alignment_ = 0;
        other.buf_ = nullptr;
        return *this;
    }

    ~MemBuf() noexcept
    {
        alloc_.Deallocate( buf_ );
    }

public:
    inline void* GetBuf() const noexcept
    {
        return buf_;
    }

private:
    size_t size_;
    size_t capacity_;
    size_t alignment_;
    void* buf_;
    I_Allocator& alloc_;
};

} // namespace util

} // namespace core

} // namespace crypt_gost
