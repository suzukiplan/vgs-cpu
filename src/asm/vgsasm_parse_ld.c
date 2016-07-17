#include "vgsasm.h"

int parse_ld(struct line_data* line, int i)
{
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
            return _parse_arl(line, i, A4, VGSCPU_OP_LD_A_1);
        case B4:
            return _parse_arl(line, i, B4, VGSCPU_OP_LD_B_1);
        case C4:
            return _parse_arl(line, i, C4, VGSCPU_OP_LD_C_1);
        case D4:
            return _parse_arl(line, i, D4, VGSCPU_OP_LD_D_1);
    }
    sprintf(line[i].error, "syntax error: unknown register was specified: %s", line[i].token[1]);
    return -1;
}
