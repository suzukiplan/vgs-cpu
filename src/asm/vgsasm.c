#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vgscpu_internal.h"

#define LOGV printf

struct program_table {
    char input[1024];
    char output[1024];
} PT;

int check_arguments(int argc, char* argv[])
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

int main(int argc, char* argv[])
{
    if (check_arguments(argc, argv)) {
        puts("usage: vgsasm [-o output.bin] input.asm");
        return 1;
    }
    LOGV("assembling: %s -> %s\n", PT.input, PT.output);
    return 0;
}