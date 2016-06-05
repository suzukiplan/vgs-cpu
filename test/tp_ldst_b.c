#include "tp.h"

int test_ld_b_literal(struct vgscpu_context *c)
{
    const char *TAG = "test_ld_b_literal";
    unsigned short s;
    unsigned int i;
    unsigned char op1[] = {VGSCPU_OP_LD_B_1, 0xCD, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_LD_B_2, 0xAB, 0xCD, VGSCPU_OP_BRK};
    unsigned char op3[] = {VGSCPU_OP_LD_B_4, 0x67, 0x89, 0xAB, 0xCD, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.b != 0x000000CD) FAILED(TAG, __LINE__);

    vgscpu_load_program(c, op2, sizeof(op2));
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.b != 0x0000CDAB) FAILED(TAG, __LINE__);

    vgscpu_load_program(c, op3, sizeof(op3));
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.b != 0xCDAB8967) FAILED(TAG, __LINE__);
    return 0;
}

int test_ld_b_registry(struct vgscpu_context *c)
{
    const char *TAG = "test_ld_b_registry";
    unsigned char op1[] = {VGSCPU_OP_LD_B_A, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_LD_B_C, VGSCPU_OP_BRK};
    unsigned char op3[] = {VGSCPU_OP_LD_B_D, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));
    c->r.a = 0x01020304;
    c->r.c = 0x05060708;
    c->r.d = 0x090a0b0c;
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.b != 0x01020304) FAILED(TAG, __LINE__);

    vgscpu_load_program(c, op2, sizeof(op2));
    c->r.a = 0x01020304;
    c->r.c = 0x05060708;
    c->r.d = 0x090a0b0c;
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.b != 0x05060708) FAILED(TAG, __LINE__);

    vgscpu_load_program(c, op3, sizeof(op3));
    c->r.a = 0x01020304;
    c->r.c = 0x05060708;
    c->r.d = 0x090a0b0c;
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.b != 0x090a0b0c) FAILED(TAG, __LINE__);
    return 0;
}

int test_ld_b_memory(struct vgscpu_context *c)
{
    const char *TAG = "test_ld_b_memory";
    const unsigned char m[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    unsigned char op1[] = {VGSCPU_OP_LD_B_M1, 0x08, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_LD_B_M2, 0x08, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op3[] = {VGSCPU_OP_LD_B_M4, 0x08, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));
    memcpy(&c->m, m, sizeof(m));
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.b != 0x00000008) FAILED(TAG, __LINE__);

    vgscpu_load_program(c, op2, sizeof(op2));
    memcpy(&c->m, m, sizeof(m));
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.b != 0x00000908) FAILED(TAG, __LINE__);

    vgscpu_load_program(c, op3, sizeof(op3));
    memcpy(&c->m, m, sizeof(m));
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->r.b != 0x0b0a0908) FAILED(TAG, __LINE__);
    return 0;
}

int test_st_b(struct vgscpu_context *c)
{
    const char *TAG = "test_st_b";
    const unsigned char m[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    unsigned char op1[] = {VGSCPU_OP_ST_B_M1, 0x08, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_ST_B_M2, 0x08, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op3[] = {VGSCPU_OP_ST_B_M4, 0x08, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));
    memcpy(&c->m, m, sizeof(m));
    c->r.b = 0x12345678;
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->m[0x8] != 0x78) FAILED(TAG, __LINE__);
    if (c->m[0x9] != 0x09) FAILED(TAG, __LINE__);
    if (c->m[0xa] != 0x0a) FAILED(TAG, __LINE__);
    if (c->m[0xb] != 0x0b) FAILED(TAG, __LINE__);

    vgscpu_load_program(c, op2, sizeof(op2));
    memcpy(&c->m, m, sizeof(m));
    c->r.b = 0x12345678;
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->m[0x8] != 0x78) FAILED(TAG, __LINE__);
    if (c->m[0x9] != 0x56) FAILED(TAG, __LINE__);
    if (c->m[0xa] != 0x0a) FAILED(TAG, __LINE__);
    if (c->m[0xb] != 0x0b) FAILED(TAG, __LINE__);

    vgscpu_load_program(c, op3, sizeof(op3));
    memcpy(&c->m, m, sizeof(m));
    c->r.b = 0x12345678;
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (c->m[0x8] != 0x78) FAILED(TAG, __LINE__);
    if (c->m[0x9] != 0x56) FAILED(TAG, __LINE__);
    if (c->m[0xa] != 0x34) FAILED(TAG, __LINE__);
    if (c->m[0xb] != 0x12) FAILED(TAG, __LINE__);
    return 0;
}

int main()
{
    struct vgscpu_context *c = (struct vgscpu_context *)vgscpu_create_context();
    if (!c) return -1;
    int result = -1;

    if (test_ld_b_literal(c)) goto END_TEST;
    if (test_ld_b_registry(c)) goto END_TEST;
    if (test_ld_b_memory(c)) goto END_TEST;
    if (test_st_b(c)) goto END_TEST;

    result = 0;
    puts("success");
END_TEST:
    vgscpu_release_context(c);
    return result;
}