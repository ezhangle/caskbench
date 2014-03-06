#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>

#include "caskbench.h"

int
ca_setup_rectangles(caskbench_context_t *ctx)
{
    cairo_t *cr = ctx->cairo_cr;

    cairo_set_antialias (cr, CAIRO_ANTIALIAS_DEFAULT);
    cairo_set_source_rgb (cr, 0, 0, 0);
    cairo_paint (cr);
    cairo_set_line_width (cr, 1);
    return 1;
}

void
ca_teardown_rectangles(void)
{
}

int
ca_test_rectangles(caskbench_context_t *ctx)
{
    int i;
    double w, h, x, y;
    cairo_t *cr = ctx->cairo_cr;

    for (i=0; i<ctx->size; i++) {
        randomize_color (cr);

        w = trunc( (0.5*(double)ctx->canvas_width*rand())/RAND_MAX ) + 1;
        h = trunc( (0.5*(double)ctx->canvas_height*rand())/RAND_MAX ) + 1;
        x = trunc( (0.5*(double)ctx->canvas_width*rand())/RAND_MAX );
        y = trunc( (0.5*(double)ctx->canvas_height*rand())/RAND_MAX );
        cairo_rectangle(cr, x+1, y+1, w, h);

        cairo_stroke (cr);
    }
    return 1;
}
