#include "tp.h"

int test_load_program(struct vgscpu_context *c)
{
    void *m = malloc(VGSCPU_PROGRAM_SIZE_DEFAULT + 1);
    TEST(__FILE__, __LINE__, vgscpu_load_program(c, m, VGSCPU_PROGRAM_SIZE_DEFAULT), 0);
    TEST(__FILE__, __LINE__, c->psize, VGSCPU_PROGRAM_SIZE_DEFAULT);
    TEST(__FILE__, __LINE__, vgscpu_load_program(c, m, VGSCPU_PROGRAM_SIZE_DEFAULT + 1), -1);
    TEST(__FILE__, __LINE__, strcmp(c->error, "TOO BIG PROGRAM"), 0);
    TEST(__FILE__, __LINE__, c->psize, VGSCPU_PROGRAM_SIZE_DEFAULT);
    free(m);
    return 0;
}

int main()
{
    struct vgscpu_context *c = (struct vgscpu_context *)vgscpu_create_context();
    if (!c) return -1;
    int result = -1;

    if (test_load_program(c)) goto END_TEST;

    result = 0;
    puts("success");
END_TEST:
    vgscpu_release_context(c);
    return result;
}