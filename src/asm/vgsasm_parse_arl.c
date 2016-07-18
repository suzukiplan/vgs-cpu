#include "vgsasm.h"

int _parse_arl(struct line_data* line, int i, int r, int op)
{
    unsigned int v;
    unsigned short s;
    int m;
    if (-1 != check_GR(line[i].token[2])) {
        line[i].oplen = 1;
        switch (r) {
            case A4:
                switch (check_GR(line[i].token[2])) {
                    case B4:
                        line[i].op[0] = op + 3;
                        break;
                    case C4:
                        line[i].op[0] = op + 4;
                        break;
                    case D4:
                        line[i].op[0] = op + 5;
                        break;
                    default:
                        sprintf(line[i].error, "syntax error: invalid register: %s", line[i].token[2]);
                        return -1;
                }
                return 0;
            case B4:
                switch (check_GR(line[i].token[2])) {
                    case A4:
                        line[i].op[0] = op + 3;
                        break;
                    case C4:
                        line[i].op[0] = op + 4;
                        break;
                    case D4:
                        line[i].op[0] = op + 5;
                        break;
                    default:
                        sprintf(line[i].error, "syntax error: invalid register: %s", line[i].token[2]);
                        return -1;
                }
                return 0;
            case C4:
                switch (check_GR(line[i].token[2])) {
                    case A4:
                        line[i].op[0] = op + 3;
                        break;
                    case B4:
                        line[i].op[0] = op + 4;
                        break;
                    case D4:
                        line[i].op[0] = op + 5;
                        break;
                    default:
                        sprintf(line[i].error, "syntax error: invalid register: %s", line[i].token[2]);
                        return -1;
                }
                return 0;
            case D4:
                switch (check_GR(line[i].token[2])) {
                    case A4:
                        line[i].op[0] = op + 3;
                        break;
                    case B4:
                        line[i].op[0] = op + 4;
                        break;
                    case C4:
                        line[i].op[0] = op + 5;
                        break;
                    default:
                        sprintf(line[i].error, "syntax error: invalid register: %s", line[i].token[2]);
                        return -1;
                }
                return 0;
        }
    } else if (0 == check_literal(line[i].token[2], &v)) {
        if (v < 0x100) {
            line[i].op[0] = op;
            line[i].op[1] = v & 0xff;
            line[i].oplen = 2;
        } else if (v < 0x10000) {
            s = v & 0xffff;
            line[i].op[0] = op + 1;
            memcpy(&line[i].op[1], &s, 2);
            line[i].oplen = 3;
        } else {
            line[i].op[0] = op + 2;
            memcpy(&line[i].op[1], &v, 4);
            line[i].oplen = 5;
        }
        return 0;
    } else if (0 == check_address(line[i].token[2], &v, &m)) {
        memcpy(&line[i].op[1], &v, 4);
        line[i].oplen = 5;
        switch (m) {
            case M1:
                line[i].op[0] = op + 6;
                break;
            case M2:
                line[i].op[0] = op + 7;
                break;
            case M4:
                line[i].op[0] = op + 8;
                break;
        }
        return 0;
    }
    sprintf(line[i].error, "syntax error: invalid argument: %s", line[i].token[2]);
    return -1;
}
