#ifndef _WIN32
#include <sys/time.h>
#endif
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
    data = malloc(1 + *size);
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

int print_registers(struct vgscpu_context* c)
{
    printf("c->r.a = %08X, c->r.b = %08X, c->r.c = %08X, c->r.d = %08X\n", c->r.a, c->r.b, c->r.c, c->r.d);
    printf("c->r.p = %08X, c->r.s = %08X, c->f.z = %08X, c->f.q = %08X\n", c->r.p, c->r.s, c->f.z, c->f.z);
    return 0;
}

int main(int argc, char* argv[])
{
    char cmd[4096];
    char* bin;
    int size;
    struct vgscpu_context* c = (struct vgscpu_context*)vgscpu_create_context();
    int result;
#ifndef _WIN32
    long usec;
    double sec;
    struct timeval tvStart;
    struct timeval tvEnd;
#endif

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

    vgscpu_regist_interrupt(c, 0, print_registers);

    c->r.d = 0x1;
#ifndef _WIN32
    gettimeofday(&tvStart, NULL);
#endif
    if (vgscpu_run(c)) {
        printf("failed: %s\n", vgscpu_get_last_error(c));
        return 4;
    }
#ifndef _WIN32
    gettimeofday(&tvEnd, NULL);
    usec = tvEnd.tv_sec - tvStart.tv_sec;
    usec *= 1000000;
    usec += tvEnd.tv_usec;
    usec -= tvStart.tv_usec;
    sec = usec / 1000000.0;
    printf("executed: %fsec\n", sec);
#endif
    result = (int)c->r.d;

    vgscpu_release_context(c);

    if (1 != result) {
        puts("failed");
    } else {
        puts("test success");
    }
    return result == 1 ? 0 : 5;
}