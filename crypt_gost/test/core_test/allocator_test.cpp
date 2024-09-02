#include <cstring>
#include <tuple>
#include <functional>

#include <core/allocator/heap_allocator.hpp>
#include <core/allocator/stack_allocator.hpp>

#include <gtest/gtest.h>

using namespace crypt_gost::core::allocator;

typedef std::function< I_Allocator&( void ) > AllocGetter;
using test_param_t = std::tuple< AllocGetter, size_t >;

void RandomMemory( std::vector<unsigned char>& buf )
{
    for( auto& byte : buf )
    {
        byte = std::rand();
    }
}

void ChechAlignment( void* ptr, size_t alignment )
{
    if( alignment )
    {
        ASSERT_EQ( 0, (size_t)ptr % alignment);
    }
}

class AllocatorTest : public testing::TestWithParam< test_param_t >
{
public:
    AllocatorTest() : buf1( 64 ), buf2( 200 ), buf3( 150 ), buf4( 10 ) 
    {
        RandomMemory(buf1);
        RandomMemory(buf2);
        RandomMemory(buf3);
        RandomMemory(buf4);
    };
    ~AllocatorTest() = default;

    std::vector<unsigned char> buf1;
    std::vector<unsigned char> buf2;
    std::vector<unsigned char> buf3;
    std::vector<unsigned char> buf4;
};

TEST_P( AllocatorTest, AllocatorTesting )
{
    auto allocGetter = std::get<0>(GetParam());
    auto& allocator = allocGetter();
    auto alignment = std::get<1>(GetParam());

    void* ptr1 = allocator.Allocate(64, alignment);
    ASSERT_NE(ptr1, nullptr);
    ChechAlignment(ptr1, alignment);
    std::memcpy( ptr1, buf1.data(), buf1.size() );

    void* ptr2 = allocator.Allocate(200, alignment);
    ASSERT_NE(ptr2, nullptr);
    ChechAlignment(ptr2, alignment);
    std::memcpy( ptr2, buf2.data(), buf2.size() );

    void* ptr3 = allocator.Allocate(150, alignment);
    ASSERT_NE(ptr3, nullptr);
    ChechAlignment(ptr3, alignment);
    std::memcpy( ptr3, buf3.data(), buf3.size() );

    void* ptr4 = allocator.Allocate(10, alignment);
    ASSERT_NE(ptr4, nullptr);
    ChechAlignment(ptr4, alignment);
    std::memcpy( ptr4, buf4.data(), buf4.size() );

    ASSERT_EQ(0, std::memcmp( ptr1, buf1.data(), buf1.size() ));
    ASSERT_EQ(0, std::memcmp( ptr2, buf2.data(), buf2.size() ));
    ASSERT_EQ(0, std::memcmp( ptr3, buf3.data(), buf3.size() ));
    ASSERT_EQ(0, std::memcmp( ptr4, buf4.data(), buf4.size() ));

    allocator.Deallocate(ptr1);
    ASSERT_EQ(0, std::memcmp( ptr2, buf2.data(), buf2.size() ));
    ASSERT_EQ(0, std::memcmp( ptr3, buf3.data(), buf3.size() ));
    ASSERT_EQ(0, std::memcmp( ptr4, buf4.data(), buf4.size() ));

    allocator.Deallocate(ptr3);
    ASSERT_EQ(0, std::memcmp( ptr2, buf2.data(), buf2.size() ));
    ASSERT_EQ(0, std::memcmp( ptr4, buf4.data(), buf4.size() ));

    allocator.Deallocate(ptr4);
    ASSERT_EQ(0, std::memcmp( ptr2, buf2.data(), buf2.size() ));

    allocator.Deallocate(ptr2);
}

INSTANTIATE_TEST_CASE_P(CoreTest, AllocatorTest,
    ::testing::Combine(
        ::testing::Values(HeapAllocator::GetInstance,
                          StackAllocator::GetInstance
                          ),
        ::testing::Values(0, 8, 16, 32, 64)
    )
);
