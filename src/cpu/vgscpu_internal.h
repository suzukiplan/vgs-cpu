#include "vgscpu_op.h"
#include "vgscpu_op_acu_b.h"
#include "vgscpu_op_acu_c.h"
#include "vgscpu_op_acu_d.h"

#define VGSCPU_MEMORY_SIZE_DEFAULT 4194304
#define VGSCPU_STACK_SIZE_DEFAULT 262144
#define VGSCPU_PROGRAM_SIZE_DEFAULT 65536 - 4

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
    unsigned char* p;            /* program */
    unsigned char* s;            /* stack */
    unsigned char* m;            /* main memory */
};
