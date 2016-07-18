#include "vgsasm.h"

int parse_int(struct line_data* line, int i)
{
    unsigned int v;
    if (line[i].toknum < 2) {
        sprintf(line[i].error, "syntax error: required argument was not specified");
        return -1;
    }
    if (2 < line[i].toknum) {
        sprintf(line[i].error, "syntax error: extra argument was specified: %s", line[i].token[2]);
        return -1;
    }
    if (check_literal(line[i].token[1], &v) || 0xff < v) {
        sprintf(line[i].error, "syntax error: invalid argument was specified: %s", line[i].token[1]);
        return -1;
    }
    line[i].op[0] = VGSCPU_OP_INT;
    line[i].op[1] = v & 0xff;
    line[i].oplen = 2;
    return 0;
}
