#ifndef BASE_MEMORY_H
#define BASE_MEMORY_H

////////////////////////////////////////////////////////////////////////////////
// NOTE(phong): Dependencies

#include "base_type.h"
#include "base_context_crack.h"
#include "base_math.h"
#include <stdlib.h> //malloc, free, calloc, realloc
#include <stddef.h> //max_align_t

////////////////////////////////////////////////////////////////////////////////
// NOTE(phong): Alignment 

// NOTE(phong): pre c11 for future compat

// #if COMPILER_MSVC
// # define AlignOf(T) (Size)__alignof(T)
// #elif defined(__clang__)
// # define AlignOf(T) (Size)__alignof__(T)
// #elif COMPILER_GCC
// # define AlignOf(T) (Size)__alignof__(T)
// #else
// # define AlignOf(T) ((Size)&((struct { S8 c; T d; } *)0)->d)
// // NOTE(phong):            |                          |
// //                         |          +---------------+
// //                  |      |          |      The struct in memory:                      
// //  example T = S32 |      |          V    +-----+-----+-----+-----+
// //  the offset addr |      |     nullptr ->|  c  |     |     |     |
// //  is align value  |      |               +-----+-----+-----+-----+
// //                  |      +----> offset ->|  d  |  d  |  d  |  d  |
// //                  |                      +-----+-----+-----+-----+
// #endif

#define AlignOf(T) (Size)_Alignof(T)

////////////////////////////////////////////////////////////////////////////////
// NOTE(phong): Global Defaults

#define ARENA_MINIMUM_ALIGNMENT AlignOf(max_align_t)
#define ARENA_REGION_DEFAULT_CAPACITY KB(64)

////////////////////////////////////////////////////////////////////////////////
// NOTE(phong): Memory Region

typedef struct Region Region;
struct Region 
{
    Region *next;
    Size capacity;
    char data[];
};

// NOTE(Phong): region creaction/destruction

Region *region_create(Size capacity);
void region_free(Region *r);

////////////////////////////////////////////////////////////////////////////////
// NOTE(phong): Arena / Bump / Linear allocator
// Arenas are optimized for _small_, frequent, short lived allocations.

typedef struct Arena Arena;
struct Arena
{
    Region *begin, *end;
    Size count; //curr off count
};

typedef struct ArenaMarker ArenaMarker;
struct ArenaMarker
{
    Region *end;
    Size count; 
};

// NOTE(phong): arena creation/destruction
void arena_init_size(Arena *arena, Size size);
void arena_init(Arena *arena);
void arena_free(Arena *arena);

// NOTE(phong): arena push/reset
void* arena_push(Arena *arena, Size size, Size align);
void arena_reset(Arena *arena);

// NOTE(phong): arena snapshot/rewind
void arena_snapshot(Arena *arena, ArenaMarker *ret);
void arena_rewind(Arena *arena, ArenaMarker *mark);

// NOTE(phong): arena macros
#define push_array(a, T, n) arena_push((a), sizeof(T)*(n), AlignOf(T))


////////////////////////////////////////////////////////////////////////////////
// NOTE(phong): Pool allocator (Arena + FreeList)


#endif //BASE_MEMORY_H