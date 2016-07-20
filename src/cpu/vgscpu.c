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

#define ASSERT_IF_STACK_OVERFLOW(SIZE)       \
    if (c->sizeS <= c->r.s + SIZE) {         \
        sprintf(c->error, "STACK OVERFLOW"); \
        loop_flag = 0;                       \
        ret = -1;                            \
        break;                               \
    }

#define ASSERT_IF_STACK_UNDERFLOW(SIZE)       \
    if (c->r.s < SIZE) {                      \
        sprintf(c->error, "STACK UNDERFLOW"); \
        loop_flag = 0;                        \
        ret = -1;                             \
        break;                                \
    }

#define ASSERT_IF_OUT_OF_MAIN_MEMORY(PTR, SIZE)  \
    if (c->sizeM <= PTR) {                       \
        sprintf(c->error, "OUT OF MAIN MEMORY"); \
        loop_flag = 0;                           \
        ret = -1;                                \
        break;                                   \
    } else if (c->sizeM < PTR + SIZE) {          \
        sprintf(c->error, "OUT OF MAIN MEMORY"); \
        loop_flag = 0;                           \
        ret = -1;                                \
        break;                                   \
    }

#define ASSERT_IF_ZERO_DIVIDE(NUMBER)     \
    if (0 == NUMBER) {                    \
        sprintf(c->error, "ZERO DIVIDE"); \
        loop_flag = 0;                    \
        ret = -1;                         \
        break;                            \
    }

#define PUSH4(N)                      \
    {                                 \
        ASSERT_IF_STACK_OVERFLOW(4);  \
        unsigned int v = N;           \
        memcpy(&c->s[c->r.s], &v, 4); \
        c->r.s += 4;                  \
    }

void *vgscpu_create_context()
{
    return vgscpu_create_specific_context(VGSCPU_PROGRAM_SIZE_DEFAULT, VGSCPU_STACK_SIZE_DEFAULT, VGSCPU_MEMORY_SIZE_DEFAULT);
}

void *vgscpu_create_specific_context(unsigned int ps, unsigned int ss, unsigned int ms)
{
    struct vgscpu_context *result;
    result = (struct vgscpu_context *)malloc(sizeof(struct vgscpu_context));
    if (NULL == result) return NULL;
    memset(result, 0, sizeof(struct vgscpu_context));
    result->p = (unsigned char *)malloc(ps);
    result->sizeP = ps;
    result->s = (unsigned char *)malloc(ss);
    result->sizeS = ss;
    result->m = (unsigned char *)malloc(ms);
    result->sizeM = ms;
    if (!result->p || !result->s || !result->m) {
        vgscpu_release_context(result);
        return NULL;
    }
    return result;
}

void vgscpu_release_context(void *ctx)
{
    struct vgscpu_context *c = (struct vgscpu_context *)ctx;
    if (c) {
        if (c->p) free(c->p);
        if (c->s) free(c->s);
        if (c->m) free(c->m);
        memset(c, 0x42, sizeof(struct vgscpu_context));
        free(c);
    }
}

void vgscpu_regist_interrupt(void *ctx, unsigned char n, int (*interrupt)(struct vgscpu_context *))
{
    struct vgscpu_context *c = (struct vgscpu_context *)ctx;
    c->interrupt[n] = interrupt;
}

