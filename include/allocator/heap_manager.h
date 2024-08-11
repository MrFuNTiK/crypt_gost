#pragma once

#include <allocator/heap_chunk.h>
#include <allocator/helper.h>

/**
 * @brief List of chunks, managed by HeapManager.
 * 
 */
typedef struct
{
    HeapChunk* head;
    HeapChunk* tail;
} HeapChunks;

/**
 * @brief Heap manager.
 * 
 */
typedef struct
{
    void* heap;                     ///< Pointer to the heap buffer.
    size_t heapSize;                ///< Size of heap buffer.
    HeapChunks avaliableChunks;     ///< List of chunks that are avaliable for use.
    HeapChunks inUseChunks;         ///< List if chunks that are in use.
    int deallocCtr;                 ///< Counter of deallocation calls.
    OnMemoryRelease_fn onReleaseCb; ///< On-memory-release callback.
} HeapManager;


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize HeapManager at the given memory buffer.
 * 
 * @note Only given memory buffer is used to return as allocated memory
 * and store whole manage-helping data structures.
 * 
 * @param[in] ptr Pointer to memory buffer. 
 * @param[in] heapSize Size of memory buffer.
 * @param[in] cb On-memory-release callback.
 * 
 * @return HeapManager* Pointer to initialized HeapManager. Always equals \p ptr.
 */
HeapManager* HeapManager_Initialize( void* ptr, size_t heapSize, OnMemoryRelease_fn cb );

/**
 * @brief Request memory from the heap.
 * 
 * @param[in] manager HeapManager
 * @param[in] size Size of allocated memory
 * @param[in] alignment Alignment of allocated memory
 * 
 * @return void* - Pointer to allocated memory.
 * @retval !NULL - in case of success.
 * @retval NULL - in case of allocation failure.
 */
void* HeapManager_Allocate( HeapManager* manager, size_t size, size_t alignment );

/**
 * @brief Deallocate memory.
 * 
 * @param[in] manager HeapManager, which was used ti allocate the memory.
 * @param[in] ptr Pointer to allocated memory.
 */
void HeapManager_Deallocate( HeapManager* manager, void* ptr );

/**
 * @brief Debugging dump memory chunks to stdout.
 * 
 * @param[in] manager HeapManager.
 */
void HeapManager_DumpChunks( HeapManager* manager );

#ifdef __cplusplus
}
#endif
