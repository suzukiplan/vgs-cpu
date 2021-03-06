#define VGSCPU_OP_BRK 0x00 /* break */

/* stack and load/store (a) */
#define VGSCPU_OP_PUSH_A1 0x01 /* push: a -> stack (1byte) */
#define VGSCPU_OP_PUSH_A2 0x02 /* push: a -> stack (2byte) */
#define VGSCPU_OP_PUSH_A4 0x03 /* push: a -> stack (4byte) */
#define VGSCPU_OP_POP_A1 0x04  /* pop: stack -> a (1byte) */
#define VGSCPU_OP_POP_A2 0x05  /* pop: stack -> a (2byte) */
#define VGSCPU_OP_POP_A4 0x06  /* pop: stack -> a (4byte) */
#define VGSCPU_OP_LD_A_1 0x07  /* load: n -> a (1byte) */
#define VGSCPU_OP_LD_A_2 0x08  /* load: n -> a (2byte) */
#define VGSCPU_OP_LD_A_4 0x09  /* load: n -> a (4byte) */
#define VGSCPU_OP_LD_A_B 0x0a  /* load: b -> a */
#define VGSCPU_OP_LD_A_C 0x0b  /* load: c -> a */
#define VGSCPU_OP_LD_A_D 0x0c  /* load: d -> a */
#define VGSCPU_OP_LD_A_M1 0x0d /* load: &memory[ptr](1byte) -> a */
#define VGSCPU_OP_LD_A_M2 0x0e /* load: &memory[ptr](1byte) -> a */
#define VGSCPU_OP_LD_A_M4 0x0f /* load: &memory[ptr](1byte) -> a */
#define VGSCPU_OP_ST_A_M1 0x10 /* store: a -> &memory[ptr](1byte) */
#define VGSCPU_OP_ST_A_M2 0x11 /* store: a -> &memory[ptr](2byte) */
#define VGSCPU_OP_ST_A_M4 0x12 /* store: a -> &memory[ptr](4byte) */
#define VGSCPU_OP_INC_A 0x13   /* increment: a++ */
#define VGSCPU_OP_DEC_A 0x14   /* decrement: a-- */
#define VGSCPU_OP_NOT_A 0x15   /* not: a = !a */

/* stack and load/store (b) */
#define VGSCPU_OP_PUSH_B1 0x16 /* push: b -> stack (1byte) */
#define VGSCPU_OP_PUSH_B2 0x17 /* push: b -> stack (2byte) */
#define VGSCPU_OP_PUSH_B4 0x18 /* push: b -> stack (4byte) */
#define VGSCPU_OP_POP_B1 0x19  /* pop: stack -> b (1byte) */
#define VGSCPU_OP_POP_B2 0x1a  /* pop: stack -> b (2byte) */
#define VGSCPU_OP_POP_B4 0x1b  /* pop: stack -> b (4byte) */
#define VGSCPU_OP_LD_B_1 0x1c  /* load: n -> b (1byte) */
#define VGSCPU_OP_LD_B_2 0x1d  /* load: n -> b (2byte) */
#define VGSCPU_OP_LD_B_4 0x1e  /* load: n -> b (4byte) */
#define VGSCPU_OP_LD_B_A 0x1f  /* load: a -> b */
#define VGSCPU_OP_LD_B_C 0x20  /* load: c -> b */
#define VGSCPU_OP_LD_B_D 0x21  /* load: d -> b */
#define VGSCPU_OP_LD_B_M1 0x22 /* load: &memory[ptr](1byte) -> b */
#define VGSCPU_OP_LD_B_M2 0x23 /* load: &memory[ptr](1byte) -> b */
#define VGSCPU_OP_LD_B_M4 0x24 /* load: &memory[ptr](1byte) -> b */
#define VGSCPU_OP_ST_B_M1 0x25 /* store: b -> &memory[ptr](1byte) */
#define VGSCPU_OP_ST_B_M2 0x26 /* store: b -> &memory[ptr](2byte) */
#define VGSCPU_OP_ST_B_M4 0x27 /* store: b -> &memory[ptr](4byte) */
#define VGSCPU_OP_INC_B 0x28   /* increment: b++ */
#define VGSCPU_OP_DEC_B 0x29   /* decrement: b-- */
#define VGSCPU_OP_NOT_B 0x2a   /* not: b = !b */

/* stack and load/store (c) */
#define VGSCPU_OP_PUSH_C1 0x2b /* push: c -> stack (1byte) */
#define VGSCPU_OP_PUSH_C2 0x2c /* push: c -> stack (2byte) */
#define VGSCPU_OP_PUSH_C4 0x2d /* push: c -> stack (4byte) */
#define VGSCPU_OP_POP_C1 0x2e  /* pop: stack -> c (1byte) */
#define VGSCPU_OP_POP_C2 0x2f  /* pop: stack -> c (2byte) */
#define VGSCPU_OP_POP_C4 0x30  /* pop: stack -> c (4byte) */
#define VGSCPU_OP_LD_C_1 0x31  /* load: n -> c (1byte) */
#define VGSCPU_OP_LD_C_2 0x32  /* load: n -> c (2byte) */
#define VGSCPU_OP_LD_C_4 0x33  /* load: n -> c (4byte) */
#define VGSCPU_OP_LD_C_A 0x34  /* load: a -> c */
#define VGSCPU_OP_LD_C_B 0x35  /* load: b -> c */
#define VGSCPU_OP_LD_C_D 0x36  /* load: d -> c */
#define VGSCPU_OP_LD_C_M1 0x37 /* load: &memory[ptr](1byte) -> c */
#define VGSCPU_OP_LD_C_M2 0x38 /* load: &memory[ptr](1byte) -> c */
#define VGSCPU_OP_LD_C_M4 0x39 /* load: &memory[ptr](1byte) -> c */
#define VGSCPU_OP_ST_C_M1 0x3a /* store: c -> &memory[ptr](1byte) */
#define VGSCPU_OP_ST_C_M2 0x3b /* store: c -> &memory[ptr](2byte) */
#define VGSCPU_OP_ST_C_M4 0x3c /* store: c -> &memory[ptr](4byte) */
#define VGSCPU_OP_INC_C 0x3d   /* increment: c++ */
#define VGSCPU_OP_DEC_C 0x3e   /* decrement: c-- */
#define VGSCPU_OP_NOT_C 0x3f   /* not: c = !c */

/* stack and load/store (d) */
#define VGSCPU_OP_PUSH_D1 0x40 /* push: d -> stack (1byte) */
#define VGSCPU_OP_PUSH_D2 0x41 /* push: d -> stack (2byte) */
#define VGSCPU_OP_PUSH_D4 0x42 /* push: d -> stack (4byte) */
#define VGSCPU_OP_POP_D1 0x43  /* pop: stack -> d (1byte) */
#define VGSCPU_OP_POP_D2 0x44  /* pop: stack -> d (2byte) */
#define VGSCPU_OP_POP_D4 0x45  /* pop: stack -> d (4byte) */
#define VGSCPU_OP_LD_D_1 0x46  /* load: n -> d (1byte) */
#define VGSCPU_OP_LD_D_2 0x47  /* load: n -> d (2byte) */
#define VGSCPU_OP_LD_D_4 0x48  /* load: n -> d (4byte) */
#define VGSCPU_OP_LD_D_A 0x49  /* load: a -> d */
#define VGSCPU_OP_LD_D_B 0x4a  /* load: b -> d */
#define VGSCPU_OP_LD_D_C 0x4b  /* load: c -> d */
#define VGSCPU_OP_LD_D_M1 0x4c /* load: &memory[ptr](1byte) -> d */
#define VGSCPU_OP_LD_D_M2 0x4d /* load: &memory[ptr](1byte) -> d */
#define VGSCPU_OP_LD_D_M4 0x4e /* load: &memory[ptr](1byte) -> d */
#define VGSCPU_OP_ST_D_M1 0x4f /* store: d -> &memory[ptr](1byte) */
#define VGSCPU_OP_ST_D_M2 0x50 /* store: d -> &memory[ptr](2byte) */
#define VGSCPU_OP_ST_D_M4 0x51 /* store: d -> &memory[ptr](4byte) */
#define VGSCPU_OP_INC_D 0x52   /* increment: d++ */
#define VGSCPU_OP_DEC_D 0x53   /* decrement: d-- */
#define VGSCPU_OP_NOT_D 0x54   /* not: d = !d */

/* shift */
#define VGSCPU_OP_SHL_A 0x55 /* left-shift: a (logical shift) */
#define VGSCPU_OP_SHL_B 0x56 /* left-shift: b (logical shift) */
#define VGSCPU_OP_SHL_C 0x57 /* left-shift: c (logical shift) */
#define VGSCPU_OP_SHL_D 0x58 /* left-shift: d (logical shift) */
#define VGSCPU_OP_SHR_A 0x59 /* right-shift: a (logical shift) */
#define VGSCPU_OP_SHR_B 0x5a /* right-shift: b (logical shift) */
#define VGSCPU_OP_SHR_C 0x5b /* right-shift: c (logical shift) */
#define VGSCPU_OP_SHR_D 0x5c /* right-shift: d (logical shift) */

/* add */
#define VGSCPU_OP_ADD_A_1 0x5d  /* add: a = a + literal(1byte) */
#define VGSCPU_OP_ADD_A_2 0x5e  /* add: a = a + literal(2byte) */
#define VGSCPU_OP_ADD_A_4 0x5f  /* add: a = a + literal(4byte) */
#define VGSCPU_OP_ADD_A_B 0x60  /* add: a = a + b */
#define VGSCPU_OP_ADD_A_C 0x61  /* add: a = a + c */
#define VGSCPU_OP_ADD_A_D 0x62  /* add: a = a + d */
#define VGSCPU_OP_ADD_A_M1 0x63 /* add: a = a + &memory[ptr](1byte) */
#define VGSCPU_OP_ADD_A_M2 0x64 /* add: a = a + &memory[ptr](2byte) */
#define VGSCPU_OP_ADD_A_M4 0x65 /* add: a = a + &memory[ptr](4byte) */

/* sub */
#define VGSCPU_OP_SUB_A_1 0x66  /* sub: a = a - literal(1byte) */
#define VGSCPU_OP_SUB_A_2 0x67  /* sub: a = a - literal(2byte) */
#define VGSCPU_OP_SUB_A_4 0x68  /* sub: a = a - literal(4byte) */
#define VGSCPU_OP_SUB_A_B 0x69  /* sub: a = a - b */
#define VGSCPU_OP_SUB_A_C 0x6a  /* sub: a = a - c */
#define VGSCPU_OP_SUB_A_D 0x6b  /* sub: a = a - d */
#define VGSCPU_OP_SUB_A_M1 0x6c /* sub: a = a - &memory[ptr](1byte) */
#define VGSCPU_OP_SUB_A_M2 0x6d /* sub: a = a - &memory[ptr](2byte) */
#define VGSCPU_OP_SUB_A_M4 0x6e /* sub: a = a - &memory[ptr](4byte) */

/* mul */
#define VGSCPU_OP_MUL_A_1 0x6f  /* mul: a = a * literal(1byte) */
#define VGSCPU_OP_MUL_A_2 0x70  /* mul: a = a * literal(2byte) */
#define VGSCPU_OP_MUL_A_4 0x71  /* mul: a = a * literal(4byte) */
#define VGSCPU_OP_MUL_A_B 0x72  /* mul: a = a * b */
#define VGSCPU_OP_MUL_A_C 0x73  /* mul: a = a * c */
#define VGSCPU_OP_MUL_A_D 0x74  /* mul: a = a * d */
#define VGSCPU_OP_MUL_A_M1 0x75 /* mul: a = a * &memory[ptr](1byte) */
#define VGSCPU_OP_MUL_A_M2 0x76 /* mul: a = a * &memory[ptr](2byte) */
#define VGSCPU_OP_MUL_A_M4 0x77 /* mul: a = a * &memory[ptr](4byte) */

/* div */
#define VGSCPU_OP_DIV_A_1 0x78  /* div: a = a / literal(1byte) */
#define VGSCPU_OP_DIV_A_2 0x79  /* div: a = a / literal(2byte) */
#define VGSCPU_OP_DIV_A_4 0x7a  /* div: a = a / literal(4byte) */
#define VGSCPU_OP_DIV_A_B 0x7b  /* div: a = a / b */
#define VGSCPU_OP_DIV_A_C 0x7c  /* div: a = a / c */
#define VGSCPU_OP_DIV_A_D 0x7d  /* div: a = a / d */
#define VGSCPU_OP_DIV_A_M1 0x7e /* div: a = a / &memory[ptr](1byte) */
#define VGSCPU_OP_DIV_A_M2 0x7f /* div: a = a / &memory[ptr](2byte) */
#define VGSCPU_OP_DIV_A_M4 0x80 /* div: a = a / &memory[ptr](4byte) */

/* mod */
#define VGSCPU_OP_MOD_A_1 0x81  /* mod: a = a % literal(1byte) */
#define VGSCPU_OP_MOD_A_2 0x82  /* mod: a = a % literal(2byte) */
#define VGSCPU_OP_MOD_A_4 0x83  /* mod: a = a % literal(4byte) */
#define VGSCPU_OP_MOD_A_B 0x84  /* mod: a = a % b */
#define VGSCPU_OP_MOD_A_C 0x85  /* mod: a = a % c */
#define VGSCPU_OP_MOD_A_D 0x86  /* mod: a = a % d */
#define VGSCPU_OP_MOD_A_M1 0x87 /* mod: a = a % &memory[ptr](1byte) */
#define VGSCPU_OP_MOD_A_M2 0x88 /* mod: a = a % &memory[ptr](2byte) */
#define VGSCPU_OP_MOD_A_M4 0x89 /* mod: a = a % &memory[ptr](4byte) */

/* and */
#define VGSCPU_OP_AND_A_1 0x8a  /* and: a = a & literal(1byte) */
#define VGSCPU_OP_AND_A_2 0x8b  /* and: a = a & literal(2byte) */
#define VGSCPU_OP_AND_A_4 0x8c  /* and: a = a & literal(4byte) */
#define VGSCPU_OP_AND_A_B 0x8d  /* and: a = a & b */
#define VGSCPU_OP_AND_A_C 0x8e  /* and: a = a & c */
#define VGSCPU_OP_AND_A_D 0x8f  /* and: a = a & d */
#define VGSCPU_OP_AND_A_M1 0x90 /* and: a = a & &memory[ptr](1byte) */
#define VGSCPU_OP_AND_A_M2 0x91 /* and: a = a & &memory[ptr](2byte) */
#define VGSCPU_OP_AND_A_M4 0x92 /* and: a = a & &memory[ptr](4byte) */

/* or */
#define VGSCPU_OP_OR_A_1 0x93  /* or: a = a | literal(1byte) */
#define VGSCPU_OP_OR_A_2 0x94  /* or: a = a | literal(2byte) */
#define VGSCPU_OP_OR_A_4 0x95  /* or: a = a | literal(4byte) */
#define VGSCPU_OP_OR_A_B 0x96  /* or: a = a | b */
#define VGSCPU_OP_OR_A_C 0x97  /* or: a = a | c */
#define VGSCPU_OP_OR_A_D 0x98  /* or: a = a | d */
#define VGSCPU_OP_OR_A_M1 0x99 /* or: a = a | &memory[ptr](1byte) */
#define VGSCPU_OP_OR_A_M2 0x9a /* or: a = a | &memory[ptr](2byte) */
#define VGSCPU_OP_OR_A_M4 0x9b /* or: a = a | &memory[ptr](4byte) */

/* xor */
#define VGSCPU_OP_XOR_A_1 0x9c  /* xor: a = a ^ literal(1byte) */
#define VGSCPU_OP_XOR_A_2 0x9d  /* xor: a = a ^ literal(2byte) */
#define VGSCPU_OP_XOR_A_4 0x9e  /* xor: a = a ^ literal(4byte) */
#define VGSCPU_OP_XOR_A_B 0x9f  /* xor: a = a ^ b */
#define VGSCPU_OP_XOR_A_C 0xa0  /* xor: a = a ^ c */
#define VGSCPU_OP_XOR_A_D 0xa1  /* xor: a = a ^ d */
#define VGSCPU_OP_XOR_A_M1 0xa2 /* xor: a = a ^ &memory[ptr](1byte) */
#define VGSCPU_OP_XOR_A_M2 0xa3 /* xor: a = a ^ &memory[ptr](2byte) */
#define VGSCPU_OP_XOR_A_M4 0xa4 /* xor: a = a ^ &memory[ptr](4byte) */

/* compare (result: vgscpu_status_flag.eq) */
#define VGSCPU_OP_CMP_A_1 0xa5  /* cmp: a and literal(1byte) */
#define VGSCPU_OP_CMP_A_2 0xa6  /* cmp: a and literal(2byte) */
#define VGSCPU_OP_CMP_A_4 0xa7  /* cmp: a and literal(4byte) */
#define VGSCPU_OP_CMP_A_B 0xa8  /* cmp: a and b */
#define VGSCPU_OP_CMP_A_C 0xa9  /* cmp: a and c */
#define VGSCPU_OP_CMP_A_D 0xaa  /* cmp: a and d */
#define VGSCPU_OP_CMP_A_M1 0xab /* cmp: a and &memory[ptr](1byte) */
#define VGSCPU_OP_CMP_A_M2 0xac /* cmp: a and &memory[ptr](2byte) */
#define VGSCPU_OP_CMP_A_M4 0xad /* cmp: a and &memory[ptr](4byte) */

/* compare - two's complement (result: vgscpu_status_flag.eq) */
#define VGSCPU_OP_CMP2_A_1 0xae  /* cmp: a and literal(1byte) */
#define VGSCPU_OP_CMP2_A_2 0xaf  /* cmp: a and literal(2byte) */
#define VGSCPU_OP_CMP2_A_4 0xb0  /* cmp: a and literal(4byte) */
#define VGSCPU_OP_CMP2_A_B 0xb1  /* cmp: a and b */
#define VGSCPU_OP_CMP2_A_C 0xb2  /* cmp: a and c */
#define VGSCPU_OP_CMP2_A_D 0xb3  /* cmp: a and d */
#define VGSCPU_OP_CMP2_A_M1 0xb4 /* cmp: a and &memory[ptr](1byte) */
#define VGSCPU_OP_CMP2_A_M2 0xb5 /* cmp: a and &memory[ptr](2byte) */
#define VGSCPU_OP_CMP2_A_M4 0xb6 /* cmp: a and &memory[ptr](4byte) */

/* load/store from register memory */
#define VGSCPU_OP_LD_A_RM 0xb7
#define VGSCPU_OP_LD_B_RM 0xb8
#define VGSCPU_OP_LD_C_RM 0xb9
#define VGSCPU_OP_LD_D_RM 0xba
#define VGSCPU_OP_ST_A_RM 0xbb
#define VGSCPU_OP_ST_B_RM 0xbc
#define VGSCPU_OP_ST_C_RM 0xbd
#define VGSCPU_OP_ST_D_RM 0xbe

/* accumulator (exclude A) */
#define VGSCPU_OP_ACU_B 0xdd
#define VGSCPU_OP_ACU_C 0xde
#define VGSCPU_OP_ACU_D 0xdf

/* branch */
#define VGSCPU_OP_JMP 0xf4 /* jump to specific address */
#define VGSCPU_OP_JZ 0xf5  /* jump when zero flag set */
#define VGSCPU_OP_JNZ 0xf6 /* jump when zero flag reset */
#define VGSCPU_OP_JE 0xf7  /* jump when equal flag = 0 */
#define VGSCPU_OP_JNE 0xf8 /* jump when equal flag != 0 */
#define VGSCPU_OP_JN 0xf9  /* jump when equal flag = -1 */
#define VGSCPU_OP_JNN 0xfa /* jump when equal flag != -1 */
#define VGSCPU_OP_JP 0xfb  /* jump when equal flag = 1 */
#define VGSCPU_OP_JNP 0xfc /* jump when equal flag != 1 */
#define VGSCPU_OP_CAL 0xfd /* call to specific address */
#define VGSCPU_OP_RET 0xfe /* return to called */
#define VGSCPU_OP_INT 0xff /* execute interrupt */
