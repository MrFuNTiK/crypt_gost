
#include <assert.h>
#include <string.h>

#include <core/allocator/heap_manager/heap_chunk.h>

#include "ptr_helper.h"

#define HEAP_CHUNK_BEGIN_STRING "HeapChunk begin"
#define HEAP_CHUNK_END_STRING   "HeapChunk end"

static size_t _HeapChunk_TotalMemoryInUse( void* ptr, size_t chunkSize, size_t alignment );

HeapChunk* HeapChunk_CreateAt( void* ptr, size_t chunkSize, size_t alignment )
{
    assert( ptr );

    HeapChunk* chunk = ( HeapChunk* )ptr;
    memset( chunk, 0, sizeof( *chunk ) );

    void* memPtr = ptr + sizeof( HeapChunk );

    memPtr = SHIFT_PTR_UPTO_ALIGNMENT( memPtr, alignment );
    chunk->region.ptr = memPtr;
    chunk->region.size = chunkSize;
    HeapChunk_AddMarkers( chunk );
    memset( memPtr, 0, chunkSize );

    return chunk;
}

void HeapChunk_Release( HeapChunk* chunk, OnMemoryRelease_fn fn )
{
    assert( chunk );
    assert( chunk->next == NULL );
    assert( chunk->prev == NULL );

    if( fn )
    {
        fn( chunk->region.ptr, chunk->region.size );
    }
}

void* HeapChunk_GetFirstAfterChunk( HeapChunk* chunk )
{
    assert( chunk );
    HeapChunk_AssertChunkMarkers( chunk );
    return SHIFT_PTR_RIGHT( chunk->region.ptr, chunk->region.size );
}

int HeapChunk_CheckSize( size_t requiredChunkSize,
                         size_t alignment,
                         void* ptr,
                         size_t avaliableMemory )
{
    return _HeapChunk_TotalMemoryInUse( ptr, requiredChunkSize, alignment ) <= avaliableMemory;
}

static size_t _HeapChunk_TotalMemoryInUse( void* ptr, size_t chunkSize, size_t alignment )
{
    size_t totalSize = sizeof( HeapChunk ) + chunkSize;
    return totalSize
           + DIFF_UPTO_ALIGNMENT( SHIFT_PTR_RIGHT( ptr, sizeof( HeapChunk ) ), alignment );
}

HeapChunk* HeapChunk_CutFromBegin( HeapChunk** chunk, size_t size, size_t alignment )
{
    assert( chunk );
    assert( *chunk );

    const size_t totalUsedSize = _HeapChunk_TotalMemoryInUse( *chunk, size, alignment );

    if( totalUsedSize
        > PTR_DIFF( *chunk, SHIFT_PTR_RIGHT( ( *chunk )->region.ptr, ( *chunk )->region.size ) ) )
    {
        return NULL;
    }

    void* rightBorder = HeapChunk_GetFirstAfterChunk( *chunk );

    HeapChunk* rightChunk = ( HeapChunk* )SHIFT_PTR_RIGHT( *chunk, totalUsedSize );

    // Shift chunk pointer from the beginning of chunk struct to struct size.
    rightChunk->region.ptr = SHIFT_PTR_RIGHT( rightChunk, sizeof( *rightChunk ) );
    rightChunk->region.size = PTR_DIFF( rightChunk->region.ptr, rightBorder );
    rightChunk->prev = ( *chunk )->prev;
    rightChunk->next = ( *chunk )->next;
    HeapChunk_AddMarkers( rightChunk );

    if( rightChunk->next )
    {
        rightChunk->next->prev = rightChunk;
    }
    if( rightChunk->prev )
    {
        rightChunk->prev->next = rightChunk;
    }

    HeapChunk* ret = HeapChunk_CreateAt( *chunk, size, alignment );
    HeapChunk_AssertChunkMarkers( ret );
    HeapChunk_AssertChunkMarkers( rightChunk );
    *chunk = rightChunk;

    return ret;
}

void HeapChunk_AssertChunkMarkers( HeapChunk* chunk )
{
    assert( chunk );
#ifndef NDEBUG
    assert(
        0
        == strncmp( chunk->_beginMarker, HEAP_CHUNK_BEGIN_STRING, sizeof( chunk->_beginMarker ) ) );
    assert( 0 == strncmp( chunk->_endMarker, HEAP_CHUNK_END_STRING, sizeof( chunk->_endMarker ) ) );
#endif

    ( void )chunk;
}

void HeapChunk_AddMarkers( HeapChunk* chunk )
{
    assert( chunk );

#ifndef NDEBUG
    strncpy( chunk->_beginMarker, HEAP_CHUNK_BEGIN_STRING, sizeof( chunk->_beginMarker ) );
    strncpy( chunk->_endMarker, HEAP_CHUNK_END_STRING, sizeof( chunk->_endMarker ) );
#endif // NDEBUG
    ( void )chunk;
}
