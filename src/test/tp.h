#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vgscpu.h"
#include "vgscpu_internal.h"

#define TEST(F, L, OPERATION, EXPECT)                                                                                                        \
    {                                                                                                                                        \
        printf(".");                                                                                                                         \
        {                                                                                                                                    \
            int actual = OPERATION;                                                                                                          \
            if (actual != (int)EXPECT) {                                                                                                     \
                printf("test-failed: actual=%d, expected=%d, file=%s, line=%d, error=%s\n", actual, EXPECT, F, L, vgscpu_get_last_error(c)); \
                printf("c->r.a = %08X, c->r.b = %08X, c->r.c = %08X, c->r.d = %08X\n", c->r.a, c->r.b, c->r.c, c->r.d);                      \
                printf("c->r.p = %08X, c->r.s = %08X, c->f.z = %08X, c->f.q = %08X\n", c->r.p, c->r.s, c->f.z, c->f.z);                      \
                return -1;                                                                                                                   \
            }                                                                                                                                \
        }                                                                                                                                    \
    }
