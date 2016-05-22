#include "vgscpu.h"
#include "vgscpu_internal.h"
#define FAILED(F,L) {printf("test-failed: function=%s, line=%d\n", F,L); return -1;}

int text_push(vgscpu_context* c) {
    const char* TAG = "text_push";
    unsigned short s;
    unsigned int i;
    unsigned char op[] = {
        VGSCPU_OP_PUSH_A1,
        VGSCPU_OP_PUSH_A2,
        VGSCPU_OP_PUSH_A4,
        VGSCPU_OP_BRK
    };

    c->r.a = 0x12345678;
    vgscpu_load_program(c, op, sizeof(op));
    vgscpu_run(c);

    if (c->r.a != 0x12345678) FAILED(TAG, __LINE__);
    if (c->r.s != 7) FAILED(TAG, __LINE__);
    if (c->r.m[0] != 0x78) FAILED(TAG, __LINE__);
    s = 0x5678;
    if (memcmp(&c->r.m[1], &s, 2) FAILED(TAG, __LINE__);
    i = 0x12345678;
    if (memcmp(&c->r.m[3], &i, 4) FAILED(TAG, __LINE__);
    return 0;
}

int main() {
    struct vgscpu_context* c = (struct vgscpu_context*)vgscpu_create_context();
    if (!ctx) return -1;
    int result = -1;

    if (text_push(c)) goto END_TEST;

    result = 0;
END_TEST:
    vgscpu_release_context(c);
    return result;
}