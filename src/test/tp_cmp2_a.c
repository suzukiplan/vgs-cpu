#include "tp.h"

int test_program_memory(struct vgscpu_context *c)
{
    unsigned char op1[] = {VGSCPU_OP_CMP2_A_1, 0x00, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_CMP2_A_2, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op3[] = {VGSCPU_OP_CMP2_A_4, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op4[] = {VGSCPU_OP_CMP2_A_B, VGSCPU_OP_BRK};
    unsigned char op5[] = {VGSCPU_OP_CMP2_A_C, VGSCPU_OP_BRK};
    unsigned char op6[] = {VGSCPU_OP_CMP2_A_D, VGSCPU_OP_BRK};
    unsigned char op7[] = {VGSCPU_OP_CMP2_A_M1, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op8[] = {VGSCPU_OP_CMP2_A_M2, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op9[] = {VGSCPU_OP_CMP2_A_M4, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};

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
    unsigned int m = c->sizeM;
    unsigned char op1[] = {VGSCPU_OP_CMP2_A_M1, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_CMP2_A_M2, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op3[] = {VGSCPU_OP_CMP2_A_M4, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};

    m = c->sizeM - 1;
    memcpy(&op1[1], &m, 4);
    vgscpu_load_program(c, op1, sizeof(op1));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    m = c->sizeM;
    memcpy(&op1[1], &m, 4);
    vgscpu_load_program(c, op1, sizeof(op1));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF MAIN MEMORY"), 0);
    m = c->sizeM + 1;
    memcpy(&op1[1], &m, 4);
    vgscpu_load_program(c, op1, sizeof(op1));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF MAIN MEMORY"), 0);

    m = c->sizeM - 2;
    memcpy(&op2[1], &m, 4);
    vgscpu_load_program(c, op2, sizeof(op2));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    m = c->sizeM - 1;
    memcpy(&op2[1], &m, 4);
    vgscpu_load_program(c, op2, sizeof(op2));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF MAIN MEMORY"), 0);
    m = c->sizeM + 1;
    memcpy(&op2[1], &m, 4);
    vgscpu_load_program(c, op2, sizeof(op2));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF MAIN MEMORY"), 0);

    m = c->sizeM - 4;
    memcpy(&op3[1], &m, 4);
    vgscpu_load_program(c, op3, sizeof(op3));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    m = c->sizeM - 3;
    memcpy(&op3[1], &m, 4);
    vgscpu_load_program(c, op3, sizeof(op3));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF MAIN MEMORY"), 0);
    m = c->sizeM + 1;
    memcpy(&op3[1], &m, 4);
    vgscpu_load_program(c, op3, sizeof(op3));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF MAIN MEMORY"), 0);
    return 0;
}

int test_cmp2_a_1(struct vgscpu_context *c)
{
    unsigned char op[] = {VGSCPU_OP_CMP2_A_1, 0x81, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));

    c->r.a = -128;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->f.q, -1);

    c->r.a = -127;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->f.q, 0);

    c->r.a = -126;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->f.q, 1);

    c->r.a = 0;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->f.q, 1);
    return 0;
}

int test_cmp2_a_2(struct vgscpu_context *c)
{
    unsigned char op[] = {VGSCPU_OP_CMP2_A_2, 0x01, 0x80, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));

    c->r.a = -32768;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->f.q, -1);

    c->r.a = -32767;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->f.q, 0);

    c->r.a = -32766;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->f.q, 1);

    c->r.a = 0;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->f.q, 1);
    return 0;
}

int test_cmp2_a_4(struct vgscpu_context *c)
{
    unsigned char op[] = {VGSCPU_OP_CMP2_A_4, 0x01, 0x00, 0x00, 0x80, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));

    c->r.a = -2147483648;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->f.q, -1);

    c->r.a = -2147483647;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->f.q, 0);

    c->r.a = -2147483646;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->f.q, 1);

    c->r.a = 0;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->f.q, 1);
    return 0;
}

