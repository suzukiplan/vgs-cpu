#include <stdio.h>
#include <stdlib.h>
#include "vgscpu.h"
#include "vgscpu_internal.h"

char* load_bin(const char* path, int* size)
{
    FILE* fp;
    void* data;

    fp = fopen(path, "rb");
    if (NULL == fp) {
        return NULL;
    }
    if (-1 == fseek(fp, 0, SEEK_END)) {
        fclose(fp);
        return NULL;
    }
    *size = (int)ftell(fp);
    if (*size < 1) {
        fclose(fp);
        return NULL;
    }
    if (-1 == fseek(fp, 0, SEEK_SET)) {
        fclose(fp);
        return NULL;
    }
    data = malloc(*size);
    if (NULL == data) {
        fclose(fp);
        return NULL;
    }
    ((char*)data)[*size] = '\0';
    if (*size != fread(data, 1, (size_t)*size, fp)) {
        fclose(fp);
        free(data);
        return NULL;
    }
    fclose(fp);
    return data;
}

int main(int argc, char* argv[])
{
    char cmd[4096];
    char* bin;
    int size;
    struct vgscpu_context* c = (struct vgscpu_context*)vgscpu_create_context();
    int result;

    if (!c) return -1;

    if (argc < 2) {
        puts("usage: asmtest test-source");
        return 1;
    }

    sprintf(cmd, "./vgsasm -o tp.bin %s", argv[1]);
    if (0 != system(cmd)) {
        puts("failed");
        return 2;
    }

    if (NULL == (bin = load_bin("tp.bin", &size))) {
        puts("failed");
        return 3;
    }
    vgscpu_load_program(c, bin, size);
    free(bin);

    c->r.d = 0x1;
    if (vgscpu_run(c)) {
        puts("failed");
        return 4;
    }
    result = (int)c->r.d;

    vgscpu_release_context(c);

    if (1 != result) {
        puts("failed");
    } else {
        puts("test success");
    }
    return result == 1 ? 0 : 5;
}