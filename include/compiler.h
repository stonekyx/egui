#ifndef COMPILER_H

#define COMPILER_H

#if defined(__GNUC__) && (__GNUC__ >= 3)

    #define likely(x)   __builtin_expect(!!(x), 1)
    #define unlikely(x) __builtin_expect(!!(x), 0)
    #define __NORETURN  __attribute__((__noreturn__))
    #define __MALLOC    __attribute__((__malloc__))

#else

    #define likely(x)   (x)
    #define unlikely(x) (x)
    #define __NORETURN
    #define __MALLOC

#endif

#define NOT_USED(x) ((void)(x))

#endif
