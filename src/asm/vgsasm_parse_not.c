#include "vgsasm.h"

int parse_not(struct line_data* line, int i)
{
    int r;
    if (line[i].toknum < 2) {
        sprintf(line[i].error, "syntax error: required argument was not specified");
        return -1;
    }
    if (2 < line[i].toknum) {
        sprintf(line[i].error, "syntax error: extra argument was specified: %s", line[i].token[2]);
        return -1;
    }
    line[i].oplen = 1;
    switch (check_GR(line[i].token[1])) {
        case A4:
            line[i].op[0] = VGSCPU_OP_NOT_A;
            return 0;
        case B4:
            line[i].op[0] = VGSCPU_OP_NOT_B;
            return 0;
        case C4:
            line[i].op[0] = VGSCPU_OP_NOT_C;
            return 0;
        case D4:
            line[i].op[0] = VGSCPU_OP_NOT_D;
            return 0;
    }
    sprintf(line[i].error, "syntax error: unknown register was specified: %s", line[i].token[1]);
    return -1;
}
