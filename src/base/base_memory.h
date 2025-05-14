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
// NOTE(phong): Alignment and pre c11 for future compat

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
# define AlignOf(T) (Size)_Alignof(T)
#elif COMPILER_MSVC
# define AlignOf(T) (Size)__alignof(T)
#elif defined(__clang__)
# define AlignOf(T) (Size)__alignof__(T)
#elif COMPILER_GCC
# define AlignOf(T) (Size)__alignof__(T)
#else
# define AlignOf(T) ((Size)&((struct { S8 c; T d; } *)0)->d)
// NOTE(phong):            |                          |
//                         |          +---------------+
//                  |      |          |      The struct in memory:                      
//  example T = S32 |      |          V    +-----+-----+-----+-----+
//  the offset addr |      |     nullptr ->|  c  |     |     |     |
//  is align value  |      |               +-----+-----+-----+-----+
//                  |      +----> offset ->|  d  |  d  |  d  |  d  |
//                  |                      +-----+-----+-----+-----+
#endif

// NOTE(phong): bytes of padding needed to align ptr x to a power-of-two alignment b
#define AlignPadPow2(x, b) (-(UPtr)(x) & ((b) - 1))

////////////////////////////////////////////////////////////////////////////////
// NOTE(phong): Global Defaults

#define ARENA_MEMORY_REGION_DEFAULT_CAPACITY KB(64)

////////////////////////////////////////////////////////////////////////////////
// NOTE(phong): Memory Region

typedef struct MemoryRegion MemoryRegion;
struct MemoryRegion 
{
    MemoryRegion *next;
    Size capacity;
    char data[];
};

// NOTE(Phong): Memory region creaction/destruction

MemoryRegion *memory_region_create(Size capacity);
void memory_region_free(MemoryRegion *r);

////////////////////////////////////////////////////////////////////////////////
// NOTE(phong): Arena / Bump / Linear allocator
// Arenas are optimized for _small_, frequent, short lived allocations.

typedef struct Arena Arena;
struct Arena
{
    MemoryRegion *begin, *end;
    Size count; //curr off count
};

typedef struct ArenaMarker ArenaMarker;
struct ArenaMarker
{
    MemoryRegion *end;
    Size count; 
};

// NOTE(phong): arena creation/destruction
void arena_init_size(Arena *arena, Size size);
void arena_init(Arena *arena);
void arena_free(Arena *arena);

// NOTE(phong): arena push/reset/trim
void* arena_push(Arena *arena, Size size, Size align);
void arena_reset(Arena *arena);
void arena_trim(Arena *arena);

// NOTE(phong): arena snapshot/rewind
void arena_snapshot(Arena *arena, ArenaMarker *ret);
void arena_rewind(Arena *arena, ArenaMarker *mark);

// NOTE(phong): arena macros
#define arena_alloc(a, T, n) arena_push((a), sizeof(T)*(n), AlignOf(T))


////////////////////////////////////////////////////////////////////////////////
// NOTE(phong): Pool allocator (Arena + FreeList)

// typedef struct Pool Pool;
// struct Pool
// {
//     /* data */
// };



#endif //BASE_MEMORY_H