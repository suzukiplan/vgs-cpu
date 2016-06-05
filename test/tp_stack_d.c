#include "tp.h"

int test_push(struct vgscpu_context *c)
{
    unsigned short s;
    unsigned int i;
    unsigned char op[] = {VGSCPU_OP_PUSH_D1, VGSCPU_OP_PUSH_D2, VGSCPU_OP_PUSH_D4, VGSCPU_OP_BRK};

    c->r.d = 0x12345678;
    vgscpu_load_program(c, op, sizeof(op));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0x12345678);
    TEST(__FILE__, __LINE__, c->r.s, 7);
    TEST(__FILE__, __LINE__, c->s[0], 0x78);
    s = 0x5678;
    TEST(__FILE__, __LINE__, memcmp(&c->s[1], &s, 2), 0);
    i = 0x12345678;
    TEST(__FILE__, __LINE__, memcmp(&c->s[3], &i, 4), 0);
    return 0;
}

int error_push_stack_overflow_1(struct vgscpu_context *c)
{
    unsigned char op[] = {VGSCPU_OP_PUSH_D1, VGSCPU_OP_INC_A, VGSCPU_OP_JMP, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));
    c->r.a = 0;
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "STACK OVERFLOW"), 0);
    TEST(__FILE__, __LINE__, c->r.a, VGSCPU_STACK_SIZE - 1);
    return 0;
}

int error_push_stack_overflow_2(struct vgscpu_context *c)
{
    unsigned char op[] = {VGSCPU_OP_PUSH_D2, VGSCPU_OP_INC_A, VGSCPU_OP_JMP, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));
    c->r.a = 0;
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "STACK OVERFLOW"), 0);
    TEST(__FILE__, __LINE__, c->r.a, VGSCPU_STACK_SIZE / 2 - 1);
    return 0;
}

int error_push_stack_overflow_4(struct vgscpu_context *c)
{
    unsigned char op[] = {VGSCPU_OP_PUSH_D4, VGSCPU_OP_INC_A, VGSCPU_OP_JMP, 0x00, 0x00, 0x00, 0x00, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));
    c->r.a = 0;
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "STACK OVERFLOW"), 0);
    TEST(__FILE__, __LINE__, c->r.a, VGSCPU_STACK_SIZE / 4 - 1);
    return 0;
}

int test_pop1(struct vgscpu_context *c)
{
    unsigned short s;
    unsigned int i;
    unsigned char op[] = {VGSCPU_OP_PUSH_D1, VGSCPU_OP_POP_D1, VGSCPU_OP_BRK};

    c->r.d = 0x12345678;
    vgscpu_load_program(c, op, sizeof(op));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0x78);
    TEST(__FILE__, __LINE__, c->r.s, 0);
    return 0;
}

int test_pop2(struct vgscpu_context *c)
{
    unsigned short s;
    unsigned int i;
    unsigned char op[] = {VGSCPU_OP_PUSH_D2, VGSCPU_OP_POP_D2, VGSCPU_OP_BRK};

    c->r.d = 0x12345678;
    vgscpu_load_program(c, op, sizeof(op));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0x5678);
    TEST(__FILE__, __LINE__, c->r.s, 0);
    return 0;
}

int test_pop4(struct vgscpu_context *c)
{
    unsigned short s;
    unsigned int i;
    unsigned char op[] = {VGSCPU_OP_PUSH_D4, VGSCPU_OP_POP_D4, VGSCPU_OP_BRK};

    c->r.d = 0x12345678;
    vgscpu_load_program(c, op, sizeof(op));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0x12345678);
    TEST(__FILE__, __LINE__, c->r.s, 0);
    return 0;
}

int main()
{
    struct vgscpu_context *c = (struct vgscpu_context *)vgscpu_create_context();
    if (!c) return -1;
    int result = -1;

    if (test_push(c)) goto END_TEST;
    if (error_push_stack_overflow_1(c)) goto END_TEST;
    if (error_push_stack_overflow_2(c)) goto END_TEST;
    if (error_push_stack_overflow_4(c)) goto END_TEST;
    if (test_pop1(c)) goto END_TEST;
    if (test_pop2(c)) goto END_TEST;
    if (test_pop4(c)) goto END_TEST;

    result = 0;
    puts("success");
END_TEST:
    vgscpu_release_context(c);
    return result;
}