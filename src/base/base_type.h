#ifndef BASE_TYPES_H
#define BASE_TYPES_H

////////////////////////////////
// NOTE(phong): Standard Includes

#include <stddef.h> //size_t ptrdiff_t
#include <stdint.h>

////////////////////////////////
// NOTE(phong): Base Type

typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;
typedef int8_t    i8;
typedef int16_t   i16;
typedef int32_t   i32;
typedef int64_t   i64;
typedef float     f32;
typedef double    f64;
typedef size_t    uSize;
typedef ptrdiff_t iSize;

#endif //BASE_TYPES_H