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
            if (actual != EXPECT) {                                                                                                          \
                printf("test-failed: actual=%d, expected=%d, file=%s, line=%d, error=%s\n", actual, EXPECT, F, L, vgscpu_get_last_error(c)); \
                return -1;                                                                                                                   \
            }                                                                                                                                \
        }                                                                                                                                    \
    }
