#pragma once

#include <stdlib.h>
#include <stdint.h>

#include <core/allocator/heap_manager/helper.h>

// clang-format off

/**
 * @brief Memory region.
 * 
 */
typedef struct 
{
    unsigned char* ptr;     ///< Pointer to managed memory.
    size_t size;            ///< Size of managed memory.
} HeapChunk_Region;

/**
 * @brief Single heap chunk.
 * 
 */
typedef struct HeapChunk_st
{
#ifndef NDEBUG
    char _beginMarker[ 32 ];        ///< Debugging buffer showing chunk start in memory dump.
#endif // NDEBUG
    HeapChunk_Region region;        ///< Memory region.
    struct HeapChunk_st* next;      ///< Next chunk in the list.
    struct HeapChunk_st* prev;      ///< Previous chunk in the list.
#ifndef NDEBUG
    char _endMarker[ 32 ];          ///< Debugging buffer showing chunk end in memory dump.
#endif // NDEBUG
} HeapChunk;

// clang-format on

/**
 * @brief Create chunk at specified address.
 *
 * @param[in] ptr Address where to create chunk.
 * @param[in] chunkSize Size of memory, which is managed by this chunk.
 * @param[in] alignment Alignment of memory, which is managed by this chunk.
 *
 * @return - HeapChunk* Created Chunk.
 */
HeapChunk* HeapChunk_CreateAt( void* ptr, size_t chunkSize, size_t alignment );

/**
 * @brief Release memory, managed by the chunk.
 *
 * @param[in] chunk Memory chunk.
 * @param[in] fn Memory-release function.
 */
void HeapChunk_Release( HeapChunk* chunk, OnMemoryRelease_fn fn );

/**
 * @brief Get address of next byte after the managed memory region.
 *
 * @param[in] chunk Memory chunk.
 * @param[in] alignment Required alignment of address.
 *
 * @return void* Address
 */
void* HeapChunk_GetFirstAfterChunk( HeapChunk* chunk, size_t alignment );

/**
 * @brief Check if available memory \p availableMemory is enough
 * to create chunk on address \p ptr, which must manage memory of size \p requiredChunkSize and
 * aligned by \p alignment.
 *
 *
 * @param[in] requiredChunkSize Required chunk size.
 * @param[in] alignment Required alignment of managed memory.
 * @param[in] ptr Address where chunk is to be created.
 * @param[in] availableMemory Available memory.
 *
 * @return - Result.
 * @retval 1 - If memory is enough.
 * @retval 0 - Otherwise.
 */
int HeapChunk_CheckSize( size_t requiredChunkSize,
                         size_t alignment,
                         void* ptr,
                         size_t availableMemory );

/**
 * @brief Reserve memory for new chunk at the start of existing chunk
 * and reduces it.
 *
 * New a address of reducible chunk \p chunk is written to \p chunk.
 *
 * @param[in,out] chunk Reducible chunk.
 * @param[in] size Size of memory, managed by new chunk.
 * @param[in] alignment Alignment of memory, managed by new chunk
 *
 * @return HeapChunk* - New reserved chunk. Actually equals to *chunk.
 */
HeapChunk* HeapChunk_CutFromBegin( HeapChunk** chunk, size_t size, size_t alignment );

/**
 * @brief Check whether the chunk has begin- and end-bound markers.
 *
 * @param[in] chunk Memory chunk.
 */
void HeapChunk_AssertChunkMarkers( HeapChunk* chunk );

/**
 * @brief Add begin- and end-bound markers to the chunk.
 *
 * @param[in] chunk Memory chunk.
 */
void HeapChunk_AddMarkers( HeapChunk* chunk );
