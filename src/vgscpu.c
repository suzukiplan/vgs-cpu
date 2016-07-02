#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vgscpu.h"
#include "vgscpu_internal.h"

#define ASSERT_IF_OUT_OF_PROGRAM_MEMORY(SIZE)                             \
    if (c->psize <= c->r.p + SIZE) {                                      \
        sprintf(c->error, "OUT OF PROGRAM MEMORY: $%08X", c->r.p + SIZE); \
        loop_flag = 0;                                                    \
        ret = -1;                                                         \
        break;                                                            \
    }

#define ASSERT_IF_OUT_OF_PROGRAM_MEMORY_SPECIFIC(PTR)           \
    if (c->psize <= PTR) {                                      \
        sprintf(c->error, "OUT OF PROGRAM MEMORY: $%08X", PTR); \
        loop_flag = 0;                                          \
        ret = -1;                                               \
        break;                                                  \
    }

#define ASSERT_IF_STACK_OVERFLOW(SIZE)        \
    if (VGSCPU_STACK_SIZE <= c->r.s + SIZE) { \
        sprintf(c->error, "STACK OVERFLOW");  \
        loop_flag = 0;                        \
        ret = -1;                             \
        break;                                \
    }

#define ASSERT_IF_STACK_UNDERFLOW(SIZE)       \
    if (c->r.s < SIZE) {                      \
        sprintf(c->error, "STACK UNDERFLOW"); \
        loop_flag = 0;                        \
        ret = -1;                             \
        break;                                \
    }

#define ASSERT_IF_OUT_OF_MAIN_MEMORY(PTR, SIZE)   \
    if (VGSCPU_MEMORY_SIZE <= PTR) {              \
        sprintf(c->error, "OUT OF MAIN MEMORY");  \
        loop_flag = 0;                            \
        ret = -1;                                 \
        break;                                    \
    } else if (VGSCPU_MEMORY_SIZE < PTR + SIZE) { \
        sprintf(c->error, "OUT OF MAIN MEMORY");  \
        loop_flag = 0;                            \
        ret = -1;                                 \
        break;                                    \
    }

#define ASSERT_IF_ZERO_DIVIDE(NUMBER)     \
    if (0 == NUMBER) {                    \
        sprintf(c->error, "ZERO DIVIDE"); \
        loop_flag = 0;                    \
        ret = -1;                         \
        break;                            \
    }

void *vgscpu_create_context()
{
    struct vgscpu_context *result;
    result = (struct vgscpu_context *)malloc(sizeof(struct vgscpu_context));
    if (NULL == result) return NULL;
    memset(result, 0, sizeof(struct vgscpu_context));
    return result;
}

void vgscpu_release_context(void *ctx)
{
    free(ctx);
}

int vgscpu_load_program(void *ctx, void *pg, size_t size)
{
    struct vgscpu_context *c = (struct vgscpu_context *)ctx;
    if (VGSCPU_PROGRAM_SIZE < (unsigned int)size) {
        sprintf(c->error, "TOO BIG PROGRAM");
        return -1;
    }
    c->psize = (unsigned int)size;
    memcpy(c->p, pg, c->psize);
    return 0;
}

