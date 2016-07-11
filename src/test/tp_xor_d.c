#include "tp.h"

int test_program_memory(struct vgscpu_context *c)
{
    unsigned char op1[] = {VGSCPU_OP_ACU_D, VGSCPU_OP_XOR_D_1, 0x00, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_ACU_D, VGSCPU_OP_XOR_D_2, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op3[] = {VGSCPU_OP_ACU_D, VGSCPU_OP_XOR_D_4, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op4[] = {VGSCPU_OP_ACU_D, VGSCPU_OP_XOR_D_A, VGSCPU_OP_BRK};
    unsigned char op5[] = {VGSCPU_OP_ACU_D, VGSCPU_OP_XOR_D_B, VGSCPU_OP_BRK};
    unsigned char op6[] = {VGSCPU_OP_ACU_D, VGSCPU_OP_XOR_D_C, VGSCPU_OP_BRK};
    unsigned char op7[] = {VGSCPU_OP_ACU_D, VGSCPU_OP_XOR_D_M1, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op8[] = {VGSCPU_OP_ACU_D, VGSCPU_OP_XOR_D_M2, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op9[] = {VGSCPU_OP_ACU_D, VGSCPU_OP_XOR_D_M4, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    vgscpu_load_program(c, op1, sizeof(op1) - 1);
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000003"), 0);

    vgscpu_load_program(c, op2, sizeof(op2));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    vgscpu_load_program(c, op2, sizeof(op2) - 1);
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000004"), 0);

    vgscpu_load_program(c, op3, sizeof(op3));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    vgscpu_load_program(c, op3, sizeof(op3) - 1);
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000006"), 0);

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
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000006"), 0);

    vgscpu_load_program(c, op8, sizeof(op8));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    vgscpu_load_program(c, op8, sizeof(op8) - 1);
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000006"), 0);

    vgscpu_load_program(c, op9, sizeof(op9));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    vgscpu_load_program(c, op9, sizeof(op9) - 1);
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000006"), 0);

    return 0;
}

int test_main_memory(struct vgscpu_context *c)
{
    unsigned int m = c->sizeM;
    unsigned char op1[] = {VGSCPU_OP_ACU_D, VGSCPU_OP_XOR_D_M1, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_ACU_D, VGSCPU_OP_XOR_D_M2, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op3[] = {VGSCPU_OP_ACU_D, VGSCPU_OP_XOR_D_M4, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};

    m = c->sizeM - 1;
    memcpy(&op1[2], &m, 4);
    vgscpu_load_program(c, op1, sizeof(op1));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    m = c->sizeM;
    memcpy(&op1[2], &m, 4);
    vgscpu_load_program(c, op1, sizeof(op1));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF MAIN MEMORY"), 0);
    m = c->sizeM + 1;
    memcpy(&op1[2], &m, 4);
    vgscpu_load_program(c, op1, sizeof(op1));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF MAIN MEMORY"), 0);

    m = c->sizeM - 2;
    memcpy(&op2[2], &m, 4);
    vgscpu_load_program(c, op2, sizeof(op2));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    m = c->sizeM - 1;
    memcpy(&op2[2], &m, 4);
    vgscpu_load_program(c, op2, sizeof(op2));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF MAIN MEMORY"), 0);
    m = c->sizeM + 1;
    memcpy(&op2[2], &m, 4);
    vgscpu_load_program(c, op2, sizeof(op2));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF MAIN MEMORY"), 0);

    m = c->sizeM - 4;
    memcpy(&op3[2], &m, 4);
    vgscpu_load_program(c, op3, sizeof(op3));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    m = c->sizeM - 3;
    memcpy(&op3[2], &m, 4);
    vgscpu_load_program(c, op3, sizeof(op3));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF MAIN MEMORY"), 0);
    m = c->sizeM + 1;
    memcpy(&op3[2], &m, 4);
    vgscpu_load_program(c, op3, sizeof(op3));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF MAIN MEMORY"), 0);
    return 0;
}

int test_xor_d_1(struct vgscpu_context *c)
{
    unsigned char op[] = {VGSCPU_OP_ACU_D, VGSCPU_OP_XOR_D_1, 0xf0, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));

    c->r.d = 0xcc;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0x3c);
    TEST(__FILE__, __LINE__, c->f.z, 0);

    c->r.d = 0xf0;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0);
    TEST(__FILE__, __LINE__, c->f.z, 1);
    return 0;
}

int test_xor_d_2(struct vgscpu_context *c)
{
    unsigned char op[] = {VGSCPU_OP_ACU_D, VGSCPU_OP_XOR_D_2, 0xff, 0xf0, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));

    c->r.d = 0xabcd;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0x5b32);
    TEST(__FILE__, __LINE__, c->f.z, 0);

    c->r.d = 0xf0ff;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0);
    TEST(__FILE__, __LINE__, c->f.z, 1);
    return 0;
}

