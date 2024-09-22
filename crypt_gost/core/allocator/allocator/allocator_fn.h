#include <core/allocator/allocator.h>

#ifdef __cplusplus
extern "C" {
#endif

void* HeapAllocate( size_t size, size_t alignment );

void HeapDeallocate( void* ptr );

void* StackAllocate( size_t size, size_t alignment );

void StackDeallocate( void* ptr );

#ifdef __cplusplus
}
#endif
