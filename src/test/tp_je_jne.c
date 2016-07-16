#include "tp.h"

int test_program_memory_je(struct vgscpu_context *c)
{
    unsigned char op1[] = {VGSCPU_OP_JE, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_JE, 0x05, VGSCPU_OP_BRK};
    unsigned char op3[] = {VGSCPU_OP_JE, 0x05, 0x00, VGSCPU_OP_BRK};
    unsigned char op4[] = {VGSCPU_OP_JE, 0x05, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op5[] = {VGSCPU_OP_JE, 0x05, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op6[] = {VGSCPU_OP_JE, 0x06, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));
    c->f.q = 0;
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

int test_program_memory_jne(struct vgscpu_context *c)
{
    unsigned char op1[] = {VGSCPU_OP_JNE, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_JNE, 0x05, VGSCPU_OP_BRK};
    unsigned char op3[] = {VGSCPU_OP_JNE, 0x05, 0x00, VGSCPU_OP_BRK};
    unsigned char op4[] = {VGSCPU_OP_JNE, 0x05, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op5[] = {VGSCPU_OP_JNE, 0x05, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op6[] = {VGSCPU_OP_JNE, 0x06, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};

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

int test_je(struct vgscpu_context *c)
{
    unsigned char op1[] = {VGSCPU_OP_LD_A_1, 0x01, VGSCPU_OP_LD_B_1, 0x01, VGSCPU_OP_CMP_A_B, VGSCPU_OP_JE, 0x0b, 0x00, 0x00, 0x00, VGSCPU_OP_INC_B, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_LD_A_1, 0x02, VGSCPU_OP_LD_B_1, 0x01, VGSCPU_OP_CMP_A_B, VGSCPU_OP_JE, 0x0b, 0x00, 0x00, 0x00, VGSCPU_OP_INC_B, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.a, 0x01);
    TEST(__FILE__, __LINE__, c->r.b, 0x01);

    vgscpu_load_program(c, op2, sizeof(op2));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.a, 0x02);
    TEST(__FILE__, __LINE__, c->r.b, 0x02);
    return 0;
}

int test_jne(struct vgscpu_context *c)
{
    unsigned char op1[] = {VGSCPU_OP_LD_A_1, 0x01, VGSCPU_OP_LD_B_1, 0x01, VGSCPU_OP_CMP_A_B, VGSCPU_OP_JNE, 0x0b, 0x00, 0x00, 0x00, VGSCPU_OP_INC_B, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_LD_A_1, 0x02, VGSCPU_OP_LD_B_1, 0x01, VGSCPU_OP_CMP_A_B, VGSCPU_OP_JNE, 0x0b, 0x00, 0x00, 0x00, VGSCPU_OP_INC_B, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.a, 0x01);
    TEST(__FILE__, __LINE__, c->r.b, 0x02);

    vgscpu_load_program(c, op2, sizeof(op2));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.a, 0x02);
    TEST(__FILE__, __LINE__, c->r.b, 0x01);
    return 0;
}

int main()
{
    struct vgscpu_context *c = (struct vgscpu_context *)vgscpu_create_context();
    if (!c) return -1;
    int result = -1;

    if (test_program_memory_je(c)) goto END_TEST;
    if (test_program_memory_jne(c)) goto END_TEST;
    if (test_je(c)) goto END_TEST;
    if (test_jne(c)) goto END_TEST;

    result = 0;
    puts("success");
END_TEST:
    vgscpu_release_context(c);
    return result;
}