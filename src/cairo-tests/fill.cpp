#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>

#include "caskbench.h"

int
ca_setup_fill(caskbench_context_t *ctx)
{
    return 1;
}

void
ca_teardown_fill(void)
{
}

int
ca_test_fill(caskbench_context_t *ctx)
{
    int i;
    cairo_t *cr = ctx->cairo_cr;

    for (i=0; i<ctx->size; i++) {
        randomize_color (cr);
        cairo_arc (cr, 40*i, 40, 30, 0, 2*M_PI);
        cairo_fill (cr);
    }
    return 1;
}
