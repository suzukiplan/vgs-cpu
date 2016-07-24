#include "vgsasm.h"
#include "vgscpu.h"

static struct program_table PT;

static int show_op(unsigned int rp)
{
    int i, j;
    unsigned int addr;

    for (addr = 0, i = 0; i < PT.line_number; i++) {
        if (PT.line[i].oplen) {
            if (addr == rp) {
                printf("%08x:", addr);
                for (j = 0; j < PT.line[i].oplen; j++) {
                    printf(" %02X", (int)PT.line[i].op[j]);
                }
                for (; j < 8; j++) {
                    printf("   ");
                }
                printf(":");
                for (j = 0; j < PT.line[i].toknum; j++) {
                    switch (j) {
                        case 0:
                            printf(" %4s", PT.line[i].token[j]);
                            break;
                        case 2:
                            printf(", %s", PT.line[i].token[j]);
                            break;
                        default:
                            printf(" %s", PT.line[i].token[j]);
                            break;
                    }
                }
                printf("\n");
                return 0;
            }
            addr += PT.line[i].oplen;
        }
    }
    return -1;
}

int vgsdrun(struct program_table* pt)
{
    void* ctx;
    void* pg;
    int psize;
    int i;

    memcpy(&PT, pt, sizeof(PT));

    for (psize = 0, i = 0; i < PT.line_number; i++) {
        if (PT.line[i].oplen) {
            psize += PT.line[i].oplen;
        }
    }

    if (0 == psize) {
        puts("empty");
        return -1;
    }

    pg = malloc(psize);
    if (NULL == pg) {
        puts("no memory");
        return -1;
    }

    for (psize = 0, i = 0; i < PT.line_number; i++) {
        if (PT.line[i].oplen) {
            memcpy(&((char*)pg)[psize], PT.line[i].op, PT.line[i].oplen);
            psize += PT.line[i].oplen;
        }
    }

    ctx = vgscpu_create_context();
    if (NULL == ctx) {
        puts("could not create context");
        free(pg);
        return -1;
    }

    if (vgscpu_load_program(ctx, pg, psize)) {
        puts("could not load program");
        free(pg);
        vgscpu_release_context(ctx);
        return -1;
    }
    free(pg);

    if (vgscpu_run(ctx)) {
        printf("CPU error detected: %s\n", vgscpu_get_last_error(ctx));
        return -1;
    }

    vgscpu_release_context(ctx);
    return 0;
}

void vgsdrun_callback(struct vgscpu_context* c)
{
    if (show_op(c->r.p)) {
        printf("invalid address: $%x\n", c->r.p);
    } else if (0x00 == c->p[c->r.p]) {
        printf("\n[registers]\n");
        printf("c->r.a = %08X, c->r.b = %08X, c->r.c = %08X, c->r.d = %08X\n", c->r.a, c->r.b, c->r.c, c->r.d);
        printf("c->r.p = %08X, c->r.s = %08X, c->f.z = %08X, c->f.q = %08X\n", c->r.p, c->r.s, c->f.z, c->f.z);
    }
}
