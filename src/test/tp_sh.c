#include "tp.h"

int test_program_memory(struct vgscpu_context *c)
{
    unsigned char op1[] = {VGSCPU_OP_SHR_A, 0x01, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_SHR_B, 0x01, VGSCPU_OP_BRK};
    unsigned char op3[] = {VGSCPU_OP_SHR_C, 0x01, VGSCPU_OP_BRK};
    unsigned char op4[] = {VGSCPU_OP_SHR_D, 0x01, VGSCPU_OP_BRK};
    unsigned char op5[] = {VGSCPU_OP_SHL_A, 0x01, VGSCPU_OP_BRK};
    unsigned char op6[] = {VGSCPU_OP_SHL_B, 0x01, VGSCPU_OP_BRK};
    unsigned char op7[] = {VGSCPU_OP_SHL_C, 0x01, VGSCPU_OP_BRK};
    unsigned char op8[] = {VGSCPU_OP_SHL_D, 0x01, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    vgscpu_load_program(c, op1, sizeof(op1) - 1);
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000002"), 0);

    vgscpu_load_program(c, op2, sizeof(op2));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    vgscpu_load_program(c, op2, sizeof(op2) - 1);
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000002"), 0);

    vgscpu_load_program(c, op3, sizeof(op3));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    vgscpu_load_program(c, op3, sizeof(op3) - 1);
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000002"), 0);

    vgscpu_load_program(c, op4, sizeof(op4));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    vgscpu_load_program(c, op4, sizeof(op4) - 1);
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000002"), 0);

    vgscpu_load_program(c, op5, sizeof(op5));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    vgscpu_load_program(c, op5, sizeof(op5) - 1);
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000002"), 0);

    vgscpu_load_program(c, op6, sizeof(op6));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    vgscpu_load_program(c, op6, sizeof(op6) - 1);
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000002"), 0);

    vgscpu_load_program(c, op7, sizeof(op7));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    vgscpu_load_program(c, op7, sizeof(op7) - 1);
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000002"), 0);

    vgscpu_load_program(c, op8, sizeof(op8));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    vgscpu_load_program(c, op8, sizeof(op8) - 1);
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000002"), 0);
    return 0;
}

int test_shr_a(struct vgscpu_context *c)
{
    unsigned char op[] = {VGSCPU_OP_SHR_A, 0x10, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));
    c->r.a = 0xffff0000;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.a, 0x0000ffff);
    return 0;
}

int test_shl_a(struct vgscpu_context *c)
{
    unsigned char op[] = {VGSCPU_OP_SHL_A, 0x10, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));
    c->r.a = 0x0000ffff;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.a, 0xffff0000);
    return 0;
}

int test_shr_b(struct vgscpu_context *c)
{
    unsigned char op[] = {VGSCPU_OP_SHR_B, 0x10, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));
    c->r.b = 0xffff0000;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.b, 0x0000ffff);
    return 0;
}

int test_shl_b(struct vgscpu_context *c)
{
    unsigned char op[] = {VGSCPU_OP_SHL_B, 0x10, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));
    c->r.b = 0x0000ffff;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.b, 0xffff0000);
    return 0;
}

int test_shr_c(struct vgscpu_context *c)
{
    unsigned char op[] = {VGSCPU_OP_SHR_C, 0x10, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));
    c->r.c = 0xffff0000;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.c, 0x0000ffff);
    return 0;
}

int test_shl_c(struct vgscpu_context *c)
{
    unsigned char op[] = {VGSCPU_OP_SHL_C, 0x10, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));
    c->r.c = 0x0000ffff;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.c, 0xffff0000);
    return 0;
}

int test_shr_d(struct vgscpu_context *c)
{
    unsigned char op[] = {VGSCPU_OP_SHR_D, 0x10, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));
    c->r.d = 0xffff0000;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0x0000ffff);
    return 0;
}

int test_shl_d(struct vgscpu_context *c)
{
    unsigned char op[] = {VGSCPU_OP_SHL_D, 0x10, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));
    c->r.d = 0x0000ffff;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0xffff0000);
    return 0;
}

int main()
{
    struct vgscpu_context *c = (struct vgscpu_context *)vgscpu_create_context();
    if (!c) return -1;
    int result = -1;

    if (test_program_memory(c)) goto END_TEST;
    if (test_shr_a(c)) goto END_TEST;
    if (test_shl_a(c)) goto END_TEST;
    if (test_shr_b(c)) goto END_TEST;
    if (test_shl_b(c)) goto END_TEST;
    if (test_shr_c(c)) goto END_TEST;
    if (test_shl_c(c)) goto END_TEST;
    if (test_shr_d(c)) goto END_TEST;
    if (test_shl_d(c)) goto END_TEST;

    result = 0;
    puts("success");
END_TEST:
    vgscpu_release_context(c);
    return result;
}