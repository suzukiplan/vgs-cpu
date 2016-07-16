#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vgscpu_internal.h"

#define LOGV printf

struct line_data {
    char* buffer;
    int number;
    char* token[8];
    int toknum;
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
    int i;

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
        if (0 < len && '\r' == buf[len - 1]) buf[len - 1] = '\0';
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