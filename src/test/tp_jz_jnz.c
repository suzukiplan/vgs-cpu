#include "tp.h"

int test_program_memory_jz(struct vgscpu_context *c)
{
    unsigned char op1[] = {VGSCPU_OP_JZ, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_JZ, 0x05, VGSCPU_OP_BRK};
    unsigned char op3[] = {VGSCPU_OP_JZ, 0x05, 0x00, VGSCPU_OP_BRK};
    unsigned char op4[] = {VGSCPU_OP_JZ, 0x05, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op5[] = {VGSCPU_OP_JZ, 0x05, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op6[] = {VGSCPU_OP_JZ, 0x06, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));
    c->f.z = 1;
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

int test_program_memory_jz_1(struct vgscpu_context *c)
{
    unsigned char op1[] = {VGSCPU_OP_JZ_1, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_JZ_1, 0x00, VGSCPU_OP_BRK};
    unsigned char op3[] = {VGSCPU_OP_JZ_1, 0x01, VGSCPU_OP_BRK};
    unsigned char op4[] = {VGSCPU_OP_JZ_1, 0x02, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));
    c->f.z = 1;
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000002"), 0);

    vgscpu_load_program(c, op2, sizeof(op2));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);

    vgscpu_load_program(c, op3, sizeof(op3));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000003"), 0);

    vgscpu_load_program(c, op4, sizeof(op4));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000004"), 0);
    return 0;
}

int test_program_memory_jz_2(struct vgscpu_context *c)
{
    unsigned char op1[] = {VGSCPU_OP_JZ_2, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_JZ_2, 0x00, VGSCPU_OP_BRK};
    unsigned char op3[] = {VGSCPU_OP_JZ_2, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op4[] = {VGSCPU_OP_JZ_2, 0x01, 0x00, VGSCPU_OP_BRK};
    unsigned char op5[] = {VGSCPU_OP_JZ_2, 0x02, 0x00, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));
    c->f.z = 1;
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000003"), 0);

    vgscpu_load_program(c, op2, sizeof(op2));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000003"), 0);

    vgscpu_load_program(c, op3, sizeof(op3));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);

    vgscpu_load_program(c, op4, sizeof(op4));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000004"), 0);

    vgscpu_load_program(c, op5, sizeof(op5));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000005"), 0);
    return 0;
}

int test_program_memory_jnz(struct vgscpu_context *c)
{
    unsigned char op1[] = {VGSCPU_OP_JNZ, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_JNZ, 0x05, VGSCPU_OP_BRK};
    unsigned char op3[] = {VGSCPU_OP_JNZ, 0x05, 0x00, VGSCPU_OP_BRK};
    unsigned char op4[] = {VGSCPU_OP_JNZ, 0x05, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op5[] = {VGSCPU_OP_JNZ, 0x05, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op6[] = {VGSCPU_OP_JNZ, 0x06, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));
    c->f.z = 0;
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

int test_program_memory_jnz_1(struct vgscpu_context *c)
{
    unsigned char op1[] = {VGSCPU_OP_JNZ_1, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_JNZ_1, 0x00, VGSCPU_OP_BRK};
    unsigned char op3[] = {VGSCPU_OP_JNZ_1, 0x01, VGSCPU_OP_BRK};
    unsigned char op4[] = {VGSCPU_OP_JNZ_1, 0x02, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));
    c->f.z = 0;
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000002"), 0);

    vgscpu_load_program(c, op2, sizeof(op2));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);

    vgscpu_load_program(c, op3, sizeof(op3));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000003"), 0);

    vgscpu_load_program(c, op4, sizeof(op4));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000004"), 0);
    return 0;
}

int test_program_memory_jnz_2(struct vgscpu_context *c)
{
    unsigned char op1[] = {VGSCPU_OP_JNZ_2, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_JNZ_2, 0x00, VGSCPU_OP_BRK};
    unsigned char op3[] = {VGSCPU_OP_JNZ_2, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op4[] = {VGSCPU_OP_JNZ_2, 0x01, 0x00, VGSCPU_OP_BRK};
    unsigned char op5[] = {VGSCPU_OP_JNZ_2, 0x02, 0x00, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));
    c->f.z = 0;
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000003"), 0);

    vgscpu_load_program(c, op2, sizeof(op2));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000003"), 0);

    vgscpu_load_program(c, op3, sizeof(op3));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);

    vgscpu_load_program(c, op4, sizeof(op4));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000004"), 0);

    vgscpu_load_program(c, op5, sizeof(op5));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000005"), 0);
    return 0;
}

int test_jz(struct vgscpu_context *c)
{
    unsigned char op1[] = {VGSCPU_OP_LD_A_1, 0x01, VGSCPU_OP_LD_B_1, 0x00, VGSCPU_OP_DEC_A, VGSCPU_OP_JZ, 0x0b, 0x00, 0x00, 0x00, VGSCPU_OP_INC_B, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_LD_A_1, 0x02, VGSCPU_OP_LD_B_1, 0x00, VGSCPU_OP_DEC_A, VGSCPU_OP_JZ, 0x0b, 0x00, 0x00, 0x00, VGSCPU_OP_INC_B, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.a, 0x00);
    TEST(__FILE__, __LINE__, c->r.b, 0x00);

    vgscpu_load_program(c, op2, sizeof(op2));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.a, 0x01);
    TEST(__FILE__, __LINE__, c->r.b, 0x01);
    return 0;
}

