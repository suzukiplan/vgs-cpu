#include "vgsasm.h"

int parse_shift(struct line_data* line, int i, int lr)
{
    int r;
    unsigned int v;
    if (line[i].toknum < 3) {
        sprintf(line[i].error, "syntax error: required argument was not specified");
        return -1;
    }
    if (3 < line[i].toknum) {
        sprintf(line[i].error, "syntax error: extra argument was specified: %s", line[i].token[3]);
        return -1;
    }
    if (check_literal(line[i].token[2], &v) || 0xff < v) {
        sprintf(line[i].error, "syntax error: invalid argument was specified: %s", line[i].token[2]);
        return -1;
    }
    line[i].op[1] = v & 0xff;
    line[i].oplen = 2;
    switch (check_GR(line[i].token[1])) {
        case A4:
            line[i].op[0] = (lr == SHIFT_LEFT) ? VGSCPU_OP_SHL_A : VGSCPU_OP_SHR_A;
            return 0;
        case B4:
            line[i].op[0] = (lr == SHIFT_LEFT) ? VGSCPU_OP_SHL_B : VGSCPU_OP_SHR_B;
            return 0;
        case C4:
            line[i].op[0] = (lr == SHIFT_LEFT) ? VGSCPU_OP_SHL_C : VGSCPU_OP_SHR_C;
            return 0;
        case D4:
            line[i].op[0] = (lr == SHIFT_LEFT) ? VGSCPU_OP_SHL_D : VGSCPU_OP_SHR_D;
            return 0;
    }
    sprintf(line[i].error, "syntax error: unknown register was specified: %s", line[i].token[1]);
    return -1;
}
