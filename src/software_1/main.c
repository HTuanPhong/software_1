#include "../base/base_include.h"

#include <stdio.h>
#include <xmmintrin.h>

int main() {
    Arena arena = {0};
    arena_init(&arena);

    void *yay = arena_push(&arena, sizeof(int)*10, AlignOf(int));

    (void)yay;

    arena_free(&arena);

    return 0;
}