int test_jz_1(struct vgscpu_context *c)
{
    unsigned char op1[] = {VGSCPU_OP_LD_A_1, 0x01, VGSCPU_OP_LD_B_1, 0x00, VGSCPU_OP_DEC_A, VGSCPU_OP_JZ_1, 0x01, VGSCPU_OP_INC_B, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_LD_A_1, 0x02, VGSCPU_OP_LD_B_1, 0x00, VGSCPU_OP_DEC_A, VGSCPU_OP_JZ_1, 0x01, VGSCPU_OP_INC_B, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.a, 0x00);
    TEST(__FILE__, __LINE__, c->r.b, 0x00);

    vgscpu_load_program(c, op2, sizeof(op2));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.a, 0x01);
    TEST(__FILE__, __LINE__, c->r.b, 0x01);
    return 0;
}

int test_jz_2(struct vgscpu_context *c)
{
    unsigned char op1[] = {VGSCPU_OP_LD_A_1, 0x01, VGSCPU_OP_LD_B_1, 0x00, VGSCPU_OP_DEC_A, VGSCPU_OP_JZ_2, 0x01, 0x00, VGSCPU_OP_INC_B, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_LD_A_1, 0x02, VGSCPU_OP_LD_B_1, 0x00, VGSCPU_OP_DEC_A, VGSCPU_OP_JZ_2, 0x01, 0x00, VGSCPU_OP_INC_B, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.a, 0x00);
    TEST(__FILE__, __LINE__, c->r.b, 0x00);

    vgscpu_load_program(c, op2, sizeof(op2));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.a, 0x01);
    TEST(__FILE__, __LINE__, c->r.b, 0x01);
    return 0;
}

int test_jnz(struct vgscpu_context *c)
{
    unsigned char op1[] = {VGSCPU_OP_LD_A_1, 0x01, VGSCPU_OP_LD_B_1, 0x00, VGSCPU_OP_DEC_A, VGSCPU_OP_JNZ, 0x0b, 0x00, 0x00, 0x00, VGSCPU_OP_INC_B, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_LD_A_1, 0x02, VGSCPU_OP_LD_B_1, 0x00, VGSCPU_OP_DEC_A, VGSCPU_OP_JNZ, 0x0b, 0x00, 0x00, 0x00, VGSCPU_OP_INC_B, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.a, 0x00);
    TEST(__FILE__, __LINE__, c->r.b, 0x01);

    vgscpu_load_program(c, op2, sizeof(op2));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.a, 0x01);
    TEST(__FILE__, __LINE__, c->r.b, 0x00);
    return 0;
}

int test_jnz_1(struct vgscpu_context *c)
{
    unsigned char op1[] = {VGSCPU_OP_LD_A_1, 0x01, VGSCPU_OP_LD_B_1, 0x00, VGSCPU_OP_DEC_A, VGSCPU_OP_JNZ_1, 0x01, VGSCPU_OP_INC_B, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_LD_A_1, 0x02, VGSCPU_OP_LD_B_1, 0x00, VGSCPU_OP_DEC_A, VGSCPU_OP_JNZ_1, 0x01, VGSCPU_OP_INC_B, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.a, 0x00);
    TEST(__FILE__, __LINE__, c->r.b, 0x01);

    vgscpu_load_program(c, op2, sizeof(op2));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.a, 0x01);
    TEST(__FILE__, __LINE__, c->r.b, 0x00);
    return 0;
}

int test_jnz_2(struct vgscpu_context *c)
{
    unsigned char op1[] = {VGSCPU_OP_LD_A_1, 0x01, VGSCPU_OP_LD_B_1, 0x00, VGSCPU_OP_DEC_A, VGSCPU_OP_JNZ_2, 0x01, 0x00, VGSCPU_OP_INC_B, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_LD_A_1, 0x02, VGSCPU_OP_LD_B_1, 0x00, VGSCPU_OP_DEC_A, VGSCPU_OP_JNZ_2, 0x01, 0x00, VGSCPU_OP_INC_B, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.a, 0x00);
    TEST(__FILE__, __LINE__, c->r.b, 0x01);

    vgscpu_load_program(c, op2, sizeof(op2));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.a, 0x01);
    TEST(__FILE__, __LINE__, c->r.b, 0x00);
    return 0;
}

int main()
{
    struct vgscpu_context *c = (struct vgscpu_context *)vgscpu_create_context();
    if (!c) return -1;
    int result = -1;

    if (test_program_memory_jz(c)) goto END_TEST;
    if (test_program_memory_jz_1(c)) goto END_TEST;
    if (test_program_memory_jz_2(c)) goto END_TEST;
    if (test_program_memory_jnz(c)) goto END_TEST;
    if (test_program_memory_jnz_1(c)) goto END_TEST;
    if (test_program_memory_jnz_2(c)) goto END_TEST;
    if (test_jz(c)) goto END_TEST;
    if (test_jz_1(c)) goto END_TEST;
    if (test_jz_2(c)) goto END_TEST;
    if (test_jnz(c)) goto END_TEST;
    if (test_jnz_1(c)) goto END_TEST;
    if (test_jnz_2(c)) goto END_TEST;

    result = 0;
    puts("success");
END_TEST:
    vgscpu_release_context(c);
    return result;
}