int vgscpu_load_program(void *ctx, void *pg, size_t size)
{
    struct vgscpu_context *c = (struct vgscpu_context *)ctx;
    if (c->sizeP < (unsigned int)size) {
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
    unsigned int *r;
    unsigned int a;
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
                PUSH4(c->r.a);
                c->r.p++;
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
                PUSH4(c->r.b);
                c->r.p++;
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
                PUSH4(c->r.c);
                c->r.p++;
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
                PUSH4(c->r.d);
                c->r.p++;
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
             * load/store from register specified address
             *----------------------------------------------------------------
             */
            case VGSCPU_OP_LD_A_RM:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                c->r.p++;
                switch (c->p[c->r.p++]) {
                    case 0x11: /* B1 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.b, 1);
                        c->r.a = c->m[c->r.b];
                        break;
                    case 0x12: /* B2 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.b, 2);
                        memcpy(&s, &c->m[c->r.b], 2);
                        c->r.a = s;
                        break;
                    case 0x14: /* B4 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.b, 4);
                        memcpy(&c->r.a, &c->m[c->r.b], 4);
                        break;
                    case 0x21: /* C1 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.c, 1);
                        c->r.a = c->m[c->r.c];
                        break;
                    case 0x22: /* C2 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.c, 2);
                        memcpy(&s, &c->m[c->r.c], 2);
                        c->r.a = s;
                        break;
                    case 0x24: /* C4 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.c, 4);
                        memcpy(&c->r.a, &c->m[c->r.c], 4);
                        break;
                    case 0x31: /* D1 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.d, 1);
                        c->r.a = c->m[c->r.d];
                        break;
                    case 0x32: /* D2 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.d, 2);
                        memcpy(&s, &c->m[c->r.d], 2);
                        c->r.a = s;
                        break;
                    case 0x34: /* D4 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.d, 4);
                        memcpy(&c->r.a, &c->m[c->r.d], 4);
                        break;
                    default:
                        sprintf(c->error, "INVALID ARGUMENT($%02X)", (int)c->p[c->r.p - 1]);
                        loop_flag = 0;
                        ret = -1;
                }
                break;
            case VGSCPU_OP_LD_B_RM:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                c->r.p++;
                switch (c->p[c->r.p++]) {
                    case 0x01: /* A1 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.a, 1);
                        c->r.b = c->m[c->r.a];
                        break;
                    case 0x02: /* A2 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.a, 2);
                        memcpy(&s, &c->m[c->r.a], 2);
                        c->r.b = s;
                        break;
                    case 0x04: /* A4 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.a, 4);
                        memcpy(&c->r.b, &c->m[c->r.a], 4);
                        break;
                    case 0x21: /* C1 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.c, 1);
                        c->r.b = c->m[c->r.c];
                        break;
                    case 0x22: /* C2 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.c, 2);
                        memcpy(&s, &c->m[c->r.c], 2);
                        c->r.b = s;
                        break;
                    case 0x24: /* C4 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.c, 4);
                        memcpy(&c->r.b, &c->m[c->r.c], 4);
                        break;
                    case 0x31: /* D1 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.d, 1);
                        c->r.b = c->m[c->r.d];
                        break;
                    case 0x32: /* D2 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.d, 2);
                        memcpy(&s, &c->m[c->r.d], 2);
                        c->r.b = s;
                        break;
                    case 0x34: /* D4 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.d, 4);
                        memcpy(&c->r.b, &c->m[c->r.d], 4);
                        break;
                    default:
                        sprintf(c->error, "INVALID ARGUMENT($%02X)", (int)c->p[c->r.p - 1]);
                        loop_flag = 0;
                        ret = -1;
                }
                break;
            case VGSCPU_OP_LD_C_RM:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                c->r.p++;
                switch (c->p[c->r.p++]) {
                    case 0x01: /* A1 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.a, 1);
                        c->r.c = c->m[c->r.a];
                        break;
                    case 0x02: /* A2 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.a, 2);
                        memcpy(&s, &c->m[c->r.a], 2);
                        c->r.c = s;
                        break;
                    case 0x04: /* A4 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.a, 4);
                        memcpy(&c->r.c, &c->m[c->r.a], 4);
                        break;
                    case 0x11: /* B1 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.b, 1);
                        c->r.c = c->m[c->r.b];
                        break;
                    case 0x12: /* B2 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.b, 2);
                        memcpy(&s, &c->m[c->r.b], 2);
                        c->r.c = s;
                        break;
                    case 0x14: /* B4 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.b, 4);
                        memcpy(&c->r.c, &c->m[c->r.b], 4);
                        break;
                    case 0x31: /* D1 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.d, 1);
                        c->r.c = c->m[c->r.d];
                        break;
                    case 0x32: /* D2 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.d, 2);
                        memcpy(&s, &c->m[c->r.d], 2);
                        c->r.c = s;
                        break;
                    case 0x34: /* D4 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.d, 4);
                        memcpy(&c->r.c, &c->m[c->r.d], 4);
                        break;
                    default:
                        sprintf(c->error, "INVALID ARGUMENT($%02X)", (int)c->p[c->r.p - 1]);
                        loop_flag = 0;
                        ret = -1;
                }
                break;
            case VGSCPU_OP_LD_D_RM:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                c->r.p++;
                switch (c->p[c->r.p++]) {
                    case 0x01: /* A1 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.a, 1);
                        c->r.d = c->m[c->r.a];
                        break;
                    case 0x02: /* A2 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.a, 2);
                        memcpy(&s, &c->m[c->r.a], 2);
                        c->r.d = s;
                        break;
                    case 0x04: /* A4 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.a, 4);
                        memcpy(&c->r.d, &c->m[c->r.a], 4);
                        break;
                    case 0x11: /* B1 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.b, 1);
                        c->r.d = c->m[c->r.b];
                        break;
                    case 0x12: /* B2 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.b, 2);
                        memcpy(&s, &c->m[c->r.b], 2);
                        c->r.d = s;
                        break;
                    case 0x14: /* B4 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.b, 4);
                        memcpy(&c->r.d, &c->m[c->r.b], 4);
                        break;
                    case 0x21: /* C1 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.c, 1);
                        c->r.d = c->m[c->r.c];
                        break;
                    case 0x22: /* C2 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.c, 2);
                        memcpy(&s, &c->m[c->r.c], 2);
                        c->r.d = s;
                        break;
                    case 0x24: /* C4 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.c, 4);
                        memcpy(&c->r.d, &c->m[c->r.c], 4);
                        break;
                    default:
                        sprintf(c->error, "INVALID ARGUMENT($%02X)", (int)c->p[c->r.p - 1]);
                        loop_flag = 0;
                        ret = -1;
                }
                break;
            case VGSCPU_OP_ST_A_RM:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                c->r.p++;
                switch (c->p[c->r.p++]) {
                    case 0x11: /* B1 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.b, 1);
                        c->m[c->r.b] = c->r.a & 0xff;
                        break;
                    case 0x12: /* B2 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.b, 2);
                        s = c->r.a & 0xffff;
                        memcpy(&c->m[c->r.b], &s, 2);
                        break;
                    case 0x14: /* B4 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.b, 4);
                        memcpy(&c->m[c->r.b], &c->r.a, 4);
                        break;
                    case 0x21: /* C1 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.c, 1);
                        c->m[c->r.c] = c->r.a & 0xff;
                        break;
                    case 0x22: /* C2 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.c, 2);
                        s = c->r.a & 0xffff;
                        memcpy(&c->m[c->r.c], &s, 2);
                        break;
                    case 0x24: /* C4 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.c, 4);
                        memcpy(&c->m[c->r.c], &c->r.a, 4);
                        break;
                    case 0x31: /* D1 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.d, 1);
                        c->m[c->r.d] = c->r.a & 0xff;
                        break;
                    case 0x32: /* D2 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.d, 2);
                        s = c->r.a & 0xffff;
                        memcpy(&c->m[c->r.d], &s, 2);
                        break;
                    case 0x44: /* D4 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.d, 4);
                        memcpy(&c->m[c->r.d], &c->r.a, 4);
                        break;
                    default:
                        sprintf(c->error, "INVALID ARGUMENT($%02X)", (int)c->p[c->r.p - 1]);
                        loop_flag = 0;
                        ret = -1;
                }
                break;
            case VGSCPU_OP_ST_B_RM:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                c->r.p++;
                switch (c->p[c->r.p++]) {
                    case 0x01: /* A1 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.a, 1);
                        c->m[c->r.a] = c->r.b & 0xff;
                        break;
                    case 0x02: /* A2 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.a, 2);
                        s = c->r.b & 0xffff;
                        memcpy(&c->m[c->r.a], &s, 2);
                        break;
                    case 0x04: /* A4 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.a, 4);
                        memcpy(&c->m[c->r.a], &c->r.b, 4);
                        break;
                    case 0x21: /* C1 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.c, 1);
                        c->m[c->r.c] = c->r.b & 0xff;
                        break;
                    case 0x22: /* C2 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.c, 2);
                        s = c->r.b & 0xffff;
                        memcpy(&c->m[c->r.c], &s, 2);
                        break;
                    case 0x24: /* C4 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.c, 4);
                        memcpy(&c->m[c->r.c], &c->r.b, 4);
                        break;
                    case 0x31: /* D1 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.d, 1);
                        c->m[c->r.d] = c->r.b & 0xff;
                        break;
                    case 0x32: /* D2 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.d, 2);
                        s = c->r.b & 0xffff;
                        memcpy(&c->m[c->r.d], &s, 2);
                        break;
                    case 0x44: /* D4 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.d, 4);
                        memcpy(&c->m[c->r.d], &c->r.b, 4);
                        break;
                    default:
                        sprintf(c->error, "INVALID ARGUMENT($%02X)", (int)c->p[c->r.p - 1]);
                        loop_flag = 0;
                        ret = -1;
                }
                break;
            case VGSCPU_OP_ST_C_RM:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                c->r.p++;
                switch (c->p[c->r.p++]) {
                    case 0x01: /* A1 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.a, 1);
                        c->m[c->r.a] = c->r.c & 0xff;
                        break;
                    case 0x02: /* A2 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.a, 2);
                        s = c->r.c & 0xffff;
                        memcpy(&c->m[c->r.a], &s, 2);
                        break;
                    case 0x04: /* A4 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.a, 4);
                        memcpy(&c->m[c->r.a], &c->r.c, 4);
                        break;
                    case 0x11: /* B1 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.b, 1);
                        c->m[c->r.b] = c->r.c & 0xff;
                        break;
                    case 0x12: /* B2 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.b, 2);
                        s = c->r.c & 0xffff;
                        memcpy(&c->m[c->r.b], &s, 2);
                        break;
                    case 0x14: /* B4 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.b, 4);
                        memcpy(&c->m[c->r.b], &c->r.c, 4);
                        break;
                    case 0x31: /* D1 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.d, 1);
                        c->m[c->r.d] = c->r.c & 0xff;
                        break;
                    case 0x32: /* D2 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.d, 2);
                        s = c->r.c & 0xffff;
                        memcpy(&c->m[c->r.d], &s, 2);
                        break;
                    case 0x44: /* D4 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.d, 4);
                        memcpy(&c->m[c->r.d], &c->r.c, 4);
                        break;
                    default:
                        sprintf(c->error, "INVALID ARGUMENT($%02X)", (int)c->p[c->r.p - 1]);
                        loop_flag = 0;
                        ret = -1;
                }
                break;
            case VGSCPU_OP_ST_D_RM:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                c->r.p++;
                switch (c->p[c->r.p++]) {
                    case 0x01: /* A1 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.a, 1);
                        c->m[c->r.a] = c->r.d & 0xff;
                        break;
                    case 0x02: /* A2 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.a, 2);
                        s = c->r.d & 0xffff;
                        memcpy(&c->m[c->r.a], &s, 2);
                        break;
                    case 0x04: /* A4 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.a, 4);
                        memcpy(&c->m[c->r.a], &c->r.d, 4);
                        break;
                    case 0x11: /* B1 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.b, 1);
                        c->m[c->r.b] = c->r.d & 0xff;
                        break;
                    case 0x12: /* B2 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.b, 2);
                        s = c->r.d & 0xffff;
                        memcpy(&c->m[c->r.b], &s, 2);
                        break;
                    case 0x14: /* B4 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.b, 4);
                        memcpy(&c->m[c->r.b], &c->r.d, 4);
                        break;
                    case 0x21: /* C1 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.c, 1);
                        c->m[c->r.c] = c->r.d & 0xff;
                        break;
                    case 0x22: /* C2 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.c, 2);
                        s = c->r.d & 0xffff;
                        memcpy(&c->m[c->r.c], &s, 2);
                        break;
                    case 0x24: /* C4 */
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(c->r.c, 4);
                        memcpy(&c->m[c->r.c], &c->r.d, 4);
                        break;
                    default:
                        sprintf(c->error, "INVALID ARGUMENT($%02X)", (int)c->p[c->r.p - 1]);
                        loop_flag = 0;
                        ret = -1;
                }
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
                c->f.z = (0 == c->r.a) ? 1 : 0;
                break;
            case VGSCPU_OP_SHL_B:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                c->r.p++;
                b = c->p[c->r.p++];
                c->r.b <<= b;
                c->f.z = (0 == c->r.b) ? 1 : 0;
                break;
            case VGSCPU_OP_SHL_C:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                c->r.p++;
                b = c->p[c->r.p++];
                c->r.c <<= b;
                c->f.z = (0 == c->r.c) ? 1 : 0;
                break;
            case VGSCPU_OP_SHL_D:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                c->r.p++;
                b = c->p[c->r.p++];
                c->r.d <<= b;
                c->f.z = (0 == c->r.d) ? 1 : 0;
                break;
            case VGSCPU_OP_SHR_A:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                c->r.p++;
                b = c->p[c->r.p++];
                c->r.a >>= b;
                c->f.z = (0 == c->r.a) ? 1 : 0;
                break;
            case VGSCPU_OP_SHR_B:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                c->r.p++;
                b = c->p[c->r.p++];
                c->r.b >>= b;
                c->f.z = (0 == c->r.b) ? 1 : 0;
                break;
            case VGSCPU_OP_SHR_C:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                c->r.p++;
                b = c->p[c->r.p++];
                c->r.c >>= b;
                c->f.z = (0 == c->r.c) ? 1 : 0;
                break;
            case VGSCPU_OP_SHR_D:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                c->r.p++;
                b = c->p[c->r.p++];
                c->r.d >>= b;
                c->f.z = (0 == c->r.d) ? 1 : 0;
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
             * and A
             *----------------------------------------------------------------
             */
            case VGSCPU_OP_AND_A_1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                c->r.a &= c->p[++c->r.p];
                c->r.p++;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_AND_A_2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(3);
                memcpy(&s, &c->p[++c->r.p], 2);
                c->r.p += 2;
                c->r.a &= s;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_AND_A_4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                c->r.a &= i;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_AND_A_B:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.a &= c->r.b;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_AND_A_C:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.a &= c->r.c;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_AND_A_D:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.a &= c->r.d;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_AND_A_M1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 1);
                c->r.a &= c->m[i];
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_AND_A_M2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 2);
                memcpy(&s, &c->m[i], 2);
                c->r.a &= s;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_AND_A_M4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 4);
                memcpy(&i, &c->m[i], 4);
                c->r.a &= i;
                c->f.z = c->r.a ? 0 : 1;
                break;
            /*
             *----------------------------------------------------------------
             * or A
             *----------------------------------------------------------------
             */
            case VGSCPU_OP_OR_A_1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                c->r.a |= c->p[++c->r.p];
                c->r.p++;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_OR_A_2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(3);
                memcpy(&s, &c->p[++c->r.p], 2);
                c->r.p += 2;
                c->r.a |= s;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_OR_A_4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                c->r.a |= i;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_OR_A_B:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.a |= c->r.b;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_OR_A_C:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.a |= c->r.c;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_OR_A_D:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.a |= c->r.d;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_OR_A_M1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 1);
                c->r.a |= c->m[i];
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_OR_A_M2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 2);
                memcpy(&s, &c->m[i], 2);
                c->r.a |= s;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_OR_A_M4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 4);
                memcpy(&i, &c->m[i], 4);
                c->r.a |= i;
                c->f.z = c->r.a ? 0 : 1;
                break;
            /*
             *----------------------------------------------------------------
             * xor A
             *----------------------------------------------------------------
             */
            case VGSCPU_OP_XOR_A_1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                c->r.a ^= c->p[++c->r.p];
                c->r.p++;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_XOR_A_2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(3);
                memcpy(&s, &c->p[++c->r.p], 2);
                c->r.p += 2;
                c->r.a ^= s;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_XOR_A_4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                c->r.a ^= i;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_XOR_A_B:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.a ^= c->r.b;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_XOR_A_C:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.a ^= c->r.c;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_XOR_A_D:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                c->r.a ^= c->r.d;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_XOR_A_M1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 1);
                c->r.a ^= c->m[i];
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_XOR_A_M2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 2);
                memcpy(&s, &c->m[i], 2);
                c->r.a ^= s;
                c->f.z = c->r.a ? 0 : 1;
                break;
            case VGSCPU_OP_XOR_A_M4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 4);
                memcpy(&i, &c->m[i], 4);
                c->r.a ^= i;
                c->f.z = c->r.a ? 0 : 1;
                break;
            /*
             *----------------------------------------------------------------
             * cmp A
             *----------------------------------------------------------------
             */
            case VGSCPU_OP_CMP_A_1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                b = c->p[++c->r.p];
                c->r.p++;
                if (c->r.a < b) {
                    c->f.q = -1;
                } else if (c->r.a != b) {
                    c->f.q = 1;
                } else {
                    c->f.q = 0;
                }
                break;
            case VGSCPU_OP_CMP_A_2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(3);
                memcpy(&s, &c->p[++c->r.p], 2);
                c->r.p += 2;
                if (c->r.a < s) {
                    c->f.q = -1;
                } else if (c->r.a != s) {
                    c->f.q = 1;
                } else {
                    c->f.q = 0;
                }
                break;
            case VGSCPU_OP_CMP_A_4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                if (c->r.a < i) {
                    c->f.q = -1;
                } else if (c->r.a != i) {
                    c->f.q = 1;
                } else {
                    c->f.q = 0;
                }
                break;
            case VGSCPU_OP_CMP_A_B:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                if (c->r.a < c->r.b) {
                    c->f.q = -1;
                } else if (c->r.a != c->r.b) {
                    c->f.q = 1;
                } else {
                    c->f.q = 0;
                }
                break;
            case VGSCPU_OP_CMP_A_C:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                if (c->r.a < c->r.c) {
                    c->f.q = -1;
                } else if (c->r.a != c->r.c) {
                    c->f.q = 1;
                } else {
                    c->f.q = 0;
                }
                break;
            case VGSCPU_OP_CMP_A_D:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                if (c->r.a < c->r.d) {
                    c->f.q = -1;
                } else if (c->r.a != c->r.d) {
                    c->f.q = 1;
                } else {
                    c->f.q = 0;
                }
                break;
            case VGSCPU_OP_CMP_A_M1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 1);
                if (c->r.a < c->m[i]) {
                    c->f.q = -1;
                } else if (c->r.a != c->m[i]) {
                    c->f.q = 1;
                } else {
                    c->f.q = 0;
                }
                break;
            case VGSCPU_OP_CMP_A_M2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 2);
                memcpy(&s, &c->m[i], 2);
                if (c->r.a < s) {
                    c->f.q = -1;
                } else if (c->r.a != s) {
                    c->f.q = 1;
                } else {
                    c->f.q = 0;
                }
                break;
            case VGSCPU_OP_CMP_A_M4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 4);
                memcpy(&i, &c->m[i], 4);
                if (c->r.a < i) {
                    c->f.q = -1;
                } else if (c->r.a != i) {
                    c->f.q = 1;
                } else {
                    c->f.q = 0;
                }
                break;
            /*
             *----------------------------------------------------------------
             * cmp2 A
             *----------------------------------------------------------------
             */
            case VGSCPU_OP_CMP2_A_1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                b = c->p[++c->r.p];
                c->r.p++;
                if ((int)c->r.a < (char)b) {
                    c->f.q = -1;
                } else if ((int)c->r.a != (char)b) {
                    c->f.q = 1;
                } else {
                    c->f.q = 0;
                }
                break;
            case VGSCPU_OP_CMP2_A_2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(3);
                memcpy(&s, &c->p[++c->r.p], 2);
                c->r.p += 2;
                if ((int)c->r.a < (short)s) {
                    c->f.q = -1;
                } else if ((int)c->r.a != (short)s) {
                    c->f.q = 1;
                } else {
                    c->f.q = 0;
                }
                break;
            case VGSCPU_OP_CMP2_A_4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                if ((int)c->r.a < (int)i) {
                    c->f.q = -1;
                } else if (c->r.a != i) {
                    c->f.q = 1;
                } else {
                    c->f.q = 0;
                }
                break;
            case VGSCPU_OP_CMP2_A_B:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                if ((int)c->r.a < (int)c->r.b) {
                    c->f.q = -1;
                } else if (c->r.a != c->r.b) {
                    c->f.q = 1;
                } else {
                    c->f.q = 0;
                }
                break;
            case VGSCPU_OP_CMP2_A_C:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                if ((int)c->r.a < (int)c->r.c) {
                    c->f.q = -1;
                } else if (c->r.a != c->r.c) {
                    c->f.q = 1;
                } else {
                    c->f.q = 0;
                }
                break;
            case VGSCPU_OP_CMP2_A_D:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                c->r.p++;
                if ((int)c->r.a < (int)c->r.d) {
                    c->f.q = -1;
                } else if (c->r.a != c->r.d) {
                    c->f.q = 1;
                } else {
                    c->f.q = 0;
                }
                break;
            case VGSCPU_OP_CMP2_A_M1:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 1);
                if ((int)c->r.a < (char)c->m[i]) {
                    c->f.q = -1;
                } else if ((int)c->r.a != (char)c->m[i]) {
                    c->f.q = 1;
                } else {
                    c->f.q = 0;
                }
                break;
            case VGSCPU_OP_CMP2_A_M2:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 2);
                memcpy(&s, &c->m[i], 2);
                if ((int)c->r.a < (short)s) {
                    c->f.q = -1;
                } else if ((int)c->r.a != (short)s) {
                    c->f.q = 1;
                } else {
                    c->f.q = 0;
                }
                break;
            case VGSCPU_OP_CMP2_A_M4:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                memcpy(&i, &c->p[++c->r.p], 4);
                c->r.p += 4;
                ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 4);
                memcpy(&i, &c->m[i], 4);
                if ((int)c->r.a < (int)i) {
                    c->f.q = -1;
                } else if (c->r.a != i) {
                    c->f.q = 1;
                } else {
                    c->f.q = 0;
                }
                break;
            /*
             *----------------------------------------------------------------
             * accumulator of B, C, D
             *----------------------------------------------------------------
             */
            case VGSCPU_OP_ACU_B:
            case VGSCPU_OP_ACU_C:
            case VGSCPU_OP_ACU_D:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                switch (c->p[c->r.p]) {
                    case VGSCPU_OP_ACU_B:
                        r = &c->r.b;
                        a = VGSCPU_OP_ACU_B;
                        break;
                    case VGSCPU_OP_ACU_C:
                        r = &c->r.c;
                        a = VGSCPU_OP_ACU_C;
                        break;
                    case VGSCPU_OP_ACU_D:
                        r = &c->r.d;
                        a = VGSCPU_OP_ACU_D;
                        break;
                }
                switch (c->p[++c->r.p]) {
                    /*
                     *----------------------------------------------------------------
                     * add B, C, D
                     *----------------------------------------------------------------
                     */
                    case VGSCPU_OP_ADD_B_1:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                        (*r) += c->p[++c->r.p];
                        c->r.p++;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_ADD_B_2:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(3);
                        memcpy(&s, &c->p[++c->r.p], 2);
                        c->r.p += 2;
                        (*r) += s;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_ADD_B_4:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        (*r) += i;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_ADD_B_A:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                        c->r.p++;
                        (*r) += c->r.a;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_ADD_B_C:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                        c->r.p++;
                        if (VGSCPU_OP_ACU_B == a) {
                            (*r) += c->r.c;
                        } else {
                            (*r) += c->r.b;
                        }
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_ADD_B_D:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                        c->r.p++;
                        if (VGSCPU_OP_ACU_D == a) {
                            (*r) += c->r.c;
                        } else {
                            (*r) += c->r.d;
                        }
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_ADD_B_M1:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 1);
                        (*r) += c->m[i];
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_ADD_B_M2:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 2);
                        memcpy(&s, &c->m[i], 2);
                        (*r) += s;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_ADD_B_M4:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 4);
                        memcpy(&i, &c->m[i], 4);
                        (*r) += i;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    /*
                     *----------------------------------------------------------------
                     * sub B, C, D
                     *----------------------------------------------------------------
                     */
                    case VGSCPU_OP_SUB_B_1:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                        (*r) -= c->p[++c->r.p];
                        c->r.p++;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_SUB_B_2:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(3);
                        memcpy(&s, &c->p[++c->r.p], 2);
                        c->r.p += 2;
                        (*r) -= s;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_SUB_B_4:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        (*r) -= i;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_SUB_B_A:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                        c->r.p++;
                        (*r) -= c->r.a;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_SUB_B_C:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                        c->r.p++;
                        if (VGSCPU_OP_ACU_B == a) {
                            (*r) -= c->r.c;
                        } else {
                            (*r) -= c->r.b;
                        }
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_SUB_B_D:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                        c->r.p++;
                        if (VGSCPU_OP_ACU_D == a) {
                            (*r) -= c->r.c;
                        } else {
                            (*r) -= c->r.d;
                        }
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_SUB_B_M1:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 1);
                        (*r) -= c->m[i];
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_SUB_B_M2:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 2);
                        memcpy(&s, &c->m[i], 2);
                        (*r) -= s;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_SUB_B_M4:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 4);
                        memcpy(&i, &c->m[i], 4);
                        (*r) -= i;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    /*
                     *----------------------------------------------------------------
                     * mul B, C, D
                     *----------------------------------------------------------------
                     */
                    case VGSCPU_OP_MUL_B_1:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                        (*r) *= c->p[++c->r.p];
                        c->r.p++;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_MUL_B_2:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(3);
                        memcpy(&s, &c->p[++c->r.p], 2);
                        c->r.p += 2;
                        (*r) *= s;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_MUL_B_4:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        (*r) *= i;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_MUL_B_A:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                        c->r.p++;
                        (*r) *= c->r.a;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_MUL_B_C:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                        c->r.p++;
                        if (VGSCPU_OP_ACU_B == a) {
                            (*r) *= c->r.c;
                        } else {
                            (*r) *= c->r.b;
                        }
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_MUL_B_D:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                        c->r.p++;
                        if (VGSCPU_OP_ACU_D == a) {
                            (*r) *= c->r.c;
                        } else {
                            (*r) *= c->r.d;
                        }
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_MUL_B_M1:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 1);
                        (*r) *= c->m[i];
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_MUL_B_M2:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 2);
                        memcpy(&s, &c->m[i], 2);
                        (*r) *= s;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_MUL_B_M4:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 4);
                        memcpy(&i, &c->m[i], 4);
                        (*r) *= i;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    /*
                     *----------------------------------------------------------------
                     * div B, C, D
                     *----------------------------------------------------------------
                     */
                    case VGSCPU_OP_DIV_B_1:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                        ASSERT_IF_ZERO_DIVIDE(c->p[++c->r.p]);
                        (*r) /= c->p[c->r.p];
                        c->r.p++;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_DIV_B_2:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(3);
                        memcpy(&s, &c->p[++c->r.p], 2);
                        c->r.p += 2;
                        ASSERT_IF_ZERO_DIVIDE(s);
                        (*r) /= s;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_DIV_B_4:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        ASSERT_IF_ZERO_DIVIDE(i);
                        (*r) /= i;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_DIV_B_A:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                        c->r.p++;
                        ASSERT_IF_ZERO_DIVIDE(c->r.a);
                        (*r) /= c->r.a;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_DIV_B_C:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                        c->r.p++;
                        if (VGSCPU_OP_ACU_B == a) {
                            ASSERT_IF_ZERO_DIVIDE(c->r.c);
                            (*r) /= c->r.c;
                        } else {
                            ASSERT_IF_ZERO_DIVIDE(c->r.b);
                            (*r) /= c->r.b;
                        }
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_DIV_B_D:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                        c->r.p++;
                        if (VGSCPU_OP_ACU_D == a) {
                            ASSERT_IF_ZERO_DIVIDE(c->r.c);
                            (*r) /= c->r.c;
                        } else {
                            ASSERT_IF_ZERO_DIVIDE(c->r.d);
                            (*r) /= c->r.d;
                        }
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_DIV_B_M1:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 1);
                        ASSERT_IF_ZERO_DIVIDE(c->m[i]);
                        (*r) /= c->m[i];
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_DIV_B_M2:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 2);
                        memcpy(&s, &c->m[i], 2);
                        ASSERT_IF_ZERO_DIVIDE(s);
                        (*r) /= s;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_DIV_B_M4:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 4);
                        memcpy(&i, &c->m[i], 4);
                        ASSERT_IF_ZERO_DIVIDE(i);
                        (*r) /= i;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    /*
                     *----------------------------------------------------------------
                     * mod B, C, D
                     *----------------------------------------------------------------
                     */
                    case VGSCPU_OP_MOD_B_1:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                        ASSERT_IF_ZERO_DIVIDE(c->p[++c->r.p]);
                        (*r) %= c->p[c->r.p];
                        c->r.p++;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_MOD_B_2:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(3);
                        memcpy(&s, &c->p[++c->r.p], 2);
                        c->r.p += 2;
                        ASSERT_IF_ZERO_DIVIDE(s);
                        (*r) %= s;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_MOD_B_4:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        ASSERT_IF_ZERO_DIVIDE(i);
                        (*r) %= i;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_MOD_B_A:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                        c->r.p++;
                        ASSERT_IF_ZERO_DIVIDE((c->r.a));
                        (*r) %= c->r.a;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_MOD_B_C:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                        c->r.p++;
                        if (VGSCPU_OP_ACU_B == a) {
                            ASSERT_IF_ZERO_DIVIDE(c->r.c);
                            (*r) %= c->r.c;
                        } else {
                            ASSERT_IF_ZERO_DIVIDE(c->r.b);
                            (*r) %= c->r.b;
                        }
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_MOD_B_D:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                        c->r.p++;
                        if (VGSCPU_OP_ACU_D == a) {
                            ASSERT_IF_ZERO_DIVIDE(c->r.c);
                            (*r) %= c->r.c;
                        } else {
                            ASSERT_IF_ZERO_DIVIDE(c->r.d);
                            (*r) %= c->r.d;
                        }
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_MOD_B_M1:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 1);
                        ASSERT_IF_ZERO_DIVIDE(c->m[i]);
                        (*r) %= c->m[i];
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_MOD_B_M2:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 2);
                        memcpy(&s, &c->m[i], 2);
                        ASSERT_IF_ZERO_DIVIDE(s);
                        (*r) %= s;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_MOD_B_M4:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 4);
                        memcpy(&i, &c->m[i], 4);
                        ASSERT_IF_ZERO_DIVIDE(i);
                        (*r) %= i;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    /*
                     *----------------------------------------------------------------
                     * and B, C, D
                     *----------------------------------------------------------------
                     */
                    case VGSCPU_OP_AND_B_1:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                        (*r) &= c->p[++c->r.p];
                        c->r.p++;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_AND_B_2:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(3);
                        memcpy(&s, &c->p[++c->r.p], 2);
                        c->r.p += 2;
                        (*r) &= s;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_AND_B_4:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        (*r) &= i;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_AND_B_A:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                        c->r.p++;
                        (*r) &= c->r.a;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_AND_B_C:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                        c->r.p++;
                        if (VGSCPU_OP_ACU_B == a) {
                            (*r) &= c->r.c;
                        } else {
                            (*r) &= c->r.b;
                        }
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_AND_B_D:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                        c->r.p++;
                        if (VGSCPU_OP_ACU_D == a) {
                            (*r) &= c->r.c;
                        } else {
                            (*r) &= c->r.d;
                        }
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_AND_B_M1:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 1);
                        (*r) &= c->m[i];
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_AND_B_M2:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 2);
                        memcpy(&s, &c->m[i], 2);
                        (*r) &= s;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_AND_B_M4:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 4);
                        memcpy(&i, &c->m[i], 4);
                        (*r) &= i;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    /*
                     *----------------------------------------------------------------
                     * or B, C, D
                     *----------------------------------------------------------------
                     */
                    case VGSCPU_OP_OR_B_1:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                        (*r) |= c->p[++c->r.p];
                        c->r.p++;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_OR_B_2:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(3);
                        memcpy(&s, &c->p[++c->r.p], 2);
                        c->r.p += 2;
                        (*r) |= s;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_OR_B_4:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        (*r) |= i;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_OR_B_A:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                        c->r.p++;
                        (*r) |= c->r.a;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_OR_B_C:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                        c->r.p++;
                        if (VGSCPU_OP_ACU_B == a) {
                            (*r) |= c->r.c;
                        } else {
                            (*r) |= c->r.b;
                        }
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_OR_B_D:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                        c->r.p++;
                        if (VGSCPU_OP_ACU_D == a) {
                            (*r) |= c->r.c;
                        } else {
                            (*r) |= c->r.d;
                        }
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_OR_B_M1:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 1);
                        (*r) |= c->m[i];
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_OR_B_M2:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 2);
                        memcpy(&s, &c->m[i], 2);
                        (*r) |= s;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_OR_B_M4:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 4);
                        memcpy(&i, &c->m[i], 4);
                        (*r) |= i;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    /*
                     *----------------------------------------------------------------
                     * xor B, C, D
                     *----------------------------------------------------------------
                     */
                    case VGSCPU_OP_XOR_B_1:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                        (*r) ^= c->p[++c->r.p];
                        c->r.p++;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_XOR_B_2:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(3);
                        memcpy(&s, &c->p[++c->r.p], 2);
                        c->r.p += 2;
                        (*r) ^= s;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_XOR_B_4:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        (*r) ^= i;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_XOR_B_A:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                        c->r.p++;
                        (*r) ^= c->r.a;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_XOR_B_C:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                        c->r.p++;
                        if (VGSCPU_OP_ACU_B == a) {
                            (*r) ^= c->r.c;
                        } else {
                            (*r) ^= c->r.b;
                        }
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_XOR_B_D:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                        c->r.p++;
                        if (VGSCPU_OP_ACU_D == a) {
                            (*r) ^= c->r.c;
                        } else {
                            (*r) ^= c->r.d;
                        }
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_XOR_B_M1:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 1);
                        (*r) ^= c->m[i];
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_XOR_B_M2:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 2);
                        memcpy(&s, &c->m[i], 2);
                        (*r) ^= s;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    case VGSCPU_OP_XOR_B_M4:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 4);
                        memcpy(&i, &c->m[i], 4);
                        (*r) ^= i;
                        c->f.z = (*r) ? 0 : 1;
                        break;
                    /*
                     *----------------------------------------------------------------
                     * cmp B, C, D
                     *----------------------------------------------------------------
                     */
                    case VGSCPU_OP_CMP_B_1:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                        b = c->p[++c->r.p];
                        c->r.p++;
                        if ((*r) < b) {
                            c->f.q = -1;
                        } else if ((*r) != b) {
                            c->f.q = 1;
                        } else {
                            c->f.q = 0;
                        }
                        break;
                    case VGSCPU_OP_CMP_B_2:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(3);
                        memcpy(&s, &c->p[++c->r.p], 2);
                        c->r.p += 2;
                        if ((*r) < s) {
                            c->f.q = -1;
                        } else if ((*r) != s) {
                            c->f.q = 1;
                        } else {
                            c->f.q = 0;
                        }
                        break;
                    case VGSCPU_OP_CMP_B_4:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        if ((*r) < i) {
                            c->f.q = -1;
                        } else if ((*r) != i) {
                            c->f.q = 1;
                        } else {
                            c->f.q = 0;
                        }
                        break;
                    case VGSCPU_OP_CMP_B_A:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                        c->r.p++;
                        if ((*r) < c->r.a) {
                            c->f.q = -1;
                        } else if ((*r) != c->r.a) {
                            c->f.q = 1;
                        } else {
                            c->f.q = 0;
                        }
                        break;
                    case VGSCPU_OP_CMP_B_C:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                        c->r.p++;
                        if (VGSCPU_OP_ACU_B == a) {
                            if ((*r) < c->r.c) {
                                c->f.q = -1;
                            } else if ((*r) != c->r.c) {
                                c->f.q = 1;
                            } else {
                                c->f.q = 0;
                            }
                        } else {
                            if ((*r) < c->r.b) {
                                c->f.q = -1;
                            } else if ((*r) != c->r.b) {
                                c->f.q = 1;
                            } else {
                                c->f.q = 0;
                            }
                        }
                        break;
                    case VGSCPU_OP_CMP_B_D:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                        c->r.p++;
                        if (VGSCPU_OP_ACU_D == a) {
                            if ((*r) < c->r.c) {
                                c->f.q = -1;
                            } else if ((*r) != c->r.c) {
                                c->f.q = 1;
                            } else {
                                c->f.q = 0;
                            }
                        } else {
                            if ((*r) < c->r.d) {
                                c->f.q = -1;
                            } else if ((*r) != c->r.d) {
                                c->f.q = 1;
                            } else {
                                c->f.q = 0;
                            }
                        }
                        break;
                    case VGSCPU_OP_CMP_B_M1:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 1);
                        if ((*r) < c->m[i]) {
                            c->f.q = -1;
                        } else if ((*r) != c->m[i]) {
                            c->f.q = 1;
                        } else {
                            c->f.q = 0;
                        }
                        break;
                    case VGSCPU_OP_CMP_B_M2:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 2);
                        memcpy(&s, &c->m[i], 2);
                        if ((*r) < s) {
                            c->f.q = -1;
                        } else if ((*r) != s) {
                            c->f.q = 1;
                        } else {
                            c->f.q = 0;
                        }
                        break;
                    case VGSCPU_OP_CMP_B_M4:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 4);
                        memcpy(&i, &c->m[i], 4);
                        if ((*r) < i) {
                            c->f.q = -1;
                        } else if ((*r) != i) {
                            c->f.q = 1;
                        } else {
                            c->f.q = 0;
                        }
                        break;
                    /*
                     *----------------------------------------------------------------
                     * cmp2 B, C, D
                     *----------------------------------------------------------------
                     */
                    case VGSCPU_OP_CMP2_B_1:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                        b = c->p[++c->r.p];
                        c->r.p++;
                        if ((int)(*r) < (char)b) {
                            c->f.q = -1;
                        } else if ((int)(*r) != (char)b) {
                            c->f.q = 1;
                        } else {
                            c->f.q = 0;
                        }
                        break;
                    case VGSCPU_OP_CMP2_B_2:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(3);
                        memcpy(&s, &c->p[++c->r.p], 2);
                        c->r.p += 2;
                        if ((int)(*r) < (short)s) {
                            c->f.q = -1;
                        } else if ((int)(*r) != (short)s) {
                            c->f.q = 1;
                        } else {
                            c->f.q = 0;
                        }
                        break;
                    case VGSCPU_OP_CMP2_B_4:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        if ((int)(*r) < (int)i) {
                            c->f.q = -1;
                        } else if ((*r) != i) {
                            c->f.q = 1;
                        } else {
                            c->f.q = 0;
                        }
                        break;
                    case VGSCPU_OP_CMP2_B_A:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                        c->r.p++;
                        if ((int)(*r) < (int)c->r.a) {
                            c->f.q = -1;
                        } else if ((*r) != c->r.a) {
                            c->f.q = 1;
                        } else {
                            c->f.q = 0;
                        }
                        break;
                    case VGSCPU_OP_CMP2_B_C:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                        c->r.p++;
                        if (VGSCPU_OP_ACU_B == a) {
                            if ((int)(*r) < (int)c->r.c) {
                                c->f.q = -1;
                            } else if ((*r) != c->r.c) {
                                c->f.q = 1;
                            } else {
                                c->f.q = 0;
                            }
                        } else {
                            if ((int)(*r) < (int)c->r.b) {
                                c->f.q = -1;
                            } else if ((*r) != c->r.b) {
                                c->f.q = 1;
                            } else {
                                c->f.q = 0;
                            }
                        }
                        break;
                    case VGSCPU_OP_CMP2_B_D:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(1);
                        c->r.p++;
                        if (VGSCPU_OP_ACU_D == a) {
                            if ((int)(*r) < (int)c->r.c) {
                                c->f.q = -1;
                            } else if ((*r) != c->r.c) {
                                c->f.q = 1;
                            } else {
                                c->f.q = 0;
                            }
                        } else {
                            if ((int)(*r) < (int)c->r.d) {
                                c->f.q = -1;
                            } else if ((*r) != c->r.d) {
                                c->f.q = 1;
                            } else {
                                c->f.q = 0;
                            }
                        }
                        break;
                    case VGSCPU_OP_CMP2_B_M1:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 1);
                        if ((int)(*r) < (char)c->m[i]) {
                            c->f.q = -1;
                        } else if ((int)(*r) != (char)c->m[i]) {
                            c->f.q = 1;
                        } else {
                            c->f.q = 0;
                        }
                        break;
                    case VGSCPU_OP_CMP2_B_M2:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 2);
                        memcpy(&s, &c->m[i], 2);
                        if ((int)(*r) < (short)s) {
                            c->f.q = -1;
                        } else if ((int)(*r) != (short)s) {
                            c->f.q = 1;
                        } else {
                            c->f.q = 0;
                        }
                        break;
                    case VGSCPU_OP_CMP2_B_M4:
                        ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                        memcpy(&i, &c->p[++c->r.p], 4);
                        c->r.p += 4;
                        ASSERT_IF_OUT_OF_MAIN_MEMORY(i, 4);
                        memcpy(&i, &c->m[i], 4);
                        if ((int)(*r) < (int)i) {
                            c->f.q = -1;
                        } else if ((*r) != i) {
                            c->f.q = 1;
                        } else {
                            c->f.q = 0;
                        }
                        break;
                    default:
                        sprintf(c->error, "UNKNOWN INSTRUCTION(%02X)", (int)c->p[c->r.p]);
                        loop_flag = 0;
                        ret = -1;
                }
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
            case VGSCPU_OP_CAL:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(5);
                PUSH4(c->r.p + 5);
                memcpy(&i, &c->p[c->r.p + 1], 4);
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY_SPECIFIC(i);
                c->r.p = i;
                break;
            case VGSCPU_OP_RET:
                ASSERT_IF_STACK_UNDERFLOW(4);
                c->r.s -= 4;
                memcpy(&i, &c->s[c->r.s], 4);
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY_SPECIFIC(i);
                c->r.p = i;
                break;
            case VGSCPU_OP_INT:
                ASSERT_IF_OUT_OF_PROGRAM_MEMORY(2);
                c->r.p++;
                if (!c->interrupt[c->p[c->r.p]]) {
                    sprintf(c->error, "INTERRUPT NOT REGISTERED: $%02X", (int)c->p[c->r.p]);
                    loop_flag = 0;
                    ret = -1;
                    break;
                }
                c->r.d = c->interrupt[c->p[c->r.p]](c);
                c->r.p++;
                break;
            default:
                sprintf(c->error, "UNKNOWN INSTRUCTION: $%02X", (int)c->p[c->r.p]);
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
