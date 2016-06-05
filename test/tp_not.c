#include <stdio.h>
#include <string.h>
#include "vgscpu.h"
#include "vgscpu_internal.h"
#define FAILED(F, L)                                         \
    {                                                        \
        printf("test-failed: function=%s, line=%d\n", F, L); \
        return -1;                                           \
    }

int test_not_a(struct vgscpu_context *c)
{
    const char *TAG = "test_not_a";
    unsigned char op[] = {VGSCPU_OP_NOT_A, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));
    c->r.a = 0x0;
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.a != 0xffffffff) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);

    c->r.a = 0xffffffff;
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.a != 0) FAILED(TAG, __LINE__);
    if (c->f.z != 1) FAILED(TAG, __LINE__);

    c->r.a = 0xf0f0f0f0;
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.a != 0x0f0f0f0f) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);

    c->r.a = 0x0f0f0f0f;
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.a != 0xf0f0f0f0) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);

    c->r.a = 0x12345678;
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.a != 0xedcba987) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);

    c->r.a = 0xedcba987;
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.a != 0x12345678) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);
    return 0;
}

int test_not_b(struct vgscpu_context *c)
{
    const char *TAG = "test_not_b";
    unsigned char op[] = {VGSCPU_OP_NOT_B, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));
    c->r.b = 0x0;
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.b != 0xffffffff) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);

    c->r.b = 0xffffffff;
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.b != 0) FAILED(TAG, __LINE__);
    if (c->f.z != 1) FAILED(TAG, __LINE__);

    c->r.b = 0xf0f0f0f0;
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.b != 0x0f0f0f0f) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);

    c->r.b = 0x0f0f0f0f;
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.b != 0xf0f0f0f0) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);

    c->r.b = 0x12345678;
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.b != 0xedcba987) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);

    c->r.b = 0xedcba987;
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.b != 0x12345678) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);
    return 0;
}

int test_not_c(struct vgscpu_context *c)
{
    const char *TAG = "test_not_c";
    unsigned char op[] = {VGSCPU_OP_NOT_C, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));
    c->r.c = 0x0;
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.c != 0xffffffff) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);

    c->r.c = 0xffffffff;
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.c != 0) FAILED(TAG, __LINE__);
    if (c->f.z != 1) FAILED(TAG, __LINE__);

    c->r.c = 0xf0f0f0f0;
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.c != 0x0f0f0f0f) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);

    c->r.c = 0x0f0f0f0f;
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.c != 0xf0f0f0f0) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);

    c->r.c = 0x12345678;
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.c != 0xedcba987) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);

    c->r.c = 0xedcba987;
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.c != 0x12345678) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);
    return 0;
}

int test_not_d(struct vgscpu_context *c)
{
    const char *TAG = "test_not_d";
    unsigned char op[] = {VGSCPU_OP_NOT_D, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));
    c->r.d = 0x0;
    vgscpu_run(c);
    if (c->r.d != 0xffffffff) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);

    c->r.d = 0xffffffff;
    vgscpu_run(c);
    if (c->r.d != 0) FAILED(TAG, __LINE__);
    if (c->f.z != 1) FAILED(TAG, __LINE__);

    c->r.d = 0xf0f0f0f0;
    vgscpu_run(c);
    if (c->r.d != 0x0f0f0f0f) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);

    c->r.d = 0x0f0f0f0f;
    vgscpu_run(c);
    if (c->r.d != 0xf0f0f0f0) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);

    c->r.d = 0x12345678;
    vgscpu_run(c);
    if (c->r.d != 0xedcba987) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);

    c->r.d = 0xedcba987;
    vgscpu_run(c);
    if (c->r.d != 0x12345678) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);
    return 0;
}

int main()
{
    struct vgscpu_context *c = (struct vgscpu_context *)vgscpu_create_context();
    if (!c) return -1;
    int result = -1;

    if (test_not_a(c)) goto END_TEST;
    if (test_not_b(c)) goto END_TEST;
    if (test_not_c(c)) goto END_TEST;
    if (test_not_d(c)) goto END_TEST;

    result = 0;
    puts("success");
END_TEST:
    vgscpu_release_context(c);
    return result;
}