int vgscpu_run(void *ctx)
{
    struct vgscpu_context *c = (struct vgscpu_context *)ctx;
    int loop_flag = 1;
    int ret = 0;
    unsigned short s;
    unsigned int i;
    unsigned char b;
    c->r.p = 0;
    c->r.s = 0;
    do {
        switch (c->p[c->r.p]) {
            case VGSCPU_OP_BRK:
                loop_flag = 0;
                break;
            /*
             *----------------------------------------------------------------
             * stack and load/store (a)
             *----------------------------------------------------------------
             */
            case VGSCPU_OP_PUSH_A1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                ASSERT_IF_STACK_OVERFLOW(1);
                c->r.p++;
                c->s[c->r.s] = (unsigned char)(c->r.a & 0xff);
                c->r.s++;
                break;
            case VGSCPU_OP_PUSH_A2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                ASSERT_IF_STACK_OVERFLOW(2);
                c->r.p++;
                s = (unsigned short)(c->r.a & 0xffff);
                memcpy(&c->s[c->r.s], &s, 2);
                c->r.s += 2;
                break;
            case VGSCPU_OP_PUSH_A4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                ASSERT_IF_STACK_OVERFLOW(4);
                c->r.p++;
                memcpy(&c->s[c->r.s], &c->r.a, 4);
                c->r.s += 4;
                break;
            case VGSCPU_OP_POP_A1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                ASSERT_IF_STACK_UNDERFLOW(1);
                c->r.p++;
                c->r.s--;
                c->r.a = c->s[c->r.s];
                break;
            case VGSCPU_OP_POP_A2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                ASSERT_IF_STACK_UNDERFLOW(2);
                c->r.p++;
                c->r.s -= 2;
                memcpy(&s, &c->s[c->r.s], 2);
                c->r.a = s;
                break;
            case VGSCPU_OP_POP_A4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                ASSERT_IF_STACK_UNDERFLOW(4);
                c->r.p++;
                c->r.s -= 4;
                memcpy(&i, &c->s[c->r.s], 4);
                c->r.a = i;
                break;
            case VGSCPU_OP_LD_A_1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                c->r.p++;
                c->r.a = c->p[c->r.p];
                c->r.p++;
                break;
            case VGSCPU_OP_LD_A_2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(3);
                c->r.p++;
                memcpy(&s, &c->p[c->r.p], 2);
                c->r.a = s;
                c->r.p += 2;
                break;
            case VGSCPU_OP_LD_A_4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.a = i;
                c->r.p += 4;
                break;
            case VGSCPU_OP_LD_A_B:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.a = c->r.b;
                break;
            case VGSCPU_OP_LD_A_C:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.a = c->r.c;
                break;
            case VGSCPU_OP_LD_A_D:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.a = c->r.d;
                break;
            case VGSCPU_OP_LD_A_M1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 1);
                c->r.a = c->m[i];
                break;
            case VGSCPU_OP_LD_A_M2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 2);
                memcpy(&s, &c->m[i], 2);
                c->r.a = s;
                break;
            case VGSCPU_OP_LD_A_M4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 4);
                memcpy(&i, &c->m[i], 4);
                c->r.a = i;
                break;
            case VGSCPU_OP_ST_A_M1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 1);
                c->m[i] = (unsigned char)(c->r.a & 0xff);
                break;
            case VGSCPU_OP_ST_A_M2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                s = (unsigned short)(c->r.a & 0xffff);
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 2);
                memcpy(&c->m[i], &s, 2);
                break;
            case VGSCPU_OP_ST_A_M4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 4);
                memcpy(&c->m[i], &c->r.a, 4);
                break;
            case VGSCPU_OP_INC_A:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.a++;
                c->f.z = (0 == c->r.a) ? 1 : 0;
                break;
            case VGSCPU_OP_DEC_A:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.a--;
                c->f.z = (0 == c->r.a) ? 1 : 0;
                break;
            case VGSCPU_OP_NOT_A:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.a = ~c->r.a;
                c->f.z = (0 == c->r.a) ? 1 : 0;
                break;
            /*
             *----------------------------------------------------------------
             * stack and load/store (b)
             *----------------------------------------------------------------
             */
            case VGSCPU_OP_PUSH_B1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                ASSERT_IF_STACK_OVERFLOW(1);
                c->r.p++;
                c->s[c->r.s] = (unsigned char)(c->r.b & 0xff);
                c->r.s++;
                break;
            case VGSCPU_OP_PUSH_B2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                ASSERT_IF_STACK_OVERFLOW(2);
                c->r.p++;
                s = (unsigned short)(c->r.b & 0xffff);
                memcpy(&c->s[c->r.s], &s, 2);
                c->r.s += 2;
                break;
            case VGSCPU_OP_PUSH_B4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                ASSERT_IF_STACK_OVERFLOW(4);
                c->r.p++;
                memcpy(&c->s[c->r.s], &c->r.b, 4);
                c->r.s += 4;
                break;
            case VGSCPU_OP_POP_B1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                ASSERT_IF_STACK_UNDERFLOW(1);
                c->r.p++;
                c->r.s--;
                c->r.b = c->s[c->r.s];
                break;
            case VGSCPU_OP_POP_B2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                ASSERT_IF_STACK_UNDERFLOW(2);
                c->r.p++;
                c->r.s -= 2;
                memcpy(&s, &c->s[c->r.s], 2);
                c->r.b = s;
                break;
            case VGSCPU_OP_POP_B4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                ASSERT_IF_STACK_UNDERFLOW(4);
                c->r.p++;
                c->r.s -= 4;
                memcpy(&i, &c->s[c->r.s], 4);
                c->r.b = i;
                break;
            case VGSCPU_OP_LD_B_1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                c->r.p++;
                c->r.b = c->p[c->r.p];
                c->r.p++;
                break;
            case VGSCPU_OP_LD_B_2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(3);
                c->r.p++;
                memcpy(&s, &c->p[c->r.p], 2);
                c->r.b = s;
                c->r.p += 2;
                break;
            case VGSCPU_OP_LD_B_4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.b = i;
                c->r.p += 4;
                break;
            case VGSCPU_OP_LD_B_A:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.b = c->r.a;
                break;
            case VGSCPU_OP_LD_B_C:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.b = c->r.c;
                break;
            case VGSCPU_OP_LD_B_D:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.b = c->r.d;
                break;
            case VGSCPU_OP_LD_B_M1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 1);
                c->r.b = c->m[i];
                break;
            case VGSCPU_OP_LD_B_M2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 2);
                memcpy(&s, &c->m[i], 2);
                c->r.b = s;
                break;
            case VGSCPU_OP_LD_B_M4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 4);
                memcpy(&i, &c->m[i], 4);
                c->r.b = i;
                break;
            case VGSCPU_OP_ST_B_M1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 1);
                c->m[i] = (unsigned char)(c->r.b & 0xff);
                break;
            case VGSCPU_OP_ST_B_M2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                s = (unsigned short)(c->r.b & 0xffff);
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 2);
                memcpy(&c->m[i], &s, 2);
                break;
            case VGSCPU_OP_ST_B_M4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 4);
                memcpy(&c->m[i], &c->r.b, 4);
                break;
            case VGSCPU_OP_INC_B:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.b++;
                c->f.z = (0 == c->r.b) ? 1 : 0;
                break;
            case VGSCPU_OP_DEC_B:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.b--;
                c->f.z = (0 == c->r.b) ? 1 : 0;
                break;
            case VGSCPU_OP_NOT_B:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.b = ~c->r.b;
                c->f.z = (0 == c->r.b) ? 1 : 0;
                break;
            /*
             *----------------------------------------------------------------
             * stack and load/store (c)
             *----------------------------------------------------------------
             */
            case VGSCPU_OP_PUSH_C1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                ASSERT_IF_STACK_OVERFLOW(1);
                c->r.p++;
                c->s[c->r.s] = (unsigned char)(c->r.c & 0xff);
                c->r.s++;
                break;
            case VGSCPU_OP_PUSH_C2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                ASSERT_IF_STACK_OVERFLOW(2);
                c->r.p++;
                s = (unsigned short)(c->r.c & 0xffff);
                memcpy(&c->s[c->r.s], &s, 2);
                c->r.s += 2;
                break;
            case VGSCPU_OP_PUSH_C4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                ASSERT_IF_STACK_OVERFLOW(4);
                c->r.p++;
                memcpy(&c->s[c->r.s], &c->r.c, 4);
                c->r.s += 4;
                break;
            case VGSCPU_OP_POP_C1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                ASSERT_IF_STACK_UNDERFLOW(1);
                c->r.p++;
                c->r.s--;
                c->r.c = c->s[c->r.s];
                break;
            case VGSCPU_OP_POP_C2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                ASSERT_IF_STACK_UNDERFLOW(2);
                c->r.p++;
                c->r.s -= 2;
                memcpy(&s, &c->s[c->r.s], 2);
                c->r.c = s;
                break;
            case VGSCPU_OP_POP_C4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                ASSERT_IF_STACK_UNDERFLOW(4);
                c->r.p++;
                c->r.s -= 4;
                memcpy(&i, &c->s[c->r.s], 4);
                c->r.c = i;
                break;
            case VGSCPU_OP_LD_C_1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                c->r.p++;
                c->r.c = c->p[c->r.p];
                c->r.p++;
                break;
            case VGSCPU_OP_LD_C_2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(3);
                c->r.p++;
                memcpy(&s, &c->p[c->r.p], 2);
                c->r.c = s;
                c->r.p += 2;
                break;
            case VGSCPU_OP_LD_C_4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.c = i;
                c->r.p += 4;
                break;
            case VGSCPU_OP_LD_C_A:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.c = c->r.a;
                break;
            case VGSCPU_OP_LD_C_B:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.c = c->r.b;
                break;
            case VGSCPU_OP_LD_C_D:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.c = c->r.d;
                break;
            case VGSCPU_OP_LD_C_M1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 1);
                c->r.c = c->m[i];
                break;
            case VGSCPU_OP_LD_C_M2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 2);
                memcpy(&s, &c->m[i], 2);
                c->r.c = s;
                break;
            case VGSCPU_OP_LD_C_M4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 4);
                memcpy(&i, &c->m[i], 4);
                c->r.c = i;
                break;
            case VGSCPU_OP_ST_C_M1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 1);
                c->m[i] = (unsigned char)(c->r.c & 0xff);
                break;
            case VGSCPU_OP_ST_C_M2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                s = (unsigned short)(c->r.c & 0xffff);
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 2);
                memcpy(&c->m[i], &s, 2);
                break;
            case VGSCPU_OP_ST_C_M4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 4);
                memcpy(&c->m[i], &c->r.c, 4);
                break;
            case VGSCPU_OP_INC_C:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.c++;
                c->f.z = (0 == c->r.c) ? 1 : 0;
                break;
            case VGSCPU_OP_DEC_C:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.c--;
                c->f.z = (0 == c->r.c) ? 1 : 0;
                break;
            case VGSCPU_OP_NOT_C:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.c = ~c->r.c;
                c->f.z = (0 == c->r.c) ? 1 : 0;
                break;
            /*
             *----------------------------------------------------------------
             * stack and load/store (d)
             *----------------------------------------------------------------
             */
            case VGSCPU_OP_PUSH_D1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                ASSERT_IF_STACK_OVERFLOW(1);
                c->r.p++;
                c->s[c->r.s] = (unsigned char)(c->r.d & 0xff);
                c->r.s++;
                break;
            case VGSCPU_OP_PUSH_D2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                ASSERT_IF_STACK_OVERFLOW(2);
                c->r.p++;
                s = (unsigned short)(c->r.d & 0xffff);
                memcpy(&c->s[c->r.s], &s, 2);
                c->r.s += 2;
                break;
            case VGSCPU_OP_PUSH_D4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                ASSERT_IF_STACK_OVERFLOW(4);
                c->r.p++;
                memcpy(&c->s[c->r.s], &c->r.d, 4);
                c->r.s += 4;
                break;
            case VGSCPU_OP_POP_D1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                ASSERT_IF_STACK_UNDERFLOW(1);
                c->r.p++;
                c->r.s--;
                c->r.d = c->s[c->r.s];
                break;
            case VGSCPU_OP_POP_D2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                ASSERT_IF_STACK_UNDERFLOW(2);
                c->r.p++;
                c->r.s -= 2;
                memcpy(&s, &c->s[c->r.s], 2);
                c->r.d = s;
                break;
            case VGSCPU_OP_POP_D4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                ASSERT_IF_STACK_UNDERFLOW(4);
                c->r.p++;
                c->r.s -= 4;
                memcpy(&i, &c->s[c->r.s], 4);
                c->r.d = i;
                break;
            case VGSCPU_OP_LD_D_1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                c->r.p++;
                c->r.d = c->p[c->r.p];
                c->r.p++;
                break;
            case VGSCPU_OP_LD_D_2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(3);
                c->r.p++;
                memcpy(&s, &c->p[c->r.p], 2);
                c->r.d = s;
                c->r.p += 2;
                break;
            case VGSCPU_OP_LD_D_4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.d = i;
                c->r.p += 4;
                break;
            case VGSCPU_OP_LD_D_A:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.d = c->r.a;
                break;
            case VGSCPU_OP_LD_D_B:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.d = c->r.b;
                break;
            case VGSCPU_OP_LD_D_C:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.d = c->r.c;
                break;
            case VGSCPU_OP_LD_D_M1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 1);
                c->r.d = c->m[i];
                break;
            case VGSCPU_OP_LD_D_M2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 2);
                memcpy(&s, &c->m[i], 2);
                c->r.d = s;
                break;
            case VGSCPU_OP_LD_D_M4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 4);
                memcpy(&i, &c->m[i], 4);
                c->r.d = i;
                break;
            case VGSCPU_OP_ST_D_M1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 1);
                c->m[i] = (unsigned char)(c->r.d & 0xff);
                break;
            case VGSCPU_OP_ST_D_M2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                s = (unsigned short)(c->r.d & 0xffff);
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 2);
                memcpy(&c->m[i], &s, 2);
                break;
            case VGSCPU_OP_ST_D_M4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 4);
                memcpy(&c->m[i], &c->r.d, 4);
                break;
            case VGSCPU_OP_INC_D:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.d++;
                c->f.z = (0 == c->r.d) ? 1 : 0;
                break;
            case VGSCPU_OP_DEC_D:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.d--;
                c->f.z = (0 == c->r.d) ? 1 : 0;
                break;
            case VGSCPU_OP_NOT_D:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.d = ~c->r.d;
                c->f.z = (0 == c->r.d) ? 1 : 0;
                break;
            /*
             *----------------------------------------------------------------
             * shift
             *----------------------------------------------------------------
             */
            case VGSCPU_OP_SHL_A:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                c->r.p++;
                b = c->p[c->r.p++];
                c->r.a <<= b;
                break;
            case VGSCPU_OP_SHL_B:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                c->r.p++;
                b = c->p[c->r.p++];
                c->r.b <<= b;
                break;
            case VGSCPU_OP_SHL_C:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                c->r.p++;
                b = c->p[c->r.p++];
                c->r.c <<= b;
                break;
            case VGSCPU_OP_SHL_D:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                c->r.p++;
                b = c->p[c->r.p++];
                c->r.d <<= b;
                break;
            case VGSCPU_OP_SHR_A:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                c->r.p++;
                b = c->p[c->r.p++];
                c->r.a >>= b;
                break;
            case VGSCPU_OP_SHR_B:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                c->r.p++;
                b = c->p[c->r.p++];
                c->r.b >>= b;
                break;
            case VGSCPU_OP_SHR_C:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                c->r.p++;
                b = c->p[c->r.p++];
                c->r.c >>= b;
                break;
            case VGSCPU_OP_SHR_D:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                c->r.p++;
                b = c->p[c->r.p++];
                c->r.d >>= b;
                break;
            /*
             *----------------------------------------------------------------
             * add A
             *----------------------------------------------------------------
             */
            case VGSCPU_OP_ADD_A_1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                c->r.a += c->p[++c->r.p];
                c->r.p++;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_ADD_A_2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(3);
                memcpy(&s, &c->p[++c->r.p], 2);
                c->r.p += 2;
                c->r.a += s;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_ADD_A_4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                c->r.a += i;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_ADD_A_B:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.a += c->r.b;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_ADD_A_C:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.a += c->r.c;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_ADD_A_D:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.a += c->r.d;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_ADD_A_M1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 1);
                c->r.a += c->m[i];
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_ADD_A_M2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 2);
                memcpy(&s, &c->m[i], 2);
                c->r.a += s;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_ADD_A_M4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 4);
                memcpy(&i, &c->m[i], 4);
                c->r.a += i;
                c->f.z = c->r.a ? 0 : 1;
                break;
            /*
             *----------------------------------------------------------------
             * sub A
             *----------------------------------------------------------------
             */
            case VGSCPU_OP_SUB_A_1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                c->r.a -= c->p[++c->r.p];
                c->r.p++;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_SUB_A_2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(3);
                memcpy(&s, &c->p[++c->r.p], 2);
                c->r.p += 2;
                c->r.a -= s;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_SUB_A_4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                c->r.a -= i;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_SUB_A_B:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.a -= c->r.b;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_SUB_A_C:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.a -= c->r.c;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_SUB_A_D:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.a -= c->r.d;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_SUB_A_M1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 1);
                c->r.a -= c->m[i];
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_SUB_A_M2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 2);
                memcpy(&s, &c->m[i], 2);
                c->r.a -= s;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_SUB_A_M4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 4);
                memcpy(&i, &c->m[i], 4);
                c->r.a -= i;
                c->f.z = c->r.a ? 0 : 1;
                break;
            /*
             *----------------------------------------------------------------
             * mul A
             *----------------------------------------------------------------
             */
            case VGSCPU_OP_MUL_A_1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                c->r.a *= c->p[++c->r.p];
                c->r.p++;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_MUL_A_2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(3);
                memcpy(&s, &c->p[++c->r.p], 2);
                c->r.p += 2;
                c->r.a *= s;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_MUL_A_4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                c->r.a *= i;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_MUL_A_B:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.a *= c->r.b;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_MUL_A_C:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.a *= c->r.c;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_MUL_A_D:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.a *= c->r.d;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_MUL_A_M1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 1);
                c->r.a *= c->m[i];
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_MUL_A_M2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 2);
                memcpy(&s, &c->m[i], 2);
                c->r.a *= s;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_MUL_A_M4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 4);
                memcpy(&i, &c->m[i], 4);
                c->r.a *= i;
                c->f.z = c->r.a ? 0 : 1;
                break;
            /*
             *----------------------------------------------------------------
             * div A
             *----------------------------------------------------------------
             */
            case VGSCPU_OP_DIV_A_1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                ASSERT_IF_ZERO_DIVIDE(c->p[++c->r.p]);
                c->r.a /= c->p[c->r.p];
                c->r.p++;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_DIV_A_2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(3);
                memcpy(&s, &c->p[++c->r.p], 2);
                c->r.p += 2;
                ASSERT_IF_ZERO_DIVIDE(s);
                c->r.a /= s;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_DIV_A_4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_ZERO_DIVIDE(i);
                c->r.a /= i;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_DIV_A_B:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                ASSERT_IF_ZERO_DIVIDE(c->r.b);
                c->r.a /= c->r.b;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_DIV_A_C:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                ASSERT_IF_ZERO_DIVIDE(c->r.c);
                c->r.a /= c->r.c;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_DIV_A_D:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                ASSERT_IF_ZERO_DIVIDE(c->r.d);
                c->r.a /= c->r.d;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_DIV_A_M1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 1);
                ASSERT_IF_ZERO_DIVIDE(c->m[i]);
                c->r.a /= c->m[i];
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_DIV_A_M2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 2);
                memcpy(&s, &c->m[i], 2);
                ASSERT_IF_ZERO_DIVIDE(s);
                c->r.a /= s;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_DIV_A_M4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 4);
                memcpy(&i, &c->m[i], 4);
                ASSERT_IF_ZERO_DIVIDE(i);
                c->r.a /= i;
                c->f.z = c->r.a ? 0 : 1;
                break;
            /*
             *----------------------------------------------------------------
             * mod A
             *----------------------------------------------------------------
             */
            case VGSCPU_OP_MOD_A_1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                ASSERT_IF_ZERO_DIVIDE(c->p[++c->r.p]);
                c->r.a %= c->p[c->r.p];
                c->r.p++;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_MOD_A_2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(3);
                memcpy(&s, &c->p[++c->r.p], 2);
                c->r.p += 2;
                ASSERT_IF_ZERO_DIVIDE(s);
                c->r.a %= s;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_MOD_A_4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_ZERO_DIVIDE(i);
                c->r.a %= i;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_MOD_A_B:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                ASSERT_IF_ZERO_DIVIDE(c->r.b);
                c->r.a %= c->r.b;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_MOD_A_C:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                ASSERT_IF_ZERO_DIVIDE(c->r.c);
                c->r.a %= c->r.c;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_MOD_A_D:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                ASSERT_IF_ZERO_DIVIDE(c->r.d);
                c->r.a %= c->r.d;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_MOD_A_M1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 1);
                ASSERT_IF_ZERO_DIVIDE(c->m[i]);
                c->r.a %= c->m[i];
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_MOD_A_M2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 2);
                memcpy(&s, &c->m[i], 2);
                ASSERT_IF_ZERO_DIVIDE(s);
                c->r.a %= s;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_MOD_A_M4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 4);
                memcpy(&i, &c->m[i], 4);
                ASSERT_IF_ZERO_DIVIDE(i);
                c->r.a %= i;
                c->f.z = c->r.a ? 0 : 1;
                break;
            /*
             *----------------------------------------------------------------
             * branch (jump)
             *----------------------------------------------------------------
             */
            case VGSCPU_OP_JMP:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY_SPECIFIC(i);
                c->r.p = i;
                break;
            case VGSCPU_OP_JMP_1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                c->r.p++;
                i = c->r.p + c->p[c->r.p] + 1;
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY_SPECIFIC(i);
                c->r.p = i;
                break;
            case VGSCPU_OP_JMP_2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(3);
                c->r.p++;
                memcpy(&s, &c->p[c->r.p], 2);
                i = c->r.p + s + 2;
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY_SPECIFIC(i);
                c->r.p = i;
                break;
            case VGSCPU_OP_JZ:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                if (c->f.z) {
                    c->r.p++;
                    memcpy(&i, &c->p[c->r.p], 4);
                    ASSERT_IF_OUT_OF_PROGRAM_MEMORY_SPECIFIC(i);
                    c->r.p = i;
                } else {
                    c->r.p += 5;
                }
                break;
            case VGSCPU_OP_JZ_1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                if (c->f.z) {
                    c->r.p++;
                    i = c->r.p + c->p[c->r.p] + 1;
                    ASSERT_IF_OUT_OF_PROGRAM_MEMORY_SPECIFIC(i);
                    c->r.p = i;
                } else {
                    c->r.p += 2;
                }
                break;
            case VGSCPU_OP_JZ_2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(3);
                if (c->f.z) {
                    c->r.p++;
                    memcpy(&s, &c->p[c->r.p], 2);
                    i = c->r.p + s + 2;
                    ASSERT_IF_OUT_OF_PROGRAM_MEMORY_SPECIFIC(i);
                    c->r.p = i;
                } else {
                    c->r.p += 3;
                }
                break;
            case VGSCPU_OP_JNZ:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                if (!c->f.z) {
                    c->r.p++;
                    memcpy(&i, &c->p[c->r.p], 4);
                    ASSERT_IF_OUT_OF_PROGRAM_MEMORY_SPECIFIC(i);
                    c->r.p = i;
                } else {
                    c->r.p += 5;
                }
                break;
            case VGSCPU_OP_JNZ_1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                if (!c->f.z) {
                    c->r.p++;
                    i = c->r.p + c->p[c->r.p] + 1;
                    ASSERT_IF_OUT_OF_PROGRAM_MEMORY_SPECIFIC(i);
                    c->r.p = i;
                } else {
                    c->r.p += 2;
                }
                break;
            case VGSCPU_OP_JNZ_2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(3);
                if (!c->f.z) {
                    c->r.p++;
                    memcpy(&s, &c->p[c->r.p], 2);
                    i = c->r.p + s + 2;
                    ASSERT_IF_OUT_OF_PROGRAM_MEMORY_SPECIFIC(i);
                    c->r.p = i;
                } else {
                    c->r.p += 3;
                }
                break;
            case VGSCPU_OP_JE:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                if (0 == c->f.q) {
                    c->r.p++;
                    memcpy(&i, &c->p[c->r.p], 4);
                    ASSERT_IF_OUT_OF_PROGRAM_MEMORY_SPECIFIC(i);
                    c->r.p = i;
                } else {
                    c->r.p += 5;
                }
                break;
            case VGSCPU_OP_JE_1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                if (0 == c->f.q) {
                    c->r.p++;
                    i = c->r.p + c->p[c->r.p] + 1;
                    ASSERT_IF_OUT_OF_PROGRAM_MEMORY_SPECIFIC(i);
                    c->r.p = i;
                } else {
                    c->r.p += 2;
                }
                break;
            case VGSCPU_OP_JE_2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(3);
                if (0 == c->f.q) {
                    c->r.p++;
                    memcpy(&s, &c->p[c->r.p], 2);
                    i = c->r.p + s + 2;
                    ASSERT_IF_OUT_OF_PROGRAM_MEMORY_SPECIFIC(i);
                    c->r.p = i;
                } else {
                    c->r.p += 3;
                }
                break;
            case VGSCPU_OP_JNE:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                if (0 != c->f.q) {
                    c->r.p++;
                    memcpy(&i, &c->p[c->r.p], 4);
                    ASSERT_IF_OUT_OF_PROGRAM_MEMORY_SPECIFIC(i);
                    c->r.p = i;
                } else {
                    c->r.p += 5;
                }
                break;
            case VGSCPU_OP_JNE_1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                if (0 != c->f.q) {
                    c->r.p++;
                    i = c->r.p + c->p[c->r.p] + 1;
                    ASSERT_IF_OUT_OF_PROGRAM_MEMORY_SPECIFIC(i);
                    c->r.p = i;
                } else {
                    c->r.p += 2;
                }
                break;
            case VGSCPU_OP_JNE_2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(3);
                if (0 != c->f.q) {
                    c->r.p++;
                    memcpy(&s, &c->p[c->r.p], 2);
                    i = c->r.p + s + 2;
                    ASSERT_IF_OUT_OF_PROGRAM_MEMORY_SPECIFIC(i);
                    c->r.p = i;
                } else {
                    c->r.p += 3;
                }
                break;
            case VGSCPU_OP_JN:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                if (-1 == c->f.q) {
                    c->r.p++;
                    memcpy(&i, &c->p[c->r.p], 4);
                    ASSERT_IF_OUT_OF_PROGRAM_MEMORY_SPECIFIC(i);
                    c->r.p = i;
                } else {
                    c->r.p += 5;
                }
                break;
            case VGSCPU_OP_JN_1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                if (-1 == c->f.q) {
                    c->r.p++;
                    i = c->r.p + c->p[c->r.p] + 1;
                    ASSERT_IF_OUT_OF_PROGRAM_MEMORY_SPECIFIC(i);
                    c->r.p = i;
                } else {
                    c->r.p += 2;
                }
                break;
            case VGSCPU_OP_JN_2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(3);
                if (-1 == c->f.q) {
                    c->r.p++;
                    memcpy(&s, &c->p[c->r.p], 2);
                    i = c->r.p + s + 2;
                    ASSERT_IF_OUT_OF_PROGRAM_MEMORY_SPECIFIC(i);
                    c->r.p = i;
                } else {
                    c->r.p += 3;
                }
                break;
            case VGSCPU_OP_JNN:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                if (-1 != c->f.q) {
                    c->r.p++;
                    memcpy(&i, &c->p[c->r.p], 4);
                    ASSERT_IF_OUT_OF_PROGRAM_MEMORY_SPECIFIC(i);
                    c->r.p = i;
                } else {
                    c->r.p += 5;
                }
                break;
            case VGSCPU_OP_JNN_1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                if (-1 != c->f.q) {
                    c->r.p++;
                    i = c->r.p + c->p[c->r.p] + 1;
                    ASSERT_IF_OUT_OF_PROGRAM_MEMORY_SPECIFIC(i);
                    c->r.p = i;
                } else {
                    c->r.p += 2;
                }
                break;
            case VGSCPU_OP_JNN_2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(3);
                if (-1 != c->f.q) {
                    c->r.p++;
                    memcpy(&s, &c->p[c->r.p], 2);
                    i = c->r.p + s + 2;
                    ASSERT_IF_OUT_OF_PROGRAM_MEMORY_SPECIFIC(i);
                    c->r.p = i;
                } else {
                    c->r.p += 3;
                }
                break;
            case VGSCPU_OP_JP:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                if (1 == c->f.q) {
                    c->r.p++;
                    memcpy(&i, &c->p[c->r.p], 4);
                    ASSERT_IF_OUT_OF_PROGRAM_MEMORY_SPECIFIC(i);
                    c->r.p = i;
                } else {
                    c->r.p += 5;
                }
                break;
            case VGSCPU_OP_JP_1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                if (1 == c->f.q) {
                    c->r.p++;
                    i = c->r.p + c->p[c->r.p] + 1;
                    ASSERT_IF_OUT_OF_PROGRAM_MEMORY_SPECIFIC(i);
                    c->r.p = i;
                } else {
                    c->r.p += 2;
                }
                break;
            case VGSCPU_OP_JP_2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(3);
                if (1 == c->f.q) {
                    c->r.p++;
                    memcpy(&s, &c->p[c->r.p], 2);
                    i = c->r.p + s + 2;
                    ASSERT_IF_OUT_OF_PROGRAM_MEMORY_SPECIFIC(i);
                    c->r.p = i;
                } else {
                    c->r.p += 3;
                }
                break;
            case VGSCPU_OP_JNP:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                if (1 != c->f.q) {
                    c->r.p++;
                    memcpy(&i, &c->p[c->r.p], 4);
                    ASSERT_IF_OUT_OF_PROGRAM_MEMORY_SPECIFIC(i);
                    c->r.p = i;
                } else {
                    c->r.p += 5;
                }
                break;
            case VGSCPU_OP_JNP_1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                if (1 != c->f.q) {
                    c->r.p++;
                    i = c->r.p + c->p[c->r.p] + 1;
                    ASSERT_IF_OUT_OF_PROGRAM_MEMORY_SPECIFIC(i);
                    c->r.p = i;
                } else {
                    c->r.p += 2;
                }
                break;
            case VGSCPU_OP_JNP_2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(3);
                if (1 != c->f.q) {
                    c->r.p++;
                    memcpy(&s, &c->p[c->r.p], 2);
                    i = c->r.p + s + 2;
                    ASSERT_IF_OUT_OF_PROGRAM_MEMORY_SPECIFIC(i);
                    c->r.p = i;
                } else {
                    c->r.p += 3;
                }
                break;
            default:
                sprintf(c->error, "UNKNOWN INSTRUCTION(%02X)", (int)c->p[c->r.p]);
                loop_flag = 0;
                ret = -1;
        }
    } while (loop_flag);
    return ret;
}

const char *vgscpu_get_last_error(void *ctx)
{
    struct vgscpu_context *c = (struct vgscpu_context *)ctx;
    return c->error;
}
