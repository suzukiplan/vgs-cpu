#include <stdio.h>
#include <string.h>
#include "vgscpu.h"
#include "vgscpu_internal.h"

#define TEST(F, L, OPERATION, EXPECT)                                                                    \
    {                                                                                                    \
        printf(".");                                                                                     \
        {                                                                                                \
            int actual = OPERATION;                                                                      \
            if (actual != EXPECT) {                                                                      \
                printf("test-failed: actual=%d, expected=%d, file=%s, line=%d\n", actual, EXPECT, F, L); \
                return -1;                                                                               \
            }                                                                                            \
        }                                                                                                \
    }
