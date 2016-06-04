#include <stdio.h>
#include <string.h>
#include "vgscpu.h"
#include "vgscpu_internal.h"
#define FAILED(F, L)                                         \
    {                                                        \
        printf("test-failed: function=%s, line=%d\n", F, L); \
        return -1;                                           \
    }

int test_dec_a(struct vgscpu_context* c)
{
    const char* TAG = "test_dec_a";
    unsigned char op[] = {VGSCPU_OP_DEC_A, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));
    vgscpu_run(c);
    if (c->r.a != -1) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);
    vgscpu_run(c);
    if (c->r.a != -2) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);
    vgscpu_run(c);
    if (c->r.a != -3) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);
    c->r.a = 1;
    vgscpu_run(c);
    if (c->r.a != 0) FAILED(TAG, __LINE__);
    if (c->f.z != 1) FAILED(TAG, __LINE__);
    vgscpu_run(c);
    if (c->r.a != -1) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);
    return 0;
}

int main()
{
    struct vgscpu_context* c = (struct vgscpu_context*)vgscpu_create_context();
    if (!c) return -1;
    int result = -1;

    if (test_dec_a(c)) goto END_TEST;

    result = 0;
    puts("success");
END_TEST:
    vgscpu_release_context(c);
    return result;
}