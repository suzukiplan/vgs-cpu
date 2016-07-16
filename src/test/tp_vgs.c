#include "tp.h"

int test(struct vgscpu_context *c)
{
    unsigned char op1[] = {VGSCPU_OP_LD_D_1, 0xff, VGSCPU_OP_VGS, VGSCPU_OP_BRK};
    unsigned char op2[] = {VGSCPU_OP_LD_D_1, 0xff, VGSCPU_OP_VGS, 0x00, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op1, sizeof(op1));
    TEST(__FILE__, __LINE__, vgscpu_run(c), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "OUT OF PROGRAM MEMORY: $00000004"), 0);

    vgscpu_load_program(c, op2, sizeof(op2));
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0);
    return 0;
}

int main()
{
    struct vgscpu_context *c = (struct vgscpu_context *)vgscpu_create_context();
    if (!c) return -1;
    int result = -1;

    if (test(c)) goto END_TEST;

    result = 0;
    puts("success");
END_TEST:
    vgscpu_release_context(c);
    return result;
}