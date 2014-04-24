#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>

#include "caskbench.h"
#include "cairo-shapes.h"

int
ca_setup_paint(caskbench_context_t *ctx)
{
    return 1;
}

void
ca_teardown_paint()
{
}

int
ca_test_paint(caskbench_context_t *ctx)
{
    int i;

    for (i=0; i<ctx->size; i++) {
        cairoRandomizeColor(ctx);
        cairo_paint(ctx->cairo_cr);
    }
    return 1;
}
