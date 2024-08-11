project(allocator)

add_library( allocator STATIC heap_manager/heap_manager.c
                                         heap_manager/heap_chunk.c)
