#ifndef INCLUDE_VGSCPU_H
#define INCLUDE_VGSCPU_H
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

struct vgscpu_register {
    unsigned int a; /* accumulator */
    unsigned int b; /* base */
    unsigned int c; /* counter */
    unsigned int d; /* data */
    unsigned int s; /* stack pointer */
    unsigned int p; /* program counter */
};

struct vgscpu_status_flag {
    unsigned int z; /* zero flag */
    unsigned int q; /* equal flag (0: euqal, -1: negative, 1:positive) */
};

struct vgscpu_context {
    char error[1024];            /* error info */
    struct vgscpu_register r;    /* register */
    struct vgscpu_status_flag f; /* flag */
    unsigned int psize;          /* program size */
    unsigned int sizeP;          /* program limit size */
    unsigned int sizeS;          /* stack size */
    unsigned int sizeM;          /* main memory size */
    unsigned char *p;            /* program */
    unsigned char *s;            /* stack */
    unsigned char *m;            /* main memory */
    int (*interrupt[256])(struct vgscpu_context *);
};

void *vgscpu_create_context();
void *vgscpu_create_specific_context(unsigned int ps, unsigned int ss, unsigned int ms);
void vgscpu_release_context(void *ctx);
int vgscpu_load_program(void *ctx, void *pg, size_t size);
void vgscpu_regist_interrupt(void *ctx, unsigned char n, int (*interrupt)(struct vgscpu_context *));
int vgscpu_run(void *ctx);
const char *vgscpu_get_last_error(void *ctx);

#ifdef __cplusplus
};
#endif

#endif
