#include <stdio.h>
#include <string.h>
#include "vgscpu.h"
#include "vgscpu_internal.h"
#define FAILED(F, L)                                         \
    {                                                        \
        printf("test-failed: function=%s, line=%d\n", F, L); \
        return -1;                                           \
    }

int test_jz(struct vgscpu_context *c)
{
    const char *TAG = "test_jz";
    unsigned char op1[] = {VGSCPU_OP_LD_A_1, 0x01, VGSCPU_OP_LD_B_1, 0x00, VGSCPU_OP_DEC_A, VGSCPU_OP_JZ, 0x0b, 0x00, 0x00, 0x00, VGSCPU_OP_INC_B, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_LD_A_1, 0x02, VGSCPU_OP_LD_B_1, 0x00, VGSCPU_OP_DEC_A, VGSCPU_OP_JZ, 0x0b, 0x00, 0x00, 0x00, VGSCPU_OP_INC_B, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));
    vgscpu_run(c);
    if (c->r.a != 0x00) FAILED(TAG, __LINE__);
    if (c->r.b != 0x00) FAILED(TAG, __LINE__);
    if (c->f.z != 0x01) FAILED(TAG, __LINE__);

    vgscpu_load_program(c, op2, sizeof(op2));
    vgscpu_run(c);
    if (c->r.a != 0x01) FAILED(TAG, __LINE__);
    if (c->r.b != 0x01) FAILED(TAG, __LINE__);
    if (c->f.z != 0x00) FAILED(TAG, __LINE__);
    return 0;
}

int test_jz_1(struct vgscpu_context *c)
{
    const char *TAG = "test_jz_1";
    unsigned char op1[] = {VGSCPU_OP_LD_A_1, 0x01, VGSCPU_OP_LD_B_1, 0x00, VGSCPU_OP_DEC_A, VGSCPU_OP_JZ_1, 0x01, VGSCPU_OP_INC_B, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_LD_A_1, 0x02, VGSCPU_OP_LD_B_1, 0x00, VGSCPU_OP_DEC_A, VGSCPU_OP_JZ_1, 0x01, VGSCPU_OP_INC_B, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));
    vgscpu_run(c);
    if (c->r.a != 0x00) FAILED(TAG, __LINE__);
    if (c->r.b != 0x00) FAILED(TAG, __LINE__);
    if (c->f.z != 0x01) FAILED(TAG, __LINE__);

    vgscpu_load_program(c, op2, sizeof(op2));
    vgscpu_run(c);
    if (c->r.a != 0x01) FAILED(TAG, __LINE__);
    if (c->r.b != 0x01) FAILED(TAG, __LINE__);
    if (c->f.z != 0x00) FAILED(TAG, __LINE__);
    return 0;
}

int test_jz_2(struct vgscpu_context *c)
{
    const char *TAG = "test_jz_2";
    unsigned char op1[] = {VGSCPU_OP_LD_A_1, 0x01, VGSCPU_OP_LD_B_1, 0x00, VGSCPU_OP_DEC_A, VGSCPU_OP_JZ_2, 0x01, 0x00, VGSCPU_OP_INC_B, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_LD_A_1, 0x02, VGSCPU_OP_LD_B_1, 0x00, VGSCPU_OP_DEC_A, VGSCPU_OP_JZ_2, 0x01, 0x00, VGSCPU_OP_INC_B, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));
    vgscpu_run(c);
    if (c->r.a != 0x00) FAILED(TAG, __LINE__);
    if (c->r.b != 0x00) FAILED(TAG, __LINE__);
    if (c->f.z != 0x01) FAILED(TAG, __LINE__);

    vgscpu_load_program(c, op2, sizeof(op2));
    vgscpu_run(c);
    if (c->r.a != 0x01) FAILED(TAG, __LINE__);
    if (c->r.b != 0x01) FAILED(TAG, __LINE__);
    if (c->f.z != 0x00) FAILED(TAG, __LINE__);
    return 0;
}

int main()
{
    struct vgscpu_context *c = (struct vgscpu_context *)vgscpu_create_context();
    if (!c) return -1;
    int result = -1;

    if (test_jz(c)) goto END_TEST;
    if (test_jz_1(c)) goto END_TEST;
    if (test_jz_2(c)) goto END_TEST;

    result = 0;
    puts("success");
END_TEST:
    vgscpu_release_context(c);
    return result;
}