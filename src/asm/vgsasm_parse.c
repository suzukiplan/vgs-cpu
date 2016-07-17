#include "vgsasm.h"

struct line_data* parse_lines(char* buf, int* line)
{
    struct line_data* result = NULL;
    char* cp;
    char* work = buf;
    int i, j;

    *line = 1;
    while (NULL != (cp = strchr(work, '\n'))) {
        *line = *line + 1;
        *cp = '\0';
        work = cp + 1;
    }

    result = (struct line_data*)malloc(sizeof(struct line_data) * (*line));
    if (NULL == result) {
        fprintf(stderr, "no memory.\n");
        return NULL;
    }

    memset(result, 0, sizeof(struct line_data) * (*line));
    for (i = 0; i < *line; i++) {
        size_t len = strlen(buf);
        for (j = 0; buf[j]; j++) {
            if ('\r' == buf[j]) {
                buf[j] = '\0';
                break;
            } else if ('\t' == buf[j] || ',' == buf[j]) {
                buf[j] = ' ';
            }
        }
        trimstring(buf);
        result[i].number = i + 1;
        result[i].buffer = buf;
        buf += len + 1;
    }

    return result;
}

void remove_empty_line(struct line_data* line, int* len)
{
    char* cp;
    int i;
    int detect = 0;

    for (i = 0; i < *len; i++) {
        if (detect) {
            cp = line[i].buffer;
            while (*cp != '\0') {
                if (*cp == '*' && *(cp + 1) == '/') {
                    *cp = ' ';
                    cp++;
                    *cp = ' ';
                    cp++;
                    detect = 0;
                    i--;
                    break;
                } else {
                    *cp = ' ';
                    cp++;
                }
            }
        } else {
            if (NULL != (cp = strstr(line[i].buffer, "/*"))) {
                detect = 1;
                *cp = ' ';
                cp++;
                *cp = ' ';
                cp++;
                while (*cp != '\0') {
                    if (*cp == '*' && *(cp + 1) == '/') {
                        *cp = ' ';
                        cp++;
                        *cp = ' ';
                        cp++;
                        detect = 0;
                        i--;
                        break;
                    } else {
                        *cp = ' ';
                        cp++;
                    }
                }
            }
        }
    }

    for (i = 0; i < *len; i++) {
        if (NULL != (cp = strstr(line[i].buffer, "//"))) {
            *cp = '\0';
        }
    }

    for (i = 0; i < *len; i++) {
        trimstring(line[i].buffer);
        if ('\0' == line[i].buffer[0]) {
            if (i + 1 != *len) {
                memmove(&line[i], &line[i + 1], sizeof(struct line_data) * ((*len) - i - 1));
                *len = *len - 1;
                i--;
            } else {
                *len = *len - 1;
                break;
            }
        }
    }
}

void parse_token(struct line_data* line, int len)
{
    int i;
    char* w;
    for (i = 0; i < len; i++) {
        w = line[i].buffer;
        while (*w) {
            line[i].token[line[i].toknum++] = w;
            while (*w && ' ' != *w && '\t' != *w) {
                w++;
            }
            if (' ' == *w || '\t' == *w) {
                *w = '\0';
                w++;
                while (' ' == *w && '\t' == *w) {
                    w++;
                }
            }
        }
    }
}

int parse_operation(struct line_data* line, int len)
{
    int error_count = 0;
    int i;
    int r;
    for (i = 0; i < len; i++) {
        if (0 == strcasecmp(line[i].token[0], "PUSH")) {
            if (parse_push(line, i)) error_count++;
        } else if (0 == strcasecmp(line[i].token[0], "POP")) {
            if (parse_pop(line, i)) error_count++;
        } else if (0 == strcasecmp(line[i].token[0], "LD")) {
            if (parse_ld(line, i)) error_count++;
        } else if (0 == strcasecmp(line[i].token[0], "ST")) {
            if (parse_st(line, i)) error_count++;
        } else if (0 == strcasecmp(line[i].token[0], "INC")) {
            if (parse_inc(line, i)) error_count++;
        } else if (0 == strcasecmp(line[i].token[0], "DEC")) {
            if (parse_dec(line, i)) error_count++;
        } else if (0 == strcasecmp(line[i].token[0], "NOT")) {
            if (parse_not(line, i)) error_count++;
        } else if (0 == strcasecmp(line[i].token[0], "SHL")) {
            if (parse_shift(line, i, SHIFT_LEFT)) error_count++;
        } else if (0 == strcasecmp(line[i].token[0], "SHR")) {
            if (parse_shift(line, i, SHIFT_RIGHT)) error_count++;
        } else if (0 == strcasecmp(line[i].token[0], "ADD")) {
            if (parse_acu(line, i, ACU_ADD)) error_count++;
        } else if (0 == strcasecmp(line[i].token[0], "SUB")) {
            if (parse_acu(line, i, ACU_SUB)) error_count++;
        } else if (0 == strcasecmp(line[i].token[0], "MUL")) {
            if (parse_acu(line, i, ACU_MUL)) error_count++;
        } else if (0 == strcasecmp(line[i].token[0], "DIV")) {
            if (parse_acu(line, i, ACU_DIV)) error_count++;
        } else if (0 == strcasecmp(line[i].token[0], "MOD")) {
            if (parse_acu(line, i, ACU_MOD)) error_count++;
        } else if (0 == strcasecmp(line[i].token[0], "AND")) {
            if (parse_acu(line, i, ACU_AND)) error_count++;
        } else if (0 == strcasecmp(line[i].token[0], "OR")) {
            if (parse_acu(line, i, ACU_OR)) error_count++;
        } else if (0 == strcasecmp(line[i].token[0], "XOR")) {
            if (parse_acu(line, i, ACU_XOR)) error_count++;
        } else if (0 == strcasecmp(line[i].token[0], "CMP")) {
            if (parse_acu(line, i, ACU_CMP)) error_count++;
        } else if (0 == strcasecmp(line[i].token[0], "CMP2")) {
            if (parse_acu(line, i, ACU_CMP2)) error_count++;
        } else {
            sprintf(line[i].error, "syntax error: unknown operand was specified: %s", line[i].token[0]);
            error_count++;
        }
    }
    return error_count;
}
