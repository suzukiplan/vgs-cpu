#include "tp.h"

int test_program_memory_jn(struct vgscpu_context *c)
{
    unsigned char op1[] = {VGSCPU_OP_JN, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_JN, 0x05, VGSCPU_OP_BRK};
    unsigned char op3[] = {VGSCPU_OP_JN, 0x05, 0x00, VGSCPU_OP_BRK};
    unsigned char op4[] = {VGSCPU_OP_JN, 0x05, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op5[] = {VGSCPU_OP_JN, 0x05, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op6[] = {VGSCPU_OP_JN, 0x06, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));
    c->f.q = -1;
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000005"), 0);

    vgscpu_load_program(c, op2, sizeof(op2));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000005"), 0);

    vgscpu_load_program(c, op3, sizeof(op3));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000005"), 0);

    vgscpu_load_program(c, op4, sizeof(op4));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000005"), 0);

    vgscpu_load_program(c, op5, sizeof(op5));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);

    vgscpu_load_program(c, op6, sizeof(op6));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000006"), 0);
    return 0;
}

int test_program_memory_jnn(struct vgscpu_context *c)
{
    unsigned char op1[] = {VGSCPU_OP_JNN, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_JNN, 0x05, VGSCPU_OP_BRK};
    unsigned char op3[] = {VGSCPU_OP_JNN, 0x05, 0x00, VGSCPU_OP_BRK};
    unsigned char op4[] = {VGSCPU_OP_JNN, 0x05, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op5[] = {VGSCPU_OP_JNN, 0x05, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op6[] = {VGSCPU_OP_JNN, 0x06, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));
    c->f.q = 1;
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000005"), 0);

    vgscpu_load_program(c, op2, sizeof(op2));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000005"), 0);

    vgscpu_load_program(c, op3, sizeof(op3));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000005"), 0);

    vgscpu_load_program(c, op4, sizeof(op4));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000005"), 0);

    vgscpu_load_program(c, op5, sizeof(op5));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);

    vgscpu_load_program(c, op6, sizeof(op6));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000006"), 0);
    return 0;
}

int test_jn(struct vgscpu_context *c)
{
    unsigned char op1[] = {VGSCPU_OP_LD_A_1, 0x01, VGSCPU_OP_LD_B_1, 0x01, VGSCPU_OP_LD_C_1, 0x00, VGSCPU_OP_CMP_A_B, VGSCPU_OP_JN, 0x0d, 0x00, 0x00, 0x00, VGSCPU_OP_INC_C, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_LD_A_1, 0x02, VGSCPU_OP_LD_B_1, 0x01, VGSCPU_OP_LD_C_1, 0x00, VGSCPU_OP_CMP_A_B, VGSCPU_OP_JN, 0x0d, 0x00, 0x00, 0x00, VGSCPU_OP_INC_C, VGSCPU_OP_BRK};
    unsigned char op3[] = {VGSCPU_OP_LD_A_1, 0x01, VGSCPU_OP_LD_B_1, 0x02, VGSCPU_OP_LD_C_1, 0x00, VGSCPU_OP_CMP_A_B, VGSCPU_OP_JN, 0x0d, 0x00, 0x00, 0x00, VGSCPU_OP_INC_C, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.a, 0x01);
    TEST(__FILE__, __LINE__, c->r.b, 0x01);
    TEST(__FILE__, __LINE__, c->r.c, 0x01);

    vgscpu_load_program(c, op2, sizeof(op2));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.a, 0x02);
    TEST(__FILE__, __LINE__, c->r.b, 0x01);
    TEST(__FILE__, __LINE__, c->r.c, 0x01);

    vgscpu_load_program(c, op3, sizeof(op3));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.a, 0x01);
    TEST(__FILE__, __LINE__, c->r.b, 0x02);
    TEST(__FILE__, __LINE__, c->r.c, 0x00);
    return 0;
}

int test_jnn(struct vgscpu_context *c)
{
    unsigned char op1[] = {VGSCPU_OP_LD_A_1, 0x01, VGSCPU_OP_LD_B_1, 0x01, VGSCPU_OP_LD_C_1, 0x00, VGSCPU_OP_CMP_A_B, VGSCPU_OP_JNN, 0x0d, 0x00, 0x00, 0x00, VGSCPU_OP_INC_C, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_LD_A_1, 0x02, VGSCPU_OP_LD_B_1, 0x01, VGSCPU_OP_LD_C_1, 0x00, VGSCPU_OP_CMP_A_B, VGSCPU_OP_JNN, 0x0d, 0x00, 0x00, 0x00, VGSCPU_OP_INC_C, VGSCPU_OP_BRK};
    unsigned char op3[] = {VGSCPU_OP_LD_A_1, 0x01, VGSCPU_OP_LD_B_1, 0x02, VGSCPU_OP_LD_C_1, 0x00, VGSCPU_OP_CMP_A_B, VGSCPU_OP_JNN, 0x0d, 0x00, 0x00, 0x00, VGSCPU_OP_INC_C, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.a, 0x01);
    TEST(__FILE__, __LINE__, c->r.b, 0x01);
    TEST(__FILE__, __LINE__, c->r.c, 0x00);

    vgscpu_load_program(c, op2, sizeof(op2));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.a, 0x02);
    TEST(__FILE__, __LINE__, c->r.b, 0x01);
    TEST(__FILE__, __LINE__, c->r.c, 0x00);

    vgscpu_load_program(c, op3, sizeof(op3));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.a, 0x01);
    TEST(__FILE__, __LINE__, c->r.b, 0x02);
    TEST(__FILE__, __LINE__, c->r.c, 0x01);
    return 0;
}

int main()
{
    struct vgscpu_context *c = (struct vgscpu_context *)vgscpu_create_context();
    if (!c) return -1;
    int result = -1;

    if (test_program_memory_jn(c)) goto END_TEST;
    if (test_program_memory_jnn(c)) goto END_TEST;
    if (test_jn(c)) goto END_TEST;
    if (test_jnn(c)) goto END_TEST;

    result = 0;
    puts("success");
END_TEST:
    vgscpu_release_context(c);
    return result;
}