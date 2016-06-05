#include "tp.h"

int test_jmp(struct vgscpu_context *c)
{
    unsigned char op[] = {VGSCPU_OP_LD_A_1, 0xaa, VGSCPU_OP_JMP, 0x09, 0x00, 0x00, 0x00, VGSCPU_OP_LD_A_1, 0xbb, VGSCPU_OP_LD_B_1, 0xbb, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));

    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.a, 0xaa);
    TEST(__FILE__, __LINE__, c->r.b, 0xbb);
    return 0;
}

int test_jmp_1(struct vgscpu_context *c)
{
    unsigned char op[] = {VGSCPU_OP_LD_A_1, 0xaa, VGSCPU_OP_JMP_1, 0x02, VGSCPU_OP_LD_A_1, 0xbb, VGSCPU_OP_LD_B_1, 0xbb, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));

    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.a, 0xaa);
    TEST(__FILE__, __LINE__, c->r.b, 0xbb);
    return 0;
}

int test_jmp_2(struct vgscpu_context *c)
{
    unsigned char op[] = {VGSCPU_OP_LD_A_1, 0xaa, VGSCPU_OP_JMP_2, 0x02, 0x00, VGSCPU_OP_LD_A_1, 0xbb, VGSCPU_OP_LD_B_1, 0xbb, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));

    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.a, 0xaa);
    TEST(__FILE__, __LINE__, c->r.b, 0xbb);
    return 0;
}

int main()
{
    struct vgscpu_context *c = (struct vgscpu_context *)vgscpu_create_context();
    if (!c) return -1;
    int result = -1;

    if (test_jmp(c)) goto END_TEST;
    if (test_jmp_1(c)) goto END_TEST;
    if (test_jmp_2(c)) goto END_TEST;

    result = 0;
    puts("success");
END_TEST:
    vgscpu_release_context(c);
    return result;
}