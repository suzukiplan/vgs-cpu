#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vgscpu.h"
#include "vgscpu_internal.h"

void* vgscpu_create_context()
{
    struct vgscpu_context* result;
    result = (struct vgscpu_context*)malloc(sizeof(struct vgscpu_context));
    if (NULL == result) return NULL;
    memset(result, 0, sizeof(struct vgscpu_context));
    return result;
}

void vgscpu_release_context(void* ctx)
{
    free(ctx);
}

void vgscpu_load_program(void* ctx, void* pg, size_t size)
{
    struct vgscpu_context* c = (struct vgscpu_context*)ctx;
    memcpy(c->p, pg, size);
}

int vgscpu_run(void* ctx)
{
    struct vgscpu_context* c = (struct vgscpu_context*)ctx;
    int loop_flag = 1;
    int ret = 0;
    unsigned short s;
    unsigned int i;
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
                c->r.p++;
                c->s[c->r.s] = (unsigned char)(c->r.a & 0xff);
                c->r.s++;
                break;
            case VGSCPU_OP_PUSH_A2:
                c->r.p++;
                s = (unsigned short)(c->r.a & 0xffff);
                memcpy(&c->s[c->r.s], &s, 2);
                c->r.s += 2;
                break;
            case VGSCPU_OP_PUSH_A4:
                c->r.p++;
                memcpy(&c->s[c->r.s], &c->r.a, 4);
                c->r.s += 4;
                break;
            case VGSCPU_OP_POP_A1:
                c->r.p++;
                c->r.s--;
                c->r.a = c->s[c->r.s];
                break;
            case VGSCPU_OP_POP_A2:
                c->r.p++;
                c->r.s -= 2;
                memcpy(&s, &c->s[c->r.s], 2);
                c->r.a = s;
                break;
            case VGSCPU_OP_POP_A4:
                c->r.p++;
                c->r.s -= 4;
                memcpy(&i, &c->s[c->r.s], 4);
                c->r.a = i;
                break;
            case VGSCPU_OP_LD_A_1:
                c->r.p++;
                c->r.a = c->p[c->r.p];
                c->r.p++;
                break;
            case VGSCPU_OP_LD_A_2:
                c->r.p++;
                memcpy(&s, &c->p[c->r.p], 2);
                c->r.a = s;
                c->r.p += 2;
                break;
            case VGSCPU_OP_LD_A_4:
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.a = i;
                c->r.p += 4;
                break;
            case VGSCPU_OP_LD_A_B:
                c->r.p++;
                c->r.a = c->r.b;
                break;
            case VGSCPU_OP_LD_A_C:
                c->r.p++;
                c->r.a = c->r.c;
                break;
            case VGSCPU_OP_LD_A_D:
                c->r.p++;
                c->r.a = c->r.d;
                break;
            case VGSCPU_OP_LD_A_M1:
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                c->r.a = c->m[i];
                break;
            case VGSCPU_OP_LD_A_M2:
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                memcpy(&s, &c->m[i], 2);
                c->r.a = s;
                break;
            case VGSCPU_OP_LD_A_M4:
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                memcpy(&i, &c->m[i], 4);
                c->r.a = i;
                break;
            case VGSCPU_OP_ST_A_M1:
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                c->m[i] = (unsigned char)(c->r.a & 0xff);
                break;
            case VGSCPU_OP_ST_A_M2:
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                s = (unsigned short)(c->r.a & 0xffff);
                memcpy(&c->m[i], &s, 2);
                break;
            case VGSCPU_OP_ST_A_M4:
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                memcpy(&c->m[i], &c->r.a, 4);
                break;
            case VGSCPU_OP_INC_A:
                c->r.p++;
                c->r.a++;
                c->f.z = (0 == c->r.a) ? 1 : 0;
                break;
            case VGSCPU_OP_DEC_A:
                c->r.p++;
                c->r.a--;
                c->f.z = (0 == c->r.a) ? 1 : 0;
                break;
            case VGSCPU_OP_NOT_A:
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
                c->r.p++;
                c->s[c->r.s] = (unsigned char)(c->r.b & 0xff);
                c->r.s++;
                break;
            case VGSCPU_OP_PUSH_B2:
                c->r.p++;
                s = (unsigned short)(c->r.b & 0xffff);
                memcpy(&c->s[c->r.s], &s, 2);
                c->r.s += 2;
                break;
            case VGSCPU_OP_PUSH_B4:
                c->r.p++;
                memcpy(&c->s[c->r.s], &c->r.b, 4);
                c->r.s += 4;
                break;
            case VGSCPU_OP_POP_B1:
                c->r.p++;
                c->r.s--;
                c->r.b = c->s[c->r.s];
                break;
            case VGSCPU_OP_POP_B2:
                c->r.p++;
                c->r.s -= 2;
                memcpy(&s, &c->s[c->r.s], 2);
                c->r.b = s;
                break;
            case VGSCPU_OP_POP_B4:
                c->r.p++;
                c->r.s -= 4;
                memcpy(&i, &c->s[c->r.s], 4);
                c->r.b = i;
                break;
            case VGSCPU_OP_LD_B_1:
                c->r.p++;
                c->r.b = c->p[c->r.p];
                c->r.p++;
                break;
            case VGSCPU_OP_LD_B_2:
                c->r.p++;
                memcpy(&s, &c->p[c->r.p], 2);
                c->r.b = s;
                c->r.p += 2;
                break;
            case VGSCPU_OP_LD_B_4:
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.b = i;
                c->r.p += 4;
                break;
            case VGSCPU_OP_LD_B_A:
                c->r.p++;
                c->r.b = c->r.a;
                break;
            case VGSCPU_OP_LD_B_C:
                c->r.p++;
                c->r.b = c->r.c;
                break;
            case VGSCPU_OP_LD_B_D:
                c->r.p++;
                c->r.b = c->r.d;
                break;
            case VGSCPU_OP_LD_B_M1:
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                c->r.b = c->m[i];
                break;
            case VGSCPU_OP_LD_B_M2:
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                memcpy(&s, &c->m[i], 2);
                c->r.b = s;
                break;
            case VGSCPU_OP_LD_B_M4:
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                memcpy(&i, &c->m[i], 4);
                c->r.b = i;
                break;
            case VGSCPU_OP_ST_B_M1:
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                c->m[i] = (unsigned char)(c->r.b & 0xff);
                break;
            case VGSCPU_OP_ST_B_M2:
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                s = (unsigned short)(c->r.b & 0xffff);
                memcpy(&c->m[i], &s, 2);
                break;
            case VGSCPU_OP_ST_B_M4:
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                memcpy(&c->m[i], &c->r.b, 4);
                break;
            case VGSCPU_OP_INC_B:
                c->r.p++;
                c->r.b++;
                c->f.z = (0 == c->r.b) ? 1 : 0;
                break;
            case VGSCPU_OP_DEC_B:
                c->r.p++;
                c->r.b--;
                c->f.z = (0 == c->r.b) ? 1 : 0;
                break;
            case VGSCPU_OP_NOT_B:
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
                c->r.p++;
                c->s[c->r.s] = (unsigned char)(c->r.c & 0xff);
                c->r.s++;
                break;
            case VGSCPU_OP_PUSH_C2:
                c->r.p++;
                s = (unsigned short)(c->r.c & 0xffff);
                memcpy(&c->s[c->r.s], &s, 2);
                c->r.s += 2;
                break;
            case VGSCPU_OP_PUSH_C4:
                c->r.p++;
                memcpy(&c->s[c->r.s], &c->r.c, 4);
                c->r.s += 4;
                break;
            case VGSCPU_OP_POP_C1:
                c->r.p++;
                c->r.s--;
                c->r.c = c->s[c->r.s];
                break;
            case VGSCPU_OP_POP_C2:
                c->r.p++;
                c->r.s -= 2;
                memcpy(&s, &c->s[c->r.s], 2);
                c->r.c = s;
                break;
            case VGSCPU_OP_POP_C4:
                c->r.p++;
                c->r.s -= 4;
                memcpy(&i, &c->s[c->r.s], 4);
                c->r.c = i;
                break;
            case VGSCPU_OP_LD_C_1:
                c->r.p++;
                c->r.c = c->p[c->r.p];
                c->r.p++;
                break;
            case VGSCPU_OP_LD_C_2:
                c->r.p++;
                memcpy(&s, &c->p[c->r.p], 2);
                c->r.c = s;
                c->r.p += 2;
                break;
            case VGSCPU_OP_LD_C_4:
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.c = i;
                c->r.p += 4;
                break;
            case VGSCPU_OP_LD_C_A:
                c->r.p++;
                c->r.c = c->r.a;
                break;
            case VGSCPU_OP_LD_C_B:
                c->r.p++;
                c->r.c = c->r.b;
                break;
            case VGSCPU_OP_LD_C_D:
                c->r.p++;
                c->r.c = c->r.d;
                break;
            case VGSCPU_OP_LD_C_M1:
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                c->r.c = c->m[i];
                break;
            case VGSCPU_OP_LD_C_M2:
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                memcpy(&s, &c->m[i], 2);
                c->r.c = s;
                break;
            case VGSCPU_OP_LD_C_M4:
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                memcpy(&i, &c->m[i], 4);
                c->r.c = i;
                break;
            case VGSCPU_OP_ST_C_M1:
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                c->m[i] = (unsigned char)(c->r.c & 0xff);
                break;
            case VGSCPU_OP_ST_C_M2:
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                s = (unsigned short)(c->r.c & 0xffff);
                memcpy(&c->m[i], &s, 2);
                break;
            case VGSCPU_OP_ST_C_M4:
                c->r.p++;
                memcpy(&i, &c->p[c->r.p], 4);
                c->r.p += 4;
                memcpy(&c->m[i], &c->r.c, 4);
                break;
            case VGSCPU_OP_INC_C:
                c->r.p++;
                c->r.c++;
                c->f.z = (0 == c->r.c) ? 1 : 0;
                break;
            case VGSCPU_OP_DEC_C:
                c->r.p++;
                c->r.c--;
                c->f.z = (0 == c->r.c) ? 1 : 0;
                break;
            case VGSCPU_OP_NOT_C:
                c->r.p++;
                c->r.c = ~c->r.c;
                c->f.z = (0 == c->r.c) ? 1 : 0;
                break;
            default:
                sprintf(c->error, "UNKNOWN INSTRUCTION(%02X)", (int)c->p[c->r.p]);
                loop_flag = 0;
                ret = -1;
        }
    } while (loop_flag);
    return ret;
}
