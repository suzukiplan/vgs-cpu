#ifndef INCLUDE_VGSCPU_H
#define INCLUDE_VGSCPU_H
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

void *vgscpu_create_context();
void *vgscpu_create_specific_context(unsigned int plsize, unsigned int ssize, unsigned int msize);
void vgscpu_release_context(void *ctx);
int vgscpu_load_program(void *ctx, void *pg, size_t size);
int vgscpu_run(void *ctx);
const char *vgscpu_get_last_error(void *ctx);

#ifdef __cplusplus
};
#endif

#endif
