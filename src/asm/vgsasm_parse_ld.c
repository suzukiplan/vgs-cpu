#include "vgsasm.h"

static int parse_ld_a(struct line_data* line, int i)
{
    unsigned int v;
    unsigned short s;
    int m;
    if (0 == check_literal(line[i].token[2], &v)) {
        if (v < 0x100) {
            line[i].op[0] = VGSCPU_OP_LD_A_1;
            line[i].op[1] = v & 0xff;
            line[i].oplen = 2;
        } else if (v < 0x10000) {
            s = v & 0xffff;
            line[i].op[0] = VGSCPU_OP_LD_A_2;
            memcpy(&line[i].op[1], &s, 2);
            line[i].oplen = 3;
        } else {
            line[i].op[0] = VGSCPU_OP_LD_A_4;
            memcpy(&line[i].op[1], &v, 4);
            line[i].oplen = 5;
        }
        return 0;
    } else if (0 == check_address(line[i].token[2], &v, &m)) {
        memcpy(&line[i].op[1], &v, 4);
        line[i].oplen = 5;
        switch (m) {
            case M1:
                line[i].op[0] = VGSCPU_OP_LD_A_M1;
                break;
            case M2:
                line[i].op[0] = VGSCPU_OP_LD_A_M2;
                break;
            case M4:
                line[i].op[0] = VGSCPU_OP_LD_A_M4;
                break;
        }
        return 0;
    } else {
        line[i].oplen = 1;
        switch (check_GR(line[i].token[2])) {
            case B4:
                line[i].op[0] = VGSCPU_OP_LD_A_B;
                break;
            case C4:
                line[i].op[0] = VGSCPU_OP_LD_A_C;
                break;
            case D4:
                line[i].op[0] = VGSCPU_OP_LD_A_D;
                break;
            default:
                sprintf(line[i].error, "syntax error: invalid register: %s", line[i].token[2]);
                return -1;
        }
        return 0;
    }
    sprintf(line[i].error, "syntax error: invalid argument: %s", line[i].token[2]);
    return -1;
}

static int parse_ld_b(struct line_data* line, int i)
{
    unsigned int v;
    unsigned short s;
    int m;
    if (0 == check_literal(line[i].token[2], &v)) {
        if (v < 0x100) {
            line[i].op[0] = VGSCPU_OP_LD_B_1;
            line[i].op[1] = v & 0xff;
            line[i].oplen = 2;
        } else if (v < 0x10000) {
            s = v & 0xffff;
            line[i].op[0] = VGSCPU_OP_LD_B_2;
            memcpy(&line[i].op[1], &s, 2);
            line[i].oplen = 3;
        } else {
            line[i].op[0] = VGSCPU_OP_LD_B_4;
            memcpy(&line[i].op[1], &v, 4);
            line[i].oplen = 5;
        }
        return 0;
    } else if (0 == check_address(line[i].token[2], &v, &m)) {
        memcpy(&line[i].op[1], &v, 4);
        line[i].oplen = 5;
        switch (m) {
            case M1:
                line[i].op[0] = VGSCPU_OP_LD_B_M1;
                break;
            case M2:
                line[i].op[0] = VGSCPU_OP_LD_B_M2;
                break;
            case M4:
                line[i].op[0] = VGSCPU_OP_LD_B_M4;
                break;
        }
        return 0;
    } else {
        line[i].oplen = 1;
        switch (check_GR(line[i].token[2])) {
            case A4:
                line[i].op[0] = VGSCPU_OP_LD_B_A;
                break;
            case C4:
                line[i].op[0] = VGSCPU_OP_LD_B_C;
                break;
            case D4:
                line[i].op[0] = VGSCPU_OP_LD_B_D;
                break;
            default:
                sprintf(line[i].error, "syntax error: invalid register: %s", line[i].token[2]);
                return -1;
        }
        return 0;
    }
    sprintf(line[i].error, "syntax error: invalid argument: %s", line[i].token[2]);
    return -1;
}

