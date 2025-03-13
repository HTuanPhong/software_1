#ifndef BASE_TYPES_H
#define BASE_TYPES_H

////////////////////////////////////////////////////////////////////////////////
// NOTE(phong): Standard Includes

#include <stddef.h> //size_t ptrdiff_t
#include <stdint.h> //int_t
#include <stdbool.h> //bool

////////////////////////////////////////////////////////////////////////////////
// NOTE(phong): Rename base Type

typedef uint8_t   U8;
typedef uint16_t  U16;
typedef uint32_t  U32;
typedef uint64_t  U64;
typedef int8_t    S8;
typedef int16_t   S16;
typedef int32_t   S32;
typedef int64_t   S64;
typedef float     F32;
typedef double    F64;
typedef size_t    Size;
typedef uintptr_t UPtr;
typedef _Bool     Bool;


#endif //BASE_TYPES_H