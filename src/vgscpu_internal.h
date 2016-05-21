#define VGSCPU_MEMORY_SIZE 4194304
#define VGSCPU_STACK_SIZE 262144
#define VGSCPU_PROGRAM_SIZE 65536

#define VGSCPU_OP_BRK 0x00      /* break */

/* stack and load/store (a) */
#define VGSCPU_OP_PUSH_A1 0x01  /* push: a -> stack (1byte) */
#define VGSCPU_OP_PUSH_A2 0x02  /* push: a -> stack (2byte) */
#define VGSCPU_OP_PUSH_A4 0x03  /* push: a -> stack (4byte) */
#define VGSCPU_OP_POP_A1 0x04   /* pop: stack -> a (1byte) */
#define VGSCPU_OP_POP_A2 0x05   /* pop: stack -> a (2byte) */
#define VGSCPU_OP_POP_A4 0x06   /* pop: stack -> a (4byte) */
#define VGSCPU_OP_LD_A1 0x07    /* load: memory -> a (1byte) */
#define VGSCPU_OP_LD_A2 0x08    /* load: memory -> a (2byte) */
#define VGSCPU_OP_LD_A4 0x09    /* load: memory -> a (4byte) */
#define VGSCPU_OP_ST_A1 0x0A    /* store: a -> memory (1byte) */
#define VGSCPU_OP_ST_A2 0x0B    /* store: a -> memory (2byte) */
#define VGSCPU_OP_ST_A4 0x0C    /* store: a -> memory (4byte) */
#define VGSCPU_OP_INC_A 0x0D    /* increment: a++ */
#define VGSCPU_OP_DEC_A 0x0E    /* decrement: a-- */
#define VGSCPU_OP_NOT_A 0x0F    /* not: a = !a */

/* stack and load/store (b) */
#define VGSCPU_OP_PUSH_B1 0x11  /* push: b -> stack (1byte) */
#define VGSCPU_OP_PUSH_B2 0x12  /* push: b -> stack (2byte) */
#define VGSCPU_OP_PUSH_B4 0x13  /* push: b -> stack (4byte) */
#define VGSCPU_OP_POP_B1 0x14   /* pop: stack -> b (1byte) */
#define VGSCPU_OP_POP_B2 0x15   /* pop: stack -> b (2byte) */
#define VGSCPU_OP_POP_B4 0x16   /* pop: stack -> b (4byte) */
#define VGSCPU_OP_LD_B1 0x17    /* load: memory -> b (1byte) */
#define VGSCPU_OP_LD_B2 0x18    /* load: memory -> b (2byte) */
#define VGSCPU_OP_LD_B4 0x19    /* load: memory -> b (4byte) */
#define VGSCPU_OP_ST_B1 0x1A    /* store: b -> memory (1byte) */
#define VGSCPU_OP_ST_B2 0x1B    /* store: b -> memory (2byte) */
#define VGSCPU_OP_ST_B4 0x1C    /* store: b -> memory (4byte) */
#define VGSCPU_OP_INC_B 0x1D    /* increment: b++ */
#define VGSCPU_OP_DEC_B 0x1E    /* decrement: b-- */
#define VGSCPU_OP_NOT_B 0x1F    /* not: b = !b */

/* stack and load/store (c) */
#define VGSCPU_OP_PUSH_C1 0x21  /* push: c -> stack (1byte) */
#define VGSCPU_OP_PUSH_C2 0x22  /* push: c -> stack (2byte) */
#define VGSCPU_OP_PUSH_C4 0x23  /* push: c -> stack (4byte) */
#define VGSCPU_OP_POP_C1 0x24   /* pop: stack -> c (1byte) */
#define VGSCPU_OP_POP_C2 0x25   /* pop: stack -> c (2byte) */
#define VGSCPU_OP_POP_C4 0x26   /* pop: stack -> c (4byte) */
#define VGSCPU_OP_LD_C1 0x27    /* load: memory -> c (1byte) */
#define VGSCPU_OP_LD_C2 0x28    /* load: memory -> c (2byte) */
#define VGSCPU_OP_LD_C4 0x29    /* load: memory -> c (4byte) */
#define VGSCPU_OP_ST_C1 0x2A    /* store: c -> memory (1byte) */
#define VGSCPU_OP_ST_C2 0x2B    /* store: c -> memory (2byte) */
#define VGSCPU_OP_ST_C4 0x2C    /* store: c -> memory (4byte) */
#define VGSCPU_OP_INC_C 0x2D    /* increment: c++ */
#define VGSCPU_OP_DEC_C 0x2E    /* decrement: c-- */
#define VGSCPU_OP_NOT_C 0x2F    /* not: c = !c */

/* stack and load/store (d) */
#define VGSCPU_OP_PUSH_D1 0x31  /* push: d -> stack (1byte) */
#define VGSCPU_OP_PUSH_D2 0x32  /* push: d -> stack (2byte) */
#define VGSCPU_OP_PUSH_D4 0x33  /* push: d -> stack (4byte) */
#define VGSCPU_OP_POP_D1 0x34   /* pop: stack -> d (1byte) */
#define VGSCPU_OP_POP_D2 0x35   /* pop: stack -> d (2byte) */
#define VGSCPU_OP_POP_D4 0x36   /* pop: stack -> d (4byte) */
#define VGSCPU_OP_LD_D1 0x37    /* load: memory -> d (1byte) */
#define VGSCPU_OP_LD_D2 0x38    /* load: memory -> d (2byte) */
#define VGSCPU_OP_LD_D4 0x39    /* load: memory -> d (4byte) */
#define VGSCPU_OP_ST_D1 0x3A    /* store: d -> memory (1byte) */
#define VGSCPU_OP_ST_D2 0x3B    /* store: d -> memory (2byte) */
#define VGSCPU_OP_ST_D4 0x3C    /* store: d -> memory (4byte) */
#define VGSCPU_OP_INC_D 0x3D    /* increment: d++ */
#define VGSCPU_OP_DEC_D 0x3E    /* decrement: d-- */
#define VGSCPU_OP_NOT_D 0x3F    /* not: d = !d */

/* load reg & and */
#define VGSCPU_OP_LD_AB 0x40    /* load: b -> a */
#define VGSCPU_OP_LD_AC 0x41    /* load: c -> a */
#define VGSCPU_OP_LD_AD 0x42    /* load: d -> a */
#define VGSCPU_OP_LD_BA 0x43    /* load: a -> b */
#define VGSCPU_OP_LD_BC 0x44    /* load: c -> b */
#define VGSCPU_OP_LD_BD 0x45    /* load: d -> b */
#define VGSCPU_OP_LD_CA 0x46    /* load: a -> c */
#define VGSCPU_OP_LD_CB 0x47    /* load: b -> c */
#define VGSCPU_OP_LD_CD 0x48    /* load: d -> c */
#define VGSCPU_OP_LD_DA 0x49    /* load: a -> d */
#define VGSCPU_OP_LD_DB 0x4A    /* load: b -> d */
#define VGSCPU_OP_LD_DC 0x4B    /* load: c -> d */

/* shift */
#define VGSCPU_OP_SHL_A 0x4C    /* left-shift: a */
#define VGSCPU_OP_SHL_B 0x4D    /* left-shift: b */
#define VGSCPU_OP_SHL_C 0x4E    /* left-shift: c */
#define VGSCPU_OP_SHL_D 0x4F    /* left-shift: d */
#define VGSCPU_OP_SHR_A 0x50    /* right-shift: a */
#define VGSCPU_OP_SHR_B 0x51    /* right-shift: b */
#define VGSCPU_OP_SHR_C 0x52    /* right-shift: c */
#define VGSCPU_OP_SHR_D 0x53    /* right-shift: d */

/* add */
#define VGSCPU_OP_ADD_A_1 0x54  /* add: a = a + literal(1byte) */
#define VGSCPU_OP_ADD_A_2 0x55  /* add: a = a + literal(2byte) */
#define VGSCPU_OP_ADD_A_4 0x56  /* add: a = a + literal(4byte) */
#define VGSCPU_OP_ADD_A_B 0x57  /* add: a = a + b */
#define VGSCPU_OP_ADD_A_C 0x58  /* add: a = a + c */
#define VGSCPU_OP_ADD_A_D 0x59  /* add: a = a + d */
#define VGSCPU_OP_ADD_A_M1 0x5A /* add: a = a + &memory[ptr](1byte) */
#define VGSCPU_OP_ADD_A_M2 0x5B /* add: a = a + &memory[ptr](2byte) */
#define VGSCPU_OP_ADD_A_M3 0x5C /* add: a = a + &memory[ptr](2byte) */

/* sub */
#define VGSCPU_OP_SUB_A_1 0x5D  /* sub: a = a - literal(1byte) */
#define VGSCPU_OP_SUB_A_2 0x5E  /* sub: a = a - literal(2byte) */
#define VGSCPU_OP_SUB_A_4 0x5F  /* sub: a = a - literal(4byte) */
#define VGSCPU_OP_SUB_A_B 0x60  /* sub: a = a - b */
#define VGSCPU_OP_SUB_A_C 0x61  /* sub: a = a - c */
#define VGSCPU_OP_SUB_A_D 0x62  /* sub: a = a - d */
#define VGSCPU_OP_SUB_A_M1 0x63 /* sub: a = a - &memory[ptr](1byte) */
#define VGSCPU_OP_SUB_A_M2 0x64 /* sub: a = a - &memory[ptr](2byte) */
#define VGSCPU_OP_SUB_A_M3 0x65 /* sub: a = a - &memory[ptr](4byte) */

/* mul */
#define VGSCPU_OP_MUL_A_1 0x66  /* mul: a = a * literal(1byte) */
#define VGSCPU_OP_MUL_A_2 0x67  /* mul: a = a * literal(2byte) */
#define VGSCPU_OP_MUL_A_4 0x68  /* mul: a = a * literal(4byte) */
#define VGSCPU_OP_MUL_A_B 0x69  /* mul: a = a * b */
#define VGSCPU_OP_MUL_A_C 0x6A  /* mul: a = a * c */
#define VGSCPU_OP_MUL_A_D 0x6B  /* mul: a = a * d */
#define VGSCPU_OP_MUL_A_M1 0x6C /* mul: a = a * &memory[ptr](1byte) */
#define VGSCPU_OP_MUL_A_M2 0x6D /* mul: a = a * &memory[ptr](2byte) */
#define VGSCPU_OP_MUL_A_M3 0x6E /* mul: a = a * &memory[ptr](4byte) */

/* div */
#define VGSCPU_OP_DIV_A_1 0x6F  /* div: a = a / literal(1byte) */
#define VGSCPU_OP_DIV_A_2 0x70  /* div: a = a / literal(2byte) */
#define VGSCPU_OP_DIV_A_4 0x71  /* div: a = a / literal(4byte) */
#define VGSCPU_OP_DIV_A_B 0x72  /* div: a = a / b */
#define VGSCPU_OP_DIV_A_C 0x73  /* div: a = a / c */
#define VGSCPU_OP_DIV_A_D 0x74  /* div: a = a / d */
#define VGSCPU_OP_DIV_A_M1 0x75 /* div: a = a / &memory[ptr](1byte) */
#define VGSCPU_OP_DIV_A_M2 0x76 /* div: a = a / &memory[ptr](2byte) */
#define VGSCPU_OP_DIV_A_M3 0x77 /* div: a = a / &memory[ptr](4byte) */

/* and */
#define VGSCPU_OP_AND_A_1 0x78  /* and: a = a & literal(1byte) */
#define VGSCPU_OP_AND_A_2 0x79  /* and: a = a & literal(2byte) */
#define VGSCPU_OP_AND_A_4 0x7A  /* and: a = a & literal(4byte) */
#define VGSCPU_OP_AND_A_B 0x7B  /* and: a = a & b */
#define VGSCPU_OP_AND_A_C 0x7C  /* and: a = a & c */
#define VGSCPU_OP_AND_A_D 0x7D  /* and: a = a & d */
#define VGSCPU_OP_AND_A_M1 0x7E /* and: a = a & &memory[ptr](1byte) */
#define VGSCPU_OP_AND_A_M2 0x7F /* and: a = a & &memory[ptr](2byte) */
#define VGSCPU_OP_AND_A_M3 0x80 /* and: a = a & &memory[ptr](4byte) */

/* or */
#define VGSCPU_OP_OR_A_1 0x81   /* or: a = a | literal(1byte) */
#define VGSCPU_OP_OR_A_2 0x82   /* or: a = a | literal(2byte) */
#define VGSCPU_OP_OR_A_4 0x83   /* or: a = a | literal(4byte) */
#define VGSCPU_OP_OR_A_B 0x84   /* or: a = a | b */
#define VGSCPU_OP_OR_A_C 0x85   /* or: a = a | c */
#define VGSCPU_OP_OR_A_D 0x86   /* or: a = a | d */
#define VGSCPU_OP_OR_A_M1 0x87  /* or: a = a | &memory[ptr](1byte) */
#define VGSCPU_OP_OR_A_M2 0x88  /* or: a = a | &memory[ptr](2byte) */
#define VGSCPU_OP_OR_A_M3 0x89  /* or: a = a | &memory[ptr](4byte) */

/* xor */
#define VGSCPU_OP_XOR_A_1 0x8A  /* xor: a = a ^ literal(1byte) */
#define VGSCPU_OP_XOR_A_2 0x8B  /* xor: a = a ^ literal(2byte) */
#define VGSCPU_OP_XOR_A_4 0x8C  /* xor: a = a ^ literal(4byte) */
#define VGSCPU_OP_XOR_A_B 0x8D  /* xor: a = a ^ b */
#define VGSCPU_OP_XOR_A_C 0x8E  /* xor: a = a ^ c */
#define VGSCPU_OP_XOR_A_D 0x8F  /* xor: a = a ^ d */
#define VGSCPU_OP_XOR_A_M1 0x90 /* xor: a = a ^ &memory[ptr](1byte) */
#define VGSCPU_OP_XOR_A_M2 0x91 /* xor: a = a ^ &memory[ptr](2byte) */
#define VGSCPU_OP_XOR_A_M3 0x92 /* xor: a = a ^ &memory[ptr](4byte) */

/* compare (result: vgscpu_status_flag.eq) */
#define VGSCPU_CMP_A_1 0x93     /* cmp: a and literal(1byte) */
#define VGSCPU_CMP_A_2 0x94     /* cmp: a and literal(2byte) */
#define VGSCPU_CMP_A_4 0x95     /* cmp: a and literal(4byte) */
#define VGSCPU_CMP_A_B 0x96     /* cmp: a and b */
#define VGSCPU_CMP_A_C 0x97     /* cmp: a and c */
#define VGSCPU_CMP_A_D 0x98     /* cmp: a and d */
#define VGSCPU_CMP_A_M1 0x99    /* cmp: a and &memory[ptr](1byte) */
#define VGSCPU_CMP_A_M2 0x9A    /* cmp: a and &memory[ptr](2byte) */
#define VGSCPU_CMP_A_M3 0x9B    /* cmp: a and &memory[ptr](4byte) */

/* branch */
#define VGSCPU_OP_JMP 0x9C      /* jump to specific address */
#define VGSCPU_OP_JMP_1 0x9D    /* jump to current address + literal(1byte) */
#define VGSCPU_OP_JMP_2 0x9E    /* jump to current address + literal(2byte) */
#define VGSCPU_OP_JZ 0x9F       /* jump when zero flag set */
#define VGSCPU_OP_JZ_1 0xA0     /* jump when zero flag set */
#define VGSCPU_OP_JZ_2 0xA1     /* jump when zero flag set */
#define VGSCPU_OP_JNZ 0xA2      /* jump when zero flag reset */
#define VGSCPU_OP_JNZ_1 0xA3    /* jump when zero flag reset */
#define VGSCPU_OP_JNZ_2 0xA4    /* jump when zero flag reset */
#define VGSCPU_OP_JE 0xA5       /* jump when equal flag = 0 */
#define VGSCPU_OP_JE_1 0xA6     /* jump when equal flag = 0 */
#define VGSCPU_OP_JE_2 0xA7     /* jump when equal flag = 0 */
#define VGSCPU_OP_JNE 0xA8      /* jump when equal flag != 0 */
#define VGSCPU_OP_JNE_1 0xA9    /* jump when equal flag != 0 */
#define VGSCPU_OP_JNE_2 0xAA    /* jump when equal flag != 0 */
#define VGSCPU_OP_JN 0xAB       /* jump when equal flag = -1 */
#define VGSCPU_OP_JN_1 0xAC     /* jump when equal flag = -1 */
#define VGSCPU_OP_JN_2 0xAD     /* jump when equal flag = -1 */
#define VGSCPU_OP_JNN 0xAE      /* jump when equal flag != -1 */
#define VGSCPU_OP_JNN_1 0xAF    /* jump when equal flag != -1 */
#define VGSCPU_OP_JNN_2 0xB0    /* jump when equal flag != -1 */
#define VGSCPU_OP_JP 0xB1       /* jump when equal flag = 1 */
#define VGSCPU_OP_JP_1 0xB2     /* jump when equal flag = 1 */
#define VGSCPU_OP_JP_2 0xB3     /* jump when equal flag = 1 */
#define VGSCPU_OP_JNP 0xB4      /* jump when equal flag != 1 */
#define VGSCPU_OP_JNP_1 0xB5    /* jump when equal flag != 1 */
#define VGSCPU_OP_JNP_2 0xB6    /* jump when equal flag != 1 */
#define VGSCPU_OP_CAL 0xB7      /* call to specific address */
#define VGSCPU_OP_CAL_1 0xB8    /* call to current address + literal(1byte) */
#define VGSCPU_OP_CAL_2 0xB9    /* call to current address + literal(2byte) */
#define VGSCPU_OP_RET 0xBA      /* return to called */
#define VGSCPU_OP_VGS 0xBB      /* execute VGS-API */

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
    char error[1024];                       /* error info */
    struct vgs_register r;                  /* register */
    struct vgscpu_status_flag f;            /* flag */
    unsigned char m[VGSCPU_MEMORY_SIZE];    /* main memory */
    unsigned char s[VGSCPU_STACK_SIZE];     /* stack */
    unsigned char p[VGSCPU_PROGRAM_SIZE];   /* program */
};
