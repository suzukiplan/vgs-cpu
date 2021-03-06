#include "tp.h"

int test_program_memory(struct vgscpu_context *c)
{
    unsigned char op1[] = {VGSCPU_OP_ACU_C, VGSCPU_OP_MOD_C_1, 0x01, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_ACU_C, VGSCPU_OP_MOD_C_2, 0x01, 0x00, VGSCPU_OP_BRK};
    unsigned char op3[] = {VGSCPU_OP_ACU_C, VGSCPU_OP_MOD_C_4, 0x01, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op4[] = {VGSCPU_OP_ACU_C, VGSCPU_OP_MOD_C_A, VGSCPU_OP_BRK};
    unsigned char op5[] = {VGSCPU_OP_ACU_C, VGSCPU_OP_MOD_C_B, VGSCPU_OP_BRK};
    unsigned char op6[] = {VGSCPU_OP_ACU_C, VGSCPU_OP_MOD_C_D, VGSCPU_OP_BRK};
    unsigned char op7[] = {VGSCPU_OP_ACU_C, VGSCPU_OP_MOD_C_M1, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op8[] = {VGSCPU_OP_ACU_C, VGSCPU_OP_MOD_C_M2, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op9[] = {VGSCPU_OP_ACU_C, VGSCPU_OP_MOD_C_M4, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};

    c->r.a = 1;
    c->r.b = 1;
    c->r.d = 1;
    memset(c->m, 1, c->sizeM);
    c->m[0] = 1;

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
    unsigned char op1[] = {VGSCPU_OP_ACU_C, VGSCPU_OP_MOD_C_M1, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_ACU_C, VGSCPU_OP_MOD_C_M2, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op3[] = {VGSCPU_OP_ACU_C, VGSCPU_OP_MOD_C_M4, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};

    memset(c->m, 1, m);

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

int test_mod_c_1(struct vgscpu_context *c)
{
    unsigned char op1[] = {VGSCPU_OP_ACU_C, VGSCPU_OP_MOD_C_1, 0x80, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_ACU_C, VGSCPU_OP_MOD_C_1, 0x00, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));

    c->r.c = 0;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.c, 0);
    TEST(__FILE__, __LINE__, c->f.z, 1);

    c->r.c = 0x88;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.c, 8);
    TEST(__FILE__, __LINE__, c->f.z, 0);

    vgscpu_load_program(c, op2, sizeof(op2));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "ZERO DIVIDE"), 0);
    return 0;
}

int test_mod_c_2(struct vgscpu_context *c)
{
    unsigned char op1[] = {VGSCPU_OP_ACU_C, VGSCPU_OP_MOD_C_2, 0x12, 0x34, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_ACU_C, VGSCPU_OP_MOD_C_2, 0x00, 0x00, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));

    c->r.c = 0;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.c, 0);
    TEST(__FILE__, __LINE__, c->f.z, 1);

    c->r.c = 0x3419;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.c, 7);
    TEST(__FILE__, __LINE__, c->f.z, 0);

    vgscpu_load_program(c, op2, sizeof(op2));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "ZERO DIVIDE"), 0);
    return 0;
}

int test_mod_c_4(struct vgscpu_context *c)
{
    unsigned char op1[] = {VGSCPU_OP_ACU_C, VGSCPU_OP_MOD_C_4, 0x11, 0x22, 0x33, 0x44, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_ACU_C, VGSCPU_OP_MOD_C_4, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));

    c->r.c = 0;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.c, 0);
    TEST(__FILE__, __LINE__, c->f.z, 1);

    c->r.c = 0x88664428;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.c, 6);
    TEST(__FILE__, __LINE__, c->f.z, 0);

    vgscpu_load_program(c, op2, sizeof(op2));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "ZERO DIVIDE"), 0);
    return 0;
}

int test_mod_c_a(struct vgscpu_context *c)
{
    unsigned char op1[] = {VGSCPU_OP_LD_A_1, 0x11, VGSCPU_OP_ACU_C, VGSCPU_OP_MOD_C_A, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_LD_A_1, 0x00, VGSCPU_OP_ACU_C, VGSCPU_OP_MOD_C_A, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));

    c->r.c = 0;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.c, 0);
    TEST(__FILE__, __LINE__, c->f.z, 1);

    c->r.c = 0x15;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.c, 4);
    TEST(__FILE__, __LINE__, c->f.z, 0);

    vgscpu_load_program(c, op2, sizeof(op2));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "ZERO DIVIDE"), 0);
    return 0;
}

int test_mod_c_b(struct vgscpu_context *c)
{
    unsigned char op1[] = {VGSCPU_OP_LD_B_1, 0x22, VGSCPU_OP_ACU_C, VGSCPU_OP_MOD_C_B, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_LD_B_1, 0x00, VGSCPU_OP_ACU_C, VGSCPU_OP_MOD_C_B, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));

    c->r.c = 0;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.c, 0);
    TEST(__FILE__, __LINE__, c->f.z, 1);

    c->r.c = 0x25;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.c, 3);
    TEST(__FILE__, __LINE__, c->f.z, 0);

    vgscpu_load_program(c, op2, sizeof(op2));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "ZERO DIVIDE"), 0);
    return 0;
}

int test_mod_c_d(struct vgscpu_context *c)
{
    unsigned char op1[] = {VGSCPU_OP_LD_D_1, 0x33, VGSCPU_OP_ACU_C, VGSCPU_OP_MOD_C_D, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_LD_D_1, 0x00, VGSCPU_OP_ACU_C, VGSCPU_OP_MOD_C_D, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));

    c->r.c = 0;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.c, 0);
    TEST(__FILE__, __LINE__, c->f.z, 1);

    c->r.c = 0x35;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.c, 2);
    TEST(__FILE__, __LINE__, c->f.z, 0);

    vgscpu_load_program(c, op2, sizeof(op2));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "ZERO DIVIDE"), 0);
    return 0;
}

int test_mod_c_m1(struct vgscpu_context *c)
{
    unsigned char op[] = {VGSCPU_OP_ACU_C, VGSCPU_OP_MOD_C_M1, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned int i = 0x11223344;

    memcpy(&c->m[0], &i, 4);
    vgscpu_load_program(c, op, sizeof(op));

    c->r.c = 0;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.c, 0);
    TEST(__FILE__, __LINE__, c->f.z, 1);

    c->r.c = 0x54;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.c, 0x10);
    TEST(__FILE__, __LINE__, c->f.z, 0);

    i = 0;
    memcpy(&c->m[0], &i, 4);
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "ZERO DIVIDE"), 0);
    return 0;
}

int test_mod_c_m2(struct vgscpu_context *c)
{
    unsigned char op[] = {VGSCPU_OP_ACU_C, VGSCPU_OP_MOD_C_M2, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned int i = 0x11223344;

    memcpy(&c->m[0], &i, 4);
    vgscpu_load_program(c, op, sizeof(op));

    c->r.c = 0;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.c, 0);
    TEST(__FILE__, __LINE__, c->f.z, 1);

    c->r.c = 0x3355;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.c, 0x11);
    TEST(__FILE__, __LINE__, c->f.z, 0);

    i = 0;
    memcpy(&c->m[0], &i, 4);
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "ZERO DIVIDE"), 0);
    return 0;
}

int test_mod_c_m4(struct vgscpu_context *c)
{
    unsigned char op[] = {VGSCPU_OP_ACU_C, VGSCPU_OP_MOD_C_M4, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned int i = 0x11223344;

    memcpy(&c->m[0], &i, 4);
    vgscpu_load_program(c, op, sizeof(op));

    c->r.c = 0x22446688;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.c, 0);
    TEST(__FILE__, __LINE__, c->f.z, 1);

    c->r.c = 0x22446689;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.c, 1);
    TEST(__FILE__, __LINE__, c->f.z, 0);

    i = 0;
    memcpy(&c->m[0], &i, 4);
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "ZERO DIVIDE"), 0);
    return 0;
}

int main()
{
    struct vgscpu_context *c = (struct vgscpu_context *)vgscpu_create_context();
    if (!c) return -1;
    int result = -1;

    if (test_program_memory(c)) goto END_TEST;
    if (test_main_memory(c)) goto END_TEST;
    if (test_mod_c_1(c)) goto END_TEST;
    if (test_mod_c_2(c)) goto END_TEST;
    if (test_mod_c_4(c)) goto END_TEST;
    if (test_mod_c_a(c)) goto END_TEST;
    if (test_mod_c_b(c)) goto END_TEST;
    if (test_mod_c_d(c)) goto END_TEST;
    if (test_mod_c_m1(c)) goto END_TEST;
    if (test_mod_c_m2(c)) goto END_TEST;
    if (test_mod_c_m4(c)) goto END_TEST;

    result = 0;
    puts("success");
END_TEST:
    vgscpu_release_context(c);
    return result;
}