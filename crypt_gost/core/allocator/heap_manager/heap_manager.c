#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <core/allocator/heap_manager/heap_manager.h>
#include "ptr_helper.h"

#include <assert.h>

#define CHUNKS_COLLECT_FACTOR 1

static inline void _HeapManager_AppendInUseChunk( HeapManager* manager, HeapChunk* chunk );
static inline void _HeapManager_AppendAvaliableChunk( HeapManager* manager, HeapChunk* chunk );
static inline void _AppendChunk( HeapChunks* chunks, HeapChunk* chunk );

static inline void _HeapManager_ReleaseInUseChunk( HeapManager* manager, HeapChunk* chunk );
static inline void _HeapManager_ReleaseAvaliableChunk( HeapManager* manager, HeapChunk* chunk );
static inline void _ReleaseChunk( HeapChunks* chunks, HeapChunk* chunk );
static inline void _HeapManager_CollectFragmentedChunks( HeapManager* manager );

static inline void _HeapManager_SortAvaliableChunks( HeapManager* manager );
static inline void _SortChunks( HeapChunks* chunks );

void _AppendChunk( HeapChunks* chunks, HeapChunk* chunk )
{
    assert( chunks );
    assert( chunk );
    HeapChunk_AssertChunkMarkers( chunk );

    if( !chunks->head )
    {
        chunks->head = chunk;
        chunks->tail = chunk;
        chunk->prev = NULL;
        chunk->next = NULL;
        return;
    }

    chunk->prev = chunks->tail;
    chunk->next = NULL;
    chunks->tail->next = chunk;
    chunks->tail = chunk;
}

void _HeapManager_AppendInUseChunk( HeapManager* manager, HeapChunk* chunk )
{
    assert( manager );
    assert( chunk );
    HeapChunk_AssertChunkMarkers( chunk );

    _AppendChunk( &manager->inUseChunks, chunk );
}

void _HeapManager_AppendAvaliableChunk( HeapManager* manager, HeapChunk* chunk )
{
    assert( manager );
    assert( chunk );
    HeapChunk_AssertChunkMarkers( chunk );

    _AppendChunk( &manager->avaliableChunks, chunk );
}

void _ReleaseChunk( HeapChunks* chunks, HeapChunk* chunk )
{
    assert( chunks );
    assert( chunk );
    HeapChunk_AssertChunkMarkers( chunk );

    if( chunks->head == chunk )
    {
        chunks->head = chunks->head->next;
    }
    else
    {
        chunk->prev->next = chunk->next;
    }

    if( chunks->tail == chunk )
    {
        chunks->tail = chunks->tail->prev;
    }
    else
    {
        chunk->next->prev = chunk->prev;
    }
    chunk->next = NULL;
    chunk->prev = NULL;
}

void _HeapManager_ReleaseInUseChunk( HeapManager* manager, HeapChunk* chunk )
{
    assert( manager );
    assert( chunk );
    HeapChunk_AssertChunkMarkers( chunk );

    _ReleaseChunk( &manager->inUseChunks, chunk );
}

void _HeapManager_ReleaseAvaliableChunk( HeapManager* manager, HeapChunk* chunk )
{
    assert( manager );
    assert( chunk );
    HeapChunk_AssertChunkMarkers( chunk );

    _ReleaseChunk( &manager->avaliableChunks, chunk );
}

inline void _HeapManager_CollectFragmentedChunks( HeapManager* manager )
{
    assert( manager );

    _HeapManager_SortAvaliableChunks( manager );
    HeapChunk* cur = manager->avaliableChunks.head;
    for( ; cur->next; )
    {
        if( cur->next == HeapChunk_GetFirstAfterChunk( cur ) )
        {
            HeapChunk* rightMergedChunk = cur->next;
            _HeapManager_ReleaseAvaliableChunk( manager, rightMergedChunk );
            cur->region.size =
                PTR_DIFF( cur->region.ptr, HeapChunk_GetFirstAfterChunk( rightMergedChunk ) );
        }
        else
        {
            cur = cur->next;
        }
    }
}

void _SortChunks( HeapChunks* chunks )
{
    assert( chunks );

    HeapChunk* cur = chunks->head;
    int swaped;
    if( !cur->next )
    {
        return;
    }

    do
    {
        swaped = 0;
        while( cur->next )
        {
            if( ( size_t )cur > ( size_t )cur->next )
            {
                HeapChunk* newCur = cur->next;
                swaped = 1;
                _ReleaseChunk( chunks, cur );
                _AppendChunk( chunks, cur );
                cur = newCur;
            }
            else
            {
                cur = cur->next;
            }
        }
        cur = chunks->head;
    } while( swaped );
}

static inline void _HeapManager_SortAvaliableChunks( HeapManager* manager )
{
    assert( manager );
    _SortChunks( &manager->avaliableChunks );
}

HeapManager* HeapManager_Initialize( void* ptr, size_t heapSize, OnMemoryRelease_fn cb )
{
    assert( ptr );
    HeapManager* manager = ( HeapManager* )ptr;

    memset( manager, 0, sizeof( *manager ) );

    manager->heap = ptr + sizeof( HeapManager );
    manager->heapSize = heapSize - sizeof( HeapManager );

    HeapChunk* initFreeChunk =
        HeapChunk_CreateAt( manager->heap, manager->heapSize - sizeof( HeapChunk ), 0 );
    initFreeChunk->prev = NULL;
    initFreeChunk->next = NULL;

    manager->avaliableChunks.head = initFreeChunk;
    manager->avaliableChunks.tail = initFreeChunk;
    manager->onReleaseCb = cb;
    return manager;
}

void* HeapManager_Allocate( HeapManager* manager, size_t size, size_t alignment )
{
    HeapChunk* chunk = manager->avaliableChunks.head;
    HeapChunk* newAllocated = NULL;
    if( !chunk )
    {
        return NULL;
    }

    for( ; chunk; chunk = chunk->next )
    {
        HeapChunk* tmp = chunk;
        newAllocated = HeapChunk_CutFromBegin( &tmp, size, alignment );
        if( newAllocated )
        {
            if( manager->avaliableChunks.head == chunk )
            {
                manager->avaliableChunks.head = tmp;
            }
            if( manager->avaliableChunks.tail == chunk )
            {
                manager->avaliableChunks.tail = tmp;
            }
            break;
        }
    }

    if( !newAllocated )
    {
        return NULL;
    }

    _HeapManager_AppendInUseChunk( manager, newAllocated );
    assert( alignment ? ( size_t )( newAllocated->region.ptr ) % alignment == 0 : 1 );
    return newAllocated->region.ptr;
}

void HeapManager_Deallocate( HeapManager* manager, void* ptr )
{
    assert( manager );

    if( !ptr )
    {
        return;
    }

    if( ( size_t )ptr < ( size_t )manager->heap
        || ( size_t )ptr > ( size_t )manager->heap + manager->heapSize )
    {
        // TODO: SEG_FAULT?
        // printf( "NOT OF THIS HEAP" );
        return;
    }

    HeapChunk* chunk = manager->inUseChunks.tail;
    HeapChunk* found = NULL;

    /**
     * Search chunk from the end assumin the later resource was
     * allocated, the earlier it is deallocated.
     *
     */
    for( ; chunk; chunk = chunk->prev )
    {
        if( chunk->region.ptr == ptr )
        {
            found = chunk;
            break;
        }
    }

    if( !found )
    {
        // TODO: SEG_FAULT?
        // printf( "NOT FOUND\n" );
        return;
    }

    _HeapManager_ReleaseInUseChunk( manager, found );
    HeapChunk_Release( found, manager->onReleaseCb );
    _HeapManager_AppendAvaliableChunk( manager, found );
    if( manager->deallocCtr++ == CHUNKS_COLLECT_FACTOR )
    {
        _HeapManager_CollectFragmentedChunks( manager );
        manager->deallocCtr = 0;
    }
}

void HeapManager_Finalize( HeapManager* manager )
{
    assert( manager );

    for( HeapChunk* chunk = manager->inUseChunks.head; chunk != NULL; chunk = chunk->next )
    {
        HeapChunk_Release( chunk, manager->onReleaseCb );
    }
    memset( manager, 0, sizeof( *manager ) );
}

void HeapManager_DumpChunks( HeapManager* manager )
{
    HeapChunk* chunk = manager->avaliableChunks.head;
    int ctr = 0;

    printf( "##### BEGIN CHUNKS DUMP #####\n" );

    printf( "Manager location:\n" );
    printf( "\tManager start address:\t\t%p\n", manager );
    printf( "\tManager heap last address:\t%p\n\n",
            ( void* )( ( size_t )manager->heap + manager->heapSize ) );

    printf( "Avaliable chunks:\n" );
    for( ; chunk; chunk = chunk->next )
    {
        if( !chunk )
            break;
        printf( "Chunk #%d:\n", ctr );
        printf( "Chunk start:\t%p\n", chunk );
        printf( "\tptr:\t%p\n", chunk->region.ptr );
        printf( "\tsize:\t%ld\n", chunk->region.size );
        printf( "Chunk end:\t%p\n", SHIFT_PTR_RIGHT( chunk->region.ptr, chunk->region.size ) );
        ++ctr;
    }
    printf( "\n" );

    chunk = manager->inUseChunks.head;
    ctr = 0;
    printf( "In-use chunks:\n" );
    for( ; chunk; chunk = chunk->next )
    {
        if( !chunk )
            break;
        printf( "Chunk #%d:\n", ctr );
        printf( "Chunk start:\t%p\n", chunk );
        printf( "\tptr:\t%p\n", chunk->region.ptr );
        printf( "\tsize:\t%ld\n", chunk->region.size );
        printf( "Chunk end:\t%p\n", SHIFT_PTR_RIGHT( chunk->region.ptr, chunk->region.size ) );
        ++ctr;
    }
    printf( "##### END CHUNKS DUMP #####\n\n" );
}
