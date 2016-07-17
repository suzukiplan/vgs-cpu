#include "vgsasm.h"

int parse_acu(struct line_data* line, int i, int acu)
{
    unsigned char acu_op[10] = {VGSCPU_OP_ADD_A_1, VGSCPU_OP_SUB_A_1, VGSCPU_OP_MUL_A_1, VGSCPU_OP_DIV_A_1, VGSCPU_OP_MOD_A_1, VGSCPU_OP_AND_A_1, VGSCPU_OP_OR_A_1, VGSCPU_OP_XOR_A_1, VGSCPU_OP_CMP_A_1, VGSCPU_OP_CMP2_A_1};
    if (line[i].toknum < 3) {
        sprintf(line[i].error, "syntax error: required argument was not specified");
        return -1;
    }
    if (3 < line[i].toknum) {
        sprintf(line[i].error, "syntax error: extra argument was specified: %s", line[i].token[3]);
        return -1;
    }
    switch (check_GR(line[i].token[1])) {
        case A4:
            return _parse_arl(line, i, A4, acu_op[acu]);
        case B4:
            if (_parse_arl(line, i, B4, acu_op[acu])) return -1;
            line[i].oplen++;
            memmove(&line[i].op[1], &line[i].op[0], sizeof(line[i].op) - 1);
            line[i].op[0] = VGSCPU_OP_ACU_B;
            return 0;
        case C4:
            if (_parse_arl(line, i, C4, acu_op[acu])) return -1;
            line[i].oplen++;
            memmove(&line[i].op[1], &line[i].op[0], sizeof(line[i].op) - 1);
            line[i].op[0] = VGSCPU_OP_ACU_C;
            return 0;
        case D4:
            if (_parse_arl(line, i, D4, acu_op[acu])) return -1;
            line[i].oplen++;
            memmove(&line[i].op[1], &line[i].op[0], sizeof(line[i].op) - 1);
            line[i].op[0] = VGSCPU_OP_ACU_D;
            return 0;
    }
    sprintf(line[i].error, "syntax error: unknown register was specified: %s", line[i].token[1]);
    return -1;
}
