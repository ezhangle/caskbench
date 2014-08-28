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
#include "caskbench_context.h"
#include "cairo-shapes.h"

int
ca_setup_roundrect(caskbench_context_t *ctx)
{
    cairo_t *cr = ctx->cairo_cr;

    cairo_set_antialias (cr, CAIRO_ANTIALIAS_BEST);
    return 1;
}

void
ca_teardown_roundrect(void)
{
}

static void
rounded_rectangle (cairo_t *cr,
                   double x, double y, double w, double h,
                   double radius)
{
    cairo_move_to (cr, x+radius, y);
    cairo_arc (cr, x+w-radius, y+radius,   radius, M_PI + M_PI / 2, M_PI * 2        );
    cairo_arc (cr, x+w-radius, y+h-radius, radius, 0,               M_PI / 2        );
    cairo_arc (cr, x+radius,   y+h-radius, radius, M_PI/2,          M_PI            );
    cairo_arc (cr, x+radius,   y+radius,   radius, M_PI,            270 * M_PI / 180);
}

int
ca_test_roundrect(caskbench_context_t *ctx)
{
    int i;
    double line_width, x, y, radius;
    cairo_t *cr = ctx->cairo_cr;

    for (i=0; i<ctx->size; i++) {
        shapes_t shape;
        shape_copy(&ctx->shape_defaults, &shape);

        cairoRandomizeColor(ctx);

        shape.x = 10 + trunc( (((double)ctx->canvas_width-20.0)*rnd())/RAND_MAX );
        shape.y = 10 + trunc( (((double)ctx->canvas_height-20.0)*rnd())/RAND_MAX );

        /* vary radius upto half of MIN(X,Y) */
        shape.radius = (double)rnd()/RAND_MAX * 20;
        shape.width = 100;
        shape.height = 40;

        /* line_width cannot be more than twice of radius due to skia limitation - Issue #4 in skia https://github.com/Samsung/skia/issues/4 */
        shape.stroke_width = (double)rnd()/RAND_MAX * (2*shape.radius);

        cairoDrawRoundedRectangle(ctx, &shape);

        cairo_set_line_width (cr, shape.stroke_width);

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
