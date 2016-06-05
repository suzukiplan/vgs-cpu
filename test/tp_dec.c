#include "tp.h"

int test_dec_a(struct vgscpu_context *c)
{
    const char *TAG = "test_dec_a";
    unsigned char op[] = {VGSCPU_OP_DEC_A, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.a != -1) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);

    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.a != -2) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);

    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.a != -3) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);

    c->r.a = 1;
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.a != 0) FAILED(TAG, __LINE__);
    if (c->f.z != 1) FAILED(TAG, __LINE__);

    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.a != -1) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);
    return 0;
}

int test_dec_b(struct vgscpu_context *c)
{
    const char *TAG = "test_dec_b";
    unsigned char op[] = {VGSCPU_OP_DEC_B, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));

    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.b != -1) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);

    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.b != -2) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);

    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.b != -3) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);

    c->r.b = 1;
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.b != 0) FAILED(TAG, __LINE__);
    if (c->f.z != 1) FAILED(TAG, __LINE__);

    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.b != -1) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);
    return 0;
}

int test_dec_c(struct vgscpu_context *c)
{
    const char *TAG = "test_dec_c";
    unsigned char op[] = {VGSCPU_OP_DEC_C, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));

    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.c != -1) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);

    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.c != -2) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);

    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.c != -3) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);

    c->r.c = 1;
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.c != 0) FAILED(TAG, __LINE__);
    if (c->f.z != 1) FAILED(TAG, __LINE__);

    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.c != -1) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);
    return 0;
}

int test_dec_d(struct vgscpu_context *c)
{
    const char *TAG = "test_dec_d";
    unsigned char op[] = {VGSCPU_OP_DEC_D, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));

    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.d != -1) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);

    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.d != -2) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);

    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.d != -3) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);

    c->r.d = 1;
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.d != 0) FAILED(TAG, __LINE__);
    if (c->f.z != 1) FAILED(TAG, __LINE__);

    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.d != -1) FAILED(TAG, __LINE__);
    if (c->f.z != 0) FAILED(TAG, __LINE__);
    return 0;
}

int main()
{
    struct vgscpu_context *c = (struct vgscpu_context *)vgscpu_create_context();
    if (!c) return -1;
    int result = -1;

    if (test_dec_a(c)) goto END_TEST;
    if (test_dec_b(c)) goto END_TEST;
    if (test_dec_c(c)) goto END_TEST;
    if (test_dec_d(c)) goto END_TEST;

    result = 0;
    puts("success");
END_TEST:
    vgscpu_release_context(c);
    return result;
}