int test_cmp2_a_b(struct vgscpu_context *c)
{
    unsigned char op[] = {VGSCPU_OP_LD_B_4, 0x81, 0xff, 0xff, 0xff, VGSCPU_OP_CMP2_A_B, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));

    c->r.a = -128;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->f.q, -1);

    c->r.a = -127;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->f.q, 0);

    c->r.a = -126;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->f.q, 1);

    c->r.a = 0;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->f.q, 1);
    return 0;
}

int test_cmp2_a_c(struct vgscpu_context *c)
{
    unsigned char op[] = {VGSCPU_OP_LD_C_4, 0x01, 0x80, 0xff, 0xff, VGSCPU_OP_CMP2_A_C, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));

    c->r.a = -32768;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->f.q, -1);

    c->r.a = -32767;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->f.q, 0);

    c->r.a = -32766;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->f.q, 1);

    c->r.a = 0;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->f.q, 1);
    return 0;
}

int test_cmp2_a_d(struct vgscpu_context *c)
{
    unsigned char op[] = {VGSCPU_OP_LD_D_4, 0x01, 0x00, 0x00, 0x80, VGSCPU_OP_CMP2_A_D, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));

    c->r.a = -2147483648;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->f.q, -1);

    c->r.a = -2147483647;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->f.q, 0);

    c->r.a = -2147483646;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->f.q, 1);

    c->r.a = 0;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->f.q, 1);
    return 0;
}

int test_cmp2_a_m1(struct vgscpu_context *c)
{
    unsigned char op[] = {VGSCPU_OP_CMP2_A_M1, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    memset(c->m, 0, c->sizeM);
    c->m[0] = 0x81;

    vgscpu_load_program(c, op, sizeof(op));

    c->r.a = -128;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->f.q, -1);

    c->r.a = -127;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->f.q, 0);

    c->r.a = -126;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->f.q, 1);

    c->r.a = 0;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->f.q, 1);
    return 0;
}

int test_cmp2_a_m2(struct vgscpu_context *c)
{
    unsigned char op[] = {VGSCPU_OP_CMP2_A_M2, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    memset(c->m, 0, c->sizeM);
    c->m[0] = 0x01;
    c->m[1] = 0x80;

    vgscpu_load_program(c, op, sizeof(op));

    c->r.a = -32768;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->f.q, -1);

    c->r.a = -32767;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->f.q, 0);

    c->r.a = -32766;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->f.q, 1);

    c->r.a = 0;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->f.q, 1);
    return 0;
}

int test_cmp2_a_m4(struct vgscpu_context *c)
{
    unsigned char op[] = {VGSCPU_OP_CMP2_A_M4, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    memset(c->m, 0, c->sizeM);
    c->m[0] = 0x01;
    c->m[3] = 0x80;

    vgscpu_load_program(c, op, sizeof(op));

    c->r.a = -2147483648;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->f.q, -1);

    c->r.a = -2147483647;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->f.q, 0);

    c->r.a = -2147483646;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->f.q, 1);

    c->r.a = 0;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->f.q, 1);
    return 0;
}

int main()
{
    struct vgscpu_context *c = (struct vgscpu_context *)vgscpu_create_context();
    if (!c) return -1;
    int result = -1;

    if (test_program_memory(c)) goto END_TEST;
    if (test_main_memory(c)) goto END_TEST;
    if (test_cmp2_a_1(c)) goto END_TEST;
    if (test_cmp2_a_2(c)) goto END_TEST;
    if (test_cmp2_a_4(c)) goto END_TEST;
    if (test_cmp2_a_b(c)) goto END_TEST;
    if (test_cmp2_a_c(c)) goto END_TEST;
    if (test_cmp2_a_d(c)) goto END_TEST;
    if (test_cmp2_a_m1(c)) goto END_TEST;
    if (test_cmp2_a_m2(c)) goto END_TEST;
    if (test_cmp2_a_m4(c)) goto END_TEST;

    result = 0;
    puts("success");
END_TEST:
    vgscpu_release_context(c);
    return result;
}