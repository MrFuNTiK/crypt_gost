#pragma once

#include <stddef.h>

/**
 * @brief On-memory-release callback. Can be used to wipe deallocated memory.
 *
 */
typedef void ( *OnMemoryRelease_fn )( void* ptr, size_t size );