#include "vgsasm.h"

static int parse_st_internal(struct line_data* line, int i, int op)
{
    unsigned int v;
    unsigned short s;
    int m;
    if (0 == check_register_address(line[i].token[2], &m)) {
        switch (op) {
            case VGSCPU_OP_ST_A_M1:
                op = VGSCPU_OP_ST_A_RM;
                break;
            case VGSCPU_OP_ST_B_M1:
                op = VGSCPU_OP_ST_B_RM;
                break;
            case VGSCPU_OP_ST_C_M1:
                op = VGSCPU_OP_ST_C_RM;
                break;
            case VGSCPU_OP_ST_D_M1:
                op = VGSCPU_OP_ST_D_RM;
                break;
        }
        line[i].op[0] = op;
        line[i].op[1] = m & 0xff;
        line[i].oplen = 2;
        return 0;
    }
    if (0 == check_address(line[i].token[2], &v, &m)) {
        memcpy(&line[i].op[1], &v, 4);
        line[i].oplen = 5;
        switch (m) {
            case M1:
                line[i].op[0] = op;
                break;
            case M2:
                line[i].op[0] = op + 1;
                break;
            case M4:
                line[i].op[0] = op + 2;
                break;
        }
        return 0;
    }
    sprintf(line[i].error, "syntax error: invalid argument: %s", line[i].token[2]);
    return -1;
}

int parse_st(struct line_data* line, int i)
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
            return parse_st_internal(line, i, VGSCPU_OP_ST_A_M1);
        case B4:
            return parse_st_internal(line, i, VGSCPU_OP_ST_B_M1);
        case C4:
            return parse_st_internal(line, i, VGSCPU_OP_ST_C_M1);
        case D4:
            return parse_st_internal(line, i, VGSCPU_OP_ST_D_M1);
    }
    sprintf(line[i].error, "syntax error: unknown register was specified: %s", line[i].token[1]);
    return -1;
}
