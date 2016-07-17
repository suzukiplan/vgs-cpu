#include "vgsasm.h"

struct program_table {
    char input[1024];
    char output[1024];
    char* buffer;
    struct line_data* line;
    int line_number;
} PT;

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