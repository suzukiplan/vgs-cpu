#include "tp.h"

int test(struct vgscpu_context *c)
{
    unsigned char op1[] = {VGSCPU_OP_CAL, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_CAL, 0x05, VGSCPU_OP_BRK};
    unsigned char op3[] = {VGSCPU_OP_CAL, 0x05, 0x00, VGSCPU_OP_BRK};
    unsigned char op4[] = {VGSCPU_OP_CAL, 0x05, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op5[] = {VGSCPU_OP_CAL, 0x05, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op6[] = {VGSCPU_OP_CAL, 0x05, 0x00, 0x00, 0x00, VGSCPU_OP_RET, VGSCPU_OP_BRK};
    unsigned char op7[] = {VGSCPU_OP_CAL, 0x06, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};
    unsigned char op8[] = {VGSCPU_OP_CAL, 0x06, 0x00, 0x00, 0x00, VGSCPU_OP_BRK, VGSCPU_OP_RET};
    unsigned char op9[] = {VGSCPU_OP_PUSH_A1, VGSCPU_OP_CAL, 0x07, 0x00, 0x00, 0x00, VGSCPU_OP_BRK, VGSCPU_OP_RET};
    unsigned char op10[] = {VGSCPU_OP_PUSH_A2, VGSCPU_OP_CAL, 0x07, 0x00, 0x00, 0x00, VGSCPU_OP_BRK, VGSCPU_OP_RET};
    unsigned char op11[] = {VGSCPU_OP_PUSH_A4, VGSCPU_OP_CAL, 0x07, 0x00, 0x00, 0x00, VGSCPU_OP_BRK, VGSCPU_OP_RET};

    vgscpu_load_program(c, op1, sizeof(op1));
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
    TEST(__FILE__, __LINE__, strcmp(c->error, "STACK UNDERFLOW"), 0);

    vgscpu_load_program(c, op7, sizeof(op7));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000006"), 0);

    vgscpu_load_program(c, op8, sizeof(op8));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);

    vgscpu_load_program(c, op9, sizeof(op9));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "STACK OVERFLOW"), 0);

    vgscpu_load_program(c, op10, sizeof(op10));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "STACK OVERFLOW"), 0);

    vgscpu_load_program(c, op11, sizeof(op11));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "STACK OVERFLOW"), 0);
    return 0;
}

int main()
{
    struct vgscpu_context *c = (struct vgscpu_context *)vgscpu_create_specific_context(16, 5, 16);
    if (!c) return -1;
    int result = -1;

    if (test(c)) goto END_TEST;

    result = 0;
    puts("success");
END_TEST:
    vgscpu_release_context(c);
    return result;
}