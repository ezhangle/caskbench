/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#include <config.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <cairo.h>

#include "caskbench.h"
#include "cairo-shapes.h"

int
ca_setup_rectangles(caskbench_context_t *ctx)
{
    cairo_t *cr = ctx->cairo_cr;

    cairo_set_antialias (cr, CAIRO_ANTIALIAS_DEFAULT);
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
        cairoRandomizeColor(ctx);

        w = trunc( (0.5*(double)ctx->canvas_width*rand())/RAND_MAX ) + 1;
        h = trunc( (0.5*(double)ctx->canvas_height*rand())/RAND_MAX ) + 1;
        x = trunc( (0.5*(double)ctx->canvas_width*rand())/RAND_MAX );
        y = trunc( (0.5*(double)ctx->canvas_height*rand())/RAND_MAX );
        cairo_rectangle(cr, x+1, y+1, w, h);

        cairo_stroke (cr);
    }
    return 1;
}

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0)(inline-open . 0)(case-label . +))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=8:softtabstop=4:fileencoding=utf-8:textwidth=99 :
