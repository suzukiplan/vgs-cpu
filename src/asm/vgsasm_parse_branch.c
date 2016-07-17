#include "vgsasm.h"

int parse_branch(struct line_data* line, int i, unsigned char op)
{
    unsigned int v;
    int r, m;
    if (line[i].toknum < 2) {
        sprintf(line[i].error, "syntax error: required argument was not specified");
        return -1;
    }
    if (2 < line[i].toknum) {
        sprintf(line[i].error, "syntax error: extra argument was specified: %s", line[i].token[2]);
        return -1;
    }
    line[i].oplen = 5;
    // NOTE: do not set branch address while parsing. (it will be set after label address has resolved)
    if (sizeof(line[i].branch_label) <= strlen(line[i].token[1])) {
        sprintf(line[i].error, "syntax error: invalid label was specified: %s", line[i].token[1]);
        return -1;
    }
    strcpy(line[i].branch_label, line[i].token[1]);
    line[i].op[0] = op;
    return 0;
}
