#ifndef BASE_CONTEXT_CRACK_H
#define BASE_CONTEXT_CRACK_H

////////////////////////////////////////////////////////////////////////////////
// NOTE(phong): Compiler cracking

#if defined(__clang__)
# define COMPILER_CLANG 1
#elif defined(_MSC_VER)
# define COMPILER_MSVC 1
#elif defined(__GNUC__) || defined(__GNUG__)
# define COMPILER_GCC 1
#else
# error Compiler not supported.
#endif

////////////////////////////////////////////////////////////////////////////////
// NOTE(phong): Zero undefined

#if !defined(COMPILER_MSVC)
# define COMPILER_MSVC 0
#endif
#if !defined(COMPILER_GCC)
# define COMPILER_GCC 0
#endif
#if !defined(COMPILER_CLANG)
# define COMPILER_CLANG 0
#endif

#endif // BASE_CONTEXT_CRACK_H