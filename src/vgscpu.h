#ifndef INCLUDE_VGSCPU_H
#define INCLUDE_VGSCPU_H

#ifdef __cplusplus
extern "C" {
#endif

void *vgscpu_create_context();
void vgscpu_release_context(void *ctx);
void vgscpu_load_program(void *ctx, void *pg, size_t size);
int vgscpu_run(void *ctx);

#ifdef __cplusplus
};
#endif

#endif