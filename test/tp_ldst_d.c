#include "tp.h"

int test_program_memory(struct vgscpu_context *c)
{
    unsigned char op1[] = {VGSCPU_OP_LD_D_1, 0x00, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_LD_D_2, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op3[] = {VGSCPU_OP_LD_D_4, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op4[] = {VGSCPU_OP_LD_D_A, VGSCPU_OP_BRK};
    unsigned char op5[] = {VGSCPU_OP_LD_D_B, VGSCPU_OP_BRK};
    unsigned char op6[] = {VGSCPU_OP_LD_D_C, VGSCPU_OP_BRK};
    unsigned char op7[] = {VGSCPU_OP_ST_D_M1, 0x08, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op8[] = {VGSCPU_OP_ST_D_M2, 0x08, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op9[] = {VGSCPU_OP_ST_D_M4, 0x08, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    vgscpu_load_program(c, op1, sizeof(op1) - 1);
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000002"), 0);

    vgscpu_load_program(c, op2, sizeof(op2));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    vgscpu_load_program(c, op2, sizeof(op2) - 1);
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000003"), 0);

    vgscpu_load_program(c, op3, sizeof(op3));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    vgscpu_load_program(c, op3, sizeof(op3) - 1);
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000005"), 0);

    vgscpu_load_program(c, op4, sizeof(op4));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    vgscpu_load_program(c, op4, sizeof(op4) - 1);
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000001"), 0);

    vgscpu_load_program(c, op5, sizeof(op5));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    vgscpu_load_program(c, op5, sizeof(op5) - 1);
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000001"), 0);

    vgscpu_load_program(c, op6, sizeof(op6));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    vgscpu_load_program(c, op6, sizeof(op6) - 1);
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000001"), 0);

    vgscpu_load_program(c, op7, sizeof(op7));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    vgscpu_load_program(c, op7, sizeof(op7) - 1);
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000005"), 0);

    vgscpu_load_program(c, op8, sizeof(op8));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    vgscpu_load_program(c, op8, sizeof(op8) - 1);
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000005"), 0);

    vgscpu_load_program(c, op9, sizeof(op9));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    vgscpu_load_program(c, op9, sizeof(op9) - 1);
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000005"), 0);
    return 0;
}

int test_main_memory(struct vgscpu_context *c)
{
    unsigned int m = VGSCPU_MEMORY_SIZE;
    unsigned char op1[] = {VGSCPU_OP_LD_D_M1, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_LD_D_M2, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op3[] = {VGSCPU_OP_LD_D_M4, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op4[] = {VGSCPU_OP_ST_D_M1, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op5[] = {VGSCPU_OP_ST_D_M2, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op6[] = {VGSCPU_OP_ST_D_M4, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};

    m = VGSCPU_MEMORY_SIZE - 1;
    memcpy(&op1[1], &m, 4);
    vgscpu_load_program(c, op1, sizeof(op1));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    m = VGSCPU_MEMORY_SIZE;
    memcpy(&op1[1], &m, 4);
    vgscpu_load_program(c, op1, sizeof(op1));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF MAIN MEMORY"), 0);
    m = VGSCPU_MEMORY_SIZE + 1;
    memcpy(&op1[1], &m, 4);
    vgscpu_load_program(c, op1, sizeof(op1));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF MAIN MEMORY"), 0);

    m = VGSCPU_MEMORY_SIZE - 2;
    memcpy(&op2[1], &m, 4);
    vgscpu_load_program(c, op2, sizeof(op2));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    m = VGSCPU_MEMORY_SIZE - 1;
    memcpy(&op2[1], &m, 4);
    vgscpu_load_program(c, op2, sizeof(op2));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF MAIN MEMORY"), 0);
    m = VGSCPU_MEMORY_SIZE + 1;
    memcpy(&op2[1], &m, 4);
    vgscpu_load_program(c, op2, sizeof(op2));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF MAIN MEMORY"), 0);

    m = VGSCPU_MEMORY_SIZE - 4;
    memcpy(&op3[1], &m, 4);
    vgscpu_load_program(c, op3, sizeof(op3));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    m = VGSCPU_MEMORY_SIZE - 3;
    memcpy(&op3[1], &m, 4);
    vgscpu_load_program(c, op3, sizeof(op3));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF MAIN MEMORY"), 0);
    m = VGSCPU_MEMORY_SIZE + 1;
    memcpy(&op3[1], &m, 4);
    vgscpu_load_program(c, op3, sizeof(op3));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF MAIN MEMORY"), 0);

    m = VGSCPU_MEMORY_SIZE - 1;
    memcpy(&op4[1], &m, 4);
    vgscpu_load_program(c, op4, sizeof(op4));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    m = VGSCPU_MEMORY_SIZE;
    memcpy(&op4[1], &m, 4);
    vgscpu_load_program(c, op4, sizeof(op4));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF MAIN MEMORY"), 0);
    m = VGSCPU_MEMORY_SIZE + 1;
    memcpy(&op4[1], &m, 4);
    vgscpu_load_program(c, op4, sizeof(op4));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF MAIN MEMORY"), 0);

    m = VGSCPU_MEMORY_SIZE - 2;
    memcpy(&op5[1], &m, 4);
    vgscpu_load_program(c, op5, sizeof(op5));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    m = VGSCPU_MEMORY_SIZE - 1;
    memcpy(&op5[1], &m, 4);
    vgscpu_load_program(c, op5, sizeof(op5));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF MAIN MEMORY"), 0);
    m = VGSCPU_MEMORY_SIZE + 1;
    memcpy(&op5[1], &m, 4);
    vgscpu_load_program(c, op5, sizeof(op5));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF MAIN MEMORY"), 0);

    m = VGSCPU_MEMORY_SIZE - 4;
    memcpy(&op6[1], &m, 4);
    vgscpu_load_program(c, op6, sizeof(op6));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    m = VGSCPU_MEMORY_SIZE - 3;
    memcpy(&op6[1], &m, 4);
    vgscpu_load_program(c, op6, sizeof(op6));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF MAIN MEMORY"), 0);
    m = VGSCPU_MEMORY_SIZE + 1;
    memcpy(&op6[1], &m, 4);
    vgscpu_load_program(c, op6, sizeof(op6));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF MAIN MEMORY"), 0);
    return 0;
}

int test_ld_d_literal(struct vgscpu_context *c)
{
    unsigned short s;
    unsigned int i;
    unsigned char op1[] = {VGSCPU_OP_LD_D_1, 0xCD, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_LD_D_2, 0xAB, 0xCD, VGSCPU_OP_BRK};
    unsigned char op3[] = {VGSCPU_OP_LD_D_4, 0x67, 0x89, 0xAB, 0xCD, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0xCD);

    vgscpu_load_program(c, op2, sizeof(op2));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0xCDAB);

    vgscpu_load_program(c, op3, sizeof(op3));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0xCDAB8967);
    return 0;
}

int test_ld_d_registry(struct vgscpu_context *c)
{
    unsigned char op1[] = {VGSCPU_OP_LD_D_A, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_LD_D_B, VGSCPU_OP_BRK};
    unsigned char op3[] = {VGSCPU_OP_LD_D_C, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));
    c->r.a = 0x01020304;
    c->r.b = 0x05060708;
    c->r.c = 0x090a0b0c;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0x01020304);

    vgscpu_load_program(c, op2, sizeof(op2));
    c->r.a = 0x01020304;
    c->r.b = 0x05060708;
    c->r.c = 0x090a0b0c;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0x05060708);

    vgscpu_load_program(c, op3, sizeof(op3));
    c->r.a = 0x01020304;
    c->r.b = 0x05060708;
    c->r.c = 0x090a0b0c;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0x090a0b0c);
    return 0;
}

int test_ld_d_memory(struct vgscpu_context *c)
{
    const unsigned char m[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    unsigned char op1[] = {VGSCPU_OP_LD_D_M1, 0x08, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_LD_D_M2, 0x08, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op3[] = {VGSCPU_OP_LD_D_M4, 0x08, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));
    memcpy(&c->m, m, sizeof(m));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0x8);

    vgscpu_load_program(c, op2, sizeof(op2));
    memcpy(&c->m, m, sizeof(m));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0x908);

    vgscpu_load_program(c, op3, sizeof(op3));
    memcpy(&c->m, m, sizeof(m));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0xb0a0908);
    return 0;
}

int test_st_d(struct vgscpu_context *c)
{
    const unsigned char m[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    unsigned char op1[] = {VGSCPU_OP_ST_D_M1, 0x08, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_ST_D_M2, 0x08, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op3[] = {VGSCPU_OP_ST_D_M4, 0x08, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));
    memcpy(&c->m, m, sizeof(m));
    c->r.d = 0x12345678;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->m[0x8], 0x78);
    TEST(__FILE__, __LINE__, c->m[0x9], 0x9);
    TEST(__FILE__, __LINE__, c->m[0xa], 0xa);
    TEST(__FILE__, __LINE__, c->m[0xb], 0xb);

    vgscpu_load_program(c, op2, sizeof(op2));
    memcpy(&c->m, m, sizeof(m));
    c->r.d = 0x12345678;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->m[0x8], 0x78);
    TEST(__FILE__, __LINE__, c->m[0x9], 0x56);
    TEST(__FILE__, __LINE__, c->m[0xa], 0xa);
    TEST(__FILE__, __LINE__, c->m[0xb], 0xb);

    vgscpu_load_program(c, op3, sizeof(op3));
    memcpy(&c->m, m, sizeof(m));
    c->r.d = 0x12345678;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->m[0x8], 0x78);
    TEST(__FILE__, __LINE__, c->m[0x9], 0x56);
    TEST(__FILE__, __LINE__, c->m[0xa], 0x34);
    TEST(__FILE__, __LINE__, c->m[0xb], 0x12);
    return 0;
}

int main()
{
    struct vgscpu_context *c = (struct vgscpu_context *)vgscpu_create_context();
    if (!c) return -1;
    int result = -1;

    if (test_program_memory(c)) goto END_TEST;
    if (test_main_memory(c)) goto END_TEST;
    if (test_ld_d_literal(c)) goto END_TEST;
    if (test_ld_d_registry(c)) goto END_TEST;
    if (test_ld_d_memory(c)) goto END_TEST;
    if (test_st_d(c)) goto END_TEST;

    result = 0;
    puts("success");
END_TEST:
    vgscpu_release_context(c);
    return result;
}