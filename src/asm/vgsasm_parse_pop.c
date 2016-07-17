#include "vgsasm.h"

int parse_pop(struct line_data* line, int i)
{
    int r;
    int op[12] = {VGSCPU_OP_POP_A1, VGSCPU_OP_POP_A2, VGSCPU_OP_POP_A4, VGSCPU_OP_POP_B1, VGSCPU_OP_POP_B2, VGSCPU_OP_POP_B4, VGSCPU_OP_POP_C1, VGSCPU_OP_POP_C2, VGSCPU_OP_POP_C4, VGSCPU_OP_POP_D1, VGSCPU_OP_POP_D2, VGSCPU_OP_POP_D4};
    if (line[i].toknum < 2) {
        sprintf(line[i].error, "syntax error: required argument was not specified");
        return -1;
    }
    if (2 < line[i].toknum) {
        sprintf(line[i].error, "syntax error: extra argument was specified: %s", line[i].token[2]);
        return -1;
    }
    r = check_GR(line[i].token[1]);
    if (-1 == r) {
        sprintf(line[i].error, "syntax error: unknown register was specified: %s", line[i].token[1]);
        return -1;
    }
    line[i].op[0] = op[r];
    line[i].oplen = 1;
    return 0;
}
