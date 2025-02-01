#include <iostream>
#include <core/allocator/heap_manager/heap_manager.h>

#include <core/math/math.hpp>

int main()
{
    unsigned char buffer[ 1024 ] = { 0 };

    HeapManager* manager = HeapManager_Initialize( buffer, sizeof( buffer ), nullptr );
    HeapManager_DumpChunks( manager );
    std::cout << std::endl << std::endl;

    void* ptr1 = HeapManager_Allocate( manager, 64, 0 );
    HeapManager_DumpChunks( manager );
    std::cout << std::endl << std::endl;

    void* ptr2 = HeapManager_Allocate( manager, 200, 0 );
    HeapManager_DumpChunks( manager );
    std::cout << std::endl << std::endl;

    void* ptr3 = HeapManager_Allocate( manager, 150, 0 );
    HeapManager_DumpChunks( manager );
    std::cout << std::endl << std::endl;

    void* ptr4 = HeapManager_Allocate( manager, 10, 0 );
    HeapManager_DumpChunks( manager );
    std::cout << std::endl << std::endl;

    HeapManager_Deallocate( manager, ptr1 );
    HeapManager_DumpChunks( manager );
    std::cout << std::endl << std::endl;

    HeapManager_Deallocate( manager, ptr3 );
    HeapManager_DumpChunks( manager );
    std::cout << std::endl << std::endl;

    HeapManager_Deallocate( manager, ptr4 );
    HeapManager_DumpChunks( manager );
    std::cout << std::endl << std::endl;

    HeapManager_Deallocate( manager, ptr2 );
    HeapManager_DumpChunks( manager );
    HeapManager_Finalize( manager );

    std::vector< uint8_t > bytes1{ 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
                                   0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 };
    std::vector< uint8_t > bytes2{ 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
                                   0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02 };
    crypt_gost::core::math::LongNumber< 128, uint32_t > number1( bytes1.data() );
    crypt_gost::core::math::LongNumber< 128, uint32_t > number2 = number1;
    number2 = crypt_gost::core::math::LongNumber< 128, uint32_t >{ bytes2.data() };
    std::cout << "Number1 = " << number1 << std::endl;
    std::cout << "Number2 = " << number2 << std::endl;
    crypt_gost::core::math::LongNumber< 128, uint32_t > number3 = number1 ^ number2;
    std::cout << "Number3 = " << number3 << std::endl;

    crypt_gost::core::math::LongNumber< 128, uint32_t > num4 = 123;
    crypt_gost::core::math::LongNumber< 128, uint32_t > num5 = 123;
    num4 *= num5;
    std::cout << "\n\nNum4: " << num4 << std::endl;

    return 0;
}