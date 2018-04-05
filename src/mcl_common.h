#ifndef _MCL_COMMON_H_
#define _MCL_COMMON_H_

#include <math.h>
#include <stdio.h>
#include <string.h>

enum
{
    STATUS_NOT_USED = 0x00,
    STATUS_USING    = 0x01,
};

typedef uint32_t (*cb_hash)(const void*);
typedef int      (*cb_equal)(const void*, const void*);

static inline int likely(int expr)
{
#ifdef __GNUC__
    return __builtin_expect(expr, 1);
#else
    return expr;
#endif
}

static inline int unlikely(int expr)
{
#ifdef __GNUC__
    return __builtin_expect(expr, 0);
#else
    return expr;
#endif
}

#ifndef prefetch_r
#define prefetch_r(ptr) __builtin_prefetch(ptr, 0)
#endif

#ifndef prefetch_w
#define prefetch_w(ptr) __builtin_prefetch(ptr, 1)
#endif

#endif
