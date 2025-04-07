#include "base_memory.h"
#include <assert.h> // NOTE(phong): make base_assertion

////////////////////////////////////////////////////////////////////////////////
// NOTE(phong): Memory Region functions

// NOTE(Phong): Memory region creaction/destruction

// TODO(Phong): add specific backends for each OS.

MemoryRegion *memory_region_create(Size capacity) 
{
    Size size_bytes = sizeof(MemoryRegion) + capacity;
    MemoryRegion *r = malloc(size_bytes);
    assert(r);
    r->next = NULL;
    r->capacity = capacity;
    return r;
}

void memory_region_free(MemoryRegion *r) 
{
    free(r);
}

////////////////////////////////////////////////////////////////////////////////
// NOTE(phong): Arena functions

// NOTE(phong): arena creation/destruction

void arena_init_size(Arena *arena, Size size) 
{
    arena->begin = arena->end = memory_region_create(size);
    arena->count = 0;
}

void arena_init(Arena *arena) 
{
    arena_init_size(arena, ARENA_MEMORY_REGION_DEFAULT_CAPACITY);
}

void arena_free(Arena *arena) 
{
    MemoryRegion *r = arena->begin;
    while (r)
    {
        MemoryRegion *prev = r;
        r = r->next;
        memory_region_free(prev);
    }
    arena->begin = NULL;
    arena->end = NULL;
    arena->count = 0;
}

// NOTE(phong): arena push/reset/trim

void* arena_push(Arena* arena, Size size, Size align) 
{
    assert(IsPow2(align));
    assert(size > 0);
    
    Size padding = AlignPadPow2(arena->end->data + arena->count, align);
    Size real_size = size + padding;

    if (arena->count + real_size > arena->end->capacity) 
    {
        MemoryRegion *chosen_region = NULL;
        for (MemoryRegion *prev_ptr = arena->end, *ptr = arena->end->next; 
            ptr != NULL; 
            prev_ptr = ptr, ptr = ptr->next)
        {
            padding = AlignPadPow2(ptr->data, align);
            real_size = size + padding;
            if (real_size <= ptr->capacity)
            {
                chosen_region = ptr;
                prev_ptr->next = ptr->next;
                ptr->next = arena->end->next;
                arena->end = ptr;
                break;
            }
        }
        
        if (!chosen_region)
        {
            Size capacity = ARENA_MEMORY_REGION_DEFAULT_CAPACITY;
            Size max_size = size + align;
            if (capacity < max_size) capacity = max_size;
            chosen_region = memory_region_create(capacity);
            chosen_region->next = arena->end->next;
            arena->end = chosen_region;
            padding = AlignPadPow2(chosen_region->data, align);
            real_size = size + padding;
        }

        arena->end = chosen_region;
        arena->count = 0;
    }
    
    void *result = arena->end->data + arena->count + padding;
    arena->count += real_size;
    return result;
}

void arena_reset(Arena *arena) 
{
    arena->end = arena->begin;
    arena->count = 0;
}

void arena_trim(Arena *arena)
{
    MemoryRegion *region = arena->end->next;
    while (region)
    {
        MemoryRegion *prev_region = region;
        region = region->next;
        memory_region_free(prev_region);
    }
    arena->end->next = NULL;
}

// NOTE(phong): arena snapshot/rewind

void arena_snapshot(Arena *arena, ArenaMarker *ret) {
    ret->end = arena->end;
    ret->count = arena->count;
}

void arena_rewind(Arena *arena, ArenaMarker *mark) {
    arena->end = mark->end;
    arena->count = mark->count;
}