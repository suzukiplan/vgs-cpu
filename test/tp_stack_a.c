#include <stdio.h>
#include <string.h>
#include "vgscpu.h"
#include "vgscpu_internal.h"
#define FAILED(F, L)                                         \
    {                                                        \
        printf("test-failed: function=%s, line=%d\n", F, L); \
        return -1;                                           \
    }

int test_push(struct vgscpu_context *c)
{
    const char *TAG = "test_push";
    unsigned short s;
    unsigned int i;
    unsigned char op[] = {VGSCPU_OP_PUSH_A1, VGSCPU_OP_PUSH_A2, VGSCPU_OP_PUSH_A4, VGSCPU_OP_BRK};

    c->r.a = 0x12345678;
    vgscpu_load_program(c, op, sizeof(op));
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);

    if (c->r.a != 0x12345678) FAILED(TAG, __LINE__);
    if (c->r.s != 7) FAILED(TAG, __LINE__);
    if (c->s[0] != 0x78) FAILED(TAG, __LINE__);
    s = 0x5678;
    if (memcmp(&c->s[1], &s, 2)) FAILED(TAG, __LINE__);
    i = 0x12345678;
    if (memcmp(&c->s[3], &i, 4)) FAILED(TAG, __LINE__);
    return 0;
}

int error_push_stack_overflow_1(struct vgscpu_context *c)
{
    const char *TAG = "error_push_stack_overflow_1";
    unsigned char op[] = {VGSCPU_OP_PUSH_A1, VGSCPU_OP_INC_A, VGSCPU_OP_JMP, 0x00, 0x00, 0x00, 0x00};

    vgscpu_load_program(c, op, sizeof(op));
    c->r.a = 0;
    if (!vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (0 != strcmp(c->error, "STACK OVERFLOW")) FAILED(TAG, __LINE__);
    if (VGSCPU_STACK_SIZE - 1 != c->r.a) FAILED(TAG, __LINE__);

    return 0;
}

int error_push_stack_overflow_2(struct vgscpu_context *c)
{
    const char *TAG = "error_push_stack_overflow_2";
    unsigned char op[] = {VGSCPU_OP_PUSH_A2, VGSCPU_OP_INC_A, VGSCPU_OP_JMP, 0x00, 0x00, 0x00, 0x00};

    vgscpu_load_program(c, op, sizeof(op));
    c->r.a = 0;
    if (!vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (0 != strcmp(c->error, "STACK OVERFLOW")) FAILED(TAG, __LINE__);
    if (VGSCPU_STACK_SIZE / 2 - 1 != c->r.a) FAILED(TAG, __LINE__);

    return 0;
}

int error_push_stack_overflow_4(struct vgscpu_context *c)
{
    const char *TAG = "error_push_stack_overflow_4";
    unsigned char op[] = {VGSCPU_OP_PUSH_A4, VGSCPU_OP_INC_A, VGSCPU_OP_JMP, 0x00, 0x00, 0x00, 0x00};

    vgscpu_load_program(c, op, sizeof(op));
    c->r.a = 0;
    if (!vgscpu_run(c)) FAILED(TAG, __LINE__);
    if (0 != strcmp(c->error, "STACK OVERFLOW")) FAILED(TAG, __LINE__);
    if (VGSCPU_STACK_SIZE / 4 - 1 != c->r.a) FAILED(TAG, __LINE__);

    return 0;
}

int test_pop1(struct vgscpu_context *c)
{
    const char *TAG = "test_pop1";
    unsigned short s;
    unsigned int i;
    unsigned char op[] = {VGSCPU_OP_PUSH_A1, VGSCPU_OP_POP_A1, VGSCPU_OP_BRK};

    c->r.a = 0x12345678;
    vgscpu_load_program(c, op, sizeof(op));
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);

    if (c->r.a != 0x78) FAILED(TAG, __LINE__);
    if (c->r.s != 0) FAILED(TAG, __LINE__);
    return 0;
}

int test_pop2(struct vgscpu_context *c)
{
    const char *TAG = "test_pop2";
    unsigned short s;
    unsigned int i;
    unsigned char op[] = {VGSCPU_OP_PUSH_A2, VGSCPU_OP_POP_A2, VGSCPU_OP_BRK};

    c->r.a = 0x12345678;
    vgscpu_load_program(c, op, sizeof(op));
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);

    if (c->r.a != 0x5678) FAILED(TAG, __LINE__);
    if (c->r.s != 0) FAILED(TAG, __LINE__);
    return 0;
}

int test_pop4(struct vgscpu_context *c)
{
    const char *TAG = "test_pop4";
    unsigned short s;
    unsigned int i;
    unsigned char op[] = {VGSCPU_OP_PUSH_A4, VGSCPU_OP_POP_A4, VGSCPU_OP_BRK};

    c->r.a = 0x12345678;
    vgscpu_load_program(c, op, sizeof(op));
    if (vgscpu_run(c)) FAILED(TAG, __LINE__);

    if (c->r.a != 0x12345678) FAILED(TAG, __LINE__);
    if (c->r.s != 0) FAILED(TAG, __LINE__);
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