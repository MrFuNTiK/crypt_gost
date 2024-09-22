#include <core/allocator/allocator.h>
#include "allocator_fn.h"

const CoreAllocator HeapAllocator = { .allocate = HeapAllocate, .deallocate = HeapDeallocate };
const CoreAllocator StackAllocator = { .allocate = StackAllocate, .deallocate = StackDeallocate };