int test_xor_d_4(struct vgscpu_context *c)
{
    unsigned char op[] = {VGSCPU_OP_ACU_D, VGSCPU_OP_XOR_D_4, 0xf0, 0xff, 0xf0, 0xff, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));

    c->r.d = 0xabcdef12;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0x543d10e2);
    TEST(__FILE__, __LINE__, c->f.z, 0);

    c->r.d = 0xfff0fff0;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0);
    TEST(__FILE__, __LINE__, c->f.z, 1);
    return 0;
}

int test_xor_d_a(struct vgscpu_context *c)
{
    unsigned char op[] = {VGSCPU_OP_LD_A_1, 0xee, VGSCPU_OP_ACU_D, VGSCPU_OP_XOR_D_A, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));

    c->r.d = 0xff;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0x11);
    TEST(__FILE__, __LINE__, c->f.z, 0);

    c->r.d = 0xee;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0);
    TEST(__FILE__, __LINE__, c->f.z, 1);
    return 0;
}

int test_xor_d_b(struct vgscpu_context *c)
{
    unsigned char op[] = {VGSCPU_OP_LD_B_1, 0xcc, VGSCPU_OP_ACU_D, VGSCPU_OP_XOR_D_B, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));

    c->r.d = 0xff;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0x33);
    TEST(__FILE__, __LINE__, c->f.z, 0);

    c->r.d = 0xcc;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0);
    TEST(__FILE__, __LINE__, c->f.z, 1);
    return 0;
}

int test_xor_d_c(struct vgscpu_context *c)
{
    unsigned char op[] = {VGSCPU_OP_LD_C_1, 0x88, VGSCPU_OP_ACU_D, VGSCPU_OP_XOR_D_C, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));

    c->r.d = 0xff;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0x77);
    TEST(__FILE__, __LINE__, c->f.z, 0);

    c->r.d = 0x88;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0);
    TEST(__FILE__, __LINE__, c->f.z, 1);
    return 0;
}

int test_xor_d_m1(struct vgscpu_context *c)
{
    unsigned char op[] = {VGSCPU_OP_ACU_D, VGSCPU_OP_XOR_D_M1, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned int i = 0xf0e0c080;

    memcpy(&c->m[0], &i, 4);
    vgscpu_load_program(c, op, sizeof(op));

    c->r.d = 0xffffffff;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0xffffff7f);
    TEST(__FILE__, __LINE__, c->f.z, 0);

    c->r.d = 0x80;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0);
    TEST(__FILE__, __LINE__, c->f.z, 1);
    return 0;
}

int test_xor_d_m2(struct vgscpu_context *c)
{
    unsigned char op[] = {VGSCPU_OP_ACU_D, VGSCPU_OP_XOR_D_M2, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned int i = 0xf0e0c080;

    memcpy(&c->m[0], &i, 4);
    vgscpu_load_program(c, op, sizeof(op));

    c->r.d = 0xffffffff;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0xffff3f7f);
    TEST(__FILE__, __LINE__, c->f.z, 0);

    c->r.d = 0xc080;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0);
    TEST(__FILE__, __LINE__, c->f.z, 1);
    return 0;
}

int test_xor_d_m4(struct vgscpu_context *c)
{
    unsigned char op[] = {VGSCPU_OP_ACU_D, VGSCPU_OP_XOR_D_M4, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned int i = 0xf0e0c080;

    memcpy(&c->m[0], &i, 4);
    vgscpu_load_program(c, op, sizeof(op));

    c->r.d = 0xffffffff;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0x0f1f3f7f);
    TEST(__FILE__, __LINE__, c->f.z, 0);

    c->r.d = 0xf0e0c080;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0);
    TEST(__FILE__, __LINE__, c->f.z, 1);
    return 0;
}

int main()
{
    struct vgscpu_context *c = (struct vgscpu_context *)vgscpu_create_context();
    if (!c) return -1;
    int result = -1;

    if (test_program_memory(c)) goto END_TEST;
    if (test_main_memory(c)) goto END_TEST;
    if (test_xor_d_1(c)) goto END_TEST;
    if (test_xor_d_2(c)) goto END_TEST;
    if (test_xor_d_4(c)) goto END_TEST;
    if (test_xor_d_a(c)) goto END_TEST;
    if (test_xor_d_b(c)) goto END_TEST;
    if (test_xor_d_c(c)) goto END_TEST;
    if (test_xor_d_m1(c)) goto END_TEST;
    if (test_xor_d_m2(c)) goto END_TEST;
    if (test_xor_d_m4(c)) goto END_TEST;

    result = 0;
    puts("success");
END_TEST:
    vgscpu_release_context(c);
    return result;
}