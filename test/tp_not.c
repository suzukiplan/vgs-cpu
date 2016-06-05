#include "tp.h"

int test_not_a(struct vgscpu_context *c)
{
    const char *TAG = "test_not_a";
    unsigned char op[] = {VGSCPU_OP_NOT_A, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));
    c->r.a = 0x0;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.a, 0xffffffff);
    TEST(__FILE__, __LINE__, c->f.z, 0);

    c->r.a = 0xffffffff;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.a, 0);
    TEST(__FILE__, __LINE__, c->f.z, 1);

    c->r.a = 0xf0f0f0f0;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.a, 0x0f0f0f0f);
    TEST(__FILE__, __LINE__, c->f.z, 0);

    c->r.a = 0x0f0f0f0f;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.a, 0xf0f0f0f0);
    TEST(__FILE__, __LINE__, c->f.z, 0);

    c->r.a = 0x12345678;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.a, 0xedcba987);
    TEST(__FILE__, __LINE__, c->f.z, 0);

    c->r.a = 0xedcba987;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.a, 0x12345678);
    TEST(__FILE__, __LINE__, c->f.z, 0);
    return 0;
}

int test_not_b(struct vgscpu_context *c)
{
    const char *TAG = "test_not_b";
    unsigned char op[] = {VGSCPU_OP_NOT_B, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));
    c->r.b = 0x0;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.b, 0xffffffff);
    TEST(__FILE__, __LINE__, c->f.z, 0);

    c->r.b = 0xffffffff;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.b, 0);
    TEST(__FILE__, __LINE__, c->f.z, 1);

    c->r.b = 0xf0f0f0f0;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.b, 0x0f0f0f0f);
    TEST(__FILE__, __LINE__, c->f.z, 0);

    c->r.b = 0x0f0f0f0f;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.b, 0xf0f0f0f0);
    TEST(__FILE__, __LINE__, c->f.z, 0);

    c->r.b = 0x12345678;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.b, 0xedcba987);
    TEST(__FILE__, __LINE__, c->f.z, 0);

    c->r.b = 0xedcba987;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.b, 0x12345678);
    TEST(__FILE__, __LINE__, c->f.z, 0);
    return 0;
}

int test_not_c(struct vgscpu_context *c)
{
    const char *TAG = "test_not_c";
    unsigned char op[] = {VGSCPU_OP_NOT_C, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));
    c->r.c = 0x0;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.c, 0xffffffff);
    TEST(__FILE__, __LINE__, c->f.z, 0);

    c->r.c = 0xffffffff;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.c, 0);
    TEST(__FILE__, __LINE__, c->f.z, 1);

    c->r.c = 0xf0f0f0f0;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.c, 0x0f0f0f0f);
    TEST(__FILE__, __LINE__, c->f.z, 0);

    c->r.c = 0x0f0f0f0f;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.c, 0xf0f0f0f0);
    TEST(__FILE__, __LINE__, c->f.z, 0);

    c->r.c = 0x12345678;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.c, 0xedcba987);
    TEST(__FILE__, __LINE__, c->f.z, 0);

    c->r.c = 0xedcba987;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.c, 0x12345678);
    TEST(__FILE__, __LINE__, c->f.z, 0);
    return 0;
}

int test_not_d(struct vgscpu_context *c)
{
    const char *TAG = "test_not_d";
    unsigned char op[] = {VGSCPU_OP_NOT_D, VGSCPU_OP_BRK};

    vgscpu_load_program(c, op, sizeof(op));
    c->r.d = 0x0;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0xffffffff);
    TEST(__FILE__, __LINE__, c->f.z, 0);

    c->r.d = 0xffffffff;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0);
    TEST(__FILE__, __LINE__, c->f.z, 1);

    c->r.d = 0xf0f0f0f0;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0x0f0f0f0f);
    TEST(__FILE__, __LINE__, c->f.z, 0);

    c->r.d = 0x0f0f0f0f;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0xf0f0f0f0);
    TEST(__FILE__, __LINE__, c->f.z, 0);

    c->r.d = 0x12345678;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0xedcba987);
    TEST(__FILE__, __LINE__, c->f.z, 0);

    c->r.d = 0xedcba987;
    TEST(__FILE__, __LINE__, vgscpu_run(c), 0);
    TEST(__FILE__, __LINE__, c->r.d, 0x12345678);
    TEST(__FILE__, __LINE__, c->f.z, 0);
    return 0;
}

int main()
{
    struct vgscpu_context *c = (struct vgscpu_context *)vgscpu_create_context();
    if (!c) return -1;
    int result = -1;

    if (test_not_a(c)) goto END_TEST;
    if (test_not_b(c)) goto END_TEST;
    if (test_not_c(c)) goto END_TEST;
    if (test_not_d(c)) goto END_TEST;

    result = 0;
    puts("success");
END_TEST:
    vgscpu_release_context(c);
    return result;
}