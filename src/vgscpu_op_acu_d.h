/* add */
#define VGSCPU_OP_ADD_D_1 0x5d  /* add: b = b + literal(1byte) */
#define VGSCPU_OP_ADD_D_2 0x5e  /* add: b = b + literal(2byte) */
#define VGSCPU_OP_ADD_D_4 0x5f  /* add: b = b + literal(4byte) */
#define VGSCPU_OP_ADD_D_A 0x60  /* add: b = b + b */
#define VGSCPU_OP_ADD_D_B 0x61  /* add: b = b + c */
#define VGSCPU_OP_ADD_D_C 0x62  /* add: b = b + d */
#define VGSCPU_OP_ADD_D_M1 0x63 /* add: b = b + &memory[ptr](1byte) */
#define VGSCPU_OP_ADD_D_M2 0x64 /* add: b = b + &memory[ptr](2byte) */
#define VGSCPU_OP_ADD_D_M4 0x65 /* add: b = b + &memory[ptr](4byte) */

/* sub */
#define VGSCPU_OP_SUB_D_1 0x66  /* sub: b = b - literal(1byte) */
#define VGSCPU_OP_SUB_D_2 0x67  /* sub: b = b - literal(2byte) */
#define VGSCPU_OP_SUB_D_4 0x68  /* sub: b = b - literal(4byte) */
#define VGSCPU_OP_SUB_D_A 0x69  /* sub: b = b - b */
#define VGSCPU_OP_SUB_D_B 0x6a  /* sub: b = b - c */
#define VGSCPU_OP_SUB_D_C 0x6b  /* sub: b = b - d */
#define VGSCPU_OP_SUB_D_M1 0x6c /* sub: b = b - &memory[ptr](1byte) */
#define VGSCPU_OP_SUB_D_M2 0x6d /* sub: b = b - &memory[ptr](2byte) */
#define VGSCPU_OP_SUB_D_M4 0x6e /* sub: b = b - &memory[ptr](4byte) */

/* mul */
#define VGSCPU_OP_MUL_D_1 0x6f  /* mul: b = b * literal(1byte) */
#define VGSCPU_OP_MUL_D_2 0x70  /* mul: b = b * literal(2byte) */
#define VGSCPU_OP_MUL_D_4 0x71  /* mul: b = b * literal(4byte) */
#define VGSCPU_OP_MUL_D_A 0x72  /* mul: b = b * b */
#define VGSCPU_OP_MUL_D_B 0x73  /* mul: b = b * c */
#define VGSCPU_OP_MUL_D_C 0x74  /* mul: b = b * d */
#define VGSCPU_OP_MUL_D_M1 0x75 /* mul: b = b * &memory[ptr](1byte) */
#define VGSCPU_OP_MUL_D_M2 0x76 /* mul: b = b * &memory[ptr](2byte) */
#define VGSCPU_OP_MUL_D_M4 0x77 /* mul: b = b * &memory[ptr](4byte) */

/* div */
#define VGSCPU_OP_DIV_D_1 0x78  /* div: b = b / literal(1byte) */
#define VGSCPU_OP_DIV_D_2 0x79  /* div: b = b / literal(2byte) */
#define VGSCPU_OP_DIV_D_4 0x7a  /* div: b = b / literal(4byte) */
#define VGSCPU_OP_DIV_D_A 0x7b  /* div: b = b / b */
#define VGSCPU_OP_DIV_D_B 0x7c  /* div: b = b / c */
#define VGSCPU_OP_DIV_D_C 0x7d  /* div: b = b / d */
#define VGSCPU_OP_DIV_D_M1 0x7e /* div: b = b / &memory[ptr](1byte) */
#define VGSCPU_OP_DIV_D_M2 0x7f /* div: b = b / &memory[ptr](2byte) */
#define VGSCPU_OP_DIV_D_M4 0x80 /* div: b = b / &memory[ptr](4byte) */

/* mod */
#define VGSCPU_OP_MOD_D_1 0x81  /* mod: b = b % literal(1byte) */
#define VGSCPU_OP_MOD_D_2 0x82  /* mod: b = b % literal(2byte) */
#define VGSCPU_OP_MOD_D_4 0x83  /* mod: b = b % literal(4byte) */
#define VGSCPU_OP_MOD_D_A 0x84  /* mod: b = b % b */
#define VGSCPU_OP_MOD_D_B 0x85  /* mod: b = b % c */
#define VGSCPU_OP_MOD_D_C 0x86  /* mod: b = b % d */
#define VGSCPU_OP_MOD_D_M1 0x87 /* mod: b = b % &memory[ptr](1byte) */
#define VGSCPU_OP_MOD_D_M2 0x88 /* mod: b = b % &memory[ptr](2byte) */
#define VGSCPU_OP_MOD_D_M4 0x89 /* mod: b = b % &memory[ptr](4byte) */

/* and */
#define VGSCPU_OP_AND_D_1 0x8a  /* and: b = b & literal(1byte) */
#define VGSCPU_OP_AND_D_2 0x8b  /* and: b = b & literal(2byte) */
#define VGSCPU_OP_AND_D_4 0x8c  /* and: b = b & literal(4byte) */
#define VGSCPU_OP_AND_D_A 0x8d  /* and: b = b & b */
#define VGSCPU_OP_AND_D_B 0x8e  /* and: b = b & c */
#define VGSCPU_OP_AND_D_C 0x8f  /* and: b = b & d */
#define VGSCPU_OP_AND_D_M1 0x90 /* and: b = b & &memory[ptr](1byte) */
#define VGSCPU_OP_AND_D_M2 0x91 /* and: b = b & &memory[ptr](2byte) */
#define VGSCPU_OP_AND_D_M4 0x92 /* and: b = b & &memory[ptr](4byte) */

/* or */
#define VGSCPU_OP_OR_D_1 0x93  /* or: b = b | literal(1byte) */
#define VGSCPU_OP_OR_D_2 0x94  /* or: b = b | literal(2byte) */
#define VGSCPU_OP_OR_D_4 0x95  /* or: b = b | literal(4byte) */
#define VGSCPU_OP_OR_D_A 0x96  /* or: b = b | b */
#define VGSCPU_OP_OR_D_B 0x97  /* or: b = b | c */
#define VGSCPU_OP_OR_D_C 0x98  /* or: b = b | d */
#define VGSCPU_OP_OR_D_M1 0x99 /* or: b = b | &memory[ptr](1byte) */
#define VGSCPU_OP_OR_D_M2 0x9a /* or: b = b | &memory[ptr](2byte) */
#define VGSCPU_OP_OR_D_M4 0x9b /* or: b = b | &memory[ptr](4byte) */

/* xor */
#define VGSCPU_OP_XOR_D_1 0x9c  /* xor: b = b ^ literal(1byte) */
#define VGSCPU_OP_XOR_D_2 0x9d  /* xor: b = b ^ literal(2byte) */
#define VGSCPU_OP_XOR_D_4 0x9e  /* xor: b = b ^ literal(4byte) */
#define VGSCPU_OP_XOR_D_A 0x9f  /* xor: b = b ^ b */
#define VGSCPU_OP_XOR_D_B 0xa0  /* xor: b = b ^ c */
#define VGSCPU_OP_XOR_D_C 0xa1  /* xor: b = b ^ d */
#define VGSCPU_OP_XOR_D_M1 0xa2 /* xor: b = b ^ &memory[ptr](1byte) */
#define VGSCPU_OP_XOR_D_M2 0xa3 /* xor: b = b ^ &memory[ptr](2byte) */
#define VGSCPU_OP_XOR_D_M4 0xa4 /* xor: b = b ^ &memory[ptr](4byte) */

/* compare (result: vgscpu_status_flag.eq) */
#define VGSCPU_OP_CMP_D_1 0xa5  /* cmp: b and literal(1byte) */
#define VGSCPU_OP_CMP_D_2 0xa6  /* cmp: b and literal(2byte) */
#define VGSCPU_OP_CMP_D_4 0xa7  /* cmp: b and literal(4byte) */
#define VGSCPU_OP_CMP_D_A 0xa8  /* cmp: b and b */
#define VGSCPU_OP_CMP_D_B 0xa9  /* cmp: b and c */
#define VGSCPU_OP_CMP_D_C 0xaa  /* cmp: b and d */
#define VGSCPU_OP_CMP_D_M1 0xab /* cmp: b and &memory[ptr](1byte) */
#define VGSCPU_OP_CMP_D_M2 0xac /* cmp: b and &memory[ptr](2byte) */
#define VGSCPU_OP_CMP_D_M4 0xad /* cmp: b and &memory[ptr](4byte) */

/* compare - two's complement (result: vgscpu_status_flag.eq) */
#define VGSCPU_OP_CMP2_D_1 0xae  /* cmp: b and literal(1byte) */
#define VGSCPU_OP_CMP2_D_2 0xaf  /* cmp: b and literal(2byte) */
#define VGSCPU_OP_CMP2_D_4 0xb0  /* cmp: b and literal(4byte) */
#define VGSCPU_OP_CMP2_D_A 0xb1  /* cmp: b and b */
#define VGSCPU_OP_CMP2_D_B 0xb2  /* cmp: b and c */
#define VGSCPU_OP_CMP2_D_C 0xb3  /* cmp: b and d */
#define VGSCPU_OP_CMP2_D_M1 0xb4 /* cmp: b and &memory[ptr](1byte) */
#define VGSCPU_OP_CMP2_D_M2 0xb5 /* cmp: b and &memory[ptr](2byte) */
#define VGSCPU_OP_CMP2_D_M4 0xb6 /* cmp: b and &memory[ptr](4byte) */
