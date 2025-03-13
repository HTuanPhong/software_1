#include "base_memory.h"
#include <assert.h> // NOTE(phong): make base_assertion

////////////////////////////////////////////////////////////////////////////////
// NOTE(phong): Memory Region functions

// NOTE(Phong): region creaction/destruction

Region *region_create(Size capacity) 
{
    Size size_bytes = sizeof(Region) + capacity;
    Region *r = malloc(size_bytes);
    assert(r);
    r->next = NULL;
    r->capacity = capacity;
    return r;
}

void region_free(Region *r) 
{
    free(r);
}

////////////////////////////////////////////////////////////////////////////////
// NOTE(phong): Arena functions

// NOTE(phong): arena creation/destruction

void arena_init_size(Arena *arena, Size size) 
{
    arena->begin = arena->end = region_create(size);
    arena->count = 0;
}

void arena_init(Arena *arena) 
{
    arena_init_size(arena, ARENA_REGION_DEFAULT_CAPACITY);
}

void arena_free(Arena *arena) 
{
    Region *r = arena->begin;
    while (r)
    {
        Region *prev = r;
        r = r->next;
        region_free(prev);
    }
    arena->begin = NULL;
    arena->end = NULL;
    arena->count = 0;
}

// NOTE(phong): arena push/reset

void* arena_push(Arena* arena, Size size, Size align) 
{
    assert(IsPow2(align));
    
    UPtr curr_ptr = (UPtr)arena->end->data + arena->count;
    Size padding = -curr_ptr & (align - 1);
    Size real_size = size + padding;

    if (arena->count + real_size > arena->end->capacity) 
    {
        Region *chosen_region = NULL;
        Region *prev_ptr = arena->end;
        Region *ptr = arena->end->next;
        curr_ptr = (UPtr)ptr->data;
        padding = -curr_ptr & (align - 1);
        real_size = size + padding;
        while (ptr) 
        {
            if (real_size <= ptr->capacity)
            {
                chosen_region = ptr;
                prev_ptr->next = ptr->next;
                ptr->next = arena->end->next;
                arena->end = ptr;
                break;
            }
            prev_ptr = ptr;
            ptr = ptr->next;
        }

        if (!chosen_region)
        {
            Size capacity = ARENA_REGION_DEFAULT_CAPACITY;
            if (capacity < real_size) capacity = real_size;
            chosen_region = region_create(capacity);
            chosen_region->next = arena->end->next;
            arena->end = chosen_region;
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

// NOTE(phong): arena snapshot/rewind

void arena_snapshot(Arena *arena, ArenaMarker *ret) {
    ret->end = arena->end;
    ret->count = arena->count;
}

void arena_rewind(Arena *arena, ArenaMarker *mark) {
    arena->end = mark->end;
    arena->count = arena->count;
}