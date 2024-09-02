#include <iostream>
#include <allocator/heap_manager.h>

int main()
{
    unsigned char buffer [ 1024 ] = { 0 };

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

    return 0;
}