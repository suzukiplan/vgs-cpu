#include "vgsasm.h"
#include "vgscpu.h"

int vgsdrun(struct program_table* pt)
{
    int i, j, addr;
    for (addr = 0, i = 0; i < pt->line_number; i++) {
        if (pt->line[i].oplen) {
            printf("%08x:", addr);
            for (j = 0; j < pt->line[i].oplen; j++) {
                printf(" %02X", (int)pt->line[i].op[j]);
            }
            for (; j < 8; j++) {
                printf("   ");
            }
            printf(":");
            for (j = 0; j < pt->line[i].toknum; j++) {
                printf(" %s", pt->line[i].token[j]);
            }
            printf("\n");
            addr += pt->line[i].oplen;
        }
    }

    return 0;
}