static int parse_ld_c(struct line_data* line, int i)
{
    unsigned int v;
    unsigned short s;
    int m;
    if (0 == check_literal(line[i].token[2], &v)) {
        if (v < 0x100) {
            line[i].op[0] = VGSCPU_OP_LD_C_1;
            line[i].op[1] = v & 0xff;
            line[i].oplen = 2;
        } else if (v < 0x10000) {
            s = v & 0xffff;
            line[i].op[0] = VGSCPU_OP_LD_C_2;
            memcpy(&line[i].op[1], &s, 2);
            line[i].oplen = 3;
        } else {
            line[i].op[0] = VGSCPU_OP_LD_C_4;
            memcpy(&line[i].op[1], &v, 4);
            line[i].oplen = 5;
        }
        return 0;
    } else if (0 == check_address(line[i].token[2], &v, &m)) {
        memcpy(&line[i].op[1], &v, 4);
        line[i].oplen = 5;
        switch (m) {
            case M1:
                line[i].op[0] = VGSCPU_OP_LD_C_M1;
                break;
            case M2:
                line[i].op[0] = VGSCPU_OP_LD_C_M2;
                break;
            case M4:
                line[i].op[0] = VGSCPU_OP_LD_C_M4;
                break;
        }
        return 0;
    } else {
        line[i].oplen = 1;
        switch (check_GR(line[i].token[2])) {
            case A4:
                line[i].op[0] = VGSCPU_OP_LD_C_A;
                break;
            case B4:
                line[i].op[0] = VGSCPU_OP_LD_C_B;
                break;
            case D4:
                line[i].op[0] = VGSCPU_OP_LD_C_D;
                break;
            default:
                sprintf(line[i].error, "syntax error: invalid register: %s", line[i].token[2]);
                return -1;
        }
        return 0;
    }
    sprintf(line[i].error, "syntax error: invalid argument: %s", line[i].token[2]);
    return -1;
}

static int parse_ld_d(struct line_data* line, int i)
{
    unsigned int v;
    unsigned short s;
    int m;
    if (0 == check_literal(line[i].token[2], &v)) {
        if (v < 0x100) {
            line[i].op[0] = VGSCPU_OP_LD_D_1;
            line[i].op[1] = v & 0xff;
            line[i].oplen = 2;
        } else if (v < 0x10000) {
            s = v & 0xffff;
            line[i].op[0] = VGSCPU_OP_LD_D_2;
            memcpy(&line[i].op[1], &s, 2);
            line[i].oplen = 3;
        } else {
            line[i].op[0] = VGSCPU_OP_LD_D_4;
            memcpy(&line[i].op[1], &v, 4);
            line[i].oplen = 5;
        }
        return 0;
    } else if (0 == check_address(line[i].token[2], &v, &m)) {
        memcpy(&line[i].op[1], &v, 4);
        line[i].oplen = 5;
        switch (m) {
            case M1:
                line[i].op[0] = VGSCPU_OP_LD_D_M1;
                break;
            case M2:
                line[i].op[0] = VGSCPU_OP_LD_D_M2;
                break;
            case M4:
                line[i].op[0] = VGSCPU_OP_LD_D_M4;
                break;
        }
        return 0;
    } else {
        line[i].oplen = 1;
        switch (check_GR(line[i].token[2])) {
            case A4:
                line[i].op[0] = VGSCPU_OP_LD_D_A;
                break;
            case B4:
                line[i].op[0] = VGSCPU_OP_LD_D_B;
                break;
            case C4:
                line[i].op[0] = VGSCPU_OP_LD_D_C;
                break;
            default:
                sprintf(line[i].error, "syntax error: invalid register: %s", line[i].token[2]);
                return -1;
        }
        return 0;
    }
    sprintf(line[i].error, "syntax error: invalid argument: %s", line[i].token[2]);
    return -1;
}

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
            return parse_ld_a(line, i);
        case B4:
            return parse_ld_b(line, i);
        case C4:
            return parse_ld_c(line, i);
        case D4:
            return parse_ld_d(line, i);
    }
    sprintf(line[i].error, "syntax error: unknown register was specified: %s", line[i].token[1]);
    return -1;
}
