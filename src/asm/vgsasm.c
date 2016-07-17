#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vgscpu_internal.h"

#define LOGV printf
#define A1 0
#define A2 1
#define A4 2
#define B1 3
#define B2 4
#define B4 5
#define C1 6
#define C2 7
#define C4 8
#define D1 9
#define D2 10
#define D4 11

struct line_data {
    char error[1024];
    char* buffer;
    int number;
    char* token[8];
    int toknum;
    int op;
};

struct program_table {
    char input[1024];
    char output[1024];
    char* buffer;
    struct line_data* line;
    int line_number;
} PT;

static void trimstring(char* src)
{
    int i, j;
    int len;
    for (i = 0; ' ' == src[i]; i++)
        ;
    if (i) {
        for (j = 0; src[i] != '\0'; j++, i++) {
            src[j] = src[i];
        }
        src[j] = '\0';
    }
    for (len = (int)strlen(src) - 1; 0 <= len && ' ' == src[len]; len--) {
        src[len] = '\0';
    }
}

static int check_arguments(int argc, char* argv[])
{
    int i;
    memset(&PT, 0, sizeof(PT));
    for (i = 1; i < argc; i++) {
        if (0 == strcmp("-o", argv[i])) {
            if (PT.output[0]) return -1;
            if (argc <= ++i) return -1;
            if (sizeof(PT.output) <= strlen(argv[i])) return -1;
            strcpy(PT.output, argv[i]);
        } else {
            if (PT.input[0]) return -1;
            if (sizeof(PT.input) <= strlen(argv[i])) return -1;
            strcpy(PT.input, argv[i]);
        }
    }
    if (!PT.input[0]) return -1;
    if (!PT.output[0]) {
        strcpy(PT.output, PT.input);
        char* ext = strrchr(PT.output, '.');
        if (ext) *ext = '\0';
        if (sizeof(PT.output) <= strlen(PT.output) + 4) return -1;
        strcat(PT.output, ".bin");
    }
    return 0;
}

static char* load_file(const char* path)
{
    FILE* fp;
    void* data;
    int size;

    fp = fopen(path, "rb");
    if (NULL == fp) {
        return NULL;
    }
    if (-1 == fseek(fp, 0, SEEK_END)) {
        fclose(fp);
        return NULL;
    }
    size = (int)ftell(fp);
    if (size < 1) {
        fclose(fp);
        return NULL;
    }
    if (-1 == fseek(fp, 0, SEEK_SET)) {
        fclose(fp);
        return NULL;
    }
    data = malloc(size + 1);
    if (NULL == data) {
        fclose(fp);
        return NULL;
    }
    ((char*)data)[size] = '\0';
    if (size != fread(data, 1, (size_t)size, fp)) {
        fclose(fp);
        free(data);
        return NULL;
    }
    fclose(fp);
    return data;
}

static struct line_data* parse_lines(char* buf, int* line)
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
            } else if ('\t' == buf[j]) {
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

static void remove_empty_line(struct line_data* line, int* len)
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

static void parse_token(struct line_data* line, int len)
{
    int i;
    char* w;
    for (i = 0; i < len; i++) {
        w = line[i].buffer;
        while (*w) {
            line[i].token[line[i].toknum++] = w;
            while (*w && ' ' != *w && '\t' != *w)
                w++;
            if (' ' == *w || '\t' == *w) {
                *w = '\0';
                w++;
                while (' ' == *w && '\t' == *w)
                    w++;
            }
        }
    }
}

static int check_GR(char* token)
{
    if (0 == strcasecmp(token, "A")) {
        return A4;
    } else if (0 == strcasecmp(token, "AH")) {
        return A2;
    } else if (0 == strcasecmp(token, "AO")) {
        return A1;
    } else if (0 == strcasecmp(token, "B")) {
        return B4;
    } else if (0 == strcasecmp(token, "BH")) {
        return B2;
    } else if (0 == strcasecmp(token, "BO")) {
        return B1;
    } else if (0 == strcasecmp(token, "C")) {
        return C4;
    } else if (0 == strcasecmp(token, "CH")) {
        return C2;
    } else if (0 == strcasecmp(token, "CO")) {
        return C1;
    } else if (0 == strcasecmp(token, "D")) {
        return D4;
    } else if (0 == strcasecmp(token, "DH")) {
        return D2;
    } else if (0 == strcasecmp(token, "DO")) {
        return D1;
    } else {
        return -1;
    }
}

static int parse_push(struct line_data* line, int i)
{
    int r;
    int op[12] = {VGSCPU_OP_PUSH_A1, VGSCPU_OP_PUSH_A2, VGSCPU_OP_PUSH_A4, VGSCPU_OP_PUSH_B1, VGSCPU_OP_PUSH_B2, VGSCPU_OP_PUSH_B4, VGSCPU_OP_PUSH_C1, VGSCPU_OP_PUSH_C2, VGSCPU_OP_PUSH_C4, VGSCPU_OP_PUSH_D1, VGSCPU_OP_PUSH_D2, VGSCPU_OP_PUSH_D4};
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
    line[i].op = op[r];
    return 0;
}

static int parse_operation(struct line_data* line, int len)
{
    int error_count = 0;
    int i;
    int r;
    for (i = 0; i < len; i++) {
        if (0 == strcasecmp(line[i].token[0], "PUSH")) {
            if (parse_push(line, i)) error_count++;
        } else {
            sprintf(line[i].error, "syntax error: unknown operand was specified: %s", line[i].token[0]);
            error_count++;
        }
    }
    return error_count;
}

static void show_errors(struct line_data* line, int len)
{
    int i;
    int n = 0;
    for (i = 0; i < len; i++) {
        if (line[i].error[0]) {
            fprintf(stderr, "%s (line=%d)\n", line[i].error, line[i].number);
            n++;
        }
    }
    fprintf(stderr, "detected %d error%s.\n", n, 1 < n ? "s" : "");
}

int main(int argc, char* argv[])
{
    if (check_arguments(argc, argv)) {
        puts("usage: vgsasm [-o output.bin] input.asm");
        return 1;
    }
    LOGV("assembling: %s -> %s\n", PT.input, PT.output);

    PT.buffer = load_file(PT.input);
    if (NULL == PT.buffer) {
        fprintf(stderr, "file load error.\n");
        return 2;
    }

    PT.line = parse_lines(PT.buffer, &PT.line_number);
    if (NULL == PT.line) {
        return 3;
    }

    remove_empty_line(PT.line, &PT.line_number);
    if (0 == PT.line_number) {
        fprintf(stderr, "empty source file.\n");
        return 4;
    }

    parse_token(PT.line, PT.line_number);
    if (parse_operation(PT.line, PT.line_number)) {
        show_errors(PT.line, PT.line_number);
        return 5;
    }

    {
        int i, j;
        for (i = 0; i < PT.line_number; i++) {
            printf("%04d:", PT.line[i].number);
            for (j = 0; j < PT.line[i].toknum; j++) {
                printf(" <%s>", PT.line[i].token[j]);
            }
            printf("\n");
        }
    }

    free(PT.line);
    free(PT.buffer);
    return 0;
}