#include <stdio.h>
#include <string.h>
#include "vgscpu.h"
#include "vgscpu_internal.h"
#define FAILED(F,L) {printf("test-failed: function=%s, line=%d\n", F,L); return -1;}

int test_ld_a_literal(struct vgscpu_context* c) {
    const char* TAG = "text_push";
    unsigned short s;
    unsigned int i;
    unsigned char op1[] = {
        VGSCPU_OP_LD_A_1,
        0xCD,
        VGSCPU_OP_BRK
    };
    unsigned char op2[] = {
        VGSCPU_OP_LD_A_2,
        0xAB,
        0xCD,
        VGSCPU_OP_BRK
    };
    unsigned char op3[] = {
        VGSCPU_OP_LD_A_4,
        0x67,
        0x89,
        0xAB,
        0xCD,
        VGSCPU_OP_BRK
    };

    vgscpu_load_program(c, op1, sizeof(op1));
    vgscpu_run(c);
    if (c->r.a != 0x000000CD) FAILED(TAG, __LINE__);

    vgscpu_load_program(c, op2, sizeof(op2));
    vgscpu_run(c);
    if (c->r.a != 0x0000CDAB) FAILED(TAG, __LINE__);

    vgscpu_load_program(c, op3, sizeof(op3));
    vgscpu_run(c);
    if (c->r.a != 0xCDAB8967) FAILED(TAG, __LINE__);
    return 0;
}

int main() {
    struct vgscpu_context* c = (struct vgscpu_context*)vgscpu_create_context();
    if (!c) return -1;
    int result = -1;

    if (test_ld_a_literal(c)) goto END_TEST;

    result = 0;
    puts("success");
END_TEST:
    vgscpu_release_context(c);
    return result;
}