project(allocator)

add_library( allocator STATIC heap_manager/heap_manager.c
                              heap_manager/heap_chunk.c
                              allocator/stack_allocator.cpp
                              allocator/heap_allocator.cpp
                              allocator/allocator_fn.cpp
                              allocator/allocator